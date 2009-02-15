#ifndef _SCTRL_H
#define _SCTRL_H
#include <gtk/gtk.h>
#include "hcthread.h"
GtkWidget *create_ctrl_page(GtkWidget *vbox_main, GtkTooltips *tooltips,
				GtkWidget *frame_main,
				struct hcthread_struct *hcthread);
gboolean cb_ctrl_input_key_press(GtkWidget *widget,
		GdkEventKey *event, gpointer user_data);
#endif
