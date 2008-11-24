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
/*
 * hmthread_thread  main thread
 */
boolean hmthread_thread(void *p)
{
	struct hmthread_struct *hmthread = (struct hmthread_struct *)p;
	struct hmthread_mtrans trans_data;

	while (TRUE) {
		recv(hmthread->socket->tcp, &hmthread->trans,
				sizeof(struct hmthread_mtrans),0);
		switch (hmthread->trans.state) {
		case SUCCESS: /* execute success */

			break;
		case NOUSER: /* no the user in arm system */

			break;
		case HASUSER: /* a user had been login */

			break;
		case NOCOMPETENCE: /* no competence to execute command */

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
	hmthread->trans.act = MMAX;
	hmthread->down_lock = down_lock;
	hmthread->up_lock = up_lock;
	hmthread->set_user = set_user;
	hmthread->set_act = set_act;
	hmthread->set_socket = set_socket;
	hmthread->send = hmthread_send;
	hmthread->judge_competence = judge_competence;
	hmthread->print_info = statusbar_set_text;
	hmthread->print_error = print_error;

	hmthread->down_lock(hmthread);
out:
	return ret;
}


void down_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return ;
	while (!hmthread->lock)
		sleep(1);
	hmthread->lock = FALSE;
}
void up_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return;
	hmthread->lock = TRUE;
}
/*
 * set_user	set user field of hmthread_struct structure
 * @hmthread:	the structure which you will set
 * @user:	the user who use the hostclient
 * @return:	TRUE if success, FALSE if fail.
 */
boolean set_user(struct hmthread_struct *hmthread,
		struct user_struct *user)
{
	if (!hmthread || !user)
		return FALSE;
	hmthread->user = user;
	return TRUE;
}

/*
 * set_act	set act field of hmthread_struct structure
 * @hmthread:	the structure which you will set
 * @act:	the action which will be set,if it's a invalid parameter,
 * 		it will be set NONE.
 * @return:	TRUE if success, FALSE if fail.
 */
boolean set_act(struct hmthread_struct *hmthread, protocol_mthread act)
{
	if (!hmthread)
		return FALSE;
	if (act < 0 || act > MMAX - 1)
		hmthread->trans.act = MMAX;
	else
		hmthread->trans.act = act;
	return TRUE;
}

boolean set_socket(struct hmthread_struct *hmthread,
			struct hnet_struct *socket)
{
	if (!hmthread || !socket)
		return FALSE;
	hmthread->socket = socket;
	return TRUE;
}


boolean set_trans(struct hmthread_struct *hmthread,
			    struct hmthread_mtrans *mtrans)
{
	if (!hmthread || !mtrans)
		return FALSE;
	hmthread->trans = *mtrans;
	return TRUE;
}
boolean hmthread_send(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return FALSE;

}

boolean judge_competence(struct hmthread_struct *hmthread)
{
	if (!hmthread || !hmthread->user)
		return FALSE;
	if (hmthread->user->competence)
		return TRUE;
	return FALSE;
}
