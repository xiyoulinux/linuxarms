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
/* 界面结构体 */
struct hcthread_widget {
	GtkWidget *textview_ctrl;
	GtkWidget *entry_input;
	GtkWidget *label_path;
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
/*  
 *hcthread_strucnt 实时控制线程主数据结构 
 *@widget:   界面结构体
 *@socket:   建立的网络连接 
 *@trans:    传输数据
  */
struct hcthread_struct {
	linuxarms_thread_t *thread;
	struct hcthread_widget widget;
	struct hnet_struct socket;
	struct hcthread_trans trans;
	boolean competence;  /* 超级用户权限标志 */

	boolean (*send)(struct hcthread_struct *hcthread);
	boolean (*recv)(struct hcthread_struct *hcthread);
	boolean (*showinfo)(struct hcthread_struct *hcthread);
};
boolean hcthread_send(struct hcthread_struct *hcthread);
boolean hcthread_recv(struct hcthread_struct *hcthread);
boolean showinfo(struct hcthread_struct *hcthread);
boolean hcthread_init(struct hcthread_struct *hcthread);
gboolean hcthread_thread(void *p);
#endif
