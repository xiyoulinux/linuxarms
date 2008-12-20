#ifndef _FILETRANS_H
#define _FILETRANS_H
#include <gtk/gtk.h>
#include "linuxarms.h"
GtkWidget* create_popup_menu_fview(struct linuxarms_struct *linuxarms);
GtkWidget* create_filechooserdialog(struct linuxarms_struct *linuxarms);

void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_download_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_rename_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_delete_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_button_cancel_clicked(GtkButton *button, gpointer user_data);
void cb_button_open_clicked(GtkButton *button, gpointer user_data);
#endif
