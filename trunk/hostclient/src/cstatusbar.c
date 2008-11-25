#include "statusbar.h"
#include "linuxarms.h"
#include "hmthread.h"
#include "error.h"
#include <gtk/gtk.h>

static struct statusbar_struct status;

boolean statusbar_init(GtkWidget *statusbar)
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

gboolean statusbar_clock(gpointer data)
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
}
