//#define __DEBUG__
#include "proc.h"
#include "linuxarms.h"
#include "error.h"
#include "debug.h"
#include "asthread.h"
#include "assinfo.h"
#include "asprocess.h"

static boolean proc_set_ctrl(struct proc_struct *proc, protocol_sthread ctrl);
static boolean proc_set_state(struct proc_struct *proc, proc_state state);

boolean proc_init(struct proc_struct *proc, 
			 proc_state state, protocol_sthread ctrl)
{
	boolean ret = TRUE;

	LINUXARMS_POINTER(proc);
	if (state != STOP && state != CONTINUE) {
		proc->state = STOP;
		ret = FALSE;
	} else {
		proc->state = state;
	}
	if (ctrl < 0 || ctrl > SMAX) {
		proc->ctrl = SMAX;
		ret = FALSE;
	} else {
		proc->ctrl = ctrl;
	}

	proc->set_state = proc_set_state;
	proc->set_ctrl  = proc_set_ctrl;

	return ret;
}

static boolean proc_set_state(struct proc_struct *proc, proc_state state)
{
	LINUXARMS_POINTER(proc);
	if (state != CONTINUE && state != STOP) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		proc->state =STOP;
	} else {
		proc->state = state;
	}
	return TRUE;
}
static boolean proc_set_ctrl(struct proc_struct *proc, protocol_sthread ctrl)
{
	LINUXARMS_POINTER(proc);
	if (!PROTOCOL_IS_STHREAD(ctrl)) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		proc->ctrl = SMAX;
	} else {
		proc->ctrl = ctrl;
	}
	return TRUE;
}

boolean read_proc(struct asthread_struct *asthread)
{
	struct proc_struct *proc;
	
	LINUXARMS_POINTER(asthread);
	proc = &asthread->proc;
	switch (proc->ctrl) {
	case SSYSINFO:
		//read_sysinfo(asthread);
		break;
	case SPROCESS:
		debug_where();
		do_show_asprocess(asthread);
		break;
	default:
		goto out;
	}
	return TRUE;
out:
	return FALSE;
}
