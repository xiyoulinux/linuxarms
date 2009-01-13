/*
 * linuxarms/armserver/src/acsprocess.c
 * 系统进程信息显示相关函数
 * Niu Tao<niutao0602@gmail.com>
 */
#include "asprocess.h"
#include "asthread.h"
#include "proc.h"
#include <sys/types.h>
#include <dirent.h>

/*
 * 进程信息显示主调模块
 */
boolean do_show_process(struct asthread_struct *asthread)
{
	/*
	 * 调用其他模块完成读取进程信息，发送进程信息，
	 * 接收反馈信息等。
	 */
	struct asprocess_struct *asprocess = asthread->asprocess;
	struct dirent *ent;
	DIR *dir;

	dir = opendir("/proc");
	while ((ent = readdir(dir))) {
		if(*ent->d_name < '0' || *ent->d_name > '9') 
			continue;
		process_read_info(asprocess, atoi(ent->d_name));
		asprocess->send(asthread->socket.tcp, asprocess);
		/* 接收反馈信息，如果没有成功接收，则不再发送 */
		/*asprocess->recv(asthread->socket.tcp, asprocess);
		if (asprocess->trans.protocol != SRECVSUC)
			break;
		*/
		if (asthread->proc.state == STOP) {
			asprocess->trans.protocol = SSENDALL;
			asprocess->send(asprocess);
			break;
		}
	}
	closedir(dir);
}

boolean process_init(struct asprocess_struct *asprocess)
{
	if (!asprocess)
		return FALSE;
	asprocess->send = process_send_info;
	asprocess->recv = process_recv_info;
	return FALSE;
}
/*
 * 发送进程信息函数
 * @asthread:  
 */
boolean process_send_info(int tcp,struct asprocess_struct *asprocess)
{
	return anet_send(rcp, &asprocess->trans,
			sizeof(struct asprocess_struct));
}

/*
 * 接收hostclient的反馈信息（是否成功接收完毕）
 */
boolean process_recv_info(int tcp, struct asprocess_struct *asprocess)
{
	return anet_recv(rcp, &asprocess->trans,
			sizeof(struct asprocess_struct));
}
/*
 * 从proc文件系统中读取进程信息
 */
boolean process_read_info(struct asprocess_struct *asprocess,int pid)
{
}
/*
 * 杀死进程处理函数
 */
boolean kill_process(struct asprocess_struct *asprocess)
{
}

/* proc/sysinfo.c*/
int uptime(double *restrict uptime_secs, double *restrict idle_secs) {
    double up=0, idle=0;
    char *restrict savelocale;

    FILE_TO_BUF(UPTIME_FILE,uptime_fd);
    savelocale = setlocale(LC_NUMERIC, NULL);
    setlocale(LC_NUMERIC,"C");
    if (sscanf(buf, "%lf %lf", &up, &idle) < 2) {
        setlocale(LC_NUMERIC,savelocale);
        fputs("bad data in " UPTIME_FILE "\n", stderr);
            return 0;
    }   
    setlocale(LC_NUMERIC,savelocale);
    SET_IF_DESIRED(uptime_secs, up);
    SET_IF_DESIRED(idle_secs, idle);
    return up;  /* assume never be zero seconds in practice */
}

//seconds_since_boot = /proc/uptime[第一个值]
/* ps/output.c */
static int pr_pcpu(char *restrict const outbuf, const proc_t *restrict const pp){																 
	unsigned long long total_time;	 /* jiffies used by this process */
	unsigned pcpu = 0;							 /* scaled %cpu, 999 means 99.9% */
	unsigned long long seconds;			/* seconds of process life */
	
	total_time = pp->utime + pp->stime;
	if(include_dead_children)
		 total_time += (pp->cutime + pp->cstime);
	seconds = seconds_since_boot - pp->start_time / Hertz;
	if(seconds) 
		pcpu = (total_time * 1000ULL / Hertz) / seconds;
	if (pcpu > 999U)
		return snprintf(outbuf, COLWID, "%u", pcpu/10U);
	return snprintf(outbuf, COLWID, "%u.%u", pcpu/10U, pcpu%10U);
}

/* pp->vm_rss * 1000 would overflow on 32-bit systems with 64 GB memory */
static int pr_pmem(char *restrict const outbuf, const proc_t *restrict const pp){                                 
  unsigned long pmem = 0; 
  pmem = pp->vm_rss * 1000ULL / kb_main_total;
  if (pmem > 999) pmem = 999; 
  return snprintf(outbuf, COLWID, "%2u.%u", (unsigned)(pmem/10), (unsigned)(pmem%10));
}
static int do_task_stat(struct seq_file *m, struct pid_namespace *ns,
			struct pid *pid, struct task_struct *task, int whole)
{
	unsigned long vsize, eip, esp, wchan = ~0UL;
	long priority, nice;
	int tty_pgrp = -1, tty_nr = 0;
	sigset_t sigign, sigcatch;
	char state;
	pid_t ppid = 0, pgid = -1, sid = -1;
	int num_threads = 0;
	struct mm_struct *mm;
	unsigned long long start_time;
	unsigned long cmin_flt = 0, cmaj_flt = 0;
	unsigned long  min_flt = 0,  maj_flt = 0;
	cputime_t cutime, cstime, utime, stime;
	cputime_t cgtime, gtime;
	unsigned long rsslim = 0;
	char tcomm[sizeof(task->comm)];
	unsigned long flags;

