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
#include "init.h"
static int atthread_send(struct atthread_struct *atthread, int len);
static int atthread_recv(struct atthread_struct *atthread, int len);
static boolean atthread_set_protocol(struct atthread_struct *atthread, protocol_fthread protocol);
 /*
 * init_atthread	initialize atthread_struct structure. 
 * @atthread:	the structure which will be initialized.
 * @user:	the user who use armserve.
 * @scoket:		tcp socket structure
 */
boolean atthread_init(struct atthread_struct *atthread)
{
	LINUXARMS_POINTER(atthread);
	
	atthread->select = FMAX;
	atthread->mode = 0;
	atthread->socket = -1;

	atthread_trans_init(&atthread->trans);
	atthread->set_protocol = atthread_set_protocol;
	atthread->send = atthread_send;
	atthread->recv = atthread_recv;
	return TRUE;
}

void *atthread_upload(void *p)
{
	struct atthread_struct *atthread = (struct atthread_struct *)p;
	int up, len;
	char *buffer = atthread->trans.buffer;
	debug_where();
	if((atthread->socket = wait_afthread_connect()) == -1) {
		print_error(EWARNING, "create file transmit connection error\n");
		return NULL;
	}
	if ((up = open(atthread->path, O_RDONLY)) == -1) {
		print_error(EWARNING, "open upload file \"%s\" error\n", atthread->path);
		close(atthread->socket);
		return NULL;
	}
	debug_where();

	do {
		len = read(up, buffer, ATTHREAD_TRANS_SIZE);
		buffer[len] = '\0';
		if (atthread->send(atthread, len) != len) {
			perror("send");
			break;
		}
	} while (len);
	debug_print("文件上传结束\n");
	close(atthread->socket);
	close(up);
	debug_where();
	return NULL;
}
void *atthread_download(void *p)
{
	struct atthread_struct *atthread = (struct atthread_struct *)p;
	int down;
	int len;
	long long plen;
	char *buffer = atthread->trans.buffer;

	if((atthread->socket = wait_afthread_connect()) == -1) {
		print_error(EWARNING, "create file transmit connection error\n");
		return NULL;
	}
	if ((down = open(atthread->path, O_WRONLY | O_CREAT, atthread->mode)) == -1) {
		print_error(EWARNING, "create download file \"%s\"error\n", atthread->path);
		close(atthread->socket);
		return NULL;
	}
		
	debug_where();
	debug_print("file size %ld\n", atthread->size);

	plen = atthread->size;
	do {
		if ((len = atthread->recv(atthread, ATTHREAD_TRANS_SIZE)) == -1)
			goto out;
		plen = plen - len;
		if (plen < 0) {
			plen = plen + len;
			if (write(down, buffer, plen) != len)
				goto out;
			len = 0;
			break;
		}
		if (write(down, buffer, len) != len)
			goto out;
	} while (len);
	close(down);
	close(atthread->socket);
	atthread->size = 0;
	debug_print("从文件传输返回\n");
	atthread->socket = -1;
	return NULL;
out:
	print_error(EWARNING, "download file \"%s\" error\n", atthread->path);
	close(down);
	close(atthread->socket);
	atthread->socket = -1;
	unlink(atthread->path);
	atthread->size = 0;
	return NULL;
}

static int atthread_send(struct atthread_struct *atthread, int len)
{
	LINUXARMS_POINTER(atthread);
	return send(atthread->socket, atthread->trans.buffer, len, 0);
}

static int atthread_recv(struct atthread_struct *atthread, int len)
{
	LINUXARMS_POINTER(atthread);
	return recv(atthread->socket, atthread->trans.buffer, len, 0);
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
