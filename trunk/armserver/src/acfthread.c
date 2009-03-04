#define __DEBUG__
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "afthread.h"
#include "anet.h"
#include "afview.h"
#include "atthread.h"
#include "debug.h"
#include "error.h"
#include "config.h"

static boolean afthread_set_protocol(struct afthread_struct *afthread, 
					protocol_fthread protocol);
static boolean afthread_send(struct afthread_struct *afthread);
static boolean afthread_recv(struct afthread_struct *afthread);
/*
 * 初始化文件浏览和文件传输控制主数据结构
 */
boolean afthread_init(struct afthread_struct *afthread,
                      struct afview_struct *afview,
                      struct atthread_struct *atthread)
{
	            
        if (!afthread || !afview || !atthread) {
		debug_where();
                print_error(ESYSERR,"错误");
                return FALSE;
        }   
	linuxarms_thread_init(&afthread->thread);
	afthread->afview = afview;
	afthread->atthread = atthread;
	afthread->competence = FALSE;
	
	afthread->set_protocol = afthread_set_protocol;
        afthread->send = afthread_send;
	afthread->recv = afthread_recv;
	afthread_trans_init(&afthread->trans);
	anet_init(&afthread->socket, get_localhost_ip(), get_fthread_port());
	return TRUE;

}
/*
 * 文件浏览和文件传输主线程执行体
 * @p:  struct afthread_struct
 */
boolean afthread_thread(void *p)
{
	struct afthread_struct *afthread = (struct afthread_struct *)p;
	struct afview_struct *afview = afthread->afview;
	struct atthread_struct *atthread = afthread->atthread;
	linuxarms_print("create afthread thread...\n");

	afthread->thread.id = linuxarms_thread_self();

	anet_init(&afthread->socket, get_localhost_ip(),get_fthread_port());
	if (!create_tcp_server(&afthread->socket)) {
		print_error(ESYSERR,"建立文件浏览和文件传输网络连接错误");
		return FALSE;
	}
	debug_print("afthread socket ip : %s tcp: %d port: %d\n", afthread->socket.ip,
				afthread->socket.tcp, afthread->socket.port);
	while (afthread->thread.id) {
		if (!afthread->recv(afthread)) {
			linuxarms_print("asthread recv data error,exit....\n");
			exit(1);
		}
		switch (afthread->trans.protocol) {
		case FUP:
			debug_print("protocol->afthread: 上传文件\n");
			struct stat buf;
			char mode[10];
			snprintf(atthread->path, PATH_LEN, "%s/%s", 
				 afthread->trans.path, afthread->trans.rename[OLDNAME]);
			if (access(atthread->path, F_OK) == -1) {
				afthread->set_protocol(afthread, FNOEXIST);
				afthread->send(afthread);
				break;
			}
			if (stat(atthread->path, &buf) == -1) {
				afthread->set_protocol(afthread, FERROR);
				afthread->send(afthread);
				break;
			}
			debug_print("mode   0x%x\n", buf.st_mode);
			snprintf(mode, 10, "%d", buf.st_mode);
			strcpy(afthread->trans.rename[NEWNAME], mode);
			afthread->set_protocol(afthread, FUP);
			afthread->send(afthread);
			atthread->quit = FALSE;
			atthread_upload(atthread);
			break;
		case FDOWN:
			debug_print("protocol->afthread: 下载文件\n");
			if (access(afthread->trans.path, F_OK) == -1) {
				afthread->set_protocol(afthread, FNOEXIST);
				afthread->send(afthread);
				break;
			}
			snprintf(atthread->path, PATH_LEN, "%s/%s", 
				 afthread->trans.path, afthread->trans.rename[OLDNAME]);
			if (access(atthread->path, F_OK) == 0) {
				afthread->set_protocol(afthread, FEXIST);
				afthread->send(afthread);
				break;
			}
			atthread->mode = atoi(afthread->trans.rename[NEWNAME]);
			afthread->set_protocol(afthread, FDOWN);
			afthread->send(afthread);
			atthread->quit = FALSE;
			atthread_download(atthread);
			break;
		case FVIEW:
			debug_print("protocol->afthread: 浏览文件\n");
			if (access(afview->path, F_OK) == -1) {
				afthread->set_protocol(afthread, FNOEXIST);
				afthread->send(afthread);
				break;
			}
			afthread->set_protocol(afthread, FVIEW);
			afthread->send(afthread);
			afview->hide = afthread->trans.hide;
			do_file_view(afview);
			break;
		case FRENAME:
			debug_print("protocol->afthread: 重命名文件\n");
			afthread_rename(afthread);
			break;
		case FDELETE:
			debug_print("protocol->afthread: 删除文件\n");
			afthread_delete(afthread);
			break;
		default:
			break;
		}
	}
	return TRUE;
}

