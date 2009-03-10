/*
 * linuxarms/armseerver/src/acthread.h
 * chen Jifeng<chengov@gmail.com>
 */
#ifndef _ACTHREAD_H
#define _ACTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
#include "anet.h"
#include "thread.h"

#define ACTHREAD_TRANS_SIZE 512
#define COMMAND_NOT_FOUND 32512
#define TEMP_FILE "/tmp/result.txt"
/*  
 * acthread_trans 实时控制时传送的数据
 * @trans:     控制协议
 * @buffer:    传输的数据
 */
struct acthread_trans {
	protocol_cthread protocol;
	char buffer[ACTHREAD_TRANS_SIZE];
};

boolean acthread_trans_init(struct acthread_trans *trans);
boolean acthread_trans_set_protocol(struct acthread_trans *trans,protocol_cthread protocol);
boolean acthread_trans_set_buf(struct acthread_trans *trans, const char *buf);
/*  
 *acthread_strucnt 实时控制线程主数据结构 
 *@socket:   建立的网络连接 
 *@trans:    传输数据
  */
struct acthread_struct {
	struct linuxarms_thread thread;
	struct anet_struct socket;
	struct acthread_trans trans;
	boolean competence;

	boolean (*send)(struct acthread_struct *acthread);
	boolean (*recv)(struct acthread_struct *acthread);
};
boolean acthread_init(struct acthread_struct *acthread);
void *acthread_thread(void *p);
#endif
