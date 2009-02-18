#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "fview.h"
#include "mwindow.h"
#include "support.h"
#include "linuxarms.h"
#include "hfthread.h"
gulong button_pressed_signal_id;
static char *file_info[] = {
	"文件名",
	"类型",
	"大小",	
	"用户",
	"修改时间"
};

GtkListStore  *create_page_fview(struct linuxarms_struct *linuxarms)
{
	GtkWidget *notebook_main = linuxarms->mwindow->notebook;
	struct hfview_struct *hfview = linuxarms->hfthread->hfview;
	GtkWidget *vbox_fview;
	GtkWidget *vbox_fpath;
	GtkWidget *hbox_fpath;
	GtkWidget *label_fpath;
	GtkWidget *entry_fpath;
	GtkWidget *fixed6;
	GtkWidget *scrolledwindow_fview;
	GtkWidget *treeview_fview;
	GtkWidget *label_fview;
	GtkCellRenderer *cell_renderer;
	GtkListStore *list_store;
	GtkTreeIter iter;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection;

	int i;
	
	vbox_fview = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_fview);
	gtk_container_add(GTK_CONTAINER(notebook_main), vbox_fview);

	vbox_fpath = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_fpath);
	gtk_box_pack_start(GTK_BOX(vbox_fview), vbox_fpath, FALSE, FALSE, 0);

	hbox_fpath = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox_fpath);
	gtk_box_pack_start(GTK_BOX(vbox_fpath), hbox_fpath, TRUE, TRUE, 0);

	label_fpath = gtk_label_new(_("\346\226\207\344\273\266\350\267\257\345\276\204"));
	gtk_widget_show(label_fpath);
	gtk_box_pack_start(GTK_BOX(hbox_fpath), label_fpath, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_fpath, 60, -1);

	entry_fpath = gtk_entry_new();
	gtk_widget_show(entry_fpath);
	gtk_box_pack_start(GTK_BOX(hbox_fpath), entry_fpath, TRUE, TRUE, 0);
	gtk_entry_set_max_length(GTK_ENTRY(entry_fpath), 512);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_fpath), 9679);
	gtk_entry_set_editable(GTK_ENTRY(entry_fpath), FALSE);
	gtk_entry_set_text(GTK_ENTRY(entry_fpath), "/");
	
	fixed6 = gtk_fixed_new ();
	gtk_widget_show (fixed6);
	gtk_box_pack_start (GTK_BOX (hbox_fpath), fixed6, FALSE, FALSE, 0);
	gtk_widget_set_size_request (fixed6, 21, -1);

	scrolledwindow_fview = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow_fview);
	gtk_box_pack_start(GTK_BOX(vbox_fview), scrolledwindow_fview, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow_fview),
				       GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow_fview),
	    				    GTK_SHADOW_IN);

	label_fview = gtk_label_new(_("\346\265\217\350\247\210\346\226\207\344\273\266"));
	gtk_widget_show(label_fview);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook_main), 
	gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook_main), 2), label_fview);
	GTK_WIDGET_SET_FLAGS(label_fview, GTK_CAN_FOCUS);
	
	list_store = gtk_list_store_new(FILE_INFO_COLUMNS + 1,/* 文件信息个数+1 */
					GDK_TYPE_PIXBUF,/* 图标         */
					G_TYPE_STRING,  /* 文件名       */
					G_TYPE_STRING,    /* 文件大小     */
					G_TYPE_STRING,  /* 文件类型     */
					G_TYPE_STRING,  /* 用户         */
					G_TYPE_STRING   /* 文件修改时间 */
			);
	treeview_fview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
	g_object_unref(list_store);
	gtk_widget_show(treeview_fview);
	gtk_container_add(GTK_CONTAINER(scrolledwindow_fview), treeview_fview);
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview_fview), TRUE);
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(treeview_fview), TRUE);
	
	/* 创建文件名列 */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, file_info[0]);
	//gtk_tree_view_column_set_sort_column_id(column, COL_FNAME);
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_fview), column);
	
	/* 文件图标 */
	cell_renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, cell_renderer, TRUE);
	gtk_tree_view_column_set_attributes(column, cell_renderer, 
					    "pixbuf", COL_FPIXBUF,NULL);

	/* 显示文件名 */
	cell_renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, cell_renderer, TRUE);
	gtk_tree_view_column_set_attributes(column, cell_renderer,
					    "text", COL_FNAME, NULL);
	//gtk_tree_view_column_set_expand(column, TRUE);
	//gtk_tree_view_column_set_alignment(column, 0.0);

	for (i = COL_FNAME + 1; i <= FILE_INFO_COLUMNS; i++){
		column = gtk_tree_view_column_new();
		gtk_tree_view_column_set_title(column, file_info[i - 1]);
		//gtk_tree_view_column_set_sort_column_id(column, i);
		gtk_tree_view_column_set_resizable(column, TRUE);
		gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_fview), column);
		cell_renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_column_pack_start(column, cell_renderer, TRUE);
		gtk_tree_view_column_set_attributes(column,cell_renderer,
						"text", i,NULL);
	}
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(treeview_fview),FALSE);
	/*
	list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_fview)));
	g_object_ref(list_store);
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_fview), NULL); 
	for (i = 0; i < MAX_FILE_NUMS; i++)
		gtk_list_store_append(list_store, &iter);
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_fview), GTK_TREE_MODEL(list_store));
	g_object_unref(list_store);
	*/
	button_pressed_signal_id = g_signal_connect (G_OBJECT (treeview_fview), "button_press_event",
                          G_CALLBACK (cb_fview_button_press),
			  (gpointer)linuxarms);
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_fview));
	g_signal_connect((gpointer)treeview_fview, "row-activated",
			G_CALLBACK(cb_treeview_fview_row_activated),
			(gpointer)linuxarms);
/*
  	g_signal_connect(selection, "changed", 
		         G_CALLBACK(cb_fview_selection_changed),
			 (gpointer)linuxarms);*/
	hfview->widget.path = entry_fpath;
	hfview->widget.treeview = treeview_fview;
	return list_store;
}


