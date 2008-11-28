#ifndef _FILEVIEW_H
#define _FILEVIEW_H

#include "linuxarms.h"
#include <time.h>
#define FILE_NAME_LEN 256
/*
 * afview_send   文件浏览功能中接收的文件信息
 * @ctrl:     控制状态
 * @name:     文件名
 * @size:     文件大小
 * @type:     文件类型
 * @mtime:    文件修改时间
 */
struct afview_send {
	protocol_fthread ctrl;
	char name[FILE_NAME_LEN];
	long long size;
	unsigned int type;
	time_t mtime;

	
};
/*
 * afview_recv 文件浏览功能中接收到的hostlcient的反馈信息
 * @ctrl:      接收到的反馈信息
 */
struct afview_recv {
	protocol_fthread ctrl;
};
/*
 * afview_struct   文件浏览控制数据结构
 * @path:     文件路径
 * @fsend:    要发送的文件信息
 * @frecv:    要接收的反馈信息
 * @socket:   TCP连接
 */
struct afview_struct {
	char *path;
	struct afview_send fsend;
	struct afview_recv frecv;
	struct anet_struct *socket;

	boolean (*recv)(struct afview_struct *afview);
	boolean (*send)(struct afview_struct *afview);
};

static inline boolean afview_send(struct afview_struct *afview)
{
	if (!afview)
		return FALSE;
	return anet_send(afview->socket->tcp,
			 (void *)&afview->fsend,
			 sizeof(struct afview_send));
}
static inline boolean afview_recv(struct afview_struct *afview)
{
	if (!afview)
		return FALSE;
	return anet_recv(afview->socket->tcp,
			 (void *)&afview->frecv,
			 sizeof(struct afview_recv));
}

#endif
