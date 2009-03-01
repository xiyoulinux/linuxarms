#ifndef _HFTHREAD_H
#define _HFTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
#include "anet.h"
#include "atthread.h"
#include "thread.h"
#include "afview.h"
enum {
	OLDNAME,
	NEWNAME
};
/*
 * afthread_trans 文件浏览和文件传输中传送的数据
 * @trans:   控制协议
 * @path:    文件路径
 * @rename:  重命名文件名(rename[0]-旧文件名/rename[1]-新文件名)
 * @hide:    是否显示隐藏文件
 */
struct afthread_trans {
	protocol_fthread protocol;
	char path[PATH_LEN];
	char rename[2][FILE_NAME_LEN];
	boolean hide;
};
boolean afthread_trans_init(struct afthread_trans *aftrans);
boolean afthread_trans_set_protocol(struct afthread_trans *aftrans, protocol_fthread protocol);
const char *afthread_trans_get_path(struct afthread_trans *aftrans);
/*boolean afthread_trans_set_path(struct afthread_trans *aftrans, const char *path);
boolean afthread_trans_set_reanme(struct afthread_trans *aftrans, char *oldname, char *newname);
*/
/*
 * afthread_struct 文件浏览和文件传输控制主数据结构
 * @afview:  文件浏览主数据结构
 * @atthread:  文件传输数据结构
 * @trans:   要传输的数据
 * @socket:  建立的网络连接
 */
struct afthread_struct {
	struct linuxarms_thread thread;
	struct afview_struct *afview;
	struct atthread_struct *atthread;
	struct afthread_trans trans;
	struct anet_struct socket;
	boolean competence;

	boolean (*set_protocol)(struct afthread_struct *afthread, 
				protocol_fthread protocol);
	boolean (*send)(struct afthread_struct *afthread);
	boolean (*recv)(struct afthread_struct *afthread);
};


boolean afthread_init(struct afthread_struct *afthread, 
		      struct afview_struct *afview,
		      struct atthread_struct *atthread);
boolean afthread_thread(void *p);

boolean afthread_rename(struct afthread_struct *afthread);
boolean afthread_delete(struct afthread_struct *afthread);

#endif
