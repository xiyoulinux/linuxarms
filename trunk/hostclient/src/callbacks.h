#include <gtk/gtk.h>


void
cb_login_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_logout_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_restart_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_shutdown_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_quit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_edit_upload_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_edit_downolad_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_help_topic_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_help_about_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_label_name_button_press_event       (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
cb_window_main_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);


void
cb_clist_process_click_column          (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data);

gboolean
cb_clist_process_button_press_event    (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
cb_fview_back_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
cb_fview_up_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
cb_download_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
cb_upload_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
cb_clist_fview_click_column            (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data);

gboolean
cb_clist_fview_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
cb_process_kill_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_fview_upload_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_fview_downolad_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_fview_rename_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_fview_delete_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_popup_process_kill_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_popup_process_kill_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
cb_notebook_main_move_focus_out        (GtkNotebook     *notebook,
                                        GtkDirectionType  direction,
                                        gpointer         user_data);


gboolean
on_notebook_main_focus_tab             (GtkNotebook     *notebook,
                                        GtkNotebookTab   type,
                                        gpointer         user_data);

gboolean
on_notebook_main_change_current_page   (GtkNotebook     *notebook,
                                        gint             offset,
                                        gpointer         user_data);

gboolean
on_notebook_main_select_page           (GtkNotebook     *notebook,
                                        gboolean         move_focus,
                                        gpointer         user_data);

void
cb_notebook_main_switch_page           (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data);

void
cb_notebook_switch_page                (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data);
