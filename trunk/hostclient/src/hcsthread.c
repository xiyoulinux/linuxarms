/*
 * linuxarms/hostclient/src/hcsthread.c
 * Niu Tao <niutao0602@gmail.com>
 */
#include "hsthread.h"
#include "hnet.h"
struct hsthread_struct {
	struct hssinfo_struct *ssinfo;
	struct hsprocess_struct *sprocess;
	struct hsthread_trans trans;
	struct hnet_struct socket;
	boolean lock;
	struct hsthread_timer timer;
/*
 * 初始化hsthread_struct结构
 */
boolean hsthread_init(struct hsthread_struct *hsthread,
				   struct hssinfo_struct *ssinfo.
				   struct hsprocess_struct *hsprocess,
				   struct hsthread_trans *trans,
				   struct hnet_struct *socket,
				   struct hsthread_timer *timer)
{
}
/*
 * 线程主执行体
 */
gboolean hsthread_thread(void *p)
{
	struct hsthread_struct *hsthread =
		(struct hsthread_struct *)p;
	hnet_init(&hsthread->socket,"192.168.200.79",34);
	if (!create_tcp_client(&hsthread->socket)) {
		print_error(ESYSERR,"create tcp error");
		return FALSE;
	}
	while (TRUE) {
		recv(hsthread->socket.tcp,&hsthread->trans,sizeof(struct hsthread_trans),0);
		switch (hsthread->trans.ctrl) {
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

gboolean hsthread_timer(gpointer data)
{
	struct hsthread_struct *hsthread =
		(struct hsthread_struct *)data;
	if (!hsthread) {
		print_error(WARNING,"启动定时器失败，无法更新时间");
		return FALSE;
	}
	switch (hsthread->trans->ctrl) {
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

boolean hsthread_send(struct hsthread_struct *hsthread)
{
	if (!hsthread) {
		print_error(WARNING,"不能发送数据");
		return FALSE;
	}
	if (send(hsthread->socket->tcp, &hsthread->trans, 
		 sizeof(struct hsthread_trans),0) == -1) {
		print_error(WARNING,"发送数据失败");
		return FALSE;
	}
	return TRUE;

}
