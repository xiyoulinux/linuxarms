/*
 * linuxarms/hostclient/src/chmthread.c
 * operate hmthread_struct structure.
 * Niu Tao:niutao0602@gmail.com
 */
#define __DEBUG__

#include <gtk/gtk.h>
#include <unistd.h>
#include <string.h>
#include "hmthread.h"
#include "hsthread.h"
#include "hfthread.h"
#include "hcthread.h"
#include "linuxarms.h"
#include "protocol.h"
#include "statusbar.h"
#include "error.h"
#include "debug.h"
#include "mwindow.h"
#include "login.h"
#include "config.h"
#include "message.h"
#include "thread.h"

static boolean hmthread_send(struct hmthread_struct *hmthread);
static boolean hmthread_recv(struct hmthread_struct *hmthread);
static void hmthread_down_lock(struct hmthread_struct *hmthread);
static void hmthread_up_lock(struct hmthread_struct *hmthread);
static boolean hmthread_set_protocol(struct hmthread_struct *hmthread, 
		                     protocol_mthread protocol);
static boolean hmthread_judge_competence(struct hmthread_struct *hmthread);

boolean create_window_main_timeout(gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct mwindow_struct *mwindow = linuxarms->mwindow;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hcthread_struct *hcthread = linuxarms->hcthread;
	struct login_struct *login = linuxarms->login;
	if (hmthread->protocol == MMAX)
		return TRUE;
	switch (hmthread->protocol) {
		case LOGIN: /* 用户登录成功，创建主窗口 */
			linuxarms_print("user login success, user name is %s\n", hmthread->user->name);
			debug_print("protocol->hmthread :用户登录成功....\n");
			login_config_write(login);
			login_config_free(login->config);
			gtk_widget_destroy(login->widget.window_login);
			create_window_main(linuxarms);
			gtk_window_main_set_sensitive(linuxarms);
			debug_where();
			linuxarms_thread_create(hsthread_thread, hsthread);
			debug_where();
			//linuxarms_thread_create(hfthread_thread, hfthread);
			//linuxarms_thread_create(hcthread_thread, hcthread);
			char buf[40];
			snprintf(buf, 40, "Linux ARMS[登录用户：%s]", hmthread->user->name);
			gtk_window_set_title(GTK_WINDOW(linuxarms->mwindow->window), buf);
			gtk_widget_set_sensitive(hmthread->widget.logout, TRUE);
			gtk_widget_set_sensitive(hmthread->widget.restart, TRUE);
			gtk_widget_set_sensitive(hmthread->widget.shutdown, TRUE);
			break;
		case LOGOUT:
			debug_print("protocol->hmthread :armserver已经注销...\n");
LOGOUT_RESTART_SHUTDOWN:			
			gtk_window_main_set_sensitive(linuxarms);
			gtk_widget_set_sensitive(mwindow->window, TRUE);
			gtk_widget_set_sensitive(mwindow->notebook, FALSE);
			gtk_widget_set_sensitive(mwindow->toolbar, FALSE);
			gtk_widget_set_sensitive(mwindow->frame, FALSE);
			gtk_widget_set_sensitive(hmthread->widget.login, TRUE);

			hostclient_close_all_thread(linuxarms);
			break;
		case HAVEUSER: /* 已经有一个用户登录 */
			debug_print("protocol->hmthread :已经有一个用户登录...\n");
			create_window_dialog("登录失败：已经有一个用户");
			break;
		case CHECKERR:
			debug_print("protocol->hmthread :登录验证用户信息失败...\n");
			create_window_dialog("登录失败：验证用户信息错误\n"
					"可能的原因是没有用户或者密码错误");
			break;
		case LOGERR:
			debug_print("protocol->hmthread :登录创建服务线程失败...\n");
			create_window_dialog("登录失败：可能的原因是创建\n"
					"服务线程失败");
			break;
		case MERROR:
			debug_print("protocol->hmthread :执行命令失败\n");
			statusbar_set_text("执行命令失败");
			break;
		case MSUCCESS: /* execute success */
			debug_print("protocol->hmthread :执行命令成功\n");
			statusbar_set_text("执行命令成功");
			break;
		case NOCOMPETENCE: /* 没有权限执行命令 */
			statusbar_set_text("没有权限执行命令");
			break;
		case RESTART:
			goto LOGOUT_RESTART_SHUTDOWN;
		case SHUTDOWN:
			goto LOGOUT_RESTART_SHUTDOWN;
		default:
			break;
	}
	hmthread->protocol = MMAX;
	hmthread->up_lock(hmthread);
	return TRUE;
}
/*
 * hmthread_thread  main thread
 */
