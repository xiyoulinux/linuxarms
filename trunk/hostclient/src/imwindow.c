#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "mwindow.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)
  
GtkWidget *create_window_main()
{
	
	GtkWidget *window_main;
	GtkWidget *vbox_main;
	 GtkWidget *notebook_main;
  GtkWidget *hbox_notebook;
	  GtkAccelGroup *accel_group;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  accel_group = gtk_accel_group_new ();
	window_main = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (window_main, 800, 650);
	GTK_WIDGET_SET_FLAGS (window_main, GTK_CAN_FOCUS);
	gtk_window_set_title (GTK_WINDOW (window_main), _("Linux ARMS(hostclient)"));
	gtk_window_set_position (GTK_WINDOW (window_main), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size (GTK_WINDOW (window_main), 800, 650);
	
	  vbox_main = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox_main);
  gtk_container_add (GTK_CONTAINER (window_main), vbox_main);
  
  notebook_main = gtk_notebook_new ();
  gtk_widget_show (notebook_main);
  gtk_box_pack_start (GTK_BOX (vbox_main), notebook_main, TRUE, TRUE, 0);

  hbox_notebook = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox_notebook);
  gtk_container_add (GTK_CONTAINER (notebook_main), hbox_notebook);
  GTK_WIDGET_SET_FLAGS (hbox_notebook, GTK_CAN_FOCUS);
    g_signal_connect ((gpointer) window_main, "destroy",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
                    
                      g_signal_connect ((gpointer) notebook_main, "switch_page",
                    G_CALLBACK (cb_notebook_switch_page),
                    NULL);
                      GLADE_HOOKUP_OBJECT_NO_REF (window_main, window_main, "window_main");
  GLADE_HOOKUP_OBJECT (window_main, vbox_main, "vbox_main");
    GLADE_HOOKUP_OBJECT (window_main, notebook_main, "notebook_main");
  GLADE_HOOKUP_OBJECT (window_main, hbox_notebook, "hbox_notebook");
	     GLADE_HOOKUP_OBJECT_NO_REF (window_main, tooltips, "tooltips");
	     
	       gtk_widget_grab_focus (notebook_main);
  gtk_window_add_accel_group (GTK_WINDOW (window_main), accel_group); 
}
