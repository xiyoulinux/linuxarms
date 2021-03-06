#define __DEBUG__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <error.h>

#include "acthread.h"
#include "linuxarms.h"
#include "anet.h"
#include "debug.h"
#include "error.h"
#include "login.h"
#include "fconfig.h"
#include "thread.h"

static boolean acthread_send(struct acthread_struct *acthread);
static boolean acthread_recv(struct acthread_struct *acthread);
static boolean acthread_handle(struct acthread_struct *acthread);

/*
 * 初始化实时控制主数据结构
 *
 */
boolean acthread_init(struct acthread_struct *acthread)
{
	LINUXARMS_POINTER(acthread);
	linuxarms_thread_init(&acthread->thread);
	acthread->permit = FALSE;
	acthread_trans_init(&acthread->trans);
	acthread->send = acthread_send;
	acthread->recv = acthread_recv;
	return TRUE;
}
/*
 * 发送一个请求到hostclient上
 */
static boolean acthread_send(struct acthread_struct *acthread)
{
	return anet_send(acthread->socket.tcp, &acthread->trans, 
			sizeof(struct acthread_trans));
}
/*
 *接受一个数据从hostclient
 */
static boolean acthread_recv(struct acthread_struct *acthread)
{
	return anet_recv(acthread->socket.tcp, &acthread->trans, 
			sizeof(struct acthread_trans));
}
/*
 *处理 cd 命令
 */
void do_cd(struct acthread_struct *acthread)
{
	char dir[128];
	int ret;
	char cmd[128];
	char result[128];
	char send[256];

	snprintf(cmd, 128, "%s", acthread->trans.buffer);
	if (strlen(cmd) == 2) {
		sprintf(dir,"%s", getenv("HOME"));
	} else if ((strlen(cmd) == 4) && (cmd[3] =='~')) {
		sprintf(dir,"%s", getenv("HOME"));
	} else if ((strlen(cmd) == 4) && (cmd[3] == '-')) {
		sprintf(dir, "%s", getenv("OLDPWD"));
	} else if((strlen(cmd) >4) && (cmd[3] == '~')) {
		sprintf(dir,"%s%s", getenv("HOME"), &cmd[4]);
	} else if((strlen(cmd) > 4) && (cmd[3] != '/')) {
		char buf[80];
		getcwd(buf, sizeof(buf));
		if (strlen(buf) == 1)
			sprintf(dir,"%s%s", buf, &cmd[3]);
		else
			sprintf(dir,"%s/%s", buf, &cmd[3]);
	} else if((strlen(cmd) >=4) && (cmd[3] == '/')) {
		sprintf(dir, "%s", &cmd[3]);
	}
	debug_print("dir is:%s\n", dir);
	ret = chdir(dir);
	if (ret == 0) {
		strcpy(result, "TRUE");
	} else
		strcpy(result, "FALSE没有该文件或目录");
	memset(send, 256, '\0');
	sprintf(send, "%s#%s", result, dir);
	
	acthread_trans_set_buf(&acthread->trans, send);
	acthread_trans_set_protocol(&acthread->trans, CSENDCD);
	acthread->send(acthread);
	
	return;
}
/*
 * 处理命令
 */
static boolean acthread_handle(struct acthread_struct *acthread)
{
	int ret;
	FILE *fp;
	char command[256];
	char *buffer = acthread->trans.buffer;
	
	fp = fopen(TEMP_FILE, "w");
	/* 处理命令 */
	if(strstr(buffer, "cd ") || 
			((strlen(buffer) == 2)&&strstr(buffer, "cd"))) {
		do_cd(acthread);
		fclose(fp);
		return TRUE;
	}
	snprintf(command, 256, "%s > %s", acthread->trans.buffer, TEMP_FILE);
	debug_print("the command is:%s\n", command);
	ret = system(command);
	debug_print("the result is:%d\n", ret);
	if (ret == COMMAND_NOT_FOUND) {
		fprintf(fp, "/bin/sh:%s not found!\n", 
				acthread->trans.buffer);
	} /*else if (ret != 0) {
		fprintf(fp, "there are some errors\n");
	} */
	fclose(fp);

	return TRUE;
}
/*
 *arm端实时控制线程
 *@p:  struct hcthread_struct/
 */
void *acthread_thread(void *p)
{
	struct acthread_struct *acthread = (struct acthread_struct *)p;
	int fd;
	
	linuxarms_print("create acthread thread...\n");
	acthread->thread.id = linuxarms_thread_self();
	
	while (acthread->thread.id) {
		if (!acthread->recv(acthread)) {   /* 接收数据 */
			linuxarms_print("acthread recv data error,exit....\n");
			return NULL;
		}
		if (acthread->trans.protocol == CSEND) {
			acthread_handle(acthread); /* 处理数据 */
		} else
			continue;
		fd = open(TEMP_FILE, O_RDONLY);
		do {
			memset(acthread->trans.buffer, '\0', sizeof(acthread->trans.buffer));
			if(read(fd, acthread->trans.buffer, ACTHREAD_TRANS_SIZE) != 0) {
				acthread_trans_set_protocol(&acthread->trans, CSEND);
				acthread->send(acthread); /* 发送数据 */
			} else {
				acthread_trans_set_protocol(&acthread->trans, CSENDALL);
				acthread->send(acthread);
			}
			acthread->recv(acthread); /* 接受数据 */
		} while (acthread->trans.protocol != CRECVALL);
		close(fd);
//		unlink(TEMP_FILE);
	}
	return NULL;
}
boolean acthread_trans_init(struct acthread_trans *trans)
{
	LINUXARMS_POINTER(trans);
	trans->protocol = CMAX;
	memset(trans->buffer, '\0',ACTHREAD_TRANS_SIZE);
	return TRUE;
}
boolean acthread_trans_set_protocol(struct acthread_trans *trans,protocol_cthread protocol)
{
	LINUXARMS_POINTER(trans);
	if (!PROTOCOL_IS_CTHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		trans->protocol = CMAX;
		return FALSE;
	};
	trans->protocol = protocol;
	return TRUE;
}
boolean acthread_trans_set_buf(struct acthread_trans *trans, const char *buf)
{
	LINUXARMS_POINTER(trans);
	LINUXARMS_CHAR(buf);
	if (strlen(buf) >= ACTHREAD_TRANS_SIZE) {
		debug_where();
		print_error(EWARNING, "缓冲区溢出");
		strncpy(trans->buffer, buf, ACTHREAD_TRANS_SIZE);
		return FALSE;
	}
	strcpy(trans->buffer, buf);
	return TRUE;
}
