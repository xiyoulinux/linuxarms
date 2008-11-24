#ifndef _hmthread_H
#define _hmthread_H
/*
 * linuxarms/hostclient/src/hmthread.h
 * Niu Tao:niutao0602@gmail.com
 */
#include "linuxarms.h"
#include <gtk/gtk.h>
#include "login.h"
#include "hnet.h"
#include "protocol.h"
/*
 * hmthread_mwidget	contain some GtkWidget which hmthread will use
 */
struct hmthread_widget {
	GtkWidget *login;
	GtkWidget *logout;
	GtkWidget *restart;
	GtkWidget *shutdown;
};
/*
 * hmthread_mtrans  the packet which send or receive
 * @user:	user information
 * @act:        the action arm system will execute
 * @state:      the execute state of arm system
 */
struct hmthread_mtrans {
        struct user_struct user;
        protocol_mthread act;
        protocol_return state;
};

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
	struct hmthread_mtrans trans;
	struct hmthread_widget widget;

	void (*down_lock)(struct hmthread_struct *hmthread);
	void (*up_lock)(struct hmthread_struct *hmthread);
	boolean (*set_user)(struct hmthread_struct *hmthread,
			    struct user_struct *user);
	boolean (*set_act)(struct hmthread_struct *hmthread, protocol_mthread act);
	boolean (*set_socket)(struct hmthread_struct *hmthread,
			  struct hnet_struct *socket);
	boolean (*set_trans)(struct hmthread_struct *hmthread,
			     struct hmthread_mtrans *trans);
	boolean (*send)(struct hmthread_struct *hmthread);
	boolean (*judge_competence)(struct hmthread_struct *hmthread);
	boolean (*print_info)(const char *text);
	void (*print_error)(int error,const char *text);

};

int hmthread_init(struct hmthread_struct *hmthread,
                 struct user_struct *user,
                 struct hnet_struct *ip,
                 struct hmthread_widget *widget);

void down_lock(struct hmthread_struct *hmthread);
void up_lock(struct hmthread_struct *hmthread);
boolean set_user(struct hmthread_struct *hmthread,
			struct user_struct *user);
boolean set_act(struct hmthread_struct *hmthread, protocol_mthread act);
boolean set_socket(struct hmthread_struct *hmthread,
		      struct hnet_struct *socket);
boolean set_trans(struct hmthread_struct *hmthread,
			    struct hmthread_mtrans *trans);
boolean hmthread_send(struct hmthread_struct *hmthread);
boolean judge_competence(struct hmthread_struct *hmthread);
#endif
