#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include "debug.h"
#include "message.h"
void cb_fview_back_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();
	gtk_show_message("没有权限执行");
}


void cb_fview_up_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();

}


void cb_download_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();

}


void cb_upload_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();

}
