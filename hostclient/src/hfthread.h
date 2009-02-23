#ifndef _HFTHREAD_H
#define _HFTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
#include "hnet.h"
#include "thread.h"
#include "debug.h"
#include "error.h"
#include <gtk/gtk.h>
#define PATH_LEN 512
#define FNAME_LEN 256
enum {
	OLDNAME,
	NEWNAME
};
/*
 * hfthread_trans 文件浏览和文件传输中传送的数据
 * @trans:   控制协议
 * @path:    文件路径
 * @rename:  重命名文件名(rename[0]-旧文件名/rename[1]-新文件名)
 * @hide:    是否显示隐藏文件(TRUE-不显示，FALSE-显示。默认不显示)
 */
struct hfthread_trans {
	protocol_fthread protocol;
	char path[PATH_LEN];
	char rename[2][FNAME_LEN];
	boolean hide;
};

boolean hfthread_trans_init(struct hfthread_trans *hftrans);
boolean hfthread_trans_set_protocol(struct hfthread_trans *hftrans, protocol_fthread protocol);
boolean hfthread_trans_set_path(struct hfthread_trans *hftrans, const char *path);
const char *hfthread_trans_get_path(struct hfthread_trans *hftrans);
boolean hfthread_trans_set_rename(struct hfthread_trans *hftrans, char *oldname, char *newname);

struct hfthread_widget {
	GtkWidget *window;
	GtkWidget *oldname;
	GtkWidget *newname;
};
struct htthread_struct;
struct hfviw_struct;
struct hnet_struct;
/*
 * hfthread_struct 文件浏览和文件传输控制主数据结构
 * @hfview:  文件浏览主数据结构
 * @ftrans:  问家你传输数据结构
 * @trans:   要传输的数据
 * @socket:  建立的网络连接
 */
struct hfthread_struct {
	struct linuxarms_thread thread;
	struct hfview_struct *hfview;
	struct htthread_struct *hftrans;
	struct hfthread_trans trans;
	struct hnet_struct socket;
	struct hfthread_widget widget;
	boolean competence;
	
	void (*down_lock)(struct hfthread_struct *hfthread);
	void (*up_lock)(struct hfthread_struct *hfthread);
	boolean (*set_protocol)(struct hfthread_struct *hfthread, 
			protocol_fthread protocol);
	boolean (*send)(struct hfthread_struct *hfthread);
	boolean (*recv)(struct hfthread_struct *hfthread);
};

boolean hfthread_init(struct hfthread_struct *hfthread, 
		      struct hfview_struct *hfview,
		      struct htthread_struct *hftrans);
boolean hfthread_set_trans(struct hfthread_struct *hfthread, 
			   protocol_fthread protocol,
			   char *path, char *oldname, char *newname);
gboolean hfthread_thread(void *p);
#endif
