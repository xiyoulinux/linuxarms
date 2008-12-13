#ifndef _SPROCESS_H
#define _SPROCESS_H
#include "protocol.h"
#include "linuxarms.h"

#define FNAME_LEN 30
#define PROCESS_INFO_LEN 256
/*
 * asprocess_struct 接收hostclient发送过来的进程信息
 * @num:   要发送的进程信息的总数
 * @state: 发送控制状态。如果为STOP，则停止发送
 * @info:  要发送的信息
 */
struct asprocess_trans {
	int num;
	protocol_sthread protocol;
	char info[PROCESS_INFO_LEN];
};

/*
 * asprocess_struct 实时监视主数据结构
 * @trans:  要从hostclient接收的数据
 * @kill:   要杀死的进程号
 */
struct asprocess_struct {
	struct asprocess_trans trans;
	//char fname[WFNAME_LEN];
	int kill;

	boolean (*recv)(int tcp, struct asprocess_struct *asprocess);
	boolean (*send)(int tcp, struct asprocess_struct *asprocess);
};
#endif
