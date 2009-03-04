#define __DEBUG__
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "linuxarms.h"
#include "protocol.h"
#include "anet.h"
#include "login.h"
#include "atthread.h"
#include "error.h"
#include "debug.h"
#include "atthread.h"
static int atthread_send(struct atthread_struct *atthread, int len);
static int atthread_recv(struct atthread_struct *atthread, int len);
static boolean atthread_set_protocol(struct atthread_struct *atthread, protocol_fthread protocol);
 /*
 * init_atthread	initialize atthread_struct structure. 
 * @atthread:	the structure which will be initialized.
 * @user:	the user who use armserve.
 * @scoket:		tcp socket structure
 */
boolean atthread_init(struct atthread_struct *atthread,
		  struct anet_struct *socket)
{
	LINUXARMS_POINTER(atthread);
	LINUXARMS_POINTER(socket);
	
	atthread->select = FMAX;
	atthread->mode = 0;
	atthread->socket = socket;
	atthread->quit = TRUE;

	atthread_trans_init(&atthread->trans);
	atthread->set_protocol = atthread_set_protocol;
	atthread->send = atthread_send;
	atthread->recv = atthread_recv;
	return TRUE;
}
boolean atthread_upload(struct atthread_struct *atthread)
{
	LINUXARMS_POINTER(atthread);
	int up, len;
	int i =0;

	if ((up = open(atthread->path, O_RDONLY)) == -1) {
		atthread->set_protocol(atthread, FERROR);
		atthread->send(atthread, 0);
		return FALSE;
	}
	debug_where();
	do {
		len = read(up, atthread->trans.buffer, ATTHREAD_TRANS_SIZE);
		atthread->trans.buffer[len] = '\0';
		atthread->send(atthread, len);
	} while (len);
	atthread->send(atthread, 0);
	close(up);
	debug_where();
	return TRUE;
	/*
	do {
		if (atthread->quit)
			break;
		len = read(up, atthread->trans.buffer, ATTHREAD_TRANS_SIZE);
		if (len <= 0)
			break;
		atthread->trans.buffer[len] = '\0';
		atthread->set_protocol(atthread, FSEND);
		if (!atthread->send(atthread))
			break;
		//sleep(1);
		atthread->recv(atthread);
		if (atthread->trans.protocol != FRECV) {
			debug_print("客户端接收文件信息错误\n");
			break;
		}
		i++;
	} while (TRUE);
	close(up);
	debug_print("i = %d\n", i);
	atthread->set_protocol(atthread, FSENDALL);
	atthread->send(atthread);
	debug_print("sendall = %d\n", FSENDALL);

out:
	atthread->quit = FALSE;
	return TRUE;
	*/
}
boolean atthread_download(struct atthread_struct *atthread)
{
	LINUXARMS_POINTER(atthread);
	int down;
	int len;
	if ((down = open(atthread->path, O_WRONLY | O_CREAT, atthread->mode)) == -1) {
		atthread->set_protocol(atthread, FERROR);
		atthread->send(atthread, 0);
		return FALSE;
	}
	do {
		len = atthread->recv(atthread, ATTHREAD_TRANS_SIZE);
		if (len <= 0)
			break;
		write(down, atthread->trans.buffer, len);
	} while (TRUE);
	close(down);
	return TRUE;

	/*
	do {
		if (atthread->quit) {
			close(down);
			unlink(atthread->path);
			return FALSE;
		}
		if (!atthread->recv(atthread)) {
			print_error(EWARNING, "下载文件发生错误");
			return FALSE;
		}
		switch (atthread->trans.protocol) {
		case FSEND:
			write(down, atthread->trans.buffer,strlen(atthread->trans.buffer));
			break;
		default:
			goto out;

		}
	} while (TRUE);
out:
	close(down);
	return TRUE;
	*/
}

static int atthread_send(struct atthread_struct *atthread, int len)
{
	LINUXARMS_POINTER(atthread);
	//return anet_send(atthread->socket->tcp, (void *)&atthread->trans, 
	//		 sizeof(struct atthread_trans));
	return send(atthread->socket->tcp, atthread->trans.buffer, len, 0);

}

static int atthread_recv(struct atthread_struct *atthread, int len)
{
	LINUXARMS_POINTER(atthread);
	//return anet_recv(atthread->socket->tcp, (void *)&atthread->trans, 
	//		 sizeof(struct atthread_trans));
	return recv(atthread->socket->tcp, atthread->trans.buffer, len, 0);

}
boolean atthread_trans_init(struct atthread_trans *attrans)
{
	LINUXARMS_POINTER(attrans);
	attrans->protocol = FMAX;
	memset(attrans->buffer, '\0', ATTHREAD_TRANS_SIZE);
	return TRUE;
}
boolean atthread_trans_set_protocol(struct atthread_trans *attrans, protocol_fthread protocol)
{
	LINUXARMS_POINTER(attrans);
	if (!PROTOCOL_IS_FTHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		attrans->protocol = FMAX;
		return FALSE;
	}
	attrans->protocol = protocol;
	return TRUE;
}
const char *atthread_trans_get_buf(struct atthread_trans *attrans)
{
	LINUXARMS_POINTER(attrans);
	return attrans->buffer;
}

boolean atthread_trans_set_buf(struct atthread_trans *attrans, const char *buf)
{
	LINUXARMS_POINTER(attrans);
	if (!buf) {
		print_error(EWARNING, "字符串为空");
		memset(attrans->buffer, '\0', ATTHREAD_TRANS_SIZE);
		return FALSE;
	}
	if (strlen(buf) >= ATTHREAD_TRANS_SIZE) 
		print_error(EWARNING, "字符串长度过长，可能丢失数据");	
	strncpy(attrans->buffer, buf, ATTHREAD_TRANS_SIZE);
	return TRUE;
}
static boolean atthread_set_protocol(struct atthread_struct *atthread, protocol_fthread protocol)
{
	LINUXARMS_POINTER(atthread);
	return atthread_trans_set_protocol(&atthread->trans, protocol);
}
