#include "afthread.h"
#include "anet.h"
#include "fileview.h"
#include "filetrans.h"

/*
 * 初始化文件浏览和文件传输控制主数据结构
 */
boolean afthread_init(struct afthread_struct *afthread,
                      struct afview_struct *afview,
                      struct aftrans_struct *aftrans,
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
                send(afthread->socket->tcp, &afthread->trans,
                     sizeof(struct afthread_trans), 0); 
        return TRUE;
}
/*
 * 文件浏览和文件传输主线程执行体
 * @p:  struct afthread_struct
 */
gboolean afthread_thread(void *p)
{
	struct afthread_struct *afthread =
	(struct afthread_struct *)p;
	if (!create_tcp_client(afthread->socket)) {
		print_error(ESYSERR,"建立文件浏览和文件传输网络连接错误");
		return FALSE;
	}

	while (TRUE) {
		recv(afthread->socket->tcp, &afthread->trans,
		     sizeof(struct afthread_trans), 0);
		switch (afthread->trans.trans) {
		case UP:
			break;
		case DOWN:
			break;
		case VIEW:
			break;
		case RENAME:
			break;
		case DELETE:
			break;
		default:
			break;
		}
	}
	return TRUE;
}

