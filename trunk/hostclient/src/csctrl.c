#include <gtk/gtk.h>
#include "sctrl.h"
#include "debug.h"
gboolean cb_ctrl_input_key_press(GtkWidget *widget,
			GdkEventKey *event, gpointer user_data)
{
  	debug_where();
	return FALSE;
}
