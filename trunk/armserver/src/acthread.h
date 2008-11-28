/*
 * linuxarms/hostclient/src/hcthread.c
 * chen Jifeng<chengov@gmail.com>
 */
#ifndef _ACTHREAD_H
#define _ACTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
#include "anet.h"

#define TRAS_SIZE 512
/*  
 * acthread_trans 实时控制时传送的数据
 * @trans:     控制协议
 * @buffer:    传输的数据
 */
struct acthread_trans {
	protocol_cthread trans;
	char buffer[TRAS_SIZE];
};
/*  
 *acthread_strucnt 实时控制线程主数据结构 
 *@socket:   建立的网络连接 
 *@trans:    传输数据
  */
struct acthread_struct {
	struct anet_struct socket;
	struct acthread_trans trans;

	boolean (*send)(struct acthread_struct *acthread);
	boolean (*recv)(struct acthread_struct *acthread);
	boolean (*handle)(struct acthread_struct *acthread);
};
boolean acthread_send(struct acthread_struct *acthread);
boolean acthread_recv(struct acthread_struct *acthread);
boolean acthread_handle(struct acthread_struct *acthread);
boolean acthread_init(struct acthread_struct *acthread,
		      struct acthread_trans *trans,
		      struct anet_struct *socket);
boolean acthread_thread(void *p);
#endif
