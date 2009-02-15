#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "acthread.h"
#include "linuxarms.h"
#include "anet.h"
#include "debug.h"
#include "error.h"
#include "login.h"
#include "config.h"

void do_cd(char *cmd)
{
	char dir[512];
	if (strlen(cmd) == 2) {
		sprintf(dir,"%s", getenv("HOME"));
	} else if ((strlen(cmd) == 4) && (cmd[3] =='~')) {
		sprintf(dir,"%s", getenv("HOME"));
	} else if ((strlen(cmd) == 4) && (cmd[3] == '-')) {
		sprintf(dir, "%s", getenv("OLDPWD"));
	} else if((strlen(cmd) > 4) && (cmd[3] != '/')) {
		sprintf(dir,"%s/%s", getenv("HOME"), &cmd[3]);
	} else if((strlen(cmd) >4) && (cmd[3] == '~')) {
		sprintf(dir,"%s%s", getenv("HOME"), &cmd[5]);
	} else if((strlen(cmd) >4) && (cmd[3] == '/')) {
		sprintf(dir, "%s", &cmd[3]);
	}
	if (chdir(dir) == -1) {
		debug_where();
		print_error(EWARNING, "chdir: error");
	}
}
/*
 * 初始化实时控制主数据结构
 *
 */
boolean acthread_init(struct acthread_struct *acthread)
{
	LINUXARMS_POINTER(acthread);
	acthread->thread = NULL;
	anet_init(&acthread->socket, get_localhost_ip(), get_cthread_port());
	acthread_trans_init(&acthread->trans);
	return TRUE;
}
/*
 * 发送一个请求到hostclient上
 */
boolean acthread_send(struct acthread_struct *acthread)
{
	return anet_send(acthread->socket.tcp, &acthread->trans, 
			sizeof(struct acthread_trans));
}
/*
 *接受一个数据从hostclient
 */
boolean acthread_recv(struct acthread_struct *acthread)
{
	return anet_recv(acthread->socket.tcp, &acthread->trans, 
			sizeof(struct acthread_trans));
}
boolean acthread_handle(struct acthread_struct *acthread)
{
	/* 进行重定向 */
	int fd;
	int ret;
	fd = open(TEMP_FILE, (O_RDWR | O_CREAT), 0644);
	dup2(fd, 1);
	close(fd);
	/* 处理命令 */
	ret = system(acthread->trans.buffer);
	if (ret == 127) {
		printf("/bin/sh not available\n");
	} else if (ret == -1) {
		printf("there are some errors...\n");
	} else if (ret != 0) {
		printf("command not found\n");
	} 
	 if(strstr(acthread->trans.buffer, "cd"))
		 do_cd(acthread->trans.buffer);
	return TRUE;
}
/*
 *arm端实时控制线程
 *@p:  struct hcthread_struct
 */
boolean acthread_thread(void *p)
{
	struct acthread_struct *acthread = (struct acthread_struct *)p;
	int fd;
	linuxarms_print("create acthread thread...\n");
	acthread->thread = linuxarms_thread_self();
	/* 建立网络连接 */
	anet_init(&acthread->socket, get_localhost_ip(), get_cthread_port());
	create_tcp_server(&acthread->socket);
	debug_print("acthread socket ip : %s tcp: %d port: %d\n", acthread->socket.ip,
				acthread->socket.tcp, acthread->socket.port);	
	while (acthread->thread) {
		if (!acthread_recv(acthread)) {   /* 接收数据 */
			linuxarms_print("acthread recv data error,exit....\n");
			exit(1);
		}
		if (acthread->trans.protocol == CSEND) {
			acthread_handle(acthread); /* 处理数据 */
		}
		fd = open(TEMP_FILE, O_RDWR);
		do {
			if(read(fd, acthread->trans.buffer, TRANS_SIZE) != 0) {
				acthread->trans.protocol = CSEND;
				acthread_send(acthread); /* 发送数据 */
			} else {
				acthread->trans.protocol = CSENDALL;
				acthread_send(acthread);
			}
			acthread_recv(acthread); /* 接受数据 */
		} while (acthread->trans.protocol != CRECVALL);
		close(fd);
		remove(TEMP_FILE);
	}
	return TRUE;
}
boolean acthread_trans_init(struct acthread_trans *trans)
{
	LINUXARMS_POINTER(trans);
	trans->protocol = CMAX;
	memset(trans->buffer, '\0',TRANS_SIZE);
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
	if (strlen(buf) >= TRANS_SIZE) {
		debug_where();
		print_error(EWARNING, "缓冲区溢出");
		strncpy(trans->buffer, buf, TRANS_SIZE);
		return FALSE;
	}
	strcpy(trans->buffer, buf);
	return TRUE;
}
