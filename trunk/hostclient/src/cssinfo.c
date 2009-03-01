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
#include "error.h"
#include "debug.h"
#define LABEL_TEXT_MAX 128

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

boolean hssinfo_show(struct hssinfo_struct *hssinfo)
{
	char **data, *p;
	char tmp[6][80];
	char buf[LABEL_TEXT_MAX];
	int i, len;

	LINUXARMS_POINTER(hssinfo);
	data = (char **)hssinfo->trans.buffer;

	/* 主机名 */
	snprintf(buf,LABEL_TEXT_MAX,"<b>主机名: %s</b>",data[SYS_HOSTNAME]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.hostname),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.hostname), TRUE);

	/* 发行版本 */
	len = strlen(data[SYS_RELEASE]);
	for (i = 0; i < len; i ++)
		if (data[SYS_RELEASE][i] == '\n' || data[SYS_RELEASE][i] == '\l')
			data[SYS_RELEASE][i] = ' ';
	snprintf(buf,LABEL_TEXT_MAX,"<b>发行版本: %s</b>",data[SYS_RELEASE]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.release), buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.release), TRUE);

	/* 内核 */
	sscanf(data[SYS_KERNEL],"%s %s %s", tmp[0], tmp[1], tmp[2]);
	snprintf(buf, LABEL_TEXT_MAX, "<b>内核版本: %s %s</b>", "Kernel Linux", tmp[1]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.kernel),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.kernel), TRUE);

	/* 内存  */
	sscanf(data[SYS_MEMTOTAL], "%s %s %s", tmp[0], tmp[1], tmp[2]);
	sscanf(data[SYS_MEMFREE], "%s %s %s", tmp[0], tmp[3], tmp[2]);
	snprintf(buf,LABEL_TEXT_MAX,"<b>内存    总量(M): %d |空闲(M): %d</b>",
		atoi(tmp[1]) / 1024, atoi(tmp[3]) / 1024);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.memory),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.memory), TRUE);

	/* 网络 */
	sscanf(data[SYS_NET],"%s%s%s%s%s%s%s%s%s",tmp[1],
		tmp[0],tmp[0],tmp[0],tmp[0],tmp[0],
		tmp[0],tmp[0],tmp[2]);
	snprintf(buf,LABEL_TEXT_MAX,"<b>接收(KB): %s</b>",tmp[1]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.netrecv),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.netrecv), TRUE);

	snprintf(buf,LABEL_TEXT_MAX,"<b>发送(KB): %s</b>",tmp[2]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.netsend),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.netsend), TRUE);

	/* cpu型号 */
	len = sscanf(data[SYS_CPUINFO], "%s %s", tmp[0], tmp[1]);
	p = data[SYS_CPUINFO] + strlen(tmp[0]) + strlen(tmp[1]) + len;
	snprintf(buf,LABEL_TEXT_MAX,"<b>cpu型号%s</b>", p);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.cpuinfo), buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.cpuinfo), TRUE);

	/* cpu 使用  */
	sscanf(data[SYS_CPUUSED],"%s%s%s%s%s",tmp[0],tmp[1],tmp[2],tmp[3], tmp[4]);
	/* max cpu 6*/
	sscanf(data[SYS_CPUMZ], "%s%s", tmp[0],tmp[5]);
	int total = atoi(tmp[1]) + atoi(tmp[2]) + atoi(tmp[3]);
	snprintf(buf,LABEL_TEXT_MAX,"<b>cpu使用率：(%s) %d   主频: %s</b>", "%",
		total / (total + atoi(tmp[4])) * 100,tmp[5]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.cpuused),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.cpuused), TRUE);

	/* cpu负载  */
	sscanf(data[SYS_CPULOAD], "%s%s%s",tmp[0],tmp[1],tmp[2]);
	snprintf(buf,LABEL_TEXT_MAX,"<b>cpu负载: %s  %s  %s</b>",tmp[0],tmp[1],tmp[2]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.cpuload),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.cpuload), TRUE);

	/*  硬盘 */
	snprintf(buf, LABEL_TEXT_MAX,"<b>硬盘    总容量(M): %s |空闲(M): %s</b>",
			data[SYS_DISKTOTAL],data[SYS_DISKFREE]);
	gtk_label_set_text(GTK_LABEL(hssinfo->widget.disk),buf);
	gtk_label_set_use_markup(GTK_LABEL(hssinfo->widget.disk), TRUE);
	
	return TRUE;
}


