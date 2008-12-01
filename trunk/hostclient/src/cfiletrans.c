#include <gtk/gtk.h>
#include "filetrans.h"
#include "debug.h"

void cb_fview_rename_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_fview_delete_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");
	GtkWidget *filechooserdialog = create_filechooserdialog();
	GtkFileFilter *filter = gtk_file_filter_new ();
  	gtk_file_filter_set_name (filter, "All File(*)");
  	gtk_file_filter_add_pattern(filter,"*");
  	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (filechooserdialog), filter);
  	gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (filechooserdialog), filter);
  	gtk_widget_show(filechooserdialog);
  	gtk_dialog_run (GTK_DIALOG (filechooserdialog));

}


void cb_fview_download_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");
	GtkWidget *filechooserdialog = create_filechooserdialog();
	GtkFileFilter *filter = gtk_file_filter_new ();
  	gtk_file_filter_set_name (filter, "All File(*)");
  	gtk_file_filter_add_pattern(filter,"*");
  	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (filechooserdialog), filter);
  	gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (filechooserdialog), filter);
  	gtk_widget_show(filechooserdialog);
  	gtk_dialog_run (GTK_DIALOG (filechooserdialog));
}

void cb_button_cancel_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *filechooserdialog = (GtkWidget *)user_data;
	gtk_widget_destroy (filechooserdialog);
}


void cb_button_open_clicked(GtkButton *button, gpointer user_data)
{

	GtkWidget *filechooserdialog = (GtkWidget *)user_data;
	char *file_name;
	file_name=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (filechooserdialog));
	debug_print("get file name: %s\n", file_name);
  	gtk_widget_destroy (filechooserdialog);
}
