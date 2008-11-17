/*
 * linuxarms/armserver/src/amthread.h
 * Niu Tao:niutao0602@gmail.com
 */
#include "linuxarms.h"
#include "protocol.h"
#include "proc.h"
#include <gtk/gtk.h>


/*
 * mtrans_struct  the packet which send or receive
 * @user:	user information
 * @act:        the action arm system will execute
 * @state:      the execute state of arm system
 */
struct mtrans_struct {
        struct user_struct user;
        maction act;
        exec_state state;
};

/*
 * amthread_struct	main structure
 * @login:	the user has login?TRUE is Yes,FALSE is No.
 * @lock:	is there a data sending or receiving?
 * @user:	the user who use hostclient
 * @trans:	the packet which send or receive
 * @socket:	TCP socket
 * @proc:		proc_struct structure
 * @fview:	fview_struct structures
 */ 
struct amthread_struct {
	boolean login;
	boolean lock;
	struct user_struct *user;
	struct armip_struct *socket;
	struct mtrans_struct trans;
	struct proc_struct *proc;
	struct fview_struct *fview;

	void (*down_lock)(struct amthread_struct *amthread);
	void (*up_lock)(struct amthread_struct *amthread);
	boolean (*set_user)(struct amthread_struct *amthread,
	   			      struct user_struct *user);
	boolean (*set_act)(struct amthread_struct *amthread, maction act);
	boolean (*set_socket)(struct amthread_struct *amthread,
			  struct armip_struct *socket);
	boolean (*set_trans)(struct amthread_struct *amthread,
			     struct mtrans_struct *trans);
	boolean (*set_proc)(struct amthread_struct *amthread,
			    struct proc_struct *proc);
	boolean (*set_fview)(struct amthread_struct *amthread,
			     struct fview_struct *fview);
	boolean (*send)(struct amthread_struct *amthread);
	boolean (*judge_competence)(struct amthread_struct *amthread);
};

int amthread_init(struct amthread_struct *amthread,
                 struct user_struct *user,
                 struct armip_struct *ip,
                 struct mwidget *mwidget);

void down_lock(struct amthread_struct *amthread);
void up_lock(struct amthread_struct *amthread);
boolean set_user(struct amthread_struct *amthread,
			struct user_struct *user);
boolean set_act(struct amthread_struct *amthread, maction act);
boolean set_socket(struct amthread_struct *amthread,
		      struct armip_struct *socket);
boolean set_trans(struct amthread_struct *amthread,
			    struct mtrans_struct *trans);
boolean set_fview(struct amthread_struct *amthread,
			     struct fview_struct *fview);
boolean set_proc(struct amthread_struct *amthread,
		    struct proc_struct *proc);
boolean amthread_send(struct amthread_struct *amthread);
boolean judge_competence(struct amthread_struct *amthread);
