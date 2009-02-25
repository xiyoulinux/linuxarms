/*
 * linuxarms/armserver/src/acsasprocess.c
 * 系统进程信息显示相关函数
 * Niu Tao<niutao0602@gmail.com>
 */
//#define __DEBUG__

#include "linuxarms.h"
#include "asprocess.h"
#include "amthread.h"
#include "asthread.h"
#include "protocol.h"
#include "proc.h"
#include "debug.h"
#include "error.h"

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/socket.h>


static boolean asprocess_send_info(struct asprocess_struct *asprocess);
static boolean asprocess_recv_info(struct asprocess_struct *asprocess);
static boolean asprocess_set_protocol(struct asprocess_struct *asprocess, 
					protocol_sthread protocol);
static char p_cmd[PROCESS_NAME_LEN];
static char user[PROCESS_USER_LEN];
static char p_state;
static int p_pid;
static int p_ppid, p_pgrp, p_session, p_tty_num, p_tpgid;
static unsigned long p_flags, p_min_flt, p_cmin_flt, p_maj_flt, p_cmaj_flt, p_utime, p_stime;
static long p_cutime, p_cstime, p_priority, p_nice, p_timeout, p_alarm;
static unsigned long p_start_time, p_vsize;
static long p_rss;
static unsigned long p_rss_rlim, p_start_code, p_end_code, p_start_stack;
static char buf[800];

/*
 * 进程信息显示主调模块
 */
boolean do_show_asprocess(struct asthread_struct *asthread)
{
	/*
	 * 调用其他模块完成读取进程信息，发送进程信息，
	 * 接收反馈信息等。
	 */
	struct asprocess_struct *asprocess = asthread->asprocess;
	struct dirent *ent;
	DIR *dir;
	dir = opendir("/proc");
	debug_where();
	while ((ent = readdir(dir))) {
		if (asthread->proc.state == STOP)
			goto out;
		if(*ent->d_name < '0' || *ent->d_name > '9') 
			continue;
		if (!asprocess_read_info(asprocess, atoi(ent->d_name)))
			continue;
		asprocess->set_protocol(asprocess, SPROCESS);
		asprocess->send(asprocess);
		debug_print("%s\t%d\t%s\t%c\t%f\t%f\n", asprocess->trans.name, 
				asprocess->trans.pid, asprocess->trans.user,
				asprocess->trans.state, asprocess->trans.cpu,
				asprocess->trans.mem);
		/* 接收反馈信息，如果没有成功接收，则不再发送 */
		/*asprocess->recv(asprocess);
		if (asprocess->trans.protocol != SRECVSUC)
			break;
		*/
	}
out:
	closedir(dir);
	debug_where();
	asprocess->set_protocol(asprocess, SSENDALL);
	asprocess->send(asprocess);
	return TRUE;
}

boolean asprocess_init(struct asprocess_struct *asprocess,
			int *kill, struct anet_struct *socket)
{
	LINUXARMS_POINTER(asprocess);
	LINUXARMS_POINTER(kill);
	LINUXARMS_POINTER(socket);
	asprocess_trans_init(&asprocess->trans);
	asprocess->socket = socket;
	asprocess->set_protocol = asprocess_set_protocol;
	asprocess->send = asprocess_send_info;
	asprocess->recv = asprocess_recv_info;
	asprocess->kill = kill;
	asprocess->all = FALSE;
	return TRUE;
}
/*
 * 发送进程信息函数
 * @asthread:  
 */
static boolean asprocess_send_info(struct asprocess_struct *asprocess)
{
	LINUXARMS_POINTER(asprocess);
	return anet_send(asprocess->socket->tcp, (void *)&asprocess->trans,
			sizeof(struct asprocess_trans));
}

/*
 * 接收hostclient的反馈信息（是否成功接收完毕）
 */
static boolean asprocess_recv_info(struct asprocess_struct *asprocess)
{
	LINUXARMS_POINTER(asprocess);
	return anet_recv(asprocess->socket->tcp, (void *)&asprocess->trans,
			sizeof(struct asprocess_trans));
}
/*
 * 从proc文件系统中读取进程信息
 */
