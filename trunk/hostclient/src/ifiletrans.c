#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "filetrans.h"
#include "mwindow.h"
#include "support.h"
#include "linuxarms.h"
#include "fview.h"
#include "hfthread.h"
#include "hmthread.h"

GtkWidget* create_popup_menu_fview(struct linuxarms_struct *linuxarms)
{
	struct hmthread_struct *hmthread = linuxarms->hmthread;
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
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(
		GTK_TREE_VIEW(linuxarms->hfthread->hfview->widget.treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		char *type, *user, *name;
		gtk_tree_model_get(list_store, &iter,
				COL_FNAME, &name,
				COL_FTYPE, &type,
				COL_FUSER, &user,
				-1);
		if (strcmp(type, "文件夹") == 0) {
			gtk_widget_set_sensitive(edit_upload, FALSE);
			gtk_widget_set_sensitive(fview_delete, FALSE);
		}
		if (hmthread->competence(hmthread) && strcmp(user, "root") != 0) {
			gtk_widget_set_sensitive(fview_rename, FALSE);
			gtk_widget_set_sensitive(fview_delete, FALSE);
			gtk_widget_set_sensitive(edit_upload, FALSE);
		}
		if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
			gtk_widget_set_sensitive(fview_rename, FALSE);
			gtk_widget_set_sensitive(fview_delete, FALSE);
		}
		g_free(name);
		g_free(type);
		g_free(user);
	} else {
		gtk_widget_set_sensitive(fview_rename, FALSE);
		gtk_widget_set_sensitive(fview_delete, FALSE);
		gtk_widget_set_sensitive(edit_upload, FALSE);
		gtk_widget_set_sensitive(edit_download, FALSE);
	}
	return popup_menu_fview;
}

GtkWidget* create_filechooserdialog(struct linuxarms_struct *linuxarms)
{
	GtkWidget *filechooserdialog;
	GtkWidget *dialog_vbox;
	GtkWidget *dialog_action_area;
	GtkWidget *button_cancel;
	GtkWidget *alignment9;
	GtkWidget *hbox7;
	GtkWidget *image348;
	GtkWidget *label7;
	GtkWidget *button_open;
	GtkWidget *alignment10;
	GtkWidget *hbox8;
	GtkWidget *image349;
	GtkWidget *label8;

	filechooserdialog = gtk_file_chooser_dialog_new ("Open File",
  			GTK_WINDOW(linuxarms->mwindow->window) , 
			GTK_FILE_CHOOSER_ACTION_OPEN, NULL, NULL);
	gtk_container_set_border_width (GTK_CONTAINER (filechooserdialog), 5);
	gtk_window_set_type_hint (GTK_WINDOW (filechooserdialog), GDK_WINDOW_TYPE_HINT_DIALOG);

	dialog_vbox = GTK_DIALOG(filechooserdialog)->vbox;
	gtk_widget_show(dialog_vbox);
	gtk_widget_set_size_request(dialog_vbox, 267, 267);

	dialog_action_area = GTK_DIALOG(filechooserdialog)->action_area;
	gtk_widget_show(dialog_action_area);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area), GTK_BUTTONBOX_END);

	button_cancel = gtk_button_new();
	gtk_widget_show(button_cancel);
	gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog), button_cancel, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS(button_cancel, GTK_CAN_DEFAULT);

	alignment9 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment9);
	gtk_container_add(GTK_CONTAINER(button_cancel), alignment9);

	hbox7 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox7);
	gtk_container_add(GTK_CONTAINER(alignment9), hbox7);

	image348 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image348);
	gtk_box_pack_start(GTK_BOX(hbox7), image348, FALSE, FALSE, 0);

	label7 = gtk_label_new_with_mnemonic(_("\345\217\226\346\266\210"));
	gtk_widget_show(label7);
	gtk_box_pack_start(GTK_BOX(hbox7), label7, FALSE, FALSE, 0);

	button_open = gtk_button_new();
	gtk_widget_show(button_open);
	gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog), button_open, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS(button_open, GTK_CAN_DEFAULT);

	alignment10 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment10);
	gtk_container_add(GTK_CONTAINER(button_open), alignment10);

	hbox8 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox8);
	gtk_container_add(GTK_CONTAINER(alignment10), hbox8);

	image349 = gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image349);
	gtk_box_pack_start(GTK_BOX(hbox8), image349, FALSE, FALSE, 0);

	label8 = gtk_label_new_with_mnemonic(_("选择"));
	gtk_widget_show(label8);
	gtk_box_pack_start(GTK_BOX(hbox8), label8, FALSE, FALSE, 0);

	g_signal_connect((gpointer)button_cancel, "clicked",
				   G_CALLBACK(cb_button_cancel_clicked),
				   (gpointer)filechooserdialog);
	g_signal_connect((gpointer)button_open, "clicked",
				    G_CALLBACK(cb_button_open_clicked),
				    (gpointer)filechooserdialog);
	g_signal_connect((gpointer)filechooserdialog, "delete_event",
				    G_CALLBACK(gtk_widget_destroy),
				    NULL);				   

	gtk_widget_grab_default(button_open);
	return filechooserdialog;
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

