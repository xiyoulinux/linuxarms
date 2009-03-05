#define __DEBUG__
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
#include "htthread.h"
#include "message.h"
#include "debug.h"

GtkWidget* create_filechooserdialog(struct linuxarms_struct *linuxarms)
{
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;
	GtkWidget *filechooserdialog;
	GtkWidget *dialog_vbox;
	GtkWidget *dialog_action_area;
	GtkWidget *button_cancel;
	GtkWidget *alignment9;
	GtkWidget *hbox7;
	GtkWidget *image348;
	GtkWidget *label7;
	GtkWidget *button_ok;
	GtkWidget *alignment10;
	GtkWidget *hbox8;
	GtkWidget *image349;
	GtkWidget *label_ok;

	switch (htthread->select) {
	case FDOWN:
		filechooserdialog = gtk_file_chooser_dialog_new("选择要下载的文件",
			    GTK_WINDOW(linuxarms->mwindow->window), 
			    GTK_FILE_CHOOSER_ACTION_OPEN, NULL, NULL);
		break;
	case FUP:
		filechooserdialog = gtk_file_chooser_dialog_new("选择要保存的路径",
			    GTK_WINDOW(linuxarms->mwindow->window), 
			    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);
		break;
	default:
		message_box_error(linuxarms->mwindow->window, "错误的请求方式");
		return NULL;
	}
	gtk_container_set_border_width(GTK_CONTAINER(filechooserdialog), 5);
	gtk_window_set_position(GTK_WINDOW(filechooserdialog), GTK_WIN_POS_CENTER);
	gtk_window_set_type_hint(GTK_WINDOW(filechooserdialog), GDK_WINDOW_TYPE_HINT_DIALOG);

	dialog_vbox = GTK_DIALOG(filechooserdialog)->vbox;
	gtk_widget_show(dialog_vbox);

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

	label7 = gtk_label_new_with_mnemonic(_("取消"));
	gtk_widget_show(label7);
	gtk_box_pack_start(GTK_BOX(hbox7), label7, FALSE, FALSE, 0);

	button_ok = gtk_button_new();
	gtk_widget_show(button_ok);
	gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog), button_ok, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS(button_ok, GTK_CAN_DEFAULT);

	alignment10 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment10);
	gtk_container_add(GTK_CONTAINER(button_ok), alignment10);

	hbox8 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox8);
	gtk_container_add(GTK_CONTAINER(alignment10), hbox8);

	image349 = gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image349);
	gtk_box_pack_start(GTK_BOX(hbox8), image349, FALSE, FALSE, 0);
	switch (htthread->select) {
	case FDOWN:
		label_ok = gtk_label_new_with_mnemonic(_("选择"));
		break;
	case FUP:
		label_ok = gtk_label_new_with_mnemonic(_("保存"));
		break;
	default:
		label_ok = gtk_label_new_with_mnemonic(_("打开"));
		break;

	}
	gtk_widget_show(label_ok);
	gtk_box_pack_start(GTK_BOX(hbox8), label_ok, FALSE, FALSE, 0);

	g_signal_connect((gpointer)button_cancel, "clicked",
			G_CALLBACK(cb_ftrans_button_cancel_clicked),
			(gpointer)filechooserdialog);
	g_signal_connect((gpointer)button_ok, "clicked",
			G_CALLBACK(cb_ftrans_button_ok_clicked),
			(gpointer)linuxarms);
	g_signal_connect((gpointer)filechooserdialog, "delete_event",
			G_CALLBACK(cb_ftrans_window_close),
			NULL);
	htthread->widget.choose = filechooserdialog;
	gtk_widget_grab_default(button_ok);
	return filechooserdialog;
}
/*
void create_window_trans(struct linuxarms_struct *linuxarms)
{
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;
	GtkWidget *window_trans;
	GdkPixbuf *window_trans_icon_pixbuf;
	GtkWidget *vbox9;
	GtkWidget *table_main;
	GtkWidget *label_src;
	GtkWidget *src_path;
	GtkWidget *dest_path;
	GtkWidget *label_dest;
	GtkWidget *hbox15;
	GtkWidget *progressbar;
	GtkWidget *button_cancel;
	GtkWidget *alignment15;
	GtkWidget *hbox17;
	GtkWidget *image487;
	GtkWidget *hbox16;
	GtkWidget *label_trans;
	GtkWidget *label_size;

	window_trans = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window_trans), GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request(window_trans, 350, -1);
	gtk_window_set_title(GTK_WINDOW(window_trans), _("文件传输"));
	gtk_window_set_resizable(GTK_WINDOW(window_trans), FALSE);
	window_trans_icon_pixbuf = create_pixbuf("ico-window.png");
	if(window_trans_icon_pixbuf){
		gtk_window_set_icon(GTK_WINDOW(window_trans), window_trans_icon_pixbuf);
		gdk_pixbuf_unref(window_trans_icon_pixbuf);
	}

	vbox9 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox9);
	gtk_container_add(GTK_CONTAINER(window_trans), vbox9);
	gtk_container_set_border_width(GTK_CONTAINER(vbox9), 3);

	table_main = gtk_table_new(2, 2, FALSE);
	gtk_widget_show(table_main);
	gtk_box_pack_start(GTK_BOX(vbox9), table_main, FALSE, FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(table_main), 2);

	label_src = gtk_label_new(_("源 地址："));
	gtk_widget_show(label_src);
	gtk_table_attach(GTK_TABLE(table_main), label_src, 0, 1, 0, 1,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label_src), 0, 0.5);

	src_path = gtk_label_new("");
	gtk_widget_show(src_path);
	gtk_table_attach(GTK_TABLE(table_main), src_path, 1, 2, 0, 1,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_widget_set_size_request(src_path, 290, -1);
	gtk_misc_set_alignment(GTK_MISC(src_path), 0, 0.5);

	dest_path = gtk_label_new("");
	gtk_widget_show(dest_path);
	gtk_table_attach(GTK_TABLE(table_main), dest_path, 1, 2, 1, 2,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_widget_set_size_request(dest_path, 290, -1);
	gtk_misc_set_alignment(GTK_MISC(dest_path), 0, 0.5);

	label_dest = gtk_label_new(_("目标地址："));
	gtk_widget_show(label_dest);
	gtk_table_attach(GTK_TABLE(table_main), label_dest, 0, 1, 1, 2,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label_dest), 0, 0.5);

	hbox15 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox15);
	gtk_box_pack_start(GTK_BOX(vbox9), hbox15, FALSE, FALSE, 0);

	progressbar = gtk_progress_bar_new();
	gtk_widget_show(progressbar);
	gtk_box_pack_start(GTK_BOX(hbox15), progressbar, TRUE, TRUE, 0);

	button_cancel = gtk_button_new();
	gtk_widget_show(button_cancel);
	gtk_box_pack_start(GTK_BOX(hbox15), button_cancel, FALSE, FALSE, 0);

	alignment15 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment15);
	gtk_container_add(GTK_CONTAINER(button_cancel), alignment15);

	hbox17 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox17);
	gtk_container_add(GTK_CONTAINER(alignment15), hbox17);

	image487 = gtk_image_new_from_stock("gtk-close", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image487);
	gtk_box_pack_start(GTK_BOX(hbox17), image487, FALSE, FALSE, 0);

	hbox16 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox16);
	gtk_box_pack_start(GTK_BOX(vbox9), hbox16, FALSE, FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(hbox16), 4);

	label_trans = gtk_label_new(_("已传输："));
	gtk_widget_show(label_trans);
	gtk_box_pack_start(GTK_BOX(hbox16), label_trans, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_trans, 165, -1);
	gtk_misc_set_alignment(GTK_MISC(label_trans), 0, 0.5);

	label_size = gtk_label_new(_("文件大小："));
	gtk_widget_show(label_size);
	gtk_box_pack_start(GTK_BOX(hbox16), label_size, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_size, 165, -1);
	gtk_misc_set_alignment(GTK_MISC(label_size), 0, 0.5);

	htthread->widget.progressbar = progressbar;
	htthread->widget.window_trans = window_trans;
	htthread->widget.label_src = src_path;
	htthread->widget.label_dest = dest_path;
	htthread->widget.label_trans = label_trans;
	htthread->widget.label_total = label_size;
	g_signal_connect((gpointer)window_trans, "destroy",
			G_CALLBACK(cb_window_trans_close),
			(gpointer)linuxarms);
	g_signal_connect((gpointer)button_cancel, "clicked",
			G_CALLBACK(cb_window_trans_close),
			(gpointer)linuxarms);
	debug_where();
	gtk_window_set_keep_above(GTK_WINDOW(window_trans), TRUE);
	gtk_widget_show(window_trans);
}
*/
