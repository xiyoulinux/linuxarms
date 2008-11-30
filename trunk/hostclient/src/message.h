#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <gtk/gtk.h>
#include "mwindow.h"

static void gtk_show_message(const char *msg)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window_main), 0,
			GTK_MESSAGE_INFO, GTK_BUTTONS_OK, msg);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
#endif
