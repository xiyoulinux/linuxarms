#ifndef _HELP_H
#define _HELP_H
#include <gtk/gtk.h>

GtkWidget* create_window_help();
void on_button_ok_clicked(GtkButton *button, gpointer user_data);
void on_window_help_destroy(GtkObject *object, gpointer user_data);                                        
#endif //_HELP_H
