/*
 * linuxarms/hostclient/src/cssinfo.c
 * LiNing <gforln9543@gmail.com>
 */
#define __DEBUG__
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>

#include "linuxarms.h"
#include "ssinfo.h"
#include "protocol.h"
#include "fview.h"
#include "error.h"
#include "debug.h"
#define LABEL_TEXT_MAX 128

static unsigned long cpu_jiff_user_prev = 0, cpu_jiff_nice_prev = 0, 
		     cpu_jiff_system_prev = 0, cpu_jiff_idle_prev = 0, cpu_jiff_prev = 0;
static unsigned long cpu_jiff_user_new, cpu_jiff_nice_new, 
		     cpu_jiff_system_new, cpu_jiff_idle_new, cpu_jiff_new;
static boolean hssinfo_set_protocol(struct hssinfo_struct *hssinfo, protocol_sthread protocol);
static boolean hssinfo_send(struct hssinfo_struct *hssinfo);
static boolean hssinfo_recv(struct hssinfo_struct *hssinfo);

boolean hssinfo_init(struct hssinfo_struct *hssinfo, struct hnet_struct *socket)
{
	LINUXARMS_POINTER(hssinfo);
	LINUXARMS_POINTER(socket);

	hssinfo_trans_init(&hssinfo->trans);
	hssinfo->socket = socket;

	hssinfo->set_protocol = hssinfo_set_protocol;
	hssinfo->send = hssinfo_send;
	hssinfo->recv = hssinfo_recv;

	return TRUE;
}
static boolean hssinfo_set_protocol(struct hssinfo_struct *hssinfo, protocol_sthread protocol)
{
	LINUXARMS_POINTER(hssinfo);
	return hssinfo_trans_set_protocol(&hssinfo->trans, protocol);
}
static boolean hssinfo_send(struct hssinfo_struct *hssinfo)
{
	LINUXARMS_POINTER(hssinfo);
	return hnet_send(hssinfo->socket->tcp, (void *)&hssinfo->trans,
			 sizeof(struct hssinfo_trans));
}
static boolean hssinfo_recv(struct hssinfo_struct *hssinfo)
{
	LINUXARMS_POINTER(hssinfo);
	return hnet_recv(hssinfo->socket->tcp, (void *)&hssinfo->trans,
			 sizeof(struct hssinfo_trans));
}
boolean hssinfo_trans_init(struct hssinfo_trans *hstrans)
{
	LINUXARMS_POINTER(hstrans);
	hstrans->protocol = SMAX;
	memset(hstrans->buffer, '\0', HSSINFO_SENDLINE * HSSINFO_MAX);
	return TRUE;
}
boolean hssinfo_trans_set_protocol(struct hssinfo_trans *hstrans, protocol_sthread protocol)
{
	LINUXARMS_POINTER(hstrans);
	if (!PROTOCOL_IS_STHREAD(protocol)) {
		print_error(EWARNING, "无效的协议");
		hstrans->protocol = SMAX;
		return FALSE;
	}
	hstrans->protocol = protocol;
	return TRUE;
}
boolean hssinfo_show_info(struct hssinfo_struct *hssinfo)
{
	char *p;
	char tmp[6][80];
	char buf[LABEL_TEXT_MAX];
	int len;
	//long bsize;
	unsigned long mtotal, mfree, mbuffers, mcached;
	float bsize, blocks, bfree;
	struct hssinfo_trans *hstrans;
	float cpu_time;

	LINUXARMS_POINTER(hssinfo);
	hstrans = &hssinfo->trans;
	if (!hssinfo->recv(hssinfo)) {
		print_error(EWARNING, "显示系统信息中，接收数据错误");
		return FALSE;
	}
	if (hstrans->protocol != SSUCCESS)
		return FALSE;
	debug_where();
	/* 主机名 */
	snprintf(buf,LABEL_TEXT_MAX,"<b>主机名: %s</b>",hstrans->buffer[SYS_HOSTNAME]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.hostname),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.hostname), TRUE);

	/* 发行版本 */
	len = strlen(hstrans->buffer[SYS_RELEASE]);
	p = strstr(hstrans->buffer[SYS_RELEASE], "\\n");
	if (p) {
		*p = ' ';
		*(p + 1) = ' ';
	}
	p = strstr(hstrans->buffer[SYS_RELEASE], "\\l");
	if (p) {
		*p = ' ';
		*(p + 1) = ' ';
	}
	snprintf(buf,LABEL_TEXT_MAX,"<b>发行版本: %s</b>",hstrans->buffer[SYS_RELEASE]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.release), buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.release), TRUE);

	/* 内核 */
	sscanf(hstrans->buffer[SYS_KERNEL],"%s %s %s", tmp[0], tmp[1], tmp[2]);
	snprintf(buf, LABEL_TEXT_MAX, "<b>内核版本: %s %s</b>", "Kernel Linux", tmp[1]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.kernel),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.kernel), TRUE);

	/* 内存  */
	mtotal = mfree = mbuffers = mcached = 0;
	sscanf(hstrans->buffer[SYS_MEMTOTAL], "%s %lu %s", tmp[0], &mtotal, tmp[0]);
	sscanf(hstrans->buffer[SYS_MEMFREE], "%s %lu %s\n%s %lu %s\n%s %lu", 
			tmp[0], &mfree, tmp[0], tmp[0], &mbuffers, tmp[0], tmp[0], &mcached);
	snprintf(buf,LABEL_TEXT_MAX,"<b>内存  总量: %lu MB  | 空闲: %lu MB</b>",
		mtotal / 1024, (mfree + mbuffers + mcached) / 1024);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.memory),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.memory), TRUE);

	/* 网络 */
	sscanf(hstrans->buffer[SYS_NET],"%s%s%s%s%s%s%s%s%s",tmp[1],
		tmp[0],tmp[0],tmp[0],tmp[0],tmp[0],
		tmp[0],tmp[0],tmp[2]);
	p = tmp[1] + strlen("eth0:");
	snprintf(buf,LABEL_TEXT_MAX,"<b>接收: %ld MB</b>",atol(p) / 1024 / 1024);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.netrecv),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.netrecv), TRUE);

	snprintf(buf,LABEL_TEXT_MAX,"<b>发送: %ld MB</b>",atol(tmp[2]) / 1024 / 1024);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.netsend),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.netsend), TRUE);

	/* cpu型号 */
	len = sscanf(hstrans->buffer[SYS_CPUINFO], "%s %s", tmp[0], tmp[1]);
	p = hstrans->buffer[SYS_CPUINFO];
	p = p + strlen(tmp[0]) + strlen(tmp[1]) + len;
	snprintf(buf,LABEL_TEXT_MAX,"<b>cpu型号 %s</b>", p);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.cpuinfo), buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.cpuinfo), TRUE);

	/* cpu 使用  */
	sscanf(hstrans->buffer[SYS_CPUUSED],"%s %lu %lu %lu %lu",tmp[0], &cpu_jiff_user_new,
	       &cpu_jiff_nice_new, &cpu_jiff_system_new, &cpu_jiff_idle_new);
	cpu_jiff_prev = cpu_jiff_user_prev + cpu_jiff_nice_prev + cpu_jiff_system_prev;
	cpu_jiff_new = cpu_jiff_user_new + cpu_jiff_nice_new + cpu_jiff_system_new;
	cpu_time = (float)(cpu_jiff_new - cpu_jiff_prev) / 
		   (float)((cpu_jiff_new + cpu_jiff_idle_new) - (cpu_jiff_prev + cpu_jiff_idle_prev)) * 100.0;
	cpu_jiff_user_prev = cpu_jiff_user_new;
	cpu_jiff_nice_prev = cpu_jiff_nice_new;
	cpu_jiff_system_prev = cpu_jiff_system_new;
	cpu_jiff_idle_prev = cpu_jiff_idle_new;
	/* max cpu 6*/
	len = sscanf(hstrans->buffer[SYS_CPUHZ], "%s %s", tmp[0],tmp[5]);
	p = hstrans->buffer[SYS_CPUHZ] + strlen(tmp[0]) + strlen(tmp[5]) + len;
	len = strlen(p);
	if (p[len -1] == '\n')
		p[len - 1] = ' ';
	//debug_print("CPU:  %s %s %s %s %s\n", tmp[0], tmp[1], tmp[2], tmp[3], p);
	snprintf(buf,LABEL_TEXT_MAX,"<b>cpu使用率： %.2f%s   主频:%s Mz</b>",
		 cpu_time, "%", p);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.cpuused),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.cpuused), TRUE);

	/* cpu负载  */
	sscanf(hstrans->buffer[SYS_CPULOAD], "%s%s%s",tmp[0],tmp[1],tmp[2]);
	snprintf(buf,LABEL_TEXT_MAX,"<b>cpu负载: %s  %s  %s</b>",tmp[0],tmp[1],tmp[2]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.cpuload),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.cpuload), TRUE);

	/*  硬盘 */
	sscanf(hstrans->buffer[SYS_DISK], "%f %f %f", &bsize, &blocks, &bfree);
	blocks = blocks * bsize;
	bfree = bfree * bsize;
	if (blocks < FSIZE_KB) {
		snprintf(buf, LABEL_TEXT_MAX,"<b>硬盘 总容量: %.2f B |空闲: %.2f B</b>",
			blocks, bfree);
	} else if (blocks < FSIZE_MB) {
		snprintf(buf, LABEL_TEXT_MAX,"<b>硬盘 总容量: %d.%d KB |空闲: %d.%d KB</b>",
			_fsize_kb((int)blocks), __fsize_kb((int)blocks), 
			_fsize_kb((int)bfree), __fsize_kb((int)bfree));
	} else if (blocks < FSIZE_GB) {
		snprintf(buf, LABEL_TEXT_MAX,"<b>硬盘 总容量: %d.%d MB |空闲: %d.%d MB</b>",
			_fsize_mb((int)blocks), __fsize_mb((int)blocks), 
			_fsize_mb((int)bfree), __fsize_mb((int)bfree));
	} else if (blocks < FSIZE_GB * 1024.0) {
		snprintf(buf, LABEL_TEXT_MAX,"<b>硬盘 总容量: %.0f.%.0f GB |空闲: %.0f.%.0f GB</b>",
			_fsize_gb(blocks), __fsize_gb(blocks), _fsize_gb(bfree), __fsize_gb(bfree));
	}
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.disk),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.disk), TRUE);
	return TRUE;
}


