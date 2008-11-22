#ifndef _HFTHREAD_H
#define _HFTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
/*
 * hfthread_trans 文件浏览和文件传输中传送的数据
 * @trans:   控制协议
 * @path:    文件路径
 */
struct hfthread_trans {
	struct protocol_fthread trans;
	char *path;
};
/*
 * hfthread_struct 文件浏览和文件传输控制主数据结构
 * @hfview:  文件浏览主数据结构
 * @ftrans:  问家你传输数据结构
 * @trans:   要传输的数据
 * @socket:  建立的网络连接
 */
struct hfthread_struct {
	struct hfview_struct *hfview;
	struct hftrans *ftrans;
	struct hfthread_trans trans;
	struct hnet_struct socket;

	boolean (*send)(hfthread_struct *hfthread);
};


boolean hfthread_init(struct hfthread_struct *hfthread, 
		      struct hfview_struct *hfview,
		      struct hftrans_struct *hftrans,
		      struct hfthread_trans *trans,
		      struct hnet_struct *socket);
boolean hfthread_send(struct hfthread_struct *hfthread);
gboolean hfthread_thread(void *p);
#endif
