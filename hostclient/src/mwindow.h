#ifndef _MWINDOW_H
#define _MWINDOW_H

#include <gtk/gtk.h>
#include "linuxarms.h"

struct mwindow_struct {
	GtkWidget *window;
	GtkWidget *notebook;
};
GtkWidget *create_window_main(struct linuxarms_struct *linuxarms);
void cb_notebook_switch_page(GtkNotebook *notebook,
			     GtkNotebookPage *page,
			     guint page_num, gpointer user_data);
boolean gtk_window_main_set_sensitive(struct linuxarms_struct *linuxarms);
#endif
