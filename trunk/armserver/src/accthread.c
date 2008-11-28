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
}
/*
 *接受一个数据从hostclient
 */
boolean acthread_recv(struct acthread_struct *acthread)
{
}

/*
 * armserver端实时控制处理命令
 * @p:  struct hcthread_struct
 */
boolean acthread_handle(struct acthread_struct *acthread)
{
}
boolean acthread_thread(void *p)
{
	struct acthread_struct *acthread = (struct acthread_struct *)p;
	/* 建立网络连接 */
	while (TRUE) {
		/* 接收数据 */
		acthread_handle(acthread);
		/* 发送处理结果，如果发送完成则发送协议CSENDALL */
		/* 如果未接受到协议CRECVALL ，则循环发送结果，同时向hostclient端发送协议
		 * CSEND */
		/* 如果接受到协议CRECVALL，则退出 */
	}
	return TRUE;
}
