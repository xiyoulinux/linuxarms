/*
 * linuxarms/hostclient/src/htthread.h
 * Copyright (C) 2008 wugang<xuanfeng1104@gmail.com>
 *
 * The file contains the private data stucture and API definitions
 */
#ifndef _HTTHREAD_H
#define _HTTHREAD_H
#include "linuxarms.h"
#include <gtk/gtk.h>
#include "protocol.h"
#include "hnet.h"
#include "login.h"
#include "thread.h"

#define HTTHREAD_TRANS_SIZE 4096

/*
 * htthread_mwidget	contain some GtkWidget which htthread will use
 */
struct htthread_widget {
	GtkWidget *menubar_upload;
	GtkWidget *menubar_download;
	GtkWidget *toolbar_upload;
	GtkWidget *toolbar_download;
	GtkWidget *choose;
	GtkWidget *progressbar;
	GtkWidget *label_trans;
	GtkTooltips *tooltips;
};

/*
 * @htthread_trans     文件传输的数据
 * @trans:		    控制协议
 * @buffer:		  传输的数据
 */
struct htthread_trans {
	protocol_fthread protocol;
	char buffer[HTTHREAD_TRANS_SIZE + 1];
};
boolean htthread_trans_init(struct htthread_trans *httrans);
boolean htthread_trans_set_protocol(struct htthread_trans *httrans, protocol_fthread protocol);
const char *htthread_trans_get_buf(struct htthread_trans *httrans);
boolean htthread_trans_set_buf(struct htthread_trans *httrans, const char *buf);
/*
 * @select:          下载文件/上传文件
 * @file_size:       请求传输文件的大小
 * @savefile_size:   已经传输文件的大小
 */
struct htthread_struct {
	mode_t mode;
	struct hnet_struct *socket;
	struct htthread_widget widget;
	struct htthread_trans trans;
	char path[PATH_LEN];
	boolean competence;
	protocol_fthread select;
	off_t total_size;
	off_t trans_size;
	unsigned int clock;
	boolean quit;

	boolean (*set_protocol)(struct htthread_struct *htthread, protocol_fthread protocol);
	int (*send)(struct htthread_struct *htthread, int len);
	int (*recv)(struct htthread_struct *htthread, int len);
};

/* 线程执行体 */
//boolean htthread_thread(void *p);
boolean htthread_init(struct htthread_struct *htthread, struct hnet_struct *socket);
boolean htthread_upload(struct htthread_struct *htthread);
boolean htthread_download(struct htthread_struct *htthread);
#endif
