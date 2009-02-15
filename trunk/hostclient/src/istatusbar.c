#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "statusbar.h"
#include "mwindow.h"

GtkWidget *create_statusbar(GtkWidget *vbox_main)
{
	GtkWidget *statusbar;

	statusbar = gtk_statusbar_new();
	gtk_widget_show (statusbar);
	gtk_box_pack_start(GTK_BOX(vbox_main), statusbar, FALSE, FALSE, 0);     
	statusbar_init(statusbar);
	return statusbar;
}
