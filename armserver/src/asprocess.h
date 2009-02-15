#ifndef _SPROCESS_H
#define _SPROCESS_H
#include "protocol.h"
#include "linuxarms.h"

#define FNAME_LEN 30
#define PROCESS_NAME_LEN 16
#define PROCESS_USER_LEN 20
/*
 * asprocess_struct 接收hostclient发送过来的进程信息
 * @info:  要发送的信息
 */
struct asprocess_trans {
	protocol_sthread protocol;
	char name[PROCESS_NAME_LEN];
	int pid;
	char user[PROCESS_USER_LEN];
	char state;
	float cpu;
	float mem;
};
boolean asprocess_trans_init(struct asprocess_trans *astrans);
boolean asprocess_trans_set_protocol(struct asprocess_trans *astrans, 
					protocol_sthread protocol);
boolean asprocess_trans_set_data(struct asprocess_trans *astrans, char *name,
				int pid, char *user, char state, float cpu, float mem);
/*
 * asprocess_struct 实时监视主数据结构
 * @trans:  要从hostclient接收的数据
 * @*kill:   要杀死的进程号
 */
struct asprocess_struct {
	struct asprocess_trans trans;
	struct anet_struct *socket;
	int *kill;

	boolean (*set_protocol)(struct asprocess_struct *asprocess, 
				protocol_sthread protocol);
	boolean (*recv)(struct asprocess_struct *asprocess);
	boolean (*send)(struct asprocess_struct *asprocess);
};
boolean asprocess_init(struct asprocess_struct *asprocess, 
			int *kill, struct anet_struct *socket);
boolean do_show_asprocess(struct asthread_struct *asthread);
boolean asprocess_read_info(struct asprocess_struct *asprocess,int pid);
#endif
