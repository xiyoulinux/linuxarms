#ifndef _MESSAGE_H
#define _MESSAGE_H
#include <gtk/gtk.h>
#include "linuxarms.h"
void message_box_warning(GtkWidget *window,gchar *message);
void message_box_error(GtkWidget *window,gchar *message);
void message_box_info(GtkWidget *window,gchar *message);
boolean message_box_choose(GtkWidget *window, GtkMessageType type, char *msg);
void create_window_dialog(const char *message);
extern boolean checkmult;
#endif //_MESSAGE_H
