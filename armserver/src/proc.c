#include "proc.h"
#include "linuxarms.h"


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

boolean read_proc(struct asthread_struct *asthread)
{
	
	if (!proc)
		goto out;
	switch (proc->ctrl) {
	case RSYSINFO:
		read_sysinfo(proc);
		break;
	case RPROCESS:
		do_show_process(proc);
		break;
	
	case NONE:
	default:
		goto out;
	}
	return TRUE;
	
out:
	return FALSE;
}
#endif 
