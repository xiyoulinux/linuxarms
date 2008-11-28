#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include "debug.h"
#include "about.h"
#include "help.h"
void cb_login_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("cb_login_activate\n");
}


void cb_logout_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("cb_logout_activate");
}


void cb_restart_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_shutdown_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_quit_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_process_update_three_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_process_update_five_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_process_kill_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


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

}


void cb_fview_downolad_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_help_topic_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget *window;
	window=(GtkWidget *)create_window_help();
	gtk_widget_show(window);
	debug_where();
	debug_print("");

}


void cb_help_about_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget *window;
	window=(GtkWidget *)create_window_about();
	gtk_widget_show(window);
	debug_where();
	debug_print("");

}

