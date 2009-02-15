#ifndef _ATTHREAD_H
#define _ATTHREAD_H

#include "protocol.h"
#include "anet.h"
#include "login.h"
#include "linuxarms.h"

#define ATTHREAD_PATH_LEN 80
#define TRAS_SIZE 512

/*
 * @atthread_trans     文件传输的数据
 * @trans:		    控制协议
 * @buffer:		  传输的数据
 */
struct atthread_trans {
	protocol_fthread protocol;
	char buffer[TRAS_SIZE];
};

/*
 * @file_size:       请求传输文件的大小
 * @savefile_size:   已经传输文件的大小
 */
struct atthread_struct {
	protocol_fthread select;
	long long file_size;
	long long savefile_size;
	struct user_struct *user;
	struct anet_struct socket;
	struct atthread_trans trans;
	char download_path[ATTHREAD_PATH_LEN];
	char upload_path[ATTHREAD_PATH_LEN];

	boolean (*get_download_file)(struct atthread_struct *atthread,
							char *path);
	boolean(*get_upload_file)(struct atthread_struct *atthread,
							char *path);
	boolean(*upload)(struct atthread_struct *atthread, 
				struct atthread_trans *trans);
	boolean(*download)(struct atthread_struct *atthread, 
				struct atthread_trans *trans);
	boolean (*send)(struct atthread_struct *atthread);
	boolean (*recv)(struct atthread_struct *atthread);
};

/* 初始化htthread_struct结构体 */
int atthread_init(struct atthread_struct *atthread,
	             struct user_struct *user);

/* 线程执行体 */
boolean atthread_thread(void *p);

#endif

