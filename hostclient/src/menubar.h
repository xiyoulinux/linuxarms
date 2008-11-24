#ifndef _MENUBAR_H
#define _MENUBAR_H
#include <gtk/gtk.h>

GtkWidget *create_menubar(GtkWidget *vbox_main,
			  GtkTooltips *tooltips,
			  GtkAccelGroup *accel_group);
void cb_login_activate(GtkMenuItem *menuitem, gpointer user_data);

void cb_logout_activate(GtkMenuItem *menuitem, gpointer user_data);

void cb_restart_activate (GtkMenuItem *menuitem, gpointer user_data);

void cb_shutdown_activate(GtkMenuItem *menuitem, gpointer user_data);

void cb_quit_activate (GtkMenuItem *menuitem, gpointer user_data);
void cb_process_update_three_activate(GtkMenuItem *menuitem, 
				      gpointer user_data);

void cb_process_update_five_activate(GtkMenuItem *menuitem,
				     gpointer user_data);    
void cb_process_kill_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_delete_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_downolad_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_rename_activate(GtkMenuItem *menuitem, gpointer user_data);

void cb_help_topic_activate(GtkMenuItem *menuitem, gpointer user_data);

void cb_help_about_activate(GtkMenuItem *menuitem, gpointer user_data);
#endif
