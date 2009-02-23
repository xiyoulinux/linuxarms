#include <gtk/gtk.h>
#include <string.h>

#include "filetrans.h"
#include "message.h"
#include "debug.h"
#include "error.h"
#include "linuxarms.h"
#include "hfthread.h"
#include "hmthread.h"
#include "mwindow.h"
#include "fview.h"

void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms =(struct linuxarms_struct *)user_data;
	debug_where();
	GtkWidget *filechooserdialog = create_filechooserdialog(linuxarms);
	GtkFileFilter *filter = gtk_file_filter_new();
  	gtk_file_filter_set_name(filter, "All File(*)");
  	gtk_file_filter_add_pattern(filter,"*");
  	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooserdialog), filter);
  	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(filechooserdialog), filter);
  	gtk_widget_show(filechooserdialog);
  	gtk_dialog_run(GTK_DIALOG(filechooserdialog));

}


void cb_fview_download_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms =(struct linuxarms_struct *)user_data;
	debug_where();
	GtkWidget *filechooserdialog = create_filechooserdialog(linuxarms);
	GtkFileFilter *filter = gtk_file_filter_new();
  	gtk_file_filter_set_name(filter, "All File(*)");
  	gtk_file_filter_add_pattern(filter,"*");
  	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooserdialog), filter);
  	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(filechooserdialog), filter);
  	gtk_widget_show(filechooserdialog);
  	gtk_dialog_run(GTK_DIALOG(filechooserdialog));
}

void cb_button_cancel_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *filechooserdialog =(GtkWidget *)user_data;
	gtk_widget_destroy(filechooserdialog);
}


void cb_button_open_clicked(GtkButton *button, gpointer user_data)
{

	GtkWidget *filechooserdialog =(GtkWidget *)user_data;
	char *file_name;
	file_name=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserdialog));
	debug_print("get file name: %s\n", file_name);
  	gtk_widget_destroy(filechooserdialog);
}
