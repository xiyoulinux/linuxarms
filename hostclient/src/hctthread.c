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
	htthread_trans_init(&htthread->trans);

	htthread->set_protocol = htthread_set_protocol;
	htthread->send = htthread_send;
	htthread->recv = htthread_recv;
	return TRUE;
}

static int htthread_send(struct htthread_struct *htthread, int len)
{
	LINUXARMS_POINTER(htthread);
	return send(htthread->socket->tcp, htthread->trans.buffer, len, 0);
}

static int htthread_recv(struct htthread_struct *htthread, int len)
{
	LINUXARMS_POINTER(htthread);
	return recv(htthread->socket->tcp, htthread->trans.buffer, len, 0);
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
			snprintf(tmp, 512, "上传下载成功\n%s", tiptext->tip_text);
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
boolean htthread_upload(struct htthread_struct *htthread)
{
	LINUXARMS_POINTER(htthread);
	int up;
	int len;
	char *p;
	protocol_fthread protocol;
	long long plen;
		
	debug_where();
	debug_print("file size %ld\n", htthread->total_size);
	if ((up = open(htthread->path, O_WRONLY | O_CREAT, htthread->mode)) == -1) {
		file_trans_error = TRUE;
		return FALSE;
	}
	p = htthread->trans.buffer + HTTHREAD_PROTOCOL;

	plen = htthread->total_size;
	do {
		len = htthread->recv(htthread, sizeof(htthread->trans.buffer));
		protocol = (protocol_fthread)htthread->trans.buffer[0];
		switch (protocol) {
		case FSEND:
			plen = plen - len + 1;
			if (plen < 0) {
				plen = plen + len - 1;
				write(up, p, plen);
				len = 0;
				break;
			}
			write(up, p, len - 1);
			htthread->trans_size += (len -1);
			break;
		case FSENDALL:
			debug_print("文件传输结束\n");
			len = 0;
			break;
		case FERROR:
		default:
			linuxarms_print("上传文件错误\n");
			goto out;
		}
	} while (len);
	close(up);
	htthread->trans_size = htthread->total_size;
	debug_print("从文件传输返回\n");
	return TRUE;
out:
	unlink(htthread->path);
	htthread->trans_size = htthread->total_size;
	debug_where();
	return TRUE;
}
boolean htthread_download(struct htthread_struct *htthread)
{
	LINUXARMS_POINTER(htthread);
	int down, len;
	char *p;
	fd_set rfd_set, wfd_set;
	int flag, ret;
	struct timeval timeout;

	if ((down = open(htthread->path, O_RDONLY)) == -1) {
		file_trans_error = TRUE;
		return FALSE;
	}

	p = htthread->trans.buffer + HTTHREAD_PROTOCOL;
	debug_where();
	fcntl(htthread->socket->tcp,F_SETFD,O_NONBLOCK);
	do {
		FD_ZERO(&rfd_set);
		FD_ZERO(&wfd_set);
		FD_SET(0, &rfd_set);
		FD_SET(0, &wfd_set);
		FD_SET(htthread->socket->tcp, &rfd_set);
		FD_SET(htthread->socket->tcp, &wfd_set);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;		
			
		len = read(down, p , HTTHREAD_TRANS_SIZE);
		htthread->trans_size += len;
		p[len] = '\0';
		htthread->trans.buffer[0] = (char)FSEND;
		htthread->send(htthread, len + HTTHREAD_PROTOCOL);
		
		if((ret = select(htthread->socket->tcp + 1, &rfd_set, &wfd_set, NULL, &timeout)) == 0)
			continue;
		if(FD_ISSET(0,&rfd_set)) {
			htthread->recv(htthread, sizeof (htthread->trans.buffer));
			if (htthread->trans.buffer[0] == FERROR)
				goto out;
		}
	} while (len);
	memset(htthread->trans.buffer, (char)FSENDALL,sizeof(htthread->trans.buffer));
	htthread->send(htthread, sizeof(htthread->trans.buffer));
out:
	flag = fcntl(htthread->socket->tcp, F_GETFD, 0);
	flag &= ~O_NONBLOCK;
	fcntl(htthread->socket->tcp,F_SETFD,flag);	
	close(down);
	debug_where();
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
