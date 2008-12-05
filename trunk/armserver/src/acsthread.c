/*
 * linuxarms/hostclient/src/hcsthread.c
 * Niu Tao <niutao0602@gmail.com>
 */
#include "asthread.h"
#include "anet.h"
#include "error.h"
#include "debug.h"
#include "config.h"

/*
 * 初始化asthread_struct结构
 */
boolean asthread_init(struct asthread_struct *asthread,
		      struct assinfo_struct *assinfo,
		      struct asprocess_struct *asprocess,
		      struct asthread_trans *trans,
		      struct anet_struct *socket)
{
	if (!asthread || !assinfo || !asprocess || 
	    !trans || !socket) {
		debug_where();
		print_error(ESYSERR, "初始化数据为空");
		return FALSE;
	}

	asthread->assinfo   = assinfo;
	asthread->asprocess = asprocess;
	asthread->trans    = *trans;
	asthread->socket   = *socket;

	asthread->send     = asthread_send;
	asthread->recv     = asthread_recv;
	asthread->lock     = FALSE;

	return TRUE;
}
/*
 * 线程主执行体
 */
boolean asthread_thread(void *p)
{
	struct asthread_struct *asthread =
		(struct asthread_struct *)p;
	
	anet_init(&asthread->socket, get_host_ip(), get_sthread_port());
	if (!create_tcp_server(&asthread->socket)) {
		print_error(ESYSERR,"create tcp error");
		return FALSE;
	}
	while (TRUE) {
		asthread->recv(asthread);
		//anet_recv(asthread->socket.tcp,&asthread->trans,sizeof(struct asthread_trans));
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

boolean asthread_set_trans(struct asthread_struct *asthread, protocol_sthread ctrl, int kill)
{
	if (!asthread) {
		debug_where();
		print_error(EWARNING, "无效参数");
		return FALSE;
	}
	if (!PROTOCOL_IS_STHREAD(ctrl)) {
		debug_where();
		print_error(EWARNING, "无效的协议，设置发送失败");
		return FALSE;
	}
	if (kill < -1) {
		kill = -1;
	}
	asthread->trans.ctrl = ctrl;
	asthread->trans.kill = kill;
	return TRUE;
}

/* 
 * 发送一个信息给hostclient
 */
boolean asthread_send(struct asthread_struct *asthread)
{
	if (!asthread) {
		debug_where();
		print_error(EWARNING, "无效参数");
		return FALSE;
	}
	if (!anet_send(asthread->socket.tcp, &asthread->trans, 
		 sizeof(struct asthread_trans))) {
		print_error(EWARNING,"发送数据失败");
		return FALSE;
	}
	return TRUE;
}

/* 接收hostclient发送过来的信息 */
boolean asthread_recv(struct asthread_struct *asthread)
{
	if (!asthread) {
		debug_where();
		print_error(EWARNING, "无效参数");
		return FALSE;
	}
	return anet_recv(asthread->socket.tcp, &asthread->trans, 
			sizeof(struct asthread_trans));
}
