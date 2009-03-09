#include <gtk/gtk.h>
#include "support.h"
#include "linuxarms.h"
#include "message.h"
boolean checkmult = FALSE;
void message_box_warning(GtkWidget *window, char *msg)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
				GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);	
}
void message_box_error(GtkWidget *window, char *msg)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
				GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	if (checkmult)
		gtk_main_quit();
}
boolean message_box_choose(GtkWidget *window, GtkMessageType type, char *msg)
{
	GtkWidget *dialog;
	int choose;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
				type, GTK_BUTTONS_OK_CANCEL, msg,NULL);
	choose = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	if (choose == GTK_RESPONSE_OK)
		return TRUE;
	return FALSE;
}
void message_box_info(GtkWidget *window,char *msg)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
			GTK_MESSAGE_INFO, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);	
}

/*
gboolean cb_window_dialog_delete_event(GtkWidget *widget, 
			GdkEvent *event, gpointer user_data);
void cb_button_exit_clicked(GtkButton *button, gpointer user_data);

void create_window_dialog(const char *message)
{
	GtkWidget *window_dialog;
	GtkWidget *vbox_dialog;
	GtkWidget *label_show;
	GtkWidget *hbox_dialog;
	GtkWidget *fixed8;
	GtkWidget *button_exit;
	GtkWidget *alignment11;
	GtkWidget *hbox10;
	GtkWidget *image350;
	GtkWidget *label10;
	GtkWidget *fixed9;
	char buf[256];

	window_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window_dialog),  GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request(window_dialog, 250, 200);
	gtk_window_set_title(GTK_WINDOW(window_dialog), _("提示信息"));
	gtk_window_set_resizable(GTK_WINDOW(window_dialog), FALSE);
	gtk_window_set_icon_name(GTK_WINDOW(window_dialog), "gtk-dialog-info");

	vbox_dialog = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_dialog);
	gtk_container_add(GTK_CONTAINER(window_dialog), vbox_dialog);
	
	snprintf(buf, 256, "<b>%s</b>", message);
	label_show = gtk_label_new(_(buf));
	gtk_widget_show(label_show);
	gtk_box_pack_start(GTK_BOX(vbox_dialog), label_show, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_show, -1, 150);
	gtk_label_set_use_markup(GTK_LABEL(label_show), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label_show), 0.2, 0.12);

	hbox_dialog = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox_dialog);
	gtk_box_pack_start(GTK_BOX(vbox_dialog), hbox_dialog, TRUE, TRUE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(hbox_dialog), 7);

	fixed8 = gtk_fixed_new();
	gtk_widget_show(fixed8);
	gtk_box_pack_start(GTK_BOX(hbox_dialog), fixed8, TRUE, TRUE, 0);
	gtk_widget_set_size_request(fixed8, 160, -1);

	button_exit = gtk_button_new();
	gtk_widget_show(button_exit);
	gtk_box_pack_start(GTK_BOX(hbox_dialog), button_exit, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_exit, 72, -1);

	alignment11 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment11);
	gtk_container_add(GTK_CONTAINER(button_exit), alignment11);

	hbox10 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox10);
	gtk_container_add(GTK_CONTAINER(alignment11), hbox10);

	image350 = gtk_image_new_from_stock("gtk-close", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image350);
	gtk_box_pack_start(GTK_BOX(hbox10), image350, FALSE, FALSE, 0);

	label10 = gtk_label_new_with_mnemonic(_("关闭"));
	gtk_widget_show(label10);
	gtk_box_pack_start(GTK_BOX(hbox10), label10, FALSE, FALSE, 0);

	fixed9 = gtk_fixed_new();
	gtk_widget_show(fixed9);
	gtk_box_pack_start(GTK_BOX(hbox_dialog), fixed9, TRUE, TRUE, 0);

	g_signal_connect((gpointer)window_dialog, "delete_event",
			G_CALLBACK(cb_window_dialog_delete_event),
			NULL);
	g_signal_connect((gpointer)button_exit, "clicked",
			G_CALLBACK(cb_button_exit_clicked),
			(gpointer)window_dialog);

	gtk_widget_show(window_dialog);
}
gboolean cb_window_dialog_delete_event(GtkWidget *widget, 
			GdkEvent *event, gpointer user_data)
{
	gtk_widget_destroy(widget);
	if (checkmult)
		gtk_main_quit();
	return FALSE;
}


void cb_button_exit_clicked(GtkButton *button, gpointer user_data)
{
	gtk_widget_destroy(GTK_WIDGET(user_data));
	if (checkmult)
		gtk_main_quit();
}*/
