/*
 * linuxarms/armserver/src/camthread.c
 * operate amthread_struct structure.
 * Niu Tao:niutao0602@gmail.com
 */
#include "amthread.h"


/*
 * amthread_thread  main thread
 */
boolean amthread_thread(void *p)
{
	struct amthread_struct *amthread = (struct amthread_struct *)p;
	struct mtrans_struct trans_data;

	while (TRUE) {
		recv(amthread->socket->tcp_fd, &amthread->trans,
				sizeof(struct mtrans_struct),0);
		switch (amthread->trans->act) {
		case LOGIN: 

			break;
		case LOGOUT:

			break;
		case RESTART:

			break;
		case SHUTDOWN:

			break;
		case CSTHREAD:

			break;
		case CFTHREAD:

			break;
		case CCTHREAD:

			break;
		case NONE:
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
 */

int amthread_init(struct amthread_struct *amthread,
		     struct user_struct *user,
		     struct anet_struct *scoket,
		     struct proc_struct *proc,
		     struct fview_struct *fview)
{
	int ret = 0;
	if (!amthread) {
		ret = -ENOINIT;
		goto out;
	}
	if (!user) {
		ret = -ENOUSER;
		goto out;
	}
	if (!socket) {
		ret = -ENOSOCKET;
		goto out;
	}
	if (!proc) {
		ret = -ENOPROC;
		goto out;
	}

	if (!fview) {
		ret = -EFVIEW;
		goto out;
	}
	amthread->user = user;
	amthread->scoket = scoket;
	amthread->trans.user = *user;
	amthread->trans.act = NONE;
	amthread->proc = proc;
	amthread->fview = fview;

	amthread->down_lock = down_lock;
	amthread->up_lock = up_lock;
	amthread->set_user = set_user;
	amthread->set_act = set_act;
	amthread->set_socket = set_socket;
	amthread->set_proc = set_proc;
	amthread->set_fview = set_fview;
	amthread->send = amthread_send;
	amthread->judge_competence = judge_competence;
	amthread->down_lock();
out:
	return ret;
}



void down_lock(struct amthread_struct *amthread)
{
	if (!amthread)
		return ;
	while (!amthread->lock)
		sleep(1);
	amthread->lock = FALSE;
}
void up_lock(struct amthread_struct *amthread)
{
	if (!amthread)
		return;
	amthread->lock = TRUE;
}
/*
 * set_user	set user field of amthread_struct structure
 * @amthread:	the structure which you will set
 * @user:	the user who use the hostclient
 * @return:	TRUE if success, FALSE if fail.
 */
boolean set_user(struct amthread_struct *amthread,
		struct user_struct *user)
{
	if (!amthread)
		goto out;
	if (!user)
		goto out;
	amthread->user = user;
	return TRUE;
out:
	return FALSE;
}

/*
 * set_act	set act field of amthread_struct structure
 * @amthread:	the structure which you will set
 * @act:	the action which will be set,if it's a invalid parameter,
 * 		it will be set NONE.
 * @return:	TRUE if success, FALSE if fail.
 */
boolean set_act(struct amthread_struct *amthread, protocol_mthread act)
{
	if (!amthread)
		return FALSE;
	if (act < 0 || act > ACTION_NUM - 1)
		amthread->trans.act = NONE;
	else
		amthread->trans.act = act;
	return TRUE;
}

boolean set_scoket(struct amthread_struct *amthread,
			struct anet_struct *scoket)
{
	if (!amthread || !scoket)
		return FALSE;
	amthread->scoket = scoket;
	return TRUE;
}


boolean set_trans(struct amthread_struct *amthread,
			    struct mtrans_struct *mtrans)
{
	if (!amthread || !mtrans)
		return FALSE;
	amthread->trans = *mtrans;
	return TRUE;
}

 boolean set_fview(struct amthread_struct *amthread,
		   struct fview_struct *fview)
{
}

boolean set_proc(struct amthread_struct *amthread,
                 struct proc_struct *proc)
{
}

boolean amthread_send(struct amthread_struct *amthread)
{
	if (!amthread)
		return FALSE;

}

boolean judge_competence(struct amthread_struct *amthread)
{
	if (!amthread || !amthread->user)
		return FALSE;
	if ( strcmp(amthread->user->name, "root") == 0)
		return TRUE;
	return FALSE;
}
