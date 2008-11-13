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
