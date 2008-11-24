#ifndef _MWINDOW_H
#define _MWINDOW_H

#include <gtk/gtk.h>
/*
gboolean cb_window_main_button_press_event(GtkWidget *widget,
			GdkEventButton  *event, gpointer user_data);
*/
extern GtkWidget *window_main;

void cb_notebook_switch_page(GtkNotebook *notebook,
			     GtkNotebookPage *page,
			     guint page_num, gpointer user_data);

#endif