boolean asprocess_read_info(struct asprocess_struct *asprocess, int pid)
{
	char path[32];
	int num;
	int fd;
	char *tmp;
	struct passwd *pwd;
	struct stat sb;
	char *p;
	LINUXARMS_POINTER(asprocess);
	snprintf(path, 32, "/proc/%d/stat", pid);
	if ( (fd = open(path, O_RDONLY, 0) ) == -1 ) 
		return FALSE;
	num = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	p = strrchr(path, '/');
	*p = '\0';
	stat(path, &sb);
	pwd = getpwuid(sb.st_uid);
	if (!asprocess->all && strcmp(pwd->pw_name, login_user) != 0)
		return FALSE;
	strncpy(user, pwd->pw_name, PROCESS_USER_LEN);
	if(num < 80) 
		return FALSE;
	buf[num] = '\0';
	tmp = strrchr(buf, ')');	 
	*tmp = '\0';				 
	memset(p_cmd, 0, sizeof(p_cmd));
	sscanf(buf, "%d (%15c", &p_pid, p_cmd); 
	num = sscanf(tmp + 2,				 
	   "%c "
	   "%d %d %d %d %d "
	   "%lu %lu %lu %lu %lu %lu %lu "
	   "%ld %ld %ld %ld %ld %ld "
	   "%lu %lu "
	   "%ld "
	   "%lu %lu %lu %lu",
	   &p_state,
	   &p_ppid, &p_pgrp, &p_session, &p_tty_num, &p_tpgid,
	   &p_flags, &p_min_flt, &p_cmin_flt, &p_maj_flt, &p_cmaj_flt, &p_utime, &p_stime,
	   &p_cutime, &p_cstime, &p_priority, &p_nice, &p_timeout, &p_alarm,
	   &p_start_time, &p_vsize,
	   &p_rss,
	   &p_rss_rlim, &p_start_code, &p_end_code, &p_start_stack
	);
	p_vsize /= 1024;
	// p_rss *= (PAGE_SIZE/1024);
	if(p_pid != pid) 
		return FALSE;
	strcpy(asprocess->trans.name, p_cmd);
	asprocess->trans.pid = p_pid;
	strcpy(asprocess->trans.user, user);
	asprocess->trans.state = p_state;
	asprocess->trans.cpu = 0.0;
	asprocess->trans.mem = 0,0;
	return TRUE;
}

boolean asprocess_trans_init(struct asprocess_trans *astrans)
{
	LINUXARMS_POINTER(astrans);
	astrans->protocol = SMAX;
	memset(astrans->name, '\0', PROCESS_NAME_LEN);
	astrans->pid = -1;
	memset(astrans->user, '\0', PROCESS_USER_LEN);
	astrans->state = 'S';
	astrans->cpu = 0.0;
	astrans->mem = 0.0;
	return TRUE;
}
boolean asprocess_trans_set_protocol(struct asprocess_trans *astrans, 
					protocol_sthread protocol)
{
	LINUXARMS_POINTER(astrans);
	if (!PROTOCOL_IS_STHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		astrans->protocol = SMAX;
		return FALSE;
	}
	astrans->protocol = protocol;
	return TRUE;
}
boolean asprocess_trans_set_data(struct asprocess_trans *astrans, char *name,
				int pid, char *user, char state, float cpu, float mem)
{
	boolean ret = TRUE;

	LINUXARMS_POINTER(astrans);
	if (!name || strlen(name) == 0) {
		strcpy(astrans->name, "noname");
		ret = FALSE;
	} else {
		strcpy(astrans->name, name);
	}
	if (pid < 0) {
		astrans->pid = -1;
		ret = FALSE;
	} else {
		astrans->pid = pid;
	}
	if (!user || strlen(user) == 0) {
		strcpy(astrans->user, "unkown");
		ret = FALSE;
	} else {
		strcpy(astrans->user, user);
	}
	astrans->state = state;
	if (cpu < 0) {
		astrans->cpu = 0;
		ret = FALSE;
	} else {
		astrans->cpu = cpu;
	}
	if (mem < 0) {
		astrans->mem = 0;
		ret = FALSE;
	} else {
		astrans->mem = mem;
	}
	return ret;
}

static boolean asprocess_set_protocol(struct asprocess_struct *asprocess, 
					protocol_sthread protocol)
{
	LINUXARMS_POINTER(asprocess);
	return asprocess_trans_set_protocol(&asprocess->trans, protocol);
}
