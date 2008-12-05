#include "proc.h"
#include "linuxarms.h"
#include "error.h"
#include "debug.h"
#include "asthread.h"

static boolean proc_set_ctrl(struct proc_struct *proc, protocol_sthread ctrl);
static boolean proc_set_state(struct proc_struct *proc, proc_state state);

boolean init_proc(struct proc_struct *proc, 
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

static boolean proc_set_state(struct proc_struct *proc, proc_state state)
{
	if (state != CONTINUE || state != STOP) {
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
	struct proc_struct *proc = &asthread->proc;
	
	if (!proc)
		goto out;
	switch (proc->ctrl) {
	case SYSINFO:
		//read_sysinfo(proc);
		break;
	case SPROCESS:
		//do_show_process(proc);
		break;
	
	default:
		goto out;
	}
	return TRUE;
	
out:
	return FALSE;
}
