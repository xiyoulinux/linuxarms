void
cb_clist_fview_click_column            (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data);

gboolean
cb_clist_fview_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
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
