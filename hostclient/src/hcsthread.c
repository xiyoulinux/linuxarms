/*
 * linuxarms/hostclient/src/hcsthread.c
 * Niu Tao <niutao0602@gmail.com>
 */
#include "hsthread.h"
#include "hnet.h"
#include "ssinfo.h"
#include "sprocess.h"
#include "error.h"
#include "debug.h"
#include "config.h"
#include "login.h"

/*
 * 初始化hsthread_struct结构
 * @return:  如果有一个参数为空，则返回FALSE.
 */
boolean hsthread_init(struct hsthread_struct *hsthread,
		      struct hssinfo_struct *ssinfo,
		      struct hsprocess_struct *sprocess,
		      struct hsthread_trans *trans,
		      struct hnet_struct *socket,
		      struct hsthread_timer *timer)
{
	if (!hsthread || !ssinfo || !sprocess || 
	    !trans || !socket || !timer) {
		debug_where();
		print_error(ESYSERR, "初始化数据为空");
		return FALSE;
	}

	hsthread->ssinfo   = ssinfo;
	hsthread->sprocess = sprocess;
	hsthread->trans    = *trans;
	hsthread->socket   = *socket;
	hsthread->timer    = *timer;
	
	hsthread->send     = hsthread_send;
	hsthread->recv     = hsthread_recv;
	hsthread->lock     = FALSE;

	return TRUE;
}
/*
 * 线程主执行体，负责传输arm系统信息和进程信息，并且在用户请求杀死
 * 进程的时候，传输杀死进程所需信息
 * @return:  如果该函数执行返回，则线程执行结束
 */
gboolean hsthread_thread(void *p)
{
	struct hsthread_struct *hsthread =
		(struct hsthread_struct *)p;

	hnet_init(&hsthread->socket,get_armserver_ip(), get_sthread_port());
	if (!create_tcp_client(&hsthread->socket)) {
		print_error(ESYSERR,"create tcp error");
		return FALSE;
	}
	while (TRUE) {
		hsthread->recv(hsthread);
		switch (hsthread->trans.ctrl) {
		case SYSINFO:
			/*显示系统信息*/
			break;
		case SPROCESS:
			/*显示进程信息*/
			break;
		case KILLSUC:
			/* 接收到的成功杀死进程反馈信息 */
			break;
		case KILLERR:
			/* 接收到的杀死进程失败反馈信息 */
			break;
		case SMAX:

		default:
			break;
		}
	}
	return TRUE;
}

gboolean hsthread_timer(gpointer data)
{
	struct hsthread_struct *hsthread =
		(struct hsthread_struct *)data;
	if (!hsthread) {
		print_error(EWARNING,"启动定时器失败，无法更新时间");
		return FALSE;
	}
	hsthread->send(hsthread);
	return TRUE;
}

boolean hsthread_set_trans(struct hsthread_struct *hsthread, protocol_sthread ctrl, int kill)
{
	if (!hsthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
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
	hsthread->trans.ctrl = ctrl;
	hsthread->trans.kill = kill;
	return TRUE;
}

/* 
 * 发送一个信息给armserver，其中kill为要杀死的进程的id，
 * 如果不是杀死进程，则忽略该参数(设置为-1).
 */
boolean hsthread_send(struct hsthread_struct *hsthread)
{
	if (!hsthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	
	if (!PROTOCOL_IS_STHREAD(hsthread->trans.ctrl)) {
		print_error(EWARNING, "无效的协议，发送失败");
		return FALSE;
	}
	if (!hnet_send(hsthread->socket.tcp, &hsthread->trans, 
		 sizeof(struct hsthread_trans))) {
		print_error(EWARNING,"发送数据失败");
		return FALSE;
	}
	return TRUE;
}

/* 接收armserver发送过来的信息 */
boolean hsthread_recv(struct hsthread_struct *hsthread)
{
	if (!hsthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	return hnet_recv(hsthread->socket.tcp, &hsthread->trans, 
			sizeof(struct hsthread_trans));
}

