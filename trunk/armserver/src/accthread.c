#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "acthread.h"
#include "linuxarms.h"
#include "anet.h"
#include "error.h"
#include "login.h"
#include "config.h"
/*
 * 初始化实时控制主数据结构
 *
 */
boolean acthread_init(struct acthread_struct *acthread,
		struct acthread_trans *trans,
		struct anet_struct *socket)
{
	if (!acthread || !trans || !socket )
		print_error(ENOINIT, "init error\n");
	acthread->trans = *trans;
	acthread->socket = *socket;
	return TRUE;
}
/*
 * 发送一个请求到hostclient上
 */
boolean acthread_send(struct acthread_struct *acthread)
{
	anet_send(acthread->socket.tcp, &acthread->trans, sizeof(struct acthread_trans));
	return TRUE;
}
/*
 *接受一个数据从hostclient
 */
boolean acthread_recv(struct acthread_struct *acthread)
{
	anet_recv(acthread->socket.tcp, &acthread->trans, sizeof(struct acthread_trans));
	return TRUE;
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
	/* 建立网络连接 */
	anet_init(&acthread->socket, get_host_ip(), get_cthread_port());
	create_tcp_server(&acthread->socket);
	while (TRUE) {
		acthread_recv(acthread);   /* 接收数据 */
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
	}
	return TRUE;
}
