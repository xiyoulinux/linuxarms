#include <gtk/gtk.h>
#include "help.h"
#include "support.h"

void on_window_help_destroy(GtkObject *object, gpointer user_data)
{
	GtkWidget *window_help=(GtkWidget *)user_data;
	gtk_widget_destroy(window_help);
}


void on_button_ok_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *window_help=(GtkWidget *)user_data;
	gtk_widget_destroy(window_help);
}
