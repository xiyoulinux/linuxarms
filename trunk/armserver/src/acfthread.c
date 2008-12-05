#include "afthread.h"
#include "anet.h"
#include "afview.h"
#include "atthread.h"
#include "debug.h"
#include "error.h"
#include "config.h"
#include <unistd.h>

/*
 * 初始化文件浏览和文件传输控制主数据结构
 */
boolean afthread_init(struct afthread_struct *afthread,
                      struct afview_struct *afview,
                      struct atthread_struct *aftrans,
                      struct afthread_trans *trans,
                      struct anet_struct *socket)
{
	            
        if (!afthread || !afview || !aftrans ||
	    !trans || !socket) {
		debug_where();
                print_error(ESYSERR,"错误");
                return FALSE;
        }   
	afthread->afview = afview;
	afthread->aftrans = aftrans;
	afthread->trans = *trans;
	afthread->socket = *socket;

        afthread->send = afthread_send;
	afthread->recv = afthread_recv;
	return TRUE;

}
/*
 * 文件浏览和文件传输主线程执行体
 * @p:  struct afthread_struct
 */
boolean afthread_thread(void *p)
{
	struct afthread_struct *afthread =
		(struct afthread_struct *)p;

	anet_init(&afthread->socket, get_host_ip(),get_fthread_port());
	if (!create_tcp_server(&afthread->socket)) {
		print_error(ESYSERR,"建立文件浏览和文件传输网络连接错误");
		return FALSE;
	}

	while (TRUE) {
		afthread->recv(afthread);
		switch (afthread->trans.protocol) {
		case FUP:
			break;
		case FDOWN:
			break;
		case FVIEW:
			break;
		case FRENAME:
			break;
		case FDELETE:
			break;
		default:
			break;
		}
	}
	return TRUE;
}

boolean afthread_send(struct afthread_struct *afthread)
{
	if (!afthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	return anet_send(afthread->socket.tcp, (void *)&afthread->trans,
			 sizeof(struct afthread_trans));
}
boolean afthread_recv(struct afthread_struct *afthread)
{
	if (!afthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	return anet_recv(afthread->socket.tcp, (void *)&afthread->trans,
			 sizeof(struct afthread_trans));
}
