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
#include "hfthread.h"
#include "fview.h"
#include "login.h"
#include "hnet.h"
#include "error.h"
#include "message.h"
#define PROMPT_TIMEOUT (100)
#define PROMPT_STATE (1000 / PROMPT_TIMEOUT * 3)
static int file_trans_prompt = 0;
static int label_show_timeout = 0;
static boolean label_show = FALSE;
static boolean file_trans_error = FALSE;

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
boolean htthread_init(struct htthread_struct *htthread)
{
	LINUXARMS_POINTER(htthread);

	htthread->competence = FALSE;
	htthread->select = FMAX;
	htthread->total_size = 0;
	htthread->trans_size = 0;
	htthread->mode = 0;
	htthread->clock = 0;
	htthread->total_size = 0;
	htthread->trans_size = 0;
	htthread->socket = -1;
	htthread_trans_init(&htthread->trans);

	htthread->set_protocol = htthread_set_protocol;
	htthread->send = htthread_send;
	htthread->recv = htthread_recv;
	return TRUE;
}

static int htthread_send(struct htthread_struct *htthread, int len)
{
	LINUXARMS_POINTER(htthread);
	return send(htthread->socket, htthread->trans.buffer, len, 0);
}

static int htthread_recv(struct htthread_struct *htthread, int len)
{
	LINUXARMS_POINTER(htthread);
	return recv(htthread->socket, htthread->trans.buffer, len, 0);
}
boolean window_trans_timer(gpointer p)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)p;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct htthread_struct *htthread = hfthread->hftrans;
	struct hfview_struct *hfview = hfthread->hfview;
	double val = 0;
	char tval[10];

	if (file_trans_error) {
		char msg[256];
		snprintf(msg, 256, "文件传输错误，可能的原因是在创建\n"
				   "要上传的文件或者打开要下载的\n"
				   "文件: \n%s\n"
				   "时发生错误,可能的原因是没有权限\n"
				   "创建或者给出的路径错误", htthread->path);
		memset(htthread->trans.buffer, (char)FERROR, sizeof(htthread->trans.buffer));
		htthread->send(htthread, sizeof(htthread->trans.buffer));
		htthread->trans_size = htthread->total_size = 0;
		message_box_error(htthread->widget.window_main, msg);
		file_trans_error = FALSE;
		gtk_label_set_text(GTK_LABEL(htthread->widget.label_trans),"");
		return FALSE;
	}
	if (htthread->total_size <= htthread->trans_size) {
		gtk_timeout_remove(htthread->clock);
		char tmp[512];
		GtkTooltipsData *tiptext = gtk_tooltips_data_get(htthread->widget.progressbar);

		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(htthread->widget.progressbar), "100%");
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(htthread->widget.progressbar), 1.0);
		if (htthread->select == FUP) {
			gtk_label_set_text(GTK_LABEL(htthread->widget.label_trans),"文件上传成功");
			snprintf(tmp, 512, "上传文件成功\n%s", tiptext->tip_text);
		} else {
			gtk_label_set_text(GTK_LABEL(htthread->widget.label_trans),"文件下载成功");
			snprintf(tmp, 512, "下载文件成功\n%s", tiptext->tip_text);
		}
		label_show = FALSE;
		file_trans_prompt = 0;
		htthread->trans_size = htthread->total_size = 0;
		label_show_timeout = 0;
		message_box_info(htthread->widget.window_main, tmp);
		if (htthread->select == FDOWN) {
			gtk_widget_set_sensitive(hfview->widget.hide, TRUE);
			hfthread_trans_set_path(&hfthread->trans, hfview_get_path(hfview));
			hfthread->set_protocol(hfthread, FVIEW);
			hfthread->send(hfthread);
		}
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(htthread->widget.progressbar), " ");
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(htthread->widget.progressbar), 0.0);
		gtk_label_set_text(GTK_LABEL(htthread->widget.label_trans)," ");
		gtk_tooltips_set_tip(htthread->widget.tooltips,
				htthread->widget.progressbar, " ", NULL);
		debug_where();
		return FALSE;
	}
	label_show_timeout++;
	if (label_show_timeout == 5) {
		if (label_show) {
			gtk_label_set_text(GTK_LABEL(htthread->widget.label_trans),"");
			label_show = FALSE;
		} else {
			gtk_label_set_text(GTK_LABEL(htthread->widget.label_trans),"文件传输中");
			label_show = TRUE;
		}
		label_show_timeout = 0;
	}
	val = (double)htthread->trans_size / (double)htthread->total_size;
	snprintf(tval, 10, "%.2f%s", val * 100, "%");
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(htthread->widget.progressbar), tval);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(htthread->widget.progressbar), val);
	return TRUE;
}
void *htthread_upload(void *p)
{
	struct htthread_struct *htthread = (struct htthread_struct *)p;
	int up;
	int len;
	long long plen;
	char *buffer = htthread->trans.buffer;
		
	if ((htthread->socket = wait_hfthread_connect()) == -1) {
		print_error(EWARNING, "connect file transmit server error\n");
		file_trans_error = TRUE;
		return NULL;
	}
	debug_where();
	debug_print("file size %ld\n", htthread->total_size);
	if ((up = open(htthread->path, O_WRONLY | O_CREAT, htthread->mode)) == -1) {
		print_error(EWARNING, "create file \"%s\" error\n", htthread->path);
		file_trans_error = TRUE;
		close(htthread->socket);
		return NULL;
	}

	buffer = htthread->trans.buffer;
	plen = htthread->total_size;
	do {
		len = htthread->recv(htthread, HTTHREAD_TRANS_SIZE);
		plen = plen - len;
		if (plen < 0) {
			plen = plen + len;
			if(write(up, buffer, plen) != plen)
				goto out;
				len = 0;
				break;
		}
		if(write(up, buffer, len) != len)
			goto out;
		htthread->trans_size += len;
	} while (len);
	close(up);
	close(htthread->socket);
	htthread->trans_size = htthread->total_size;
	debug_print("从文件传输返回\n");
	return NULL;
out:
	close(up);
	close(htthread->socket);
	unlink(htthread->path);
	print_error(EWARNING, "upload file \"%s\" error\n", htthread->path);
	file_trans_error = TRUE;
	debug_where();
	return NULL;
}
void *htthread_download(void *p)
{
	struct htthread_struct *htthread = (struct htthread_struct *)p;
	int down, len;
	char *buffer;

	if ((htthread->socket = wait_hfthread_connect()) == -1) {
		print_error(EWARNING, "connect file transmit server error\n");
		file_trans_error = TRUE;
		return NULL;
	}
	if ((down = open(htthread->path, O_RDONLY)) == -1) {
		file_trans_error = TRUE;
		close(htthread->socket);
		return NULL;
	}

	buffer = htthread->trans.buffer;
	do {
		len = read(down, buffer , HTTHREAD_TRANS_SIZE);
		htthread->trans_size += len;
		buffer[len] = '\0';
		if(htthread->send(htthread, len) != len)
			break;
	} while (len);
	close(down);
	close(htthread->socket);
	debug_where();
	return NULL;
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
