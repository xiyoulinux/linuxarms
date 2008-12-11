/*
 * linuxarms/hostclient/
 */
#include <string.h>
#include "hfthread.h"
#include "hnet.h"
#include "fileview.h"
#include "filetrans.h"
#include "error.h"
#include "config.h"
#include "login.h"

/*
 * 初始化文件浏览和文件传输控制主数据结构
 */
boolean hfthread_init(struct hfthread_struct *hfthread,
                      struct hfview_struct *hfview,
                      struct htthread_struct *hftrans,
                      struct hfthread_trans *trans,
                      struct hnet_struct *socket)
{
	            
        if (!hfthread || !hfview || hftrans ||
	    !trans || !socket) {
		debug_where();
                print_error(ESYSERR,"无效参数");
                return FALSE;
        }
	hfthread->hfview  =hfview;
	hfthread->hftrans = hftrans;
	hfthread->trans = *trans;
	hfthread->socket = *socket;

        hfthread->send = hfthread_send;
	hfthread->recv = hfthread_recv;

	return TRUE;
}
/*
 * 文件浏览和文件传输主线程执行体
 * @p:  struct hfthread_struct
 */
gboolean hfthread_thread(void *p)
{
	struct hfthread_struct *hfthread =
		(struct hfthread_struct *)p;

	hnet_init(&hfthread->socket, get_armserver_ip, get_fthread_port());
	if (!create_tcp_client(&hfthread->socket)) {
		print_error(ESYSERR,"建立文件浏览和文件传输网络连接错误");
		return FALSE;
	}

	while (TRUE) {
		hfthread->recv(hfthread);
		switch (hfthread->trans.protocol) {
		case FUP: /* 上传文件处理 */
			break;
		case FDOWN: /* 下载文件处理 */
			break;
		case FVIEW: /* 文件浏览处理 */
			break;
		case FRENAME: /* 重命名文件 */
			break;
		case FDELETE: /* 删除文件 */
			break;
		default:
			break;
		}
	}
	return TRUE;
}
/*
 * 设置传输结构体
 */
boolean hfthread_set_trans(struct hfthread_struct *hfthread, 
			  protocol_fthread protocol,
			  char *path, char *oldname, char *newname)
{
	if (!hfthread || !path || !oldname || !newname) {
		debug_where();
		printf(EWARNING, "无效的参数");
		goto out;
	}
	if (!PROTOCOL_IS_FTHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		goto out;
	}
	hfthread->trans.protocol = protocol;
	strncpy(hfthread->trans.path, path , PATH_LEN);
	strncpy(hfthread->trans.rename[0], oldname, FNAME_LEN);
	strncpy(hfthread->trans.rename[1], newname, FNAME_LEN);
	return TRUE;
out:
	return FALSE;
}
/*
 * 发送数据到armserver上
 */
boolean hfthread_send(struct hfthread_struct *hfthread)
{
	if (!hfthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	return hnet_send(hfthread->socket.tcp, (void *)&hfthread->trans,
			 sizeof(struct hfthread_struct));
}
/*
 * 接收armserver发送过来的数据
 */
boolean hfthread_recv(struct hfthread_struct *hfthread)
{
	if (!hfthread) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	return hnet_recv(hfthread->socket.tcp, (void *)&hfthread->trans,
			 sizeof(struct hfthread_struct));

}



