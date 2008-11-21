/*
 * linuxarms/hostclient/src/hcsthread.c
 * Niu Tao <niutao0602@gmail.com>
 */
#include "asthread.h"
#include "anet.h"
/*
 * 初始化asthread_struct结构
 */
boolean asthread_init(struct asthread_struct *asthread,
				   struct hssinfo_struct *ssinfo.
				   struct hsprocess_struct *hsprocess,
				   struct asthread_trans *trans,
				   struct anet_struct *socket,
				   struct asthread_timer *timer)
{
}
/*
 * 线程主执行体
 */
gboolean asthread_thread(void *p)
{
	struct asthread_struct *asthread =
		(struct asthread_struct *)p;
	anet_init(&asthread->socket,"192.168.200.79",34);
	if (!create_tcp_client(&asthread->socket)) {
		print_error(ESYSERR,"create tcp error");
		return FALSE;
	}
	while (TRUE) {
		recv(asthread->socket.tcp,&asthread->trans,sizeof(struct asthread_trans),0);
		switch (asthread->trans.ctrl) {
		case SYSINFO:
			/*显示系统信息*/
			break;
		case SPROCESS:
			/*显示进程信息*/
			break;
		case KILL:
		case SMAX:
		default:
			break;
		}
	}
}

gboolean asthread_timer(gpointer data)
{
	struct asthread_struct *asthread =
		(struct asthread_struct *)data;
	if (!asthread) {
		print_error(WARNING,"启动定时器失败，无法更新时间");
		return FALSE;
	}
	switch (asthread->trans->ctrl) {
		case SYSINFO:
			break;
		case SPROCESS:
			break;
		case KILL:
			break;
		case default:
			break;
	}
	return TRUE;
}

boolean asthread_send(struct asthread_struct *asthread)
{
	if (!asthread) {
		print_error(WARNING,"不能发送数据");
		return FALSE;
	}
	if (send(asthread->socket->tcp, &asthread->trans, 
		 sizeof(struct asthread_trans),0) == -1) {
		print_error(WARNING,"发送数据失败");
		return FALSE;
	}
	return TRUE;

}
