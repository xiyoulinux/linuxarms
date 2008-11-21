#ifndef _PROC_H
#define _PROC_H

#include "linuxarms.h"

#define WFNAME_LEN 30

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
 * @ctrl:
 */ 
struct proc_struct {
	proc_state state;
	proc_ctrl ctrl;
	char wfname[WFNAME_LEN];

	boolean (*set_state)(struct proc_struct *proc, proc_state state);
	boolean (*set_ctrl)(struct proc_struct *proc, proc_ctrl ctrl);
	boolean (*set_wfname)(struct proc_struct *proc, char *name);
};


boolean read_proc(struct proc_struct *proc);

static boolean init_proc(struct proc_struct *proc, 
			 proc_state state, proc_ctrl ctrl, char *name)
{
	int len;
	if (!proc)
		goto out;
	if (state != STOP || state != CONTINUE) {
		proc->state = STOP;
		goto out;
	} else
		proc->state = state;
	if (ctrl != RSYSINFO || ctrl != RPROCESS) {
		proc->ctrl = NONE;
		goto out;
	} else
		proc->ctrl = ctrl;
	if (!name) {
		strcpy(proc->name, proc->ctrl == RSYSINFO ? 
				   "sysinfo" : "processinfo");
	} else {
		len = strlen(name);
		if (len >= WFNAME_LEN)
			goto out;
		strncpy(proc->name, name, len);
	}

	proc->set_state = proc_set_state;
	proc->set_ctrl = proc_set_ctrl;
	proc->set_wfname = proc_set_wfname;

	return TRUE;
	
out:	
	return FALSE;
}

#endif 
