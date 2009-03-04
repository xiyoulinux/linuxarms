#ifndef _FILETRANS_H
#define _FILETRANS_H
#include <gtk/gtk.h>
#include "linuxarms.h"

void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_download_activate(GtkMenuItem *menuitem, gpointer user_data);

GtkWidget* create_filechooserdialog(struct linuxarms_struct *linuxarms);
void cb_ftrans_button_cancel_clicked(GtkButton *button, gpointer user_data);
void cb_ftrans_window_close(GtkWidget *widget, gpointer user_data);
void cb_ftrans_button_ok_clicked(GtkButton *button, gpointer user_data);
void create_window_trans(struct linuxarms_struct *linuxarms);
void cb_window_trans_close(GtkWidget *widget, gpointer user_data);
#endif
