#ifndef _FILETRANS_H
#define _FILETRANS_H
GtkWidget* create_popup_menu_fview(void);

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
#endif
