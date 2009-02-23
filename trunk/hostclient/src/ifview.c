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
	GtkAdjustment* vadjust;
	GtkWidget *treeview_fview;
	GtkWidget *label_fview;
	GtkCellRenderer *cell_renderer;
	GtkListStore *list_store;
	//GtkTreeIter iter;
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
	vadjust = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolledwindow_fview));

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
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_min_width(column, 200);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_fview), column);
	gtk_tree_view_set_expander_column(GTK_TREE_VIEW (treeview_fview), column);
	
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
		gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
		switch (i) {
		case COL_FTYPE:
			gtk_tree_view_column_set_min_width(column, 100);
			break;
		case COL_FSIZE:
			gtk_tree_view_column_set_min_width(column, 80);
			break;
		case COL_FUSER:
			gtk_tree_view_column_set_min_width(column, 80);
			break;
		case COL_FMTIME:
			gtk_tree_view_column_set_min_width(column, 150);
			break;
		default:
			gtk_tree_view_column_set_min_width(column, 100);
			break;
		}
		//gtk_tree_view_set_expander_column(GTK_TREE_VIEW (treeview_fview), column);
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
	g_signal_connect (G_OBJECT (treeview_fview), "button_press_event",
                          G_CALLBACK (cb_fview_button_press),
			  (gpointer)linuxarms);
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_fview));
	g_signal_connect((gpointer)treeview_fview, "row-activated",
			G_CALLBACK(cb_treeview_fview_row_activated),
			(gpointer)linuxarms);

  	g_signal_connect(selection, "changed", 
		         G_CALLBACK(cb_fview_selection_changed),
			 (gpointer)linuxarms);
	hfview->widget.vadjustment = vadjust;
	hfview->widget.path = entry_fpath;
	hfview->widget.treeview = treeview_fview;
	return list_store;
}

GtkWidget* create_popup_menu_fview(struct linuxarms_struct *linuxarms)
{
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hfview_widget *widget = &hfthread->hfview->widget;
	GtkWidget *popup_menu_fview;
	GtkWidget *popup_separator;
	GtkWidget *fview_rename;
	GtkWidget *image203;
	GtkWidget *fview_delete;
	GtkWidget *image204;
	GtkWidget *edit_separator_one;
	GtkWidget *edit_upload;
	GtkWidget *image205;
	GtkWidget *edit_download;
	GtkWidget *image206;
	GtkTooltips *tooltips;

	tooltips = gtk_tooltips_new();
	popup_menu_fview = gtk_menu_new();

	popup_separator = gtk_separator_menu_item_new();
	gtk_widget_show(popup_separator);
	gtk_container_add(GTK_CONTAINER(popup_menu_fview), popup_separator);
	gtk_widget_set_sensitive(popup_separator, FALSE);

	fview_rename = gtk_image_menu_item_new_with_mnemonic(
		_("\351\207\215\345\221\275\345\220\215\346\226\207\344\273\266"));
	gtk_widget_show(fview_rename);
	gtk_container_add(GTK_CONTAINER(popup_menu_fview), fview_rename);
	gtk_tooltips_set_tip(tooltips, fview_rename,
				_("\351\207\215\345\221\275\345\220\215\351"
				"\200\211\344\270\255\347\232\204\346\226"
				"\207\344\273\266"), NULL);

	image203 = gtk_image_new_from_stock("gtk-refresh", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image203);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(fview_rename), image203);

	fview_delete = gtk_image_menu_item_new_with_mnemonic(
			_("\345\210\240\351\231\244\346\226\207\344\273\266"));
	gtk_widget_show(fview_delete);
	gtk_container_add(GTK_CONTAINER(popup_menu_fview), fview_delete);
	gtk_tooltips_set_tip(tooltips, fview_delete,
			_("\345\210\240\351\231\244\351\200\211\344"
			"\270\255\347\232\204\346\226\207\344\273\266"), NULL);

	image204 = gtk_image_new_from_stock("gtk-delete", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image204);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(fview_delete), image204);

	edit_separator_one = gtk_separator_menu_item_new();
	gtk_widget_show(edit_separator_one);
	gtk_container_add(GTK_CONTAINER(popup_menu_fview), edit_separator_one);
	gtk_widget_set_sensitive(edit_separator_one, FALSE);

	edit_upload = gtk_image_menu_item_new_with_mnemonic(
			_("\344\270\212\344\274\240\346\226\207\344\273\266"));
	gtk_widget_show(edit_upload);
	gtk_container_add(GTK_CONTAINER(popup_menu_fview), edit_upload);
	gtk_tooltips_set_tip(tooltips, edit_upload,
			_("\344\273\216arm\347\263\273\347\273\237"
			"\344\270\212\344\274\240\346\226\207\344"
			"\273\266\345\210\260\346\234\254\346\234\272"), NULL);

	image205 = gtk_image_new_from_stock("gtk-goto-top", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image205);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(edit_upload), image205);

	edit_download = gtk_image_menu_item_new_with_mnemonic(
			_("\344\270\213\350\275\275\346\226\207\344\273\266"));
	gtk_widget_show(edit_download);
	gtk_container_add(GTK_CONTAINER(popup_menu_fview), edit_download);
	gtk_tooltips_set_tip(tooltips, edit_download,
			_("\344\273\216\346\234\254\346\234\272\344"
			"\270\213\350\275\275\346\226\207\344\273"
			"\266\345\210\260arm\347\263\273\347\273\237\344\270\212"), NULL);

	image206 = gtk_image_new_from_stock("gtk-goto-bottom", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image206);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(edit_download), image206);

	g_signal_connect((gpointer)fview_rename, "activate",
			 G_CALLBACK(cb_fview_rename_activate),
			 (gpointer)linuxarms);
	g_signal_connect((gpointer)fview_delete, "activate",
			 G_CALLBACK(cb_fview_delete_activate),
			 (gpointer)linuxarms);
	g_signal_connect((gpointer)edit_upload, "activate",
			 G_CALLBACK(cb_fview_upload_activate),
			 (gpointer)linuxarms);
	g_signal_connect((gpointer)edit_download, "activate",
			 G_CALLBACK(cb_fview_download_activate),
			 (gpointer)linuxarms);

	widget->popup_rename = fview_rename;
	widget->popup_del = fview_delete;
	widget->popup_upload = edit_upload;
	widget->popup_download = edit_download;

	return popup_menu_fview;
}

