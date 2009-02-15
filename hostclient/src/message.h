#ifndef _MESSAGE_H
#define _MESSAGE_H
#include <gtk/gtk.h>
void message_box_warning(GtkWidget *window,gchar *message);
void message_box_error(GtkWidget *window,gchar *message);
void message_box_info(GtkWidget *window,gchar *message);
void create_window_dialog(const char *message);
#endif //_MESSAGE_H
