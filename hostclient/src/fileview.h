#ifndef _FILEVIEW_H
#define _FILEVIEW_H

#include "linuxarms.h"
#include <gtk/gtk.h>
#define FILE_INFO_COLUMNS 4
enum {
	COL_FPIXBUF,
	COL_FNAME,
	COL_FSIZE,
	COL_FTYPE,
	COL_FMTIME
};

struct hfview_trans {
	char *path;
	
};
struct hfview_struct {
	char path;
};
GtkListStore *create_page_fview(GtkWidget *notebook_main);
void cb_fview_selection_changed(GtkWidget *widget, gpointer user_data);

gboolean cb_fview_button_press(GtkWidget *widget,
	                 GdkEventButton *event, gpointer user_data);


#endif
