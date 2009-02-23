/*
 * linuxarms/armserver/src/acmthread.c
 * operate amthread_struct structure.
 * Niu Tao:niutao0602@gmail.com
 */
#define __DEBUG__

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "amthread.h"
#include "linuxarms.h"
#include "protocol.h"
#include "error.h"
#include "debug.h"
#include "config.h"
#include "login.h"
#include "anet.h"
#include "afview.h"
#include "assinfo.h"
#include "asprocess.h"
#include "afthread.h"
#include "atthread.h"
#include "asthread.h"
#include "acthread.h"
#include "thread.h"

static void amthread_down_lock(struct amthread_struct *amthread);
static void amthread_up_lock(struct amthread_struct *amthread);
static boolean amthread_set_protocol(struct amthread_struct *amthread, 
                        protocol_mthread protocol);
static boolean amthread_send(struct amthread_struct *amthread);
static boolean amthread_recv(struct amthread_struct *amthread);

static int check_count = 0;
/*
 * amthread_thread  main thread
 */
boolean amthread_thread(void *p)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)p;
	struct amthread_struct *amthread = linuxarms->amthread;
	struct asthread_struct *asthread = linuxarms->asthread;
	//struct login_struct *login = linuxarms->login;
	protocol_mthread  protocol;
	boolean request = TRUE;

	linuxarms_print("create amthread thread...\n");
	amthread->thread.id = linuxarms_thread_self();
	debug_print("hmthread socket ip : %s tcp: %d port: %d\n", amthread->socket.ip,
				amthread->socket.tcp, amthread->socket.port);
	while (amthread->thread.id) {
		if (!amthread->recv(amthread)) {
			linuxarms_print("amthread recv data error,exit....\n");
			armserver_do_logout(linuxarms);
			exit(1);
		}
		switch (amthread->trans.protocol) {
		case LOGIN: 
			debug_print("protocol->amthread: 用户登录\n");
			if (request) {
				linuxarms_print("one user request login, user name is %s\n",
						amthread->trans.user.name);
				request = FALSE;
			}
			/* 验证是否有用户已经登录，如果没有，则验证用户信息，
			 * 否则发送错误信息(HASUSER)信息给hostclient端的请求
			 * 者。
			 */
			
			debug_where();
			protocol = armserver_do_login(linuxarms);
			amthread->set_protocol(amthread, protocol);
			amthread->send(amthread);
			armserver_login_result(linuxarms, protocol);
			amthread->set_protocol(amthread, MMAX);
			break;
		case LOGOUT:
			/* 撤销其他线程(实时监视线程，文件浏览线程)，执行注销 */
			debug_print("protocol->amthread: 用户注销\n");
			amthread->set_protocol(amthread, LOGOUT);
			amthread->send(amthread);
			armserver_do_logout(linuxarms);
			exit(0);
			break;
		case RESTART:
			/* 判断用户权限，如果没有权限，则发送没有权限(NOCOMPETENCE)
			 * 错误信息，否则执行重启。
			 */
			debug_print("protocol->amthread: 重启arm系统\n");
			if (!amthread->competence) {
				amthread->set_protocol(amthread, NOCOMPETENCE);
				amthread->send(amthread);
				break;
			}
			armserver_do_logout(linuxarms);
			if (system("shutdown -r 0") == -1)
				print_error(EWARNING, "Can;t restart arm system\n");
			break;
		case SHUTDOWN:
			/* 判断用户权限，如果没有权限，则发送没有权限(NOCOMPETENCE)
			 * 错误信息，否则执行关机。
			 */
			debug_print("protocol->amthread: 关闭系统\n");
			if (!amthread->competence) {
				amthread->set_protocol(amthread, NOCOMPETENCE);
				amthread->send(amthread);
				break;
			}
			armserver_do_logout(linuxarms);
			if (system("shutdown -h 0") == -1)
				print_error(EWARNING, "Can't close arm system\n");
			break;
		case CSTHREAD:
			debug_print("protocol->amthread: 控制系统信息显示线程\n");
			asthread->proc.set_state(&asthread->proc, STOP);
			break;
		case CFTHREAD:
			debug_print("protocol->amthread: 控制文件浏览线程\n");
			break;
		case CCTHREAD:
			debug_print("protocol->amthread: 控制实时控制线程\n");
			break;
		case CLOSECLIENT:
			print_error(EWARNING,"客户端关闭...\n");
			armserver_do_logout(linuxarms);
			exit(0);
		default:
			break;
		}
	}
	return TRUE;
}


