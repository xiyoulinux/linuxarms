#ifndef _HELP_H
#define _HELP_H
#include <gtk/gtk.h>
#include "linuxarms.h"
GtkWidget* create_dialog_help(GtkWidget *window_main);
void cb_help_button_clicked(GtkButton *button, gpointer user_data);
void cb_dialog_help_destroy(GtkObject *object, gpointer user_data);  
extern boolean help_window_created;                                     
#endif //_HELP_H
