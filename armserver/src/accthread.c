#include "acthread.h"
#include "linuxarms.h"
#include "anet.h"
/*
 * 初始化实时控制主数据结构
 *
 */
boolean acthread_init(struct acthread_struct *acthread,
		struct acthread_trans *trans,
		struct anet_struct *socket)
{
	return TRUE;
}
/*
 * 发送一个请求到hostclient上
 */
boolean acthread_send(struct acthread_struct *acthread)
{
	return TRUE;
}
/*
 *接受一个数据从hostclient
 */
boolean acthread_recv(struct acthread_struct *acthread)
{
	return TRUE;
}
boolean acthread_handle(struct acthread_struct *acthread)
{
	return TRUE;
}
/*
 *arm端实时控制线程
 *@p:  struct hcthread_struct
 */
boolean acthread_thread(void *p)
{
	struct acthread_struct *acthread = (struct acthread_struct *)p;
	/* 建立网络连接 */
	while (TRUE) {
		acthread_recv(acthread);   /* 接收数据 */
		if (acthread->trans.protocol == CSEND) {
			acthread_handle(acthread);
		}
		do {
			acthread_send(acthread); /* 发送数据 */
			acthread_recv(acthread); /* 接受数据 */
		} while (acthread->trans.protocol != CRECVALL);
	}
	return TRUE;
}