/*
 * init_amthread	initialize amthread_struct structure. if you not give action,
 * 		we set it NONE,it means do nothing.
 * @amthread:	the structure which will be initialized.
 * @user:	the user who use hostclient
 * @scoket:		tcp socket structure
 * @widget:	GtkWidget structure
 */
boolean amthread_init(struct amthread_struct *amthread, struct user_struct *user)
{
	LINUXARMS_POINTER(amthread);
	LINUXARMS_POINTER(user);

	linuxarms_thread_init(&amthread->thread);
	amthread->user = user;
	amthread_trans_init(&amthread->trans);
	amthread->down_lock = amthread_down_lock;
	amthread->up_lock = amthread_up_lock;
	amthread->set_protocol = amthread_set_protocol;
	amthread->send = amthread_send;
	amthread->recv = amthread_recv;
	amthread->competence = FALSE;

	amthread->up_lock(amthread);
	return TRUE;
}

static void amthread_down_lock(struct amthread_struct *amthread)
{
	if (!amthread)
		return ;
	linuxarms_thread_lock(&amthread->thread);
}
static void amthread_up_lock(struct amthread_struct *amthread)
{
	if (!amthread)
		return;
	linuxarms_thread_unlock(&amthread->thread);
}

/*
 * amthread_set_protocol	set act field of amthread_struct structure
 * @amthread:   the structure which you will set
 * @protocol:   the action which will be set,if it's a invalid parameter,
 * 		it will be set NONE.
 * @return:	TRUE if success, FALSE if fail.
 */
static boolean amthread_set_protocol(struct amthread_struct *amthread, 
			      protocol_mthread protocol)
{
	LINUXARMS_POINTER(amthread);
	return amthread_trans_set_protocol(&amthread->trans, protocol);
}

static boolean amthread_send(struct amthread_struct *amthread)
{
	LINUXARMS_POINTER(amthread);
	return anet_send(amthread->socket.tcp, (void *)&amthread->trans,
			 sizeof(struct amthread_trans));
}

