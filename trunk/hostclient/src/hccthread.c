#include "hcthread.h"
#include "linuxarms.h"
#include "hfthread.h"
/*
 * 初始化实时控制主数据结构
 *
 */
boolean hcthread_init(struct hcthread_struct *hcthread,
		struct hcthread_widget *widget, 
		struct hnet_struct *socket,
		struct hcthread_trans *trans)
{
	return TRUE;
}
/*
 * 发送一个请求到armserver上
 */
boolean hcthread_send(struct hcthread_struct *hcthread)
{
	return TRUE;
}
/*
 *接受一个数据从armserver
 */
boolean hcthread_recv(struct hcthread_struct *hcthread)
{
	return TRUE;
}

boolean showinfo(struct hcthread_struct *hcthread)
{
/* 显示信息 */
	return TRUE;
}
/*
 *实时控制主线程执行体
 *@p:  struct hcthread_struct
 */ 
gboolean hcthread_thread(void *p)
{
	struct hcthread_struct *hcthread = (struct hcthread_struct *)p;
	/* 建立网络连接 */
	while (TRUE) {
		do {
			hcthread_recv(hcthread);        /* 接收从arm端输出过来的数据 */  
			if(hcthread->trans.protocol == CSEND) {
				hcthread_send(hcthread);/* 发送协议CRECV */
				showinfo(hcthread);     /* 显示所接受的数据 */
			}
		} while (hcthread->trans.protocol != CSENDALL);
		hcthread_send(hcthread);                 /* 发送协议CRECVALL */
	}
	return TRUE;
}
