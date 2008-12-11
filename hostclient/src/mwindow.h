#ifndef _MWINDOW_H
#define _MWINDOW_H

#include <gtk/gtk.h>
#include "hsthread.h"
#include "hcthread.h"
#include "hfthread.h"
struct main_struct {
	struct hmthread_struct *hmthread;
	struct hfthread_struct *hfthread;
	struct hsthread_struct *hsthread;
	struct hcthread_struct *hcthread;
};

extern GtkWidget *window_main;
GtkWidget *create_window_main(struct main_struct *main);
void cb_notebook_switch_page(GtkNotebook *notebook,
			     GtkNotebookPage *page,
			     guint page_num, gpointer user_data);
boolean gtk_window_main_set_sensitive(struct main_struct *hmain);
#endif
