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
#include "anet.h"
#include <wait.h>
/*
 * amthread_thread  main thread
 */
boolean amthread_thread(void *p)
{
	struct amthread_struct *amthread = (struct amthread_struct *)p;

	while (TRUE) {
		anet_recv(amthread->socket->tcp, (void *)&amthread->trans,
				sizeof(struct amthread_trans));
		switch (amthread->trans.protocol) {
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
	amthread->judge_competence = amthread_judge_competence;

	amthread->down_lock(amthread);
out:
	return ret;
}


void amthread_down_lock(struct amthread_struct *amthread)
{
	if (!amthread)
		return ;
	while (!amthread->lock)
		sleep(1);
	amthread->lock = FALSE;
}
void amthread_up_lock(struct amthread_struct *amthread)
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
boolean amthread_set_protocol(struct amthread_struct *amthread, 
			      protocol_mthread protocol)
{
	if (!amthread)
		return FALSE;
	if (protocol < 0 || protocol > MMAX - 1)
		amthread->trans.protocol = MMAX;
	else
		amthread->trans.protocol = protocol;
	return TRUE;
}
/*
boolean set_socket(struct amthread_struct *amthread,
			struct anet_struct *socket)
{
	if (!amthread || !socket)
		return FALSE;
	amthread->socket = socket;
	return TRUE;
}


boolean set_trans(struct amthread_struct *amthread,
			    struct amthread_trans *trans)
{
	if (!amthread || !trans)
		return FALSE;
	amthread->trans = *trans;
	return TRUE;
}
*/
boolean amthread_send(struct amthread_struct *amthread)
{
	if (!amthread)
		return FALSE;

}

boolean amthread_judge_competence(struct amthread_struct *amthread)
{
	if (!amthread || !amthread->user)
		return FALSE;
	if (amthread->user->competence)
		return TRUE;
	return FALSE;
}
