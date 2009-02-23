#ifndef _FILEVIEW_H
#define _FILEVIEW_H

#include "linuxarms.h"
#include "protocol.h"
#include <time.h>
#include <sys/types.h>
#define FILE_NAME_LEN 256
#define FILE_USER_LEN 20

enum {
	TYPE_DIR = 4,
	TYPE_FILE = 8
};
/*
 * afview_send   文件浏览功能中接收的文件信息
 * @procotol:     控制状态
 * @name:     文件名
 * @size:     文件大小
 * @type:     文件类型
 * @mtime:    文件修改时间
 */
struct afview_send {
	protocol_fthread protocol;
	char name[FILE_NAME_LEN];
	off_t size;
	unsigned int type;
	char user[FILE_USER_LEN];
	time_t mtime;	
};

boolean afview_send_init(struct afview_send *fsend);
boolean afview_send_set_protocol(struct afview_send *fsend, protocol_fthread protocol);
boolean afview_send_set_data(struct afview_send *fsend, unsigned int type, 
				const char *name, const char *path);
/*
 * afview_recv 文件浏览功能中接收到的hostlcient的反馈信息
 * @protocol:      接收到的反馈信息
 */
struct afview_recv {
	protocol_fthread protocol;
};

boolean afview_recv_init(struct afview_recv *frecv);
boolean afview_recv_set_protocol(struct afview_recv *frecv, protocol_fthread protocol);
/*
 * ifview_struct   文件浏览控制数据结构
 * @path:     文件路径
 * @fsend:    要发送的文件信息
 * @frecv:    要接收的反馈信息
 * @socket:   TCP连接
 */
struct afview_struct {
	char *path;
	boolean hide;
	struct afview_send fsend;
	struct afview_recv frecv;
	struct anet_struct *socket;
	
	boolean (*set_protocol)(struct afview_struct *afview, protocol_fthread protocol);
	boolean (*send)(struct afview_struct *afview);
	boolean (*recv)(struct afview_struct *afview);
};

boolean afview_init(struct afview_struct *afview, char *path, struct anet_struct *socket);
boolean do_file_view(struct afview_struct *afview);
#endif
