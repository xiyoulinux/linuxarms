#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "statusbar.h"
#include "mwindow.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
	g_object_set_data_full(G_OBJECT(component), name, \
	gtk_widget_ref(widget), (GDestroyNotify)gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
	g_object_set_data(G_OBJECT(component), name, widget)
  
GtkWidget *create_statusbar(GtkWidget *vbox_main)
{
	GtkWidget *statusbar;

	statusbar = gtk_statusbar_new();
	gtk_widget_show (statusbar);
	gtk_box_pack_start(GTK_BOX(vbox_main), statusbar, FALSE, FALSE, 0);     
	GLADE_HOOKUP_OBJECT(window_main, statusbar, "statusbar");
	
	return statusbar;
}