boolean hmthread_thread(void *p)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)p;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct login_struct *login = linuxarms->login;
	struct user_struct *user = login->user;
	
	linuxarms_print("create hmthread thread...\n");	
	hmthread->thread = linuxarms_thread_self();
	user_struct_set(&hmthread->trans.user, user->ip, user->name, user->passwd);
	/* 发送登录请求 */
	hmthread->set_protocol(hmthread, LOGIN);
	hmthread->send(hmthread);
	debug_where();
	debug_print("hmthread socket ip : %s tcp: %d port: %d\n", hmthread->socket.ip,
				hmthread->socket.tcp, hmthread->socket.port);
	while (hmthread->thread) {
		if (!hmthread->recv(hmthread)) {
			linuxarms_print("hmthread recive data error\n");
			break;
		}
		hmthread->down_lock(hmthread);
		hmthread->protocol = hmthread->trans.protocol;
	}
	return TRUE;
}


/*
 * init_hmthread	initialize hmthread_struct structure. if you not give action,
 * 		we set it NONE,it means do nothing.
 * @hmthread:	the structure which will be initialized.
 * @user:	the user who use hostclient
 * @scoket:		tcp socket structure
 */

int hmthread_init(struct hmthread_struct *hmthread, struct user_struct *user)
{
	int ret = 0;
	if (!hmthread) {
		ret = ENOINIT;
		goto out;
	}
	if (!user) {
		ret = ENOINIT;
		goto out;
	}
	hmthread->thread = NULL;
	hmthread->user = user;
	hnet_init(&hmthread->socket, NULL, get_mthread_port());
	hmthread->trans.user = *user;
	hmthread->trans.protocol = MMAX;
	hmthread->down_lock = hmthread_down_lock;
	hmthread->up_lock = hmthread_up_lock;
	hmthread->set_protocol = hmthread_set_protocol;
	hmthread->send = hmthread_send;
	hmthread->recv = hmthread_recv;
	hmthread->competence = hmthread_judge_competence;

	hmthread->up_lock(hmthread);
out:
	return ret;
}

static void hmthread_down_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return ;
	while (hmthread->lock)
		sleep(1);
	hmthread->lock = TRUE;
}
static void hmthread_up_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return;
	hmthread->lock = FALSE;
}

/*
 * hmthread_set_protocol	set act field of hmthread_struct structure
 * @hmthread:   the structure which you will set
 * @protocol:   the action which will be set,if it's a invalid parameter,
 * 		it will be set NONE.
 * @return:	TRUE if success, FALSE if fail.
 */
static boolean hmthread_set_protocol(struct hmthread_struct *hmthread, 
			      protocol_mthread protocol)
{
	LINUXARMS_POINTER(hmthread);
	if (!PROTOCOL_IS_MTHREAD(protocol))
		hmthread->trans.protocol = MMAX;
	else
		hmthread->trans.protocol = protocol;
	return TRUE;
}
static boolean hmthread_send(struct hmthread_struct *hmthread)
{
	LINUXARMS_POINTER(hmthread);
	return hnet_send(hmthread->socket.tcp, (void *)&hmthread->trans, 
			sizeof(struct hmthread_trans));

}

static boolean hmthread_recv(struct hmthread_struct *hmthread)
{
	LINUXARMS_POINTER(hmthread);
	return hnet_recv(hmthread->socket.tcp, (void *)&hmthread->trans, 
			sizeof(struct hmthread_trans));

}
static boolean hmthread_judge_competence(struct hmthread_struct *hmthread)
{
	if (!hmthread || !hmthread->user)
		return FALSE;
	if (strcmp(hmthread->user->name, "root") == 0)
		return TRUE;
	return FALSE;
}

boolean hmthread_trans_init(struct hmthread_trans *hmtrans)
{
	LINUXARMS_POINTER(hmtrans);
	user_struct_init(&hmtrans->user);
	hmtrans->protocol = MMAX;
	return TRUE;
}
boolean hmthread_trans_set_protocol(struct hmthread_trans *hmtrans, 
					protocol_mthread protocol)
{
	LINUXARMS_POINTER(hmtrans);
	if (!PROTOCOL_IS_MTHREAD(protocol)) {
		hmtrans->protocol = MMAX;
		return FALSE;
	}
	hmtrans->protocol = protocol;
	return TRUE;
}

void hostclient_close_all_thread(struct linuxarms_struct *linuxarms)
{
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hcthread_struct *hcthread = linuxarms->hcthread;
	if (hsthread->thread) {
		linuxarms_thread_exit(&hsthread->thread);
		close_tcp_client(&hsthread->socket);
	}
	if (hfthread->thread) {
		linuxarms_thread_exit(&hfthread->thread);
		close_tcp_client(&hfthread->socket);
	}
	if (hcthread->thread) {
		linuxarms_thread_exit(&hcthread->thread);
		close_tcp_client(&hcthread->socket);
	}
	if (hmthread->thread) {
		linuxarms_thread_exit(&hmthread->thread);
		close_tcp_client(&hmthread->socket);
	}
}