static boolean amthread_recv(struct amthread_struct *amthread)
{
	LINUXARMS_POINTER(amthread);
	return anet_recv(amthread->socket.tcp, (void *)&amthread->trans,
			 sizeof(struct amthread_trans));

}
boolean amthread_trans_init(struct amthread_trans *amtrans)
{
	LINUXARMS_POINTER(amtrans);
	user_struct_init(&amtrans->user);
	amtrans->protocol = MMAX;
	return TRUE;
}
boolean amthread_trans_set_protocol(struct amthread_trans *amtrans, 
					protocol_mthread protocol)
{
	LINUXARMS_POINTER(amtrans);
	if (!PROTOCOL_IS_MTHREAD(protocol)) {
		amtrans->protocol = MMAX;
		return FALSE;
	}
	amtrans->protocol = protocol;
	return TRUE;
}
boolean armserver_create_all_thread(struct linuxarms_struct *linuxarms)
{
	struct afthread_struct *afthread = linuxarms->afthread;
	struct asthread_struct *asthread = linuxarms->asthread;
	//struct acthread_struct *acthread = linuxarms->acthread;
	
	debug_where();
	asthread->thread.id = linuxarms_thread_create(asthread_thread, asthread);
	debug_where();
	if (asthread->thread.id == NULL) {
		print_error(ESYSERR,"create asthread error");
		goto out;
	}
	debug_where();
	afthread->thread.id = linuxarms_thread_create(afthread_thread, afthread);
	if (afthread->thread.id == NULL) {
		print_error(ESYSERR,"create afthread error");
		goto out;
	}
	/*
	debug_where();
	acthread->thread.id = linuxarms_thread_create(acthread_thread, acthread);
	if (acthread->thread.id == NULL) {
		print_error(ESYSERR,"create afthread error");
		goto out;
	}
	*/
	return TRUE;
out:
	return FALSE;
}
void armserver_close_all_thread(struct linuxarms_struct *linuxarms)
{
	struct amthread_struct *amthread = linuxarms->amthread;
	struct afthread_struct *afthread = linuxarms->afthread;
	struct asthread_struct *asthread = linuxarms->asthread;
	struct acthread_struct *acthread = linuxarms->acthread;
	if (asthread->thread.id) {
		linuxarms_thread_exit(&asthread->thread.id);
		close_tcp_server(&asthread->socket);
	}
	if (afthread->thread.id) {
		linuxarms_thread_exit(&afthread->thread.id);
		close_tcp_server(&afthread->socket);
	}
	if (acthread->thread.id) {
		linuxarms_thread_exit(&acthread->thread.id);
		close_tcp_server(&acthread->socket);
	}
	if (amthread->thread.id) {
		linuxarms_thread_exit(&amthread->thread.id);
		close_tcp_server(&amthread->socket);
	}
}
protocol_mthread armserver_do_login(struct linuxarms_struct *linuxarms)
{
	struct login_struct *login = linuxarms->login;
	struct amthread_struct *amthread = linuxarms->amthread;
	struct asthread_struct *asthread = linuxarms->asthread;
	struct afthread_struct *afthread = linuxarms->afthread;
	struct acthread_struct *acthread = linuxarms->acthread;
	protocol_mthread protocol = LOGIN;

	if (strlen(login_user) > 0) {
		protocol = HAVEUSER;
		goto out;
	}
	strcpy(login_user, amthread->trans.user.name);
	if (check_count == LOGIN_CHECK_TIMEOUT) {
		protocol = CHECKMULT;
		goto out;
	}
	debug_where();
	user_struct_set(&login->user, amthread->trans.user.ip, 
			amthread->trans.user.name, amthread->trans.user.passwd);
	if (!login_check_user(login)) {
		protocol = CHECKERR;
		goto out;
	}
	
	amthread->competence = login_user_competence(login);
	asthread->competence = login_user_competence(login);
	afthread->competence = login_user_competence(login);
	acthread->competence = login_user_competence(login);
	debug_where();
	if (!login_set_env(login)) {
		protocol = CHECKERR;
		goto out;
	}
	if (!armserver_create_all_thread(linuxarms)) {
		protocol = LOGERR;
		goto out;
	}
out:
	return protocol;
}

void armserver_login_result(struct linuxarms_struct *linuxarms, protocol_mthread protocol)
{
	switch (protocol) {
	case HAVEUSER:
		linuxarms_print("one user have login, user name is %s\n", login_user);
		break;
	case LOGERR:
		linuxarms_print("create thread error, end login session...\n");
		memset(login_user, '\0', USER_NAME_LEN);
		armserver_close_all_thread(linuxarms);
		exit(1);
		break;
	case CHECKERR:
		linuxarms_print("check user information error,wait for next check...\n,");
		memset(login_user, '\0', USER_NAME_LEN);
		check_count++;
		break;
	case CHECKMULT:
		linuxarms_print("try to login count == %d, end login session...\n",
				LOGIN_CHECK_TIMEOUT);
		memset(login_user, '\0', USER_NAME_LEN);
		exit(1);
		break;
	case LOGIN:
		linuxarms_print("user login success, user name is %s\n", login_user);
		break;
	default:
		break;
	}
}

void armserver_do_logout(struct linuxarms_struct *linuxarms)
{
	memset(login_user, '\0', USER_NAME_LEN);
	armserver_close_all_thread(linuxarms);
}
