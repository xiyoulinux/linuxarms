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

