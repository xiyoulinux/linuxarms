
/*
 * 关于数据定义的
 */

#ifndef _SSINFO_H
#define _SSINFO_H
#include <gtk/gtk.h>
#include "linuxarms.h"
#include "hnet.h"
#include "protocol.h"
#include "hsthread.h"


#define HSSINFO_MAX 80 /* 最大数 */
#define HSSINFO_LINE 10 /* 总的行数 */
#define HSSINFO_SENDLINE 11

struct hssinfo_trans {
	protocol_sthread protocol;
	char buffer[HSSINFO_SENDLINE][HSSINFO_MAX];
};
boolean hssinfo_trans_init(struct hssinfo_trans *hstrans);
boolean hssinfo_trans_set_protocol(struct hssinfo_trans *hstrans, protocol_sthread protocol);
enum {
	SYS_HOSTNAME,
	SYS_RELEASE,
	SYS_KERNEL,
	SYS_MEMFREE,
	SYS_MEMTOTAL,
	SYS_NET,
	SYS_CPUINFO,
	SYS_CPUUSED,
	SYS_CPUHZ,
	SYS_CPULOAD,
	SYS_DISK
};
/*
 * 系统信息显示的相关控件
 */
struct hssinfo_widget {
	GtkWidget *hostname;
	GtkWidget *release;
	GtkWidget *kernel;
	GtkWidget *memory;
	GtkWidget *netrecv;
	GtkWidget *netsend;
	GtkWidget *cpuinfo;
	GtkWidget *cpuused;
	GtkWidget *cpuload;
	GtkWidget *disk;
};

/* 系统信息显示host端的结构体 
 * @widget gtk的定义
 * @socket网络 
 */
struct hssinfo_struct{
	struct hssinfo_widget widget;
	struct hnet_struct *socket;
	struct hssinfo_trans trans;
	
	boolean (*set_protocol)(struct hssinfo_struct *hssinfo, protocol_sthread protocol);
	boolean (*send)(struct hssinfo_struct *hssinfo);
	boolean (*recv)(struct hssinfo_struct *hssinfo);	
};

boolean hssinfo_show_info(struct hssinfo_struct *hssinfo);
boolean hssinfo_init(struct hssinfo_struct *hssinfo, struct hnet_struct *socket);
GtkWidget *create_page_ssinfo(struct linuxarms_struct *linuxarms);

#endif












