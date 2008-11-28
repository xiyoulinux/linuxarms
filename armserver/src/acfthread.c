#include "afthread.h"
#include "anet.h"
#include "afview.h"
#include "atthread.h"
#include "debug.h"
#include "error.h"

/*
 * 初始化文件浏览和文件传输控制主数据结构
 */
boolean afthread_init(struct afthread_struct *afthread,
                      struct afview_struct *afview,
                      struct atthread_struct *aftrans,
                      struct afthread_trans *trans,
                      struct anet_struct *socket)
{
	            
        if (!afthread) {
                print_error(ESYSERR,"错误");
                return FALSE;
        }   
        afthread->send = afthread_send;

}
/*
 * 发送一个请求到armserver上
 */
boolean afthread_send(struct afthread_struct *afthread)
{
        if (!afthread)
                anet_send(afthread->socket.tcp, &afthread->trans,
                     sizeof(struct afthread_trans)); 
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
	if (!create_tcp_client(&afthread->socket)) {
		print_error(ESYSERR,"建立文件浏览和文件传输网络连接错误");
		return FALSE;
	}

	while (TRUE) {
		anet_recv(afthread->socket.tcp, &afthread->trans,
		     sizeof(struct afthread_trans));
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

