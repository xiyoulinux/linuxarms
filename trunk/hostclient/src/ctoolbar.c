#include <gtk/gtk.h>
#include "debug.h"
#include "message.h"
#include "filetrans.h"
#include "linuxarms.h"
#include "mwindow.h"

void cb_fview_back_clicked(GtkButton *button, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	debug_where();
	message_box_error(linuxarms->mwindow->window,"没有权限执行");
}


void cb_fview_up_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();

}


void cb_download_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();
	cb_fview_download_activate(NULL, NULL);

}


void cb_upload_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();
	cb_fview_upload_activate(NULL, NULL);

}
