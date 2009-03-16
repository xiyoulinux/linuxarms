#ifndef _AMTHREAD_H
#define _AMTHREAD_H
/*
 * linuxarms/armserver/src/amthread.h
 * Niu Tao:<niutao0602@gmail.com>
 */
#include "linuxarms.h"
#include "login.h"
#include "anet.h"
#include "protocol.h"
#include "thread.h"

#define LOGIN_CHECK_TIMEOUT 3
/*
 * amthread_trans  the packet which send or receive
 * @user:          user information
 * @protocol:      the action arm system will execute
 */
struct amthread_trans {
        struct user_struct user;
        protocol_mthread protocol;
};
boolean amthread_trans_init(struct amthread_trans *amtrans);
boolean amthread_trans_set_protocol(struct amthread_trans *amtrans, 
					protocol_mthread protocol);

/*
 * main structure
 * @thread:     thread structure
 * @lock:	is there a data sending or receiving?
 * @user:	the user who use hostclient
 * @trans:	the packet which send or receive
 * @socket:	TCP socket
 */ 
struct amthread_struct {
	struct linuxarms_thread thread;
	struct user_struct *user;
	struct amthread_trans trans;
	struct anet_struct socket;
	boolean permit;

	void (*down_lock)(struct amthread_struct *amthread);
	void (*up_lock)(struct amthread_struct *amthread);
	boolean (*set_protocol)(struct amthread_struct *amthread, 
				protocol_mthread protocol);
	boolean (*send)(struct amthread_struct *amthread);
	boolean (*recv)(struct amthread_struct *amthread);

};
//extern int *have_user;
extern char *login_user;
boolean amthread_init(struct amthread_struct *amthread, struct user_struct *user);
boolean amthread_thread(void *p);
boolean armserver_create_all_thread(struct linuxarms_struct *linuxarms);
void armserver_close_all_thread(struct linuxarms_struct *linuxarms);
protocol_mthread armserver_do_login(struct linuxarms_struct *linuxarms);
void armserver_login_result(struct linuxarms_struct *linuxarms, protocol_mthread protocol);
void armserver_do_logout(struct linuxarms_struct *linuxarms);
#endif
