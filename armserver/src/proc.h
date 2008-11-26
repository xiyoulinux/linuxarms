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

static boolean init_proc(struct proc_struct *proc, 
			 proc_state state, protocol_sthread ctrl)
{
	if (!proc)
		goto out;
	if (state != STOP || state != CONTINUE) {
		proc->state = STOP;
		goto out;
	} else
		proc->state = state;
	if (ctrl < 0 || ctrl >SMAX) {
		proc->ctrl = SMAX;
		goto out;
	} else
		proc->ctrl = ctrl;

	proc->set_state = proc_set_state;
	proc->set_ctrl = proc_set_ctrl;

	return TRUE;
	
out:	
	return FALSE;
}

#endif 
