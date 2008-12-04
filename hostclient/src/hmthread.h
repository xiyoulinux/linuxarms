/*
 * linuxarms/hostclient/src/hmthread.h
 * Niu Tao:niutao0602@gmail.com
 */
#ifndef _hmthread_H
#define _hmthread_H
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
 * hmthread_trans  the packet which send or receive
 * @user:        user information
 * @protocol:    the action arm system will execute
 */
struct hmthread_trans {
        struct user_struct user;
        protocol_mthread protocol;
};

/*
 * hmthread_struct	main structure
 * @lock:	is there a data sending or receiving?
 * @user:	the user who use hostclient
 * @trans:	the packet which send or receive
 * @socket:	TCP socket
 * @widget:	GtkWidget
 */ 
struct hmthread_struct {
	boolean lock;
	struct user_struct *user;
	struct hnet_struct *socket;
	struct hmthread_trans trans;
	struct hmthread_widget widget;

	void (*down_lock)(struct hmthread_struct *hmthread);
	void (*up_lock)(struct hmthread_struct *hmthread);
	boolean (*set_protocol)(struct hmthread_struct *hmthread, 
				protocol_mthread protocol);
	boolean (*send)(struct hmthread_struct *hmthread);
	boolean (*recv)(struct hmthread_struct *hmthread);
	boolean (*judge_competence)(struct hmthread_struct *hmthread);

};

/* 初始化hmthread_struct结构体 */
int hmthread_init(struct hmthread_struct *hmthread,
                 struct user_struct *user,
                 struct hnet_struct *socket,
                 struct hmthread_widget *widget);
/* 主线程执行体 */
 boolean hmthread_thread(void *p);
/*
void hmthread_down_lock(struct hmthread_struct *hmthread);
void hmthread_up_lock(struct hmthread_struct *hmthread);
boolean hmthread_set_protocol(struct hmthread_struct *hmthread, 
		     protocol_mthread protocol);
boolean hmthread_send(struct hmthread_struct *hmthread);
boolean hmthread_recv(struct hmthread_struct *hmthread);

boolean hmthread_judge_competence(struct hmthread_struct *hmthread);
*/
#endif
