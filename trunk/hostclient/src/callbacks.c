#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

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
printf("restart\n");
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
cb_edit_upload_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_edit_downolad_activate              (GtkMenuItem     *menuitem,
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


gboolean
on_label_name_button_press_event       (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
printf("press\n");
  return FALSE;
}


gboolean
cb_window_main_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
printf("press\n");
  return FALSE;
}





void
cb_clist_process_click_column          (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data)
{
printf("click column\n");
}


gboolean
cb_clist_process_button_press_event    (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
printf("cprocess button\n");
  return FALSE;
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
cb_process_kill_activate               (GtkMenuItem     *menuitem,
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
on_popup_process_kill_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
cb_popup_process_kill_activate         (GtkMenuItem     *menuitem,
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

