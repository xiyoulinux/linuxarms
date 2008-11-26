#ifndef _TOOLBAR_H
#define _TOOLBAR_H

#include <gtk/gtk.h>
GtkWidget *create_toolbar(GtkWidget *vbox_main,
                          GtkTooltips *tooltips); 
void cb_fview_back_clicked(GtkButton *button, gpointer user_data);

void cb_fview_up_clicked(GtkButton *button, gpointer user_data);
void cb_download_clicked(GtkButton *button, gpointer user_data);

void cb_upload_clicked(GtkButton *button, gpointer user_data);
#endif