/*
 * linuxarms/hostclient/
 */
#include "hfthread.h"
#include "hnet.h"
#include "fileview.h"
#include "filetrans.h"
#include "error.h"

/*
 * 初始化文件浏览和文件传输控制主数据结构
 */
boolean hfthread_init(struct hfthread_struct *hfthread,
                      struct hfview_struct *hfview,
                      struct htthread_struct *hftrans,
                      struct hfthread_trans *trans,
                      struct hnet_struct *socket)
{
	            
        if (!hfthread) {
                print_error(ESYSERR,"错误");
                return FALSE;
        }   
        hfthread->send = hfthread_send;

}
/*
 * 发送一个请求到armserver上
 */
boolean hfthread_send(struct hfthread_struct *hfthread)
{
        if (!hfthread)
                send(hfthread->socket.tcp, &hfthread->trans,
                     sizeof(struct hfthread_trans), 0); 
        return TRUE;
}
/*
 * 文件浏览和我呢件传输主线程执行体
 * @p:  struct hfthread_struct
 */
gboolean hfthread_thread(void *p)
{
	struct hfthread_struct *hfthread =
	(struct hfthread_struct *)p;
	if (!create_tcp_client(&hfthread->socket)) {
		print_error(ESYSERR,"建立文件浏览和文件传输网络连接错误");
		return FALSE;
	}

	while (TRUE) {
		recv(hfthread->socket.tcp, &hfthread->trans,
		     sizeof(struct hfthread_trans), 0);
		switch (hfthread->trans.trans) {
		case FUP:
			break;
		case FDOWN:
			break;
		case FVIEW:

			break;
		default:
			break;
		}
	}
	return TRUE;
}

