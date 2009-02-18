#ifndef _TOOLBAR_H
#define _TOOLBAR_H

#include <gtk/gtk.h>
#include "linuxarms.h"
#include "hfthread.h"
#define BACK_NUM 3


GtkWidget *create_toolbar(GtkWidget *vbox_main,
                          GtkTooltips *tooltips,
			  struct linuxarms_struct *linuxarms); 
void cb_fview_back_clicked(GtkButton *button, gpointer user_data);

void cb_fview_up_clicked(GtkButton *button, gpointer user_data);
void cb_download_clicked(GtkButton *button, gpointer user_data);

void cb_upload_clicked(GtkButton *button, gpointer user_data);

/*num > 2*/
boolean list_head_new(int num);
boolean list_head_set_path(const char *path);
const char *list_head_get_path();
boolean list_head_free();
boolean list_head_test_next();
#endif
