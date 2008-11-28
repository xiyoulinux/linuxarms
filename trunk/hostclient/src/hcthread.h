/*
 * linuxarms/hostclient/src/hcthread.c
 * chen Jifeng<chengov@gmail.com>
 */
#ifndef _HCTHREAD_H
#define _HCTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
#include "hnet.h"

#define TRAS_SIZE 512
/* 界面结构体 */
struct hcthread_widget {
	GtkWidget *textview_ctrl;
	GtkWidget *entry_input;
	GtkWidget *label_path;
};

/*  
 * hcthread_trans 实时控制时传送的数据
 * @trans:     控制协议
 * @buffer:    传输的数据
 */
struct hcthread_trans {
	protocol_cthread trans;
	char buffer[TRAS_SIZE];
};
/*  
 *hcthread_strucnt 实时控制线程主数据结构 
 *@widget:   界面结构体
 *@socket:   建立的网络连接 
 *@trans:    传输数据
  */
struct hcthread_struct {
	struct hcthread_widget widget;
	struct hnet_struct socket;
	struct hcthread_trans trans;

	boolean (*send)(struct hcthread_struct *hcthread);
	boolean (*recv)(struct hcthread_struct *hcthread);
	boolean (*showinfo)(struct hcthread_struct *hcthread);
};
boolean hcthread_send(struct hcthread_struct *hcthread);
boolean hcthread_recv(struct hcthread_struct *hcthread);
boolean (*showinfo)(struct hcthread_struct *hcthread);
boolean hcthread_init(struct hcthread_struct *hcthread,
		      struct hcthread_widget *widget,
		      struct hnet_struct *socket,
		      struct hcthread_trans *trans);
gboolean hcthread_thread(void *p);
#endif
