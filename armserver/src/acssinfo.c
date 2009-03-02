#define __DEBUG__
#include <stdlib.h>
#include <sys/vfs.h>
#include <string.h>
#include <stdio.h>
#include "assinfo.h"
#include "proc.h"
#include "protocol.h"
#include "asthread.h"
#include "error.h"
#include "debug.h"

#define ASSINFO_LINE 10 /* 总的行数 */
#define ASSINFO_MOVLINE  4 /* 动态的行数 */
#define INIT_NUM 10
static boolean assinfo_set_protocol(struct assinfo_struct *assinfo, protocol_sthread protocol);
static boolean assinfo_send(struct assinfo_struct *assinfo);
static boolean assinfo_recv(struct assinfo_struct *assinfo);
/* 静态数据 */
static struct assinfo_file proc_files[ASSINFO_LINE] = {
	{"nokeyword",  "/proc/sys/kernel/hostname", 0},
	{"nokeyword",  "/etc/issue", 0},
	{"version",    "/proc/version", 0},
	{"MemFree",    "/proc/meminfo", INIT_NUM},
	{"MemTotal",   "/proc/meminfo", 0},
	{"eth0",       "/proc/net/dev", INIT_NUM},
	{"model name", "/proc/cpuinfo", INIT_NUM},
	{"cpu",        "/proc/stat", 0},
	{"cpu MHz",    "/proc/cpuinfo", INIT_NUM},
	{"nokeyword",  "/proc/loadavg", 0}

};
boolean assinfo_init(struct assinfo_struct *assinfo, struct anet_struct *socket)
{
	LINUXARMS_POINTER(assinfo);
	LINUXARMS_POINTER(socket);

	assinfo->pfile = proc_files;
	assinfo_trans_init(&assinfo->trans);
	assinfo->socket = socket;

	assinfo->set_protocol = assinfo_set_protocol;
	assinfo->send = assinfo_send;
	assinfo->recv = assinfo_recv;

	return TRUE;
}

static boolean assinfo_set_protocol(struct assinfo_struct *assinfo, protocol_sthread protocol)
{
	LINUXARMS_POINTER(assinfo);
	return assinfo_trans_set_protocol(&assinfo->trans, protocol);
}
static boolean assinfo_send(struct assinfo_struct *assinfo)
{
	LINUXARMS_POINTER(assinfo);
	return anet_send(assinfo->socket->tcp, (void *)&assinfo->trans,
			 sizeof(struct assinfo_trans));
}
static boolean assinfo_recv(struct assinfo_struct *assinfo)
{
	LINUXARMS_POINTER(assinfo);
	return anet_recv(assinfo->socket->tcp, (void *)&assinfo->trans,
			 sizeof(struct assinfo_trans));
}
boolean assinfo_trans_init(struct assinfo_trans *astrans)
{
	LINUXARMS_POINTER(astrans);
	astrans->protocol = SMAX;
	memset(astrans->buffer, '\0', ASSINFO_SENDLINE * ASSINFO_MAX);
	return TRUE;
}
boolean assinfo_trans_set_protocol(struct assinfo_trans *astrans, protocol_sthread protocol)
{
	LINUXARMS_POINTER(astrans);
	if (!PROTOCOL_IS_STHREAD(protocol)) {
		print_error(EWARNING, "无效的协议");
		astrans->protocol = SMAX;
		return FALSE;
	}
	astrans->protocol = protocol;
	return TRUE;
}

boolean assinfo_read_info(struct asthread_struct *asthread)
{
	struct assinfo_struct *assinfo = asthread->assinfo;
	struct proc_struct *proc = &asthread->proc;

	FILE *fp;
	char temp[ASSINFO_MAX], *buff;
	int count, countline;
	struct statfs disk;
	enum error asyserror;

	asyserror = ESUCCESS;
	if((statfs("/", &disk) == -1)){
		asyserror = ENOINIT;
		goto out;
	}	
	
	for (count = 0; count < ASSINFO_LINE; count++) {
		asyserror = ESUCCESS;
		if (proc->state != CONTINUE) {
			goto out;
		}
		if((fp = fopen(proc_files[count].filename, "r")) == NULL) {
			strcpy(assinfo->trans.buffer[count], "读取错误");
			continue;
		}
		if (strcmp(proc_files[count].keyword, "nokeyword") == 0) {
			buff = fgets(temp,ASSINFO_MAX,fp);
		} else {
			for (countline = 0; countline < proc_files[count].line; countline++) {
				if (fgets(temp,ASSINFO_MAX,fp) == NULL) {
					continue;
				}
				if (proc_files[count].line == INIT_NUM) {
					buff = strstr(temp,proc_files[count].keyword);
					if (buff) {
						proc_files[count].line = countline;
						break;
					}
				}
			} 
			fgets(temp,ASSINFO_MAX,fp);
			buff = strstr(temp,proc_files[count].keyword);
		}
		strcpy(assinfo->trans.buffer[count], buff == NULL ? "获取信息错误" : buff);
	}
	fclose(fp);
	snprintf(assinfo->trans.buffer[ASSINFO_LINE],ASSINFO_MAX,
		 "%d %lu %lu", disk.f_bsize, disk.f_blocks, disk.f_bfree);
	
	assinfo->set_protocol(assinfo, SSUCCESS);
	assinfo->send(assinfo);
	return TRUE;
out:
	assinfo->set_protocol(assinfo, SERROR);
	assinfo->send(assinfo);
	return FALSE;
}

