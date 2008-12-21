#include <gtk/gtk.h>

void message_box_warning(GtkWidget *window, char *msg)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
				GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);	
}
void message_box_error(GtkWidget *window, char *msg)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
				GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);	
}
void message_box_info(GtkWidget *window,char *msg)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
			GTK_MESSAGE_INFO, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);	
}
