/*
 * linuxarms/hostclient/src/hctthread.c
 * 
 * Copyright (C) 2008 wugang<xuanfeng1104@gmail.com>
 */
#define __DEBUG__
#include <string.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
       #include <sys/types.h>
       #include <sys/socket.h>

#include <stdio.h>
#include "htthread.h"
#include "linuxarms.h"
#include "protocol.h"
#include "login.h"
#include "hnet.h"
#include "error.h"
#include "message.h"

static boolean htthread_set_protocol(struct htthread_struct *httread, protocol_fthread protocol);
static int htthread_send(struct htthread_struct *htthread, int len);
static int htthread_recv(struct htthread_struct *htthread, int len);

/*
 * init_htthread	initialize htthread_struct structure. 
 * @htthread:	the structure which will be initialized.
 * @user:	the user who use hostclient
 * @scoket:		tcp socket structure
 * @widget:	GtkWidget structure
 */
boolean htthread_init(struct htthread_struct *htthread,
		      struct hnet_struct *socket)
{
	LINUXARMS_POINTER(htthread);
	LINUXARMS_POINTER(socket);

	htthread->competence = FALSE;
	htthread->select = FMAX;
	htthread->total_size = 0;
	htthread->trans_size = 0;
	htthread->mode = 0;
	htthread->clock = 0;
	htthread->total_size = 0;
	htthread->trans_size = 0;
	htthread->socket = socket;
	htthread->quit = TRUE;
	htthread_trans_init(&htthread->trans);

	htthread->set_protocol = htthread_set_protocol;
	htthread->send = htthread_send;
	htthread->recv = htthread_recv;
	return TRUE;
}

static int htthread_send(struct htthread_struct *htthread, int len)
{
	LINUXARMS_POINTER(htthread);
	//return hnet_send(htthread->socket->tcp, (void *)&htthread->trans, 
	//		 sizeof(struct htthread_trans));
	return send(htthread->socket->tcp, htthread->trans.buffer, len, 0);
}

static int htthread_recv(struct htthread_struct *htthread, int len)
{
	LINUXARMS_POINTER(htthread);
	//return hnet_recv(htthread->socket->tcp, (void *)&htthread->trans, 
	//		 sizeof(struct htthread_trans));
	return recv(htthread->socket->tcp, htthread->trans.buffer, len, 0);
}
static boolean window_trans_timer(gpointer p)
{
	struct htthread_struct *htthread = (struct htthread_struct *)p;
	double val = 0;
	if (htthread->total_size <= htthread->trans_size) {
		htthread->trans_size = htthread->total_size = 0;
		gtk_widget_destroy(htthread->widget.window_trans);
		return FALSE;
	}
	val = (double)htthread->trans_size / (double)htthread->total_size;
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(htthread->widget.progressbar), val);
	return TRUE;
}
boolean htthread_upload(struct htthread_struct *htthread)
{
	LINUXARMS_POINTER(htthread);
	int up;
	char msg[256];
	int i =0;
	int len;
	debug_where();
	debug_print("path: %s %x\n", htthread->path, htthread->mode);
	if ((up = open(htthread->path, O_WRONLY | O_CREAT, htthread->mode)) == -1) {
		snprintf(msg, 256, "在创建要上传的文件: \n%s\n"
				   "时发生错误,可能的原因是没有权限\n"
				   "创建或者给出的路径错误", htthread->path);
		create_window_dialog(msg);
		htthread->send(htthread, 0);
		return FALSE;
	}
	htthread->clock = gtk_timeout_add(100, window_trans_timer, (gpointer)htthread);
	do {
		len = htthread->recv(htthread, HTTHREAD_TRANS_SIZE);
		if (len <= 0)
			break;
		htthread->trans.buffer[len] = '\0';
		write(up, htthread->trans.buffer, len);
		htthread->trans_size += len;
	} while (TRUE);
	close(up);
	htthread->trans_size = htthread->total_size;
	debug_where();
	return TRUE;
	/*
	do {
		if (!htthread->recv(htthread)) {
			print_error(EWARNING, "文件上传中，接收文件信息错误");
			goto out;
		}
		switch (htthread->trans.protocol) {
		case FSEND:
			htthread->trans_size += write(up, htthread->trans.buffer, 
						strlen(htthread->trans.buffer));
			debug_print("protocol : %d\n", htthread->trans.protocol);
			break;
		case FSENDALL:
			debug_print("protocol : %d\n", htthread->trans.protocol);
			debug_print("i = %d\n", i);
			goto sendall;
		default:
			debug_print("default : %d\n", htthread->trans.protocol);
			debug_print("i = %d\n", i);
			goto out;
		}
		htthread->set_protocol(htthread, FRECV);
		htthread->send(htthread);
		i++;
		debug_print("%d\n", i);
	} while (TRUE);
sendall:
	close(up);
	debug_where();

	htthread->total_size = htthread->trans_size = 0;
	return TRUE;
out:
	htthread->total_size = htthread->trans_size = 0;
	debug_where();
	unlink(htthread->path);
	return FALSE;
	*/
}
boolean htthread_download(struct htthread_struct *htthread)
{
	LINUXARMS_POINTER(htthread);
	int down, len;
	char msg[256];

	if ((down = open(htthread->path, O_RDONLY)) == -1) {
		snprintf(msg, 256, "在打开要下载的文件: \n%s\n"
				   "时发生错误,可能的原因是没有权限\n"
				   "打开或者给出的路径错误", htthread->path);
		create_window_dialog(msg);
		return FALSE;
	}
	do {
		len = read(down, htthread->trans.buffer, HTTHREAD_TRANS_SIZE);
		htthread->set_protocol(htthread, FSEND);
		htthread->send(htthread, len);
	} while (len);
	close(down);
	//htthread->set_protocol(htthread, FSENDALL);
	//htthread->send(htthread, 0);
	return TRUE;
}
boolean htthread_trans_init(struct htthread_trans *httrans)
{
	LINUXARMS_POINTER(httrans);
	httrans->protocol = FMAX;
	memset(httrans->buffer, '\0', HTTHREAD_TRANS_SIZE);
	return TRUE;
}
boolean htthread_trans_set_protocol(struct htthread_trans *httrans, protocol_fthread protocol)
{
	LINUXARMS_POINTER(httrans);
	if (!PROTOCOL_IS_FTHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		httrans->protocol = FMAX;
		return FALSE;
	}
	httrans->protocol = protocol;
	return TRUE;
}
const char *htthread_trans_get_buf(struct htthread_trans *httrans)
{
	LINUXARMS_POINTER(httrans);
	return httrans->buffer;
}
static boolean htthread_set_protocol(struct htthread_struct *htthread, protocol_fthread protocol)
{
	LINUXARMS_POINTER(htthread);
	return htthread_trans_set_protocol(&htthread->trans, protocol);
}
