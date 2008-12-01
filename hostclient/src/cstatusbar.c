/*
 * linuxarms/hostclient/cstatusbar.c
 * 显示提示信息。最终显示的信息会以动态的方式显示5秒钟，并自动
 * 消失。如果在还没有显示完毕(没有到达5秒)就有新的提示信息要显
 * 示，则清除当前的，显示新的提示信息。
 * Niu Tao<nituao0602@gmail.com>
 */
#include "statusbar.h"
#include "linuxarms.h"
#include "hmthread.h"
#include "error.h"
#include <gtk/gtk.h>

static struct statusbar_struct status;

static boolean statusbar_init(GtkWidget *statusbar)
{
	if (!statusbar)
		print_error(ESYSERR,"statusbar not init");
	status.cid = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "");
	status.mid = gtk_statusbar_push(GTK_STATUSBAR(statusbar),status.cid, "");
	status.statusbar = statusbar;
	status.text = NULL;
	status.time = SHOW_TIMEOUT;
	status.clock = FALSE;
	status.set_text = set_text;
}

static gboolean statusbar_clock(gpointer data)
{
	if (status.time == SHOW_TIMEOUT) {
		gtk_statusbar_remove(GTK_STATUSBAR(status.statusbar),
				status.cid, status.mid);
		status.clock = FALSE;
		return FALSE;
	}
	status.time++;
	if (status.time / 2 == 0)
		gtk_statusbar_push(GTK_STATUSBAR(status.statusbar),
				status.cid,status.text);
	else
		gtk_statusbar_remove(GTK_STATUSBAR(status.statusbar),
				status.cid, status.mid);
	return TRUE;
}

boolean statusbar_set_text(char *text)
{
	if (status.clock) {
		status.clock = FALSE;
		gtk_timeout_remove(status.clock_id);
	}
	if (status.set_text(&status, text)) {
		return FALSE;
	}
	status.time = 0;
	status.clock_id = gtk_timeout_add(CLOCK_TIMEOUT, 
			(GSourceFunc)statusbar_clock, NULL);
	return TRUE;
}
