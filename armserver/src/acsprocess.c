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