void create_window_rename(struct linuxarms_struct *linuxarms)
{
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	GtkWidget *window_rename;
	GtkWidget *table;
	GtkWidget *label_old;
	GtkWidget *label_new;
	GtkWidget *entry_old;
	GtkWidget *entry_new;
	GtkWidget *hbox11;
	GtkWidget *button_rename_cancel;
	GtkWidget *alignment13;
	GtkWidget *hbox13;
	GtkWidget *image352;
	GtkWidget *label14;
	GtkWidget *fixed11;
	GtkWidget *button_rename_ok;
	GtkWidget *alignment14;
	GtkWidget *hbox14;
	GtkWidget *image353;
	GtkWidget *label15;
	GtkWidget *button_rename_help;
	GtkWidget *alignment12;
	GtkWidget *hbox12;
	GtkWidget *image351;
	GtkWidget *label13;

	window_rename = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window_rename), _("\351\207\215\345\221\275\345\220\215"));
	gtk_window_set_position(GTK_WINDOW(window_rename), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window_rename), FALSE);
	gtk_window_set_icon_name(GTK_WINDOW(window_rename), "gtk-about");
	gtk_window_set_type_hint(GTK_WINDOW(window_rename), GDK_WINDOW_TYPE_HINT_DIALOG);

	table = gtk_table_new(3, 2, FALSE);
	gtk_widget_show(table);
	gtk_container_add(GTK_CONTAINER(window_rename), table);

	label_old = gtk_label_new(_("\346\227\247\346\226\207\344\273\266\345\220\215\357\274\232"));
	gtk_widget_show(label_old);
	gtk_table_attach(GTK_TABLE(table), label_old, 0, 1, 0, 1,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label_old), 0, 0.5);

	label_new = gtk_label_new(_("\346\226\260\346\226\207\344\273\266\345\220\215\357\274\232"));
	gtk_widget_show(label_new);
	gtk_table_attach(GTK_TABLE(table), label_new, 0, 1, 1, 2,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label_new), 0, 0.5);

	entry_old = gtk_entry_new();
	gtk_widget_show(entry_old);
	gtk_table_attach(GTK_TABLE(table), entry_old, 1, 2, 0, 1,
			(GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_old), 9679);
	gtk_entry_set_max_length(GTK_ENTRY(entry_old), 256);
	gtk_widget_set_sensitive(entry_old, FALSE);
	//gtk_entry_set_text(GTK_ENTRY(entry_old), hfthread->trans.rename[OLDNAME]);

	entry_new = gtk_entry_new();
	gtk_widget_show(entry_new);
	gtk_table_attach(GTK_TABLE(table), entry_new, 1, 2, 1, 2,
			(GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_new), 9679);
	gtk_entry_set_max_length(GTK_ENTRY(entry_new), 256);

	hbox11 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox11);
	gtk_table_attach(GTK_TABLE(table), hbox11, 1, 2, 2, 3,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(GTK_FILL), 0, 0);

	button_rename_cancel = gtk_button_new();
	gtk_widget_show(button_rename_cancel);
	gtk_box_pack_start(GTK_BOX(hbox11), button_rename_cancel, FALSE, FALSE, 0);

	alignment13 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment13);
	gtk_container_add(GTK_CONTAINER(button_rename_cancel), alignment13);

	hbox13 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox13);
	gtk_container_add(GTK_CONTAINER(alignment13), hbox13);

	image352 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image352);
	gtk_box_pack_start(GTK_BOX(hbox13), image352, FALSE, FALSE, 0);

	label14 = gtk_label_new_with_mnemonic(_("\345\217\226\346\266\210"));
	gtk_widget_show(label14);
	gtk_box_pack_start(GTK_BOX(hbox13), label14, FALSE, FALSE, 0);

	fixed11 = gtk_fixed_new();
	gtk_widget_show(fixed11);
	gtk_box_pack_start(GTK_BOX(hbox11), fixed11, FALSE, TRUE, 0);
	gtk_widget_set_size_request(fixed11, 34, -1);

	button_rename_ok = gtk_button_new();
	gtk_widget_show(button_rename_ok);
	gtk_box_pack_start(GTK_BOX(hbox11), button_rename_ok, FALSE, FALSE, 0);
	gtk_widget_set_sensitive(button_rename_ok, FALSE);

	alignment14 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment14);
	gtk_container_add(GTK_CONTAINER(button_rename_ok), alignment14);

	hbox14 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox14);
	gtk_container_add(GTK_CONTAINER(alignment14), hbox14);

	image353 = gtk_image_new_from_stock("gtk-ok", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image353);
	gtk_box_pack_start(GTK_BOX(hbox14), image353, FALSE, FALSE, 0);

	label15 = gtk_label_new_with_mnemonic(_("\347\241\256\345\256\232"));
	gtk_widget_show(label15);
	gtk_box_pack_start(GTK_BOX(hbox14), label15, FALSE, FALSE, 0);

	button_rename_help = gtk_button_new();
	gtk_widget_show(button_rename_help);
	gtk_table_attach(GTK_TABLE(table), button_rename_help, 0, 1, 2, 3,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);

	alignment12 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment12);
	gtk_container_add(GTK_CONTAINER(button_rename_help), alignment12);

	hbox12 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox12);
	gtk_container_add(GTK_CONTAINER(alignment12), hbox12);

	image351 = gtk_image_new_from_stock("gtk-help", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image351);
	gtk_box_pack_start(GTK_BOX(hbox12), image351, FALSE, FALSE, 0);

	label13 = gtk_label_new_with_mnemonic(_("\345\270\256\345\212\251"));
	gtk_widget_show(label13);
	gtk_box_pack_start(GTK_BOX(hbox12), label13, FALSE, FALSE, 0);

	g_signal_connect((gpointer)window_rename, "destroy",
			G_CALLBACK(cb_window_rename_destroy),
			(gpointer)window_rename);
	g_signal_connect((gpointer)button_rename_cancel, "clicked",
			G_CALLBACK(cb_window_rename_destroy),
			(gpointer)window_rename);
	g_signal_connect((gpointer)button_rename_ok, "clicked",
			G_CALLBACK(cb_rename_ok_clicked),
			(gpointer)linuxarms);
	g_signal_connect((gpointer)button_rename_help, "clicked",
			G_CALLBACK(cb_rename_help_clicked),
			(gpointer)window_rename);
	g_signal_connect((gpointer)entry_new, "changed",
			G_CALLBACK(cb_rename_entry_changed),
			(gpointer)button_rename_ok);
	hfthread->widget.window = window_rename;
	hfthread->widget.oldname = entry_old;
	hfthread->widget.newname = entry_new;
	gtk_widget_show(window_rename);
}


