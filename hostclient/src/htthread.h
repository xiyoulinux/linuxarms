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

#define HTTHREAD_PATH_LEN 80
#define TRAS_SIZE 512

/*
 * htthread_mwidget	contain some GtkWidget which htthread will use
 */
struct htthread_widget {
	GtkWidget *menubar_upload;
	GtkWidget *menubar_download;
	GtkWidget *toolbar_upload;
	GtkWidget *toolbar_download;
};

/*
 * @htthread_trans     文件传输的数据
 * @trans:		    控制协议
 * @buffer:		  传输的数据
 */
struct htthread_trans {
	protocol_fthread protocol;
	char buffer[TRAS_SIZE];
};

/*
 * @file_size:       请求传输文件的大小
 * @savefile_size:   已经传输文件的大小
 */
struct htthread_struct {
	struct linuxarms_thread thread;
	protocol_fthread select;
	long long file_size;
	long long savefile_size;
	struct user_struct user;
	struct hnet_struct socket;
	struct htthread_widget widget;
	struct htthread_trans trans;
	char download_path[HTTHREAD_PATH_LEN];
	char upload_path[HTTHREAD_PATH_LEN];

	boolean (*get_download_file)(struct htthread_struct *htthread,
													char *path);
	boolean(*get_upload_file)(struct htthread_struct *htthread,
													char *path);
	boolean(*upload)(struct htthread_struct *htthread, 
				struct htthread_trans *trans);
	boolean(*download)(struct htthread_struct *htthread, 
				struct htthread_trans *trans);
	boolean (*send)(struct htthread_struct *htthread);
	boolean (*recv)(struct htthread_struct *htthread);
};

/* 初始化htthread_struct结构体 */
int htthread_init(struct htthread_struct *htthread,
	             struct user_struct *user,
		     struct hnet_struct *socket,
		     struct htthread_widget *widget);

/* determine trans percent */
int trans_percent(struct htthread_struct *htthread);


/* 线程执行体 */
gboolean htthread_thread(void *p);

#endif
