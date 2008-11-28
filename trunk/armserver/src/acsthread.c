/*
 * linuxarms/hostclient/src/hcsthread.c
 * Niu Tao <niutao0602@gmail.com>
 */
#include "asthread.h"
#include "anet.h"
#include "error.h"
#include "debug.h"
/*
 * 初始化asthread_struct结构
 */
boolean asthread_init(struct asthread_struct *asthread,
		      struct assinfo_struct *assinfo,
		      struct asprocess_struct *asprocess,
		      struct asthread_trans *trans,
		      struct anet_struct *socket)
{
}
/*
 * 线程主执行体
 */
boolean asthread_thread(void *p)
{
	struct asthread_struct *asthread =
		(struct asthread_struct *)p;
	anet_init(&asthread->socket,"192.168.200.79",34);
	if (!create_tcp_server(&asthread->socket)) {
		print_error(ESYSERR,"create tcp error");
		return FALSE;
	}
	while (TRUE) {
		anet_recv(asthread->socket.tcp,&asthread->trans,sizeof(struct asthread_trans));
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


boolean asthread_send(struct asthread_struct *asthread)
{
	if (!asthread) {
		print_error(EWARNING,"不能发送数据");
		return FALSE;
	}
	if (!anet_send(asthread->socket.tcp, &asthread->trans, 
		 sizeof(struct asthread_trans)) ) {
		print_error(EWARNING,"发送数据失败");
		return FALSE;
	}
	return TRUE;

}
