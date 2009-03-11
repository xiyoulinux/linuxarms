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
	char *p;
	fd_set rfd_set, wfd_set;
	int flag, ret;
	struct timeval timeout;
	if ((up = open(atthread->path, O_RDONLY)) == -1) {
		memset(atthread->trans.buffer, (char)FERROR, sizeof(atthread->trans.buffer));
		atthread->send(atthread, sizeof(atthread->trans.buffer));
		return FALSE;
	}
	p = atthread->trans.buffer + ATTHREAD_PROTOCOL;
	debug_where();
	fcntl(atthread->socket->tcp,F_SETFD,O_NONBLOCK);
	do {
		FD_ZERO(&rfd_set);
		FD_ZERO(&wfd_set);
		FD_SET(0, &rfd_set);
		FD_SET(0, &wfd_set);
		FD_SET(atthread->socket->tcp, &rfd_set);
		FD_SET(atthread->socket->tcp, &wfd_set);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;		
			
		len = read(up, p , ATTHREAD_TRANS_SIZE);
		p[len] = '\0';
		atthread->trans.buffer[0] = (char)FSEND;
		atthread->send(atthread, ATTHREAD_TRANS_SIZE + ATTHREAD_PROTOCOL);
		
		if((ret = select(atthread->socket->tcp + 1, &rfd_set, &wfd_set, NULL, &timeout)) == 0)
			continue;
		if(FD_ISSET(0,&rfd_set)) {
			atthread->recv(atthread, sizeof(atthread->trans.buffer));
			if (atthread->trans.buffer[0] == FERROR)
				goto out;
		}
	} while (len);
	memset(atthread->trans.buffer, (char)FSENDALL, sizeof(atthread->trans.buffer));
	atthread->send(atthread, sizeof(atthread->trans.buffer));
	debug_print("文件上传结束\n");
out:
	flag = fcntl(atthread->socket->tcp, F_GETFD, 0);
	flag &= ~O_NONBLOCK;
	fcntl(atthread->socket->tcp,F_SETFD,flag);	
	close(up);
	debug_where();
	return TRUE;
}
boolean atthread_download(struct atthread_struct *atthread)
{
	LINUXARMS_POINTER(atthread);
	int down;
	int len;
	char *p;
	protocol_fthread protocol;
	long long plen;

	if ((down = open(atthread->path, O_WRONLY | O_CREAT, atthread->mode)) == -1) {
		memset(atthread->trans.buffer, (char)FERROR, sizeof(atthread->trans.buffer));
		atthread->send(atthread, sizeof(atthread->trans.buffer));
		return FALSE;
	}
		
	debug_where();
	debug_print("file size %ld\n", atthread->size);
	p = atthread->trans.buffer + ATTHREAD_PROTOCOL;

	plen = atthread->size;
	do {
		len = atthread->recv(atthread,sizeof(atthread->trans.buffer));
		protocol = (protocol_fthread)atthread->trans.buffer[0];
		switch (protocol) {
		case FSEND:
			plen = plen - len + 1;
			if (plen < 0) {
				plen = plen + len - 1;
				write(down, p, plen);
				len = 0;
				break;
			}
			write(down, p, len - 1);
			break;
		case FSENDALL:
			debug_print("文件传输结束\n");
			len = 0;
			break;
		case FERROR:
		default:
			linuxarms_print("传输文件错误\n");
			goto out;
		}
	} while (len);
	close(down);
	atthread->size = 0;
	debug_print("从文件传输返回\n");
	return TRUE;
out:
	unlink(atthread->path);
	atthread->size = 0;
	debug_where();
	return TRUE;
}

static int atthread_send(struct atthread_struct *atthread, int len)
{
	LINUXARMS_POINTER(atthread);
	return send(atthread->socket->tcp, atthread->trans.buffer, len, 0);
}

static int atthread_recv(struct atthread_struct *atthread, int len)
{
	LINUXARMS_POINTER(atthread);
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
