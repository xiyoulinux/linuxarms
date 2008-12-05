/*
 * linuxarms/hostclient/src/hctthread.c
 * 
 * Copyright (C) 2008 wugang<xuanfeng1104@gmail.com>
 */

#include <string.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#include "htthread.h"
#include "linuxarms.h"
#include "protocol.h"
#include "login.h"
#include "hnet.h"
#include "error.h"


 static boolean htthread_get_download_file(struct htthread_struct *htthread, char *path);
 static boolean htthread_get_upload_file(struct htthread_struct *htthread, char *path);
 static boolean htthread_upload(struct htthread_struct *htthread, struct htthread_trans *trans);
 static boolean htthread_download(struct htthread_struct *htthread, struct htthread_trans *trans);
 static boolean htthread_send(struct htthread_struct *htthread);
 static boolean htthread_recv(struct htthread_struct *htthread);


/*
 * init_htthread	initialize htthread_struct structure. 
 * @htthread:	the structure which will be initialized.
 * @user:	the user who use hostclient
 * @scoket:		tcp socket structure
 * @widget:	GtkWidget structure
 */
int htthread_init(struct htthread_struct *htthread,
		     struct user_struct *user,
		     struct hnet_struct *socket,
		     struct htthread_widget *widget)
{
	int ret = 0;
	if (!htthread || !widget) {
		ret = ENOINIT;
		goto out;
	}
	if (!user) {
		ret = ENOINIT;
		goto out;
	}
	if (!socket) {
		ret = ENOSOCKET;
		goto out;
	}
	htthread->select = FMAX;
	htthread->socket = *socket;
	htthread->widget = *widget;
	htthread->user = *user;
	htthread->trans.protocol = FMAX;

	htthread->get_download_file = htthread_get_download_file;
	htthread->get_upload_file = htthread_get_upload_file;
	htthread->upload = htthread_upload;
	htthread->download = htthread_download;
	htthread->send = htthread_send;
	htthread->recv = htthread_recv;
	
out:
	return ret;
}
static boolean htthread_get_download_file(struct htthread_struct *htthread,
								char *path)
{
	if (!htthread || !path)
		return FALSE;
	if (htthread->select == FUP) {
		strcpy(htthread->download_path, path);
		return TRUE;
	} else 
		return FALSE;
}

static boolean htthread_get_upload_file(struct htthread_struct *htthread,
								char *path)
{
	if (!htthread || !path)
		return FALSE;
	if (htthread->select == FDOWN) {
		strcpy(htthread->upload_path, path);
		return TRUE;
	} else 
		return FALSE;
}

static boolean htthread_upload(struct htthread_struct *htthread, 
					struct htthread_trans *trans)
{
	if (!htthread || !trans)
		return FALSE;
	return create_tcp_client(&htthread->socket);		
}

static boolean htthread_download(struct htthread_struct *htthread, 
					struct htthread_trans *trans) {
	if (!htthread || !trans)
		return FALSE;
	return create_tcp_client(&htthread->socket);
}

static boolean htthread_send(struct htthread_struct *htthread) {
	if (!htthread)
		return FALSE;
	return hnet_send(htthread->socket.tcp, &htthread->trans, 
								sizeof(struct htthread_trans));

}

static boolean htthread_recv(struct htthread_struct *htthread)
{
	if (!htthread)
		return FALSE;
	return hnet_recv(htthread->socket.tcp, &htthread->trans, 
								sizeof(struct htthread_trans));

}

int trans_percent(struct htthread_struct *htthread)
{
	return 0;
}

gboolean htthread_thread(void *p)
{
	struct htthread_struct *htthread = (struct htthread_struct *)p;
	if (!create_tcp_client(&htthread->socket)) {
		print_error(ESYSERR,"建立文件传输网络连接错误");
		return FALSE;
	}
	while (TRUE) {
		switch(htthread->select) {
			case FUP: {
				if(htthread->trans.protocol == FUP)
				hnet_recv(htthread->socket.tcp, &htthread->trans,
							sizeof(struct htthread_trans));
				else
					return FALSE;
				break;
			}
			case FDOWN: {
				if(htthread->trans.protocol == FDOWN)
				hnet_send(htthread->socket.tcp, &htthread->trans,
							sizeof(struct htthread_trans));
				else 
					return FALSE;
				break;
			}
			default:return FALSE;
		}
	}
	return TRUE;
}
