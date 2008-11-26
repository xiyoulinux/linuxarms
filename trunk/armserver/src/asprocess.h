#ifndef _SPROCESS_H
#define _SPROCESS_H
#include "protocol.h"
#include "linuxarms.h"

#define PROCESS_INFO_LEN 256
/*
 * asprocess_struct 接收hostclient发送过来的进程信息
 * @num:   要发送的进程信息的总数
 * @state: 发送控制状态。如果为STOP，则停止发送
 * @info:  啊哟发送的信息
 */
struct asprocess_trans {
	int num;
	protocol_sthread state;
	char info[PROCESS_INFO_LEN];
};
/*
 * asprocess_struct 实时监视主数据结构
 * @trans:  要从hostclient接收的数据
 * @kill:   要杀死的进程号
 */
struct asprocess_struct {
	struct hsprocess_trans trans;
	int kill;

	boolean (*recv)(struct hsprocess_struct *hsprocess);
};
#endif
