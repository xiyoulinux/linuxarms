#ifndef _HTTHREAD_H
#define _HTTHREAD_H
/* 未添加 */
#include "linuxarms.h"
#include "protocol.h"
#include "hnet.h"
struct htthread_trans {
	protocol_fthread protocol;
	char buf[256];
};
struct htthread_struct {
	struct hnet_struct socket;
	struct htthread_trans trans;

	boolean (*send)(struct htthread_struct *);
	boolean (*recv)(struct htthread_struct *);
	char a; 
};
/*
static boolean httread_send(struct htthread_struct *htthread)
{
	hnet_send(htthread->socket.tcp, &httrhead->trans, sizeof(struct htthread_trans));
}

boolean upload(struct htthread_struct *htthread)
{
	create_tcp_client(&htthread->socket);
	htthread_recv(htthread);
}
*/
#endif
