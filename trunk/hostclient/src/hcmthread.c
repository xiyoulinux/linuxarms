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
#include "htthread.h"
#include "linuxarms.h"
#include "protocol.h"
#include "statusbar.h"
#include "toolbar.h"
#include "error.h"
#include "debug.h"
#include "mwindow.h"
#include "login.h"
#include "config.h"
#include "message.h"
#include "thread.h"

static void hostclient_user_login(struct linuxarms_struct *linuxarms);
static boolean hmthread_send(struct hmthread_struct *hmthread);
static boolean hmthread_recv(struct hmthread_struct *hmthread);
static void hmthread_down_lock(struct hmthread_struct *hmthread);
static void hmthread_up_lock(struct hmthread_struct *hmthread);
static boolean hmthread_set_protocol(struct hmthread_struct *hmthread, 
		                     protocol_mthread protocol);

boolean create_window_main_timeout(gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct login_struct *login = linuxarms->login;

	if (hmthread->protocol == MMAX)
		return TRUE;
	switch (hmthread->protocol) {
		case LOGIN: /* 用户登录成功，创建主窗口 */
			hostclient_user_login(linuxarms);
			break;
		case HAVEUSER: /* one user have login */
			debug_print("protocol->hmthread :已经有一个用户登录...\n");
			message_box_error(login->widget.window_login, "登录失败：已经有一个用户");
			close_tcp_client(&hmthread->socket);
			break;
		case CHECKERR:  /* check user information error */
			debug_print("protocol->hmthread :登录验证用户信息失败...\n");
			message_box_error(login->widget.window_login, 
					  "登录失败：验证用户信息错误\n"
					  "可能的原因是没有用户或者密码错误");
			break;
		case LOGERR:
			debug_print("protocol->hmthread :登录创建服务线程失败...\n");
			message_box_error(login->widget.window_login, 
					"登录失败：可能的原因是创建\n服务线程失败");
			break;
		case CHECKMULT:
			debug_print("protocol->hmthread：尝试登录次数过多...\n");
			checkmult = TRUE;
			message_box_error(login->widget.window_login, "尝试登录次数过多...\n");
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
void *hmthread_thread(void *p)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)p;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct login_struct *login = linuxarms->login;
	struct user_struct *user = &login->user;
	
	linuxarms_print("create hmthread thread...\n");	
	hmthread->thread.id = linuxarms_thread_self();

	user_struct_set(&hmthread->trans.user, user->ip, user->name, user->passwd);
	/* 发送登录请求 */
	hmthread->set_protocol(hmthread, LOGIN);
	hmthread->send(hmthread);
	hmthread->set_protocol(hmthread, MMAX);
	debug_where();
	while (hmthread->thread.id) {
		if (!hmthread->recv(hmthread)) {
			linuxarms_print("hmthread recive data error\n");
			//cb_linuxarms_window_main_close(NULL, linuxarms);
			break;
		}
		hmthread->down_lock(hmthread);
		hmthread->protocol = hmthread->trans.protocol;
	}
	return NULL;
}


/*
 * init_hmthread	initialize hmthread_struct structure. if you not give action,
 * 		we set it NONE,it means do nothing.
 * @hmthread:	the structure which will be initialized.
 * @user:	the user who use hostclient
 * @scoket:		tcp socket structure
 */

boolean hmthread_init(struct hmthread_struct *hmthread, struct user_struct *user)
{
	LINUXARMS_POINTER(hmthread);
	LINUXARMS_POINTER(user);

	linuxarms_thread_init(&hmthread->thread);
	hmthread->user = user;
	hnet_init(&hmthread->socket, NULL, get_armserver_port());
	hmthread->trans.user = *user;
	hmthread->trans.protocol = MMAX;
	hmthread->protocol = MMAX;
	hmthread->timer = -1;
	hmthread->down_lock = hmthread_down_lock;
	hmthread->up_lock = hmthread_up_lock;
	hmthread->set_protocol = hmthread_set_protocol;
	hmthread->send = hmthread_send;
	hmthread->recv = hmthread_recv;
	hmthread->competence = FALSE;

	hmthread->up_lock(hmthread);
	return TRUE;
}

static void hmthread_down_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return ;
	linuxarms_thread_lock(&hmthread->thread);
}
static void hmthread_up_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return;
	linuxarms_thread_unlock(&hmthread->thread);
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

static void hostclient_user_login(struct linuxarms_struct *linuxarms)
{
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hcthread_struct *hcthread = linuxarms->hcthread;
	struct htthread_struct *htthread = hfthread->hftrans;
	struct login_struct *login = linuxarms->login;
	char buf[40];

	linuxarms_print("user login success, user name is %s\n", hmthread->user->name);
	debug_print("protocol->hmthread :用户登录成功....\n");
	login_config_write(login);
	login_config_free(&login->config);
	hmthread->competence = login_user_competence(login);
	hsthread->competence = login_user_competence(login);
	hfthread->competence = login_user_competence(login);
	hcthread->competence = login_user_competence(login);
	htthread->competence = login_user_competence(login);
	debug_where();
	gtk_widget_destroy(login->widget.window_login);
	create_window_main(linuxarms);
	debug_where();
	gtk_window_main_set_sensitive(linuxarms);
	debug_where();

	linuxarms_thread_create(hsthread_thread, hsthread);
	debug_where();

	linuxarms_thread_create(hfthread_thread, linuxarms);
	debug_where();

	linuxarms_thread_create(hcthread_thread, hcthread);
	snprintf(buf, 40, "Linux ARMS[登录用户：%s]", hmthread->user->name);
	debug_where();
	gtk_window_set_title(GTK_WINDOW(linuxarms->mwindow->window), buf);
	gtk_widget_set_sensitive(hmthread->widget.logout, TRUE);
	gtk_widget_set_sensitive(hmthread->widget.restart, TRUE);
	gtk_widget_set_sensitive(hmthread->widget.shutdown, TRUE);
	debug_where();
}
void hostclient_user_logout(struct linuxarms_struct *linuxarms)
{
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	
	debug_where();
	if (hsthread->timer.timer != -1) {
		hsthread_close_timer(hsthread);
		debug_where();
		debug_print("删除定时器\n");
	}
	gtk_timeout_remove(hmthread->timer);
	debug_where();
	list_head_free();
}
void hostclient_close_all_thread(struct linuxarms_struct *linuxarms)
{
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hcthread_struct *hcthread = linuxarms->hcthread;

	if (hsthread->thread.id) {
		linuxarms_thread_exit(&hsthread->thread);
		close_tcp_client(&hsthread->socket);
	}
	if (hfthread->thread.id) {
		linuxarms_thread_exit(&hfthread->thread);
		close_tcp_client(&hfthread->socket);
	}
	if (hcthread->thread.id) {
		linuxarms_thread_exit(&hcthread->thread);
		close_tcp_client(&hcthread->socket);
	}
	if (hmthread->thread.id) {
		linuxarms_thread_exit(&hmthread->thread);
		close_tcp_client(&hmthread->socket);
	}
}
