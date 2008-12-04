/*
 * linuxarms/hostclient/src/chmthread.c
 * operate hmthread_struct structure.
 * Niu Tao:niutao0602@gmail.com
 */

#include "hmthread.h"
#include "linuxarms.h"
#include "protocol.h"
#include "statusbar.h"
#include "error.h"
#include <gtk/gtk.h>


static boolean hmthread_send(struct hmthread_struct *hmthread);
static boolean hmthread_recv(struct hmthread_struct *hmthread);
static void hmthread_down_lock(struct hmthread_struct *hmthread);
static void hmthread_up_lock(struct hmthread_struct *hmthread);
static boolean hmthread_set_protocol(struct hmthread_struct *hmthread, 
		                     protocol_mthread protocol);
static boolean hmthread_judge_competence(struct hmthread_struct *hmthread);
/*
 * hmthread_thread  main thread
 */
boolean hmthread_thread(void *p)
{
	struct hmthread_struct *hmthread = (struct hmthread_struct *)p;

	while (TRUE) {
		hmthread->recv(hmthread);
		switch (hmthread->trans.protocol) {
		case LOGIN: /* 用户登录成功，创建主窗口 */
			
			break;
		case MSUCCESS: /* execute success */

			break;
		case NOUSER: /* 没有用户信息 */

			break;
		case HASUSER: /* 已经有一个用户登录 */

			break;
		case NOCOMPETENCE: /* 没有权限执行命令 */

			break;
		default:
			break;
		}

	}
	return TRUE;
}


/*
 * init_hmthread	initialize hmthread_struct structure. if you not give action,
 * 		we set it NONE,it means do nothing.
 * @hmthread:	the structure which will be initialized.
 * @user:	the user who use hostclient
 * @scoket:		tcp socket structure
 * @widget:	GtkWidget structure
 */

int hmthread_init(struct hmthread_struct *hmthread,
		     struct user_struct *user,
		     struct hnet_struct *socket,
		     struct hmthread_widget *widget)
{
	int ret = 0;
	if (!hmthread || !widget) {
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

	hmthread->user = user;
	hmthread->socket = socket;
	hmthread->widget = *widget;
	hmthread->trans.user = *user;
	hmthread->trans.protocol = MMAX;
	hmthread->down_lock = hmthread_down_lock;
	hmthread->up_lock = hmthread_up_lock;
	hmthread->set_protocol = hmthread_set_protocol;
	hmthread->send = hmthread_send;
	hmthread->recv = hmthread_recv;
	hmthread->judge_competence = hmthread_judge_competence;

	hmthread->down_lock(hmthread);
out:
	return ret;
}

static void hmthread_down_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return ;
	while (!hmthread->lock)
		sleep(1);
	hmthread->lock = FALSE;
}
static void hmthread_up_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return;
	hmthread->lock = TRUE;
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
	if (!hmthread)
		return FALSE;
	if (protocol < 0 || protocol > MMAX - 1)
		hmthread->trans.protocol = MMAX;
	else
		hmthread->trans.protocol = protocol;
	return TRUE;
}
static boolean hmthread_send(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return FALSE;
	return hnet_send(hmthread->socket->tcp, &hmthread->trans, 
			sizeof(struct hmthread_trans));

}

static boolean hmthread_recv(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return FALSE;
	return hnet_recv(hmthread->socket->tcp, &hmthread->trans, 
			sizeof(struct hmthread_trans));

}
static boolean hmthread_judge_competence(struct hmthread_struct *hmthread)
{
	if (!hmthread || !hmthread->user)
		return FALSE;
	if (hmthread->user->competence)
		return TRUE;
	return FALSE;
}
