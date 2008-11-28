#ifndef _PROC_H
#define _PROC_H

#include "linuxarms.h"
#include "asthread.h"

typedef enum _State {
	STOP,
	CONTINUE,
}proc_state;
/*
 * proc_struct  when you want to read some arm system information or read arm
 * 		process information, you should use this structure.
 * @state:	when we are reading some information from proc system, but some
 * 		one say there is needless to read continue,we can set this filed
 * 		to STOP to stop read.
 * @ctrl:  读取控制字段，控制proc模块读取(系统信息/进程信息)
 */ 
struct proc_struct {
	proc_state state;
	protocol_sthread ctrl;

	boolean (*set_state)(struct proc_struct *proc, proc_state state);
	boolean (*set_ctrl)(struct proc_struct *proc, proc_ctrl ctrl);
};


boolean read_proc(struct asthread_struct *asthread);

boolean init_proc(struct proc_struct *proc, 
			 proc_state state, protocol_sthread ctrl);

#endif 
