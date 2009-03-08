#ifndef _ASTHREAD_H
#define _ASTHREAD_H
#include "linuxarms.h"
#include "protocol.h"
#include "anet.h"
#include "proc.h"
#include "assinfo.h"
#include "asprocess.h"
#include "thread.h"
/*
 * asthread_trans 系统信息显示和实时监视(进程信息显示)线程
 * 		  传送的数据
 * @protocol:     控制(见protocol/protocol_sthread)
 * @kill:         要杀死的进程的进程号(当ctrl = KILL时，该域才有效)
 * @all:          是否显示所有进程信息(all = TRUE,显示。默认为不显示)
 */
struct asthread_trans {
	protocol_sthread protocol;
	int kill;
	boolean all;
};

boolean asthread_trans_init(struct asthread_trans *astrans);
boolean asthread_trans_set_protocol(struct asthread_trans *astrans, 
					protocol_sthread protocol);
boolean asthread_trans_set_kill(struct asthread_trans *astrans, int kill);

struct proc_struct;
/*
 * asthread_struct  系统信息显示和实时监视(进程信息显示)线程
 *                  主数据结构
 * @ssinfo:    系统信息显示主数据结构
 * @sprocess:  实时监视主数据结构
 * @trans:     线程中传输的数据
 * @socket:    数据传输数据结构
 * @lock:      互斥访问锁
 * @proc:      读取proc文件系统相关数据结构
 */
struct asthread_struct {
	struct linuxarms_thread thread;
	struct assinfo_struct *assinfo;
	struct asprocess_struct *asprocess;
	struct asthread_trans trans;
	struct anet_struct socket;
	struct proc_struct proc;
	boolean competence;

	boolean (*set_protocol)(struct asthread_struct *asthread, protocol_sthread protocol);
	boolean (*send)(struct asthread_struct *asthread);
	boolean (*recv)(struct asthread_struct *asthread);

};

boolean asthread_init(struct asthread_struct *asthread,
		      struct assinfo_struct *assinfo,
		      struct asprocess_struct *asprocess);
boolean asthread_set_trans(struct asthread_struct *asthread,
				protocol_sthread protocol, int kill);
void *asthread_thread(void *p);
boolean asthread_kill_process(int pid);
#endif
