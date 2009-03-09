/*
 * linuxarms/hostclient/src/hcthread.h
 * chen Jifeng<chengov@gmail.com>
 */
#ifndef _HCTHREAD_H
#define _HCTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
#include "hnet.h"
#include "thread.h"
#include <gtk/gtk.h>
#define TRANS_SIZE 512
#define GTK_BUFFER_MAX_LINE 100
/* 界面结构体 */
struct hcthread_widget {
	GtkWidget *textview_ctrl;
	GtkWidget *entry_input;
	GtkWidget *label_path;
	GtkAdjustment *adjustment;
};

/*  
 * hcthread_trans 实时控制时传送的数据
 * @protocol:     控制协议
 * @buffer:    传输的数据
 */
struct hcthread_trans {
	protocol_cthread protocol;
	char buffer[TRANS_SIZE];
};
boolean hcthread_trans_init(struct hcthread_trans *trans);
boolean hcthread_trans_set_protocol(struct hcthread_trans *trans,protocol_cthread protocol);
boolean hcthread_trans_set_buf(struct hcthread_trans *trans, const char *buf);
/*  
 *hcthread_strucnt 实时控制线程主数据结构 
 *@widget:   界面结构体
 *@socket:   建立的网络连接 
 *@trans:    传输数据
 */
struct hcthread_struct {
	struct linuxarms_thread thread;
	struct hcthread_widget widget;
	struct hnet_struct socket;
	struct hcthread_trans trans;
	boolean competence;  /* 超级用户权限标志 */

	boolean (*send)(struct hcthread_struct *hcthread);
	boolean (*recv)(struct hcthread_struct *hcthread);
};
boolean hcthread_init(struct hcthread_struct *hcthread);
boolean showinfo(struct hcthread_struct *hcthread);
void *hcthread_thread(void *p);
#endif
