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

	while (TRUE) {
		recv(hmthread->socket->tcp, &hmthread->trans,
				sizeof(struct hmthread_trans),0);
		switch (hmthread->trans.protocol) {
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
	hmthread->trans.protocol = MMAX;
	hmthread->down_lock = hmthread_down_lock;
	hmthread->up_lock = hmthread_up_lock;
	hmthread->set_protocol = hmthread_set_protocol;
	hmthread->send = hmthread_send;
	hmthread->judge_competence = hmthread_judge_competence;

	hmthread->down_lock(hmthread);
out:
	return ret;
}


void hmthread_down_lock(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return ;
	while (!hmthread->lock)
		sleep(1);
	hmthread->lock = FALSE;
}
void hmthread_up_lock(struct hmthread_struct *hmthread)
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
boolean hmthread_set_protocol(struct hmthread_struct *hmthread, 
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
/*
boolean set_socket(struct hmthread_struct *hmthread,
			struct hnet_struct *socket)
{
	if (!hmthread || !socket)
		return FALSE;
	hmthread->socket = socket;
	return TRUE;
}


boolean set_trans(struct hmthread_struct *hmthread,
			    struct hmthread_trans *trans)
{
	if (!hmthread || !trans)
		return FALSE;
	hmthread->trans = *trans;
	return TRUE;
}
*/
boolean hmthread_send(struct hmthread_struct *hmthread)
{
	if (!hmthread)
		return FALSE;

}

boolean hmthread_judge_competence(struct hmthread_struct *hmthread)
{
	if (!hmthread || !hmthread->user)
		return FALSE;
	if (hmthread->user->competence)
		return TRUE;
	return FALSE;
}
