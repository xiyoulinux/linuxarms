#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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

#define GLADE_HOOKUP_OBJECT(component,widget,name)\
	g_object_set_data_full(G_OBJECT(component), name, \
	gtk_widget_ref(widget),(GDestroyNotify)gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name)\
	g_object_set_data(G_OBJECT(component), name, widget)
	
GtkWidget* create_popup_menu_fview(void)
{
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
			 NULL);
	g_signal_connect((gpointer)fview_delete, "activate",
			 G_CALLBACK(cb_fview_delete_activate),
			 NULL);
	g_signal_connect((gpointer)edit_upload, "activate",
			 G_CALLBACK(cb_fview_upload_activate),
			 NULL);
	g_signal_connect((gpointer)edit_download, "activate",
			 G_CALLBACK(cb_fview_download_activate),
			 NULL);

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF(popup_menu_fview, popup_menu_fview, "popup_menu_fview");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, popup_separator, "popup_separator");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, fview_rename, "fview_rename");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, image203, "image203");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, fview_delete, "fview_delete");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, image204, "image204");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, edit_separator_one, "edit_separator_one");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, edit_upload, "edit_upload");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, image205, "image205");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, edit_download, "edit_download");
	GLADE_HOOKUP_OBJECT(popup_menu_fview, image206, "image206");
	GLADE_HOOKUP_OBJECT_NO_REF(popup_menu_fview, tooltips, "tooltips");

	return popup_menu_fview;
}

GtkWidget* create_filechooserdialog(void)
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
  					 GTK_WINDOW(window_main) , GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
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

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog, filechooserdialog, "filechooserdialog");
	GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog, dialog_vbox, "dialog_vbox");
	GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog, dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT(filechooserdialog, button_cancel, "button_cancel");
	GLADE_HOOKUP_OBJECT(filechooserdialog, alignment9, "alignment9");
	GLADE_HOOKUP_OBJECT(filechooserdialog, hbox7, "hbox7");
	GLADE_HOOKUP_OBJECT(filechooserdialog, image348, "image348");
	GLADE_HOOKUP_OBJECT(filechooserdialog, label7, "label7");
	GLADE_HOOKUP_OBJECT(filechooserdialog, button_open, "button_open");
	GLADE_HOOKUP_OBJECT(filechooserdialog, alignment10, "alignment10");
	GLADE_HOOKUP_OBJECT(filechooserdialog, hbox8, "hbox8");
	GLADE_HOOKUP_OBJECT(filechooserdialog, image349, "image349");
	GLADE_HOOKUP_OBJECT(filechooserdialog, label8, "label8");

	gtk_widget_grab_default(button_open);
	return filechooserdialog;
}
