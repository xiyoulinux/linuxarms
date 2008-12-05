/*
 * linuxarms/armserver/src/acmthread.c
 * operate amthread_struct structure.
 * Niu Tao:niutao0602@gmail.com
 */

#include "amthread.h"
#include "linuxarms.h"
#include "protocol.h"
#include "error.h"
#include "debug.h"
#include "config.h"
#include "login.h"
#include "anet.h"
#include <sys/wait.h>
static void amthread_down_lock(struct amthread_struct *amthread);
static void amthread_up_lock(struct amthread_struct *amthread);
static boolean amthread_set_protocol(struct amthread_struct *amthread, 
                        protocol_mthread protocol);
static boolean amthread_send(struct amthread_struct *amthread);
static boolean amthread_recv(struct amthread_struct *amthread);
static boolean amthread_judge_competence(struct amthread_struct *amthread); 

/*
 * amthread_thread  main thread
 */
boolean amthread_thread(void *p)
{
	struct amthread_struct *amthread = (struct amthread_struct *)p;
	anet_init(amthread->socket, get_host_ip(),get_mthread_port());
	create_tcp_server(amthread->socket);

	while (TRUE) {
		amthread->recv(amthread);
		switch (amthread->trans.protocol) {
		case LOGIN: 
			/* 验证是否有用户已经登录，如果没有，则验证用户信息，
			 * 否则发送错误信息(HASUSER)信息给hostclient端的请求
			 * 者。
			 */
			break;
		case LOGOUT:
			/* 撤销其他线程(实时监视线程，文件浏览线程)，执行注销 */
			break;
		case RESTART:
			/* 判断用户权限，如果没有权限，则发送没有权限(NOCOMPETENCE)
			 * 错误信息，否则执行重启。
			 */
			break;
		case SHUTDOWN:
			/* 判断用户权限，如果没有权限，则发送没有权限(NOCOMPETENCE)
			 * 错误信息，否则执行关机。
			 */
			break;
		case CSTHREAD:
			/* 发送消息给实时监视线程，使其停止从proc获取数据 */
			break;
		case CFTHREAD:
			/* 发送消息给文件浏览线程，使其停止读取目录信息 */
			break;
		case CCTHREAD:
			/*  */
			break;
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

int amthread_init(struct amthread_struct *amthread,
		     struct user_struct *user,
		     struct anet_struct *socket)
{
	int ret = 0;
	if (!amthread) {
		ret = ENOINIT;
		goto out;
	}
	if (!user) {
		ret = ENOINIT;
		goto out;
	}
	if (!socket) {
		ret = ENOSOCKET;
		goto out;
	}

	amthread->user = user;
	amthread->socket = socket;
	amthread->trans.user = *user;
	amthread->trans.protocol = MMAX;
	amthread->down_lock = amthread_down_lock;
	amthread->up_lock = amthread_up_lock;
	amthread->set_protocol = amthread_set_protocol;
	amthread->send = amthread_send;
	amthread->recv = amthread_recv;
	amthread->judge_competence = amthread_judge_competence;

	amthread->down_lock(amthread);
out:
	return ret;
}


static void amthread_down_lock(struct amthread_struct *amthread)
{
	if (!amthread)
		return ;
	while (!amthread->lock)
		sleep(1);
	amthread->lock = FALSE;
}
static void amthread_up_lock(struct amthread_struct *amthread)
{
	if (!amthread)
		return;
	amthread->lock = TRUE;
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
	if (!amthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	if (!PROTOCOL_IS_MTHREAD(protocol))
		amthread->trans.protocol = MMAX;
	else
		amthread->trans.protocol = protocol;

	return TRUE;
}

static boolean amthread_send(struct amthread_struct *amthread)
{
	if (!amthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	return anet_send(amthread->socket->tcp, (void *)&amthread->trans,
			 sizeof(struct amthread_trans));
}

static boolean amthread_judge_competence(struct amthread_struct *amthread)
{
	if (!amthread || !amthread->user)
		return FALSE;
	if (amthread->user->competence)
		return TRUE;
	return FALSE;
}
static boolean amthread_recv(struct amthread_struct *amthread)
{
	if (!amthread) {
		debug_where();
		print(EWARNING, "无效的参数");
		return FALSE;
	}
	return anet_recv(amthread->socket->tcp, (void *)&amthread->trans,
			 sizeof(struct amthread_trans));

}
