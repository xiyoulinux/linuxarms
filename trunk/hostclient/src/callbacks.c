#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include<gdk/gdkkeysyms.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"


void
cb_login_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_logout_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_restart_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_shutdown_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_quit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_process_update_three_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_process_update_five_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_process_kill_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_fview_rename_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_fview_delete_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_fview_upload_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_fview_downolad_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_help_topic_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_help_about_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_fview_back_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
cb_fview_up_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
cb_download_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
cb_upload_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
cb_notebook_switch_page                (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data)
{

}


void
cb_clist_process_click_column          (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data)
{

}


gboolean
cb_clist_process_button_press_event    (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
cb_clist_fview_click_column            (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data)
{

}


gboolean
cb_clist_fview_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
cb_ctrl_input_activate                 (GtkEntry        *entry,
                                        gpointer         user_data)
{
printf("input\n");
}


void
cb_ctrl_input_backspace                (GtkEntry        *entry,
                                        gpointer         user_data)
{
printf("back\n");
}


void
cb_popup_process_kill_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_login_rem_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
cb_login_help_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
cb_login_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}





gboolean
cb_ctrl_input_key_press                (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
	GtkWidget *entry=widget;
	
	switch(event->keyval){
		case GDK_Return:
			printf("fgdfgd\n");
		case GDK_BackSpace:
		printf("back\n");
			
	}
	printf("%x\n",event->keyval);
  return FALSE;
}

/*
void
cb_entry_input_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{

}
*/

void
cb_entry_input_delete_text             (GtkEditable     *editable,
                                        gint             start_pos,
                                        gint             end_pos,
                                        gpointer         user_data)
{
	if(strlen(gtk_entry_get_text(GTK_ENTRY(editable)))==5)
	gtk_entry_set_text(GTK_ENTRY(editable),"root&");
printf("sgfd\n");
}

