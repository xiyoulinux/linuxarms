#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "sctrl.h"
#include "debug.h"
#include "hcthread.h"
gboolean cb_ctrl_input_key_press(GtkWidget *widget,
			GdkEventKey *event, gpointer user_data)
{
	debug_where();
	struct hcthread_struct *hcthread = 
		(struct hcthread_struct *)user_data;
	const gchar *input;
	if (event->keyval == GDK_Return) {
		input = gtk_entry_get_text(GTK_ENTRY(hcthread->widget.entry_input));
		gtk_widget_set_sensitive(hcthread->widget.entry_input, FALSE);
		debug_print("it is enter:%s\n",input);
		return TRUE;
	}
	return FALSE;
}
