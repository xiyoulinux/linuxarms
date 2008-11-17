#ifndef _hmthread_H
#define _hmthread_H
/*
 * linuxarms/hostclient/src/hmthread.h
 * Niu Tao:niutao0602@gmail.com
 */
#include "linuxarms.h"
#include <gtk/gtk.h>

/*
 * mwidget	contain some GtkWidget which hmthread will use
 */
struct mwidget {
	GtkWidget *login;
	GtkWidget *logout;
	GtkWidget *restart;
	GtkWidget *shutdown;
};
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

struct user_struct;
struct hnet_struct;
/*
 * hmthread_struct	main structure
 * @lock:	is there a data sending or receiving?
 * @user:	the user who use hostclient
 * @trans:	the packet which send or receive
 * @ip:		TCP socket
 * @widget:	GtkWidget
 */ 
struct hmthread_struct {
	boolean lock;
	struct user_struct *user;
	struct hnet_struct *socket;
	struct mtrans_struct trans;
	struct mwidget widget;

	void (*down_lock)(struct hmthread_struct *hmthread);
	void (*up_lock)(struct hmthread_struct *hmthread);
	boolean (*set_user)(struct hmthread_struct *hmthread,
			    struct user_struct *user);
	boolean (*set_act)(struct hmthread_struct *hmthread, maction act);
	boolean (*set_socket)(struct hmthread_struct *hmthread,
			  struct hnet_struct *socket);
	boolean (*set_trans)(struct hmthread_struct *hmthread,
			     struct mtrans_struct *trans);
	boolean (*send)(struct hmthread_struct *hmthread);
	boolean (*judge_competence)(struct hmthread_struct *hmthread);
	boolean (*print_info)(const char *text);
	void (*print_error)(int error,const char *text);

};

int hmthread_init(struct hmthread_struct *hmthread,
                 struct user_struct *user,
                 struct hnet_struct *ip,
                 struct mwidget *mwidget);

void down_lock(struct hmthread_struct *hmthread);
void up_lock(struct hmthread_struct *hmthread);
boolean set_user(struct hmthread_struct *hmthread,
			struct user_struct *user);
boolean set_act(struct hmthread_struct *hmthread, maction act);
boolean set_socket(struct hmthread_struct *hmthread,
		      struct hnet_struct *socket);
boolean set_trans(struct hmthread_struct *hmthread,
			    struct mtrans_struct *trans);
boolean hmthread_send(struct hmthread_struct *hmthread);
boolean judge_competence(struct hmthread_struct *hmthread);
#endif
