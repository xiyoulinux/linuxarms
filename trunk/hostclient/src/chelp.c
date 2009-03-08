#include <gtk/gtk.h>
#include "help.h"
#include "support.h"
#include "linuxarms.h"
boolean help_window_created = FALSE;
void cb_dialog_help_destroy(GtkObject *object, gpointer user_data)
{
	GtkWidget *window_help=(GtkWidget *)user_data;
	gtk_widget_destroy(window_help);
	help_window_created = FALSE;
}


void cb_help_button_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *window_help=(GtkWidget *)user_data;
	gtk_widget_destroy(window_help);
	help_window_created = FALSE;
}
