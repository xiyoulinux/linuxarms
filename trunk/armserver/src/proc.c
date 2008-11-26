#include "proc.h"
#include "linuxarms.h"

boolean read_proc(struct proc_struct *proc)
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


