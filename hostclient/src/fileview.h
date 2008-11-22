#ifndef _FILEVIEW_H
#define _FILEVIEW_H

#include "linuxarms.h"
#include <gtk/gtk.h>

struct hfview_trans {
	char *path;
	
};
struct hfview_struct {
	char path;
};

void cb_clist_fview_click_column(GtkCList *clist, 
				 gint column, gpointer user_data);

gboolean cb_clist_fview_button_press_event(GtkWidget *widget,
			GdkEventButton  *event,gpointeruser_data);
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