static boolean afthread_send(struct afthread_struct *afthread)
{
	LINUXARMS_POINTER(afthread);
	return anet_send(afthread->socket.tcp, (void *)&afthread->trans,
			 sizeof(struct afthread_trans));
}
static boolean afthread_recv(struct afthread_struct *afthread)
{
	LINUXARMS_POINTER(afthread);
	return anet_recv(afthread->socket.tcp, (void *)&afthread->trans,
			 sizeof(struct afthread_trans));
}

static boolean afthread_set_protocol(struct afthread_struct *afthread, protocol_fthread protocol)
{
	LINUXARMS_POINTER(afthread);
	return afthread_trans_set_protocol(&afthread->trans, protocol);

}
boolean afthread_trans_init(struct afthread_trans *aftrans)
{
	LINUXARMS_POINTER(aftrans);
	aftrans->protocol = FMAX;
	memset(aftrans->path, '\0', PATH_LEN);
	aftrans->path[0] = '/';
	memset(aftrans->rename, '\0', FILE_NAME_LEN * 2);
	aftrans->hide = TRUE;
	return TRUE;
}
boolean afthread_trans_set_protocol(struct afthread_trans *aftrans, protocol_fthread protocol)
{
	LINUXARMS_POINTER(aftrans);
	if (!PROTOCOL_IS_FTHREAD(protocol)) {
		aftrans->protocol = FMAX;
		return FALSE;
	}
	aftrans->protocol = protocol;
	return TRUE;
}

const char *afthread_trans_get_path(struct afthread_trans *aftrans)
{
	if (!aftrans)
		return NULL;
	return (const char *)aftrans->path;
}

/*
 * 重命名文件
 */
boolean afthread_rename(struct afthread_struct *afthread)
{
	char oldname[PATH_LEN + FILE_NAME_LEN];
	char newname[PATH_LEN + FILE_NAME_LEN];

	if (strcmp(afthread->trans.rename[OLDNAME], "." ) == 0 ||
	    strcmp(afthread->trans.rename[NEWNAME], "." ) == 0 ||
	    strcmp(afthread->trans.rename[OLDNAME], "..") == 0 ||
	    strcmp(afthread->trans.rename[NEWNAME], "..") == 0)
		goto out;
	if (access(afthread->trans.path, F_OK) == -1)
		goto out;
	sprintf(oldname, "%s/%s", afthread->trans.path, afthread->trans.rename[OLDNAME]);
	sprintf(newname, "%s/%s", afthread->trans.path, afthread->trans.rename[NEWNAME]);
	
	/* 如果原文件不存在或者信文件已经存在，则无法重命名 */
	if (access(oldname, F_OK) == -1 ||
	    access(newname, F_OK) == 0)
		goto out;
	if (rename(oldname, newname) != 0)
		goto out;
	afthread->set_protocol(afthread, FRENAMESUC);
	afthread->send(afthread);
	return TRUE;
out:
	afthread->set_protocol(afthread, FRENAMEERR);
	afthread->send(afthread);
	return FALSE;
}
/*
 * 删除文件
 */
boolean afthread_delete(struct afthread_struct *afthread)
{
	char delname[PATH_LEN + FILE_NAME_LEN];

	sprintf(delname, "%s/%s",afthread->trans.path, afthread->trans.rename[OLDNAME]);

	if (access(delname, F_OK) == -1)
		goto out;
	if (unlink(delname) == -1)
		goto out;
	afthread->set_protocol(afthread, FDELETESUC);
	afthread->send(afthread);
	return TRUE;
out:
	afthread->set_protocol(afthread, FDELETEERR);
	afthread->send(afthread);
	return FALSE;
}
