#ifndef _HFTHREAD_H
#define _HFTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
#include "anet.h"
#include "atthread.h"
#define PATH_LEN  512
#define FNAME_LEN 256
/*
 * afthread_trans 文件浏览和文件传输中传送的数据
 * @trans:   控制协议
 * @path:    文件路径
 * @rename:  重命名文件名(rename[0]-旧文件名/rename[1]-新文件名)
 */
struct afthread_trans {
	protocol_fthread protocol;
	char path[PATH_LEN];
	char rename[2][FNAME_LEN];
};
/*
 * afthread_struct 文件浏览和文件传输控制主数据结构
 * @afview:  文件浏览主数据结构
 * @ftrans:  文件传输数据结构
 * @trans:   要传输的数据
 * @socket:  建立的网络连接
 */
struct afthread_struct {
	struct afview_struct *afview;
	struct atthread_struct *aftrans;
	struct afthread_trans trans;
	struct anet_struct socket;

	boolean (*send)(struct afthread_struct *afthread);
	boolean (*recv)(struct afthread_struct *afthread);
};


boolean afthread_init(struct afthread_struct *afthread, 
		      struct afview_struct *afview,
		      struct atthread_struct *aftrans,
		      struct afthread_trans *trans,
		      struct anet_struct *socket);
boolean afthread_send(struct afthread_struct *afthread);
boolean afthread_recv(struct hfthread_struct *afthread);
boolean afthread_thread(void *p);
#endif
