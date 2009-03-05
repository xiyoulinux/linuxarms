#ifndef _STATUSBAR_H
#define _STATUSBAR_H

#include "linuxarms.h"
#include <gtk/gtk.h>

#define CLOCK_TIMEOUT 500
#define SHOW_TIMEOUT 10
/*
 * @statuabar:	状态栏指针
 * @text:	要显示的信息
 * @lock:	锁，如果当前有信息显示，其为TRUE
 * @time:	显示的时间长短(CLOCK_TIMEOUT / 1000 * SHOW_TIMEOUT)
 */
struct statusbar_struct {
	GtkWidget *statusbar;
	char *text;
	int cid;
	boolean clock;
	int clock_id;
	int time;
};
GtkWidget *create_statusbar(GtkWidget *vbox_main, GtkTooltips *tooltips, 
			    struct linuxarms_struct *linuxarms);
/* 显示系统信息 */
boolean statusbar_set_text(char *text);
boolean statusbar_init(GtkWidget *statusbar);
#endif
