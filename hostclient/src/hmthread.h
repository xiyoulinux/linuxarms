/*
 * linuxarms/hostclient/src/hmthread.h
 * Niu Tao:niutao0602@gmail.com
 */
#ifndef _HMTHREAD_H
#define _HMTHREAD_H

#include <gtk/gtk.h>
#include "linuxarms.h"
#include "login.h"
#include "hnet.h"
#include "protocol.h"
#include "thread.h"
/*
 * hmthread_mwidget	contain some GtkWidget which hmthread will use
 */
struct hmthread_widget {
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
boolean hmthread_trans_init(struct hmthread_trans *hmtrans);
boolean hmthread_trans_set_protocol(struct hmthread_trans *hmtrans, 
					protocol_mthread protocol);
/*
 * hmthread_struct	main structure
 * @lock:	is there a data sending or receiving?
 * @user:	the user who use hostclient
 * @trans:	the packet which send or receive
 * @socket:	TCP socket
 * @widget:	GtkWidget
 */ 
struct hmthread_struct {
	struct linuxarms_thread thread;
	protocol_mthread protocol;
	struct user_struct *user;
	struct hnet_struct socket;
	struct hmthread_trans trans;
	struct hmthread_widget widget;
	boolean competence;
	int timer;

	void (*down_lock)(struct hmthread_struct *hmthread);
	void (*up_lock)(struct hmthread_struct *hmthread);
	boolean (*set_protocol)(struct hmthread_struct *hmthread, 
				protocol_mthread protocol);
	boolean (*send)(struct hmthread_struct *hmthread);
	boolean (*recv)(struct hmthread_struct *hmthread);

};

/* 初始化hmthread_struct结构体 */
boolean hmthread_init(struct hmthread_struct *hmthread, struct user_struct *user);
/* 主线程执行体 */
void *hmthread_thread(void *p);
gboolean create_window_main_timeout(gpointer user_data);
void hostclient_close_all_thread(struct linuxarms_struct *linuxarms);
void hostclient_user_logout(struct linuxarms_struct *linuxarms);
void hostclient_init(struct linuxarms_struct *linuxarms);
#endif
