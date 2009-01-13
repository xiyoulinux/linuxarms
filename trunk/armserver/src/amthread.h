#ifndef _AMTHREAD_H
#define _AMTHREAD_H
/*
 * linuxarms/armserver/src/amthread.h
 * Niu Tao:niutao0602@gmail.com
 */
#include "linuxarms.h"
#include "login.h"
#include "anet.h"
#include "protocol.h"
/*
 * amthread_trans  the packet which send or receive
 * @user:          user information
 * @protocol:      the action arm system will execute
 */
struct amthread_trans {
        struct user_struct user;
        protocol_mthread protocol;
};

/*
 * amthread_struct	main structure
 * @lock:	is there a data sending or receiving?
 * @user:	the user who use hostclient
 * @trans:	the packet which send or receive
 * @socket:	TCP socket
 */ 
struct amthread_struct {
	linuxarms_thread_t thread_id;
	boolean lock;
	struct user_struct *user;
	struct anet_struct *socket;

	void (*down_lock)(struct amthread_struct *amthread);
	void (*up_lock)(struct amthread_struct *amthread);
	boolean (*set_protocol)(struct amthread_struct *amthread, 
				protocol_mthread protocol);
	boolean (*send)(struct amthread_struct *amthread);
	boolean (*recv)(struct amthread_struct *amthread);
	boolean (*judge_competence)(struct amthread_struct *amthread);

};

int amthread_init(struct amthread_struct *amthread,
                 struct user_struct *user,
                 struct anet_struct *socket);
boolean amthread_thread(void *p);
#endif
