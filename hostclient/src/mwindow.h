#ifndef _MWINDOW_H
#define _MWINDOW_H

#include <gtk/gtk.h>
#include "linuxarms.h"
#define ICON_WINDOW "ico-window.png"
struct mwindow_struct {
	GtkWidget *window;
	GtkWidget *notebook;
	GtkWidget *toolbar;
	GtkWidget *frame;
};

void create_window_main(struct linuxarms_struct *linuxarms);
void cb_notebook_switch_page(GtkNotebook *notebook, GtkNotebookPage *page,
			     guint page_num, gpointer user_data);
boolean gtk_window_main_set_sensitive(struct linuxarms_struct *linuxarms);
void cb_linuxarms_window_main_close(GtkObject *object, gpointer user_data);
gboolean cb_window_main_button_press(GtkWidget *widget,
		        GdkEventButton *event, gpointer user_data);
#endif
