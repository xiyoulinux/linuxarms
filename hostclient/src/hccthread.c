#include "hcthread.h"
#include "linuxarms.h"
/*
 * 初始化实时控制主数据结构
 *
 */
boolean hcthread_init(struct hfthread_struct *hcthread,
		struct hcthread_widget *widget, 
		struct hfthread_trans *trans,
		struct hnet_struct *socket)
{
}
/*
 * 发送一个请求到armserver上
 */
boolean hcthread_send(struct hcthread_struct *hcthread)
{
}
/*
 *接受一个数据从armserver
 */
boolean hcthread_recv(struct hcthread_struct *hcthread)
{
}

/*
 * 实时控制主线程执行体
 * @p:  struct hcthread_struct
 */
boolean showinfo(struct hcthread_struct *hcthread)
{
/* 显示信息 */
}
gboolean hcthread_thread(void *p)
{
	struct hcthread_struct *hcthread = (struct hcthread_struct *)p;
	/* 建立网络连接 */
	while (TRUE) {
		/* 接收数据 */
		/* 分析处理数据 */
		/* 如果未接受到协议CSENDALL ，则循环接受，同时向arm端发送协议
		 * CRECV */
		/* 如果接受到协议CRECVALL，则退出 */
	}
	return TRUE;
}