	state = *get_task_state(task);
	vsize = eip = esp = 0;
	mm = get_task_mm(task);
	if (mm) {
		vsize = task_vsize(mm);
		eip = KSTK_EIP(task);
		esp = KSTK_ESP(task);
	}

	get_task_comm(tcomm, task);

	sigemptyset(&sigign);
	sigemptyset(&sigcatch);
	cutime = cstime = utime = stime = cputime_zero;
	cgtime = gtime = cputime_zero;

	if (lock_task_sighand(task, &flags)) {
		struct signal_struct *sig = task->signal;

		if (sig->tty) {
			struct pid *pgrp = tty_get_pgrp(sig->tty);
			tty_pgrp = pid_nr_ns(pgrp, ns);
			put_pid(pgrp);
			tty_nr = new_encode_dev(tty_devnum(sig->tty));
		}

		num_threads = atomic_read(&sig->count);
		collect_sigign_sigcatch(task, &sigign, &sigcatch);

		cmin_flt = sig->cmin_flt;
		cmaj_flt = sig->cmaj_flt;
		cutime = sig->cutime;
		cstime = sig->cstime;
		cgtime = sig->cgtime;
		rsslim = sig->rlim[RLIMIT_RSS].rlim_cur;

		/* add up live thread stats at the group level */
		if (whole) {
			struct task_struct *t = task;
			do {
				min_flt += t->min_flt;
				maj_flt += t->maj_flt;
				utime = cputime_add(utime, task_utime(t));
				stime = cputime_add(stime, task_stime(t));
				gtime = cputime_add(gtime, task_gtime(t));
				t = next_thread(t);
			} while (t != task);

			min_flt += sig->min_flt;
			maj_flt += sig->maj_flt;
			utime = cputime_add(utime, sig->utime);
			stime = cputime_add(stime, sig->stime);
			gtime = cputime_add(gtime, sig->gtime);
		}

		sid = task_session_nr_ns(task, ns);
		ppid = task_tgid_nr_ns(task->real_parent, ns);
		pgid = task_pgrp_nr_ns(task, ns);

		unlock_task_sighand(task, &flags);
	}

	if (!whole || num_threads < 2)
		wchan = get_wchan(task);
	if (!whole) {
		min_flt = task->min_flt;
		maj_flt = task->maj_flt;
		utime = task_utime(task);
		stime = task_stime(task);
		gtime = task_gtime(task);
	}

	/* scale priority and nice values from timeslices to -20..20 */
	/* to make it look like a "normal" Unix priority/nice value  */
	priority = task_prio(task);
	nice = task_nice(task);

	/* Temporary variable needed for gcc-2.96 */
	/* convert timespec -> nsec*/
	start_time =
		(unsigned long long)task->real_start_time.tv_sec * NSEC_PER_SEC
				+ task->real_start_time.tv_nsec;
	/* convert nsec -> ticks */
	start_time = nsec_to_clock_t(start_time);

	seq_printf(m, "%d (%s) %c %d %d %d %d %d %u %lu \
%lu %lu %lu %lu %lu %ld %ld %ld %ld %d 0 %llu %lu %ld %lu %lu %lu %lu %lu \
%lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld\n",
		pid_nr_ns(pid, ns),
		tcomm,
		state,
		ppid,
		pgid,
		sid,
		tty_nr,
		tty_pgrp,
		task->flags,
		min_flt,
		cmin_flt,
		maj_flt,
		cmaj_flt,
		cputime_to_clock_t(utime),
		cputime_to_clock_t(stime),
		cputime_to_clock_t(cutime),
		cputime_to_clock_t(cstime),
		priority,
		nice,
		num_threads,
		start_time,
		vsize,
		mm ? get_mm_rss(mm) : 0,
		rsslim,
		mm ? mm->start_code : 0,
		mm ? mm->end_code : 0,
		mm ? mm->start_stack : 0,
		esp,
		eip,
		/* The signal information here is obsolete.
		 * It must be decimal for Linux 2.0 compatibility.
		 * Use /proc/#/status for real-time signals.
		 */
		task->pending.signal.sig[0] & 0x7fffffffUL,
		task->blocked.sig[0] & 0x7fffffffUL,
		sigign      .sig[0] & 0x7fffffffUL,
		sigcatch    .sig[0] & 0x7fffffffUL,
		wchan,
		0UL,
		0UL,
		task->exit_signal,
		task_cpu(task),
		task->rt_priority,
		task->policy,
		(unsigned long long)delayacct_blkio_ticks(task),
		cputime_to_clock_t(gtime),
		cputime_to_clock_t(cgtime));
	if (mm)
		mmput(mm);
	return 0;
}
