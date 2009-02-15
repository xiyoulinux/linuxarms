#include <string.h>

#include "linuxarms.h"
#include "protocol.h"
#include "anet.h"
#include "login.h"
#include "atthread.h"
#include "error.h"

static boolean atthread_get_download_file(struct atthread_struct *atthread, char *path);
static boolean atthread_get_upload_file(struct atthread_struct *atthread, char *path);
static boolean atthread_upload(struct atthread_struct *atthread, struct atthread_trans *trans);
static boolean atthread_download(struct atthread_struct *atthread, struct atthread_trans *trans);
static boolean atthread_send(struct atthread_struct *atthread);
static boolean atthread_recv(struct atthread_struct *atthread);
 
 /*
 * init_atthread	initialize atthread_struct structure. 
 * @atthread:	the structure which will be initialized.
 * @user:	the user who use armserve.
 * @scoket:		tcp socket structure
 */
int atthread_init(struct atthread_struct *atthread,
		     struct user_struct *user)
{
	int ret = 0;
	if (!atthread) {
		ret = ENOINIT;
		goto out;
	}
	if (!user) {
		ret = ENOINIT;
		goto out;
	}
	atthread->select = FMAX;
	atthread->user = user;
	atthread->trans.protocol = FMAX;

	atthread->get_download_file = atthread_get_download_file;
	atthread->get_upload_file = atthread_get_upload_file;
	atthread->upload = atthread_upload;
	atthread->download = atthread_download;
	atthread->send = atthread_send;
	atthread->recv = atthread_recv;
	
out:
	return ret;
}
static boolean atthread_get_download_file(struct atthread_struct *atthread,
								char *path)
{
	if (!atthread || !path)
		return FALSE;
	if (atthread->select == FDOWN) {
		strcpy(atthread->upload_path, path);
		return TRUE;
	} else 
		return FALSE;
}

static boolean atthread_get_upload_file(struct atthread_struct *atthread,
								char *path)
{
	if (!atthread || !path)
		return FALSE;
	if (atthread->select == FUP) {
		strcpy(atthread->download_path, path);
		return TRUE;
	} else 
		return FALSE;
}

static boolean atthread_upload(struct atthread_struct *atthread, 
					struct atthread_trans *trans)
{
	if (!atthread || !trans)
		return FALSE;
	return create_tcp_server(&atthread->socket);		
}

static boolean atthread_download(struct atthread_struct *atthread, 
					struct atthread_trans *trans) {
	if (!atthread || !trans)
		return FALSE;
	return create_tcp_server(&atthread->socket);
}

static boolean atthread_send(struct atthread_struct *atthread) {
	if (!atthread)
		return FALSE;
	return anet_send(atthread->socket.tcp, &atthread->trans, 
				sizeof(struct atthread_trans));

}

static boolean atthread_recv(struct atthread_struct *atthread)
{
	if (!atthread)
		return FALSE;
	return anet_recv(atthread->socket.tcp, &atthread->trans, 
				sizeof(struct atthread_trans));

}

boolean atthread_thread(void *p)
{
	struct atthread_struct *atthread = (struct atthread_struct *)p;
	if (!create_tcp_server(&atthread->socket)) {
		print_error(ESYSERR,"建立文件传输网络连接错误");
		return FALSE;
	}
	while (TRUE) {
		switch(atthread->select) {
			case FUP: {
				if(atthread->trans.protocol == FUP)
				anet_send(atthread->socket.tcp, &atthread->trans,
							sizeof(struct atthread_trans));
				else
					return FALSE;
				break;
			}
			case FDOWN: {
				if(atthread->trans.protocol == FDOWN)
				anet_recv(atthread->socket.tcp, &atthread->trans,
							sizeof(struct atthread_trans));
				else 
					return FALSE;
				break;
			}
			default:return FALSE;
		}
	}
	return TRUE;
}
