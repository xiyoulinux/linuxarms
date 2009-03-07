#ifndef _ATTHREAD_H
#define _ATTHREAD_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "protocol.h"
#include "anet.h"
#include "login.h"
#include "linuxarms.h"

#define ATTHREAD_TRANS_SIZE 4095
#define ATTHREAD_PROTOCOL 1

/*
 * @atthread_trans     文件传输的数据
 * @trans:		    控制协议
 * @buffer:		  传输的数据
 */
struct atthread_trans {
	protocol_fthread protocol;
	char buffer[ATTHREAD_TRANS_SIZE + ATTHREAD_PROTOCOL];
};
boolean atthread_trans_init(struct atthread_trans *attrans);
const char *atthread_trans_get_buf(struct atthread_trans *attrans);
boolean atthread_trans_set_buf(struct atthread_trans *attrans, const char *buf);
boolean atthread_trans_set_protocol(struct atthread_trans *attrans, protocol_fthread protocol);
/*
 * @file_size:       请求传输文件的大小
 * @savefile_size:   已经传输文件的大小
 */
struct atthread_struct {
	struct anet_struct *socket;
	struct atthread_trans trans;
	char path[PATH_LEN];
	protocol_fthread select;
	off_t size;
	mode_t mode;

	boolean (*set_protocol)(struct atthread_struct *atthread, protocol_fthread protocol);
	int (*send)(struct atthread_struct *atthread, int len);
	int (*recv)(struct atthread_struct *atthread, int len);
};

/* 初始化htthread_struct结构体 */
int atthread_init(struct atthread_struct *atthread,
	             struct anet_struct *socket);
boolean atthread_thread(void *p);
boolean atthread_upload(struct atthread_struct *atthread);
boolean atthread_download(struct atthread_struct *atthread);

#endif

