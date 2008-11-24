#ifndef _SCTRL_H
#define _SCTRL_H
#include <gtk/gtk.h>
GtkWidget *create_ctrl_page(GtkWidget *vbox_main, GtkTooltips *tooltips);
gboolean cb_ctrl_input_key_press(GtkWidget *widget,
		GdkEventKey *event, gpointer user_data);
#endif
