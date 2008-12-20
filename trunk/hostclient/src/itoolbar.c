#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "toolbar.h"
#include "support.h"
#include "mwindow.h"
#include "hfthread.h"
#include "fileview.h"
#include "htthread.h"

GtkWidget *create_toolbar(GtkWidget *vbox_main,
			  GtkTooltips *tooltips,
			  struct linuxarms_struct *linuxarms)
{
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct htthread_struct *hftrans = hfthread->hftrans;
	struct hfview_struct *hfview = hfthread->hfview;

	GtkWidget *toolbar;
	GtkIconSize tmp_toolbar_icon_size;
	GtkWidget *toolitem_back;
	GtkWidget *button_back;
	GtkWidget *alignment2;
	GtkWidget *vbox_tool_back;
	GtkWidget *image_back;
	GtkWidget *label_back;
	GtkWidget *toolitem_up;
	GtkWidget *button_up;
	GtkWidget *alignment3;
	GtkWidget *vbox_tool_up;
	GtkWidget *image_up;
	GtkWidget *label_up;
	GtkWidget *toolitem_tool;
	GtkWidget *vseparator_tool;
	GtkWidget *toolitem_downolad;
	GtkWidget *button_download;
	GtkWidget *alignment4;
	GtkWidget *vbox_tool_download;
	GtkWidget *image_download;
	GtkWidget *label_download;
	GtkWidget *toolitem_upload;
	GtkWidget *button_upload;
	GtkWidget *alignment5;
	GtkWidget *vbox8;
	GtkWidget *image_upload;
	GtkWidget *label_upload;

	toolbar = gtk_toolbar_new();
	gtk_widget_show(toolbar);
	gtk_box_pack_start(GTK_BOX(vbox_main), toolbar, FALSE, FALSE, 0);
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
	tmp_toolbar_icon_size = gtk_toolbar_get_icon_size(GTK_TOOLBAR(toolbar));

	toolitem_back =(GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_back);
	gtk_container_add(GTK_CONTAINER(toolbar), toolitem_back);

	button_back = gtk_button_new();
	gtk_widget_show(button_back);
	gtk_container_add(GTK_CONTAINER(toolitem_back), button_back);
	gtk_widget_set_size_request(button_back, 60, -1);
	gtk_tooltips_set_tip(tooltips, button_back,
			     _("\350\277\224\345\233\236\345\210\260\345"
			     "\210\232\346\211\215\350\256\277\351\227\256"
			     "\350\277\207\347\232\204\347\233\256\345\275\225"), NULL);

	alignment2 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment2);
	gtk_container_add(GTK_CONTAINER(button_back), alignment2);

	vbox_tool_back = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_tool_back);
	gtk_container_add(GTK_CONTAINER(alignment2), vbox_tool_back);

	image_back = gtk_image_new_from_stock("gtk-go-back", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image_back);
	gtk_box_pack_start(GTK_BOX(vbox_tool_back), image_back, TRUE, TRUE, 0);

	label_back = gtk_label_new(_("\345\220\216\351\200\200"));
	gtk_widget_show(label_back);
	gtk_box_pack_start(GTK_BOX(vbox_tool_back), label_back, FALSE, FALSE, 0);

	toolitem_up =(GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_up);
	gtk_container_add(GTK_CONTAINER(toolbar), toolitem_up);

	button_up = gtk_button_new();
	gtk_widget_show(button_up);
	gtk_container_add(GTK_CONTAINER(toolitem_up), button_up);
	gtk_widget_set_size_request(button_up, 60, -1);
	gtk_tooltips_set_tip(tooltips, button_up,
			     _("\350\277\224\345\233\236\344\270"
			     "\212\347\272\247\347\233\256\345\275\225"), NULL);

	alignment3 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment3);
	gtk_container_add(GTK_CONTAINER(button_up), alignment3);

	vbox_tool_up = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_tool_up);
	gtk_container_add(GTK_CONTAINER(alignment3), vbox_tool_up);

	image_up = gtk_image_new_from_stock("gtk-go-up", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image_up);
	gtk_box_pack_start(GTK_BOX(vbox_tool_up), image_up, TRUE, TRUE, 0);

	label_up = gtk_label_new(_("\345\220\221\344\270\212"));
	gtk_widget_show(label_up);
	gtk_box_pack_start(GTK_BOX(vbox_tool_up), label_up, FALSE, FALSE, 0);

	toolitem_tool =(GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_tool);
	gtk_container_add(GTK_CONTAINER(toolbar), toolitem_tool);

	vseparator_tool = gtk_vseparator_new();
	gtk_widget_show(vseparator_tool);
	gtk_container_add(GTK_CONTAINER(toolitem_tool), vseparator_tool);
	gtk_widget_set_size_request(vseparator_tool, 5, -1);

	toolitem_downolad =(GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_downolad);
	gtk_container_add(GTK_CONTAINER(toolbar), toolitem_downolad);

	button_download = gtk_button_new();
	gtk_widget_show(button_download);
	gtk_container_add(GTK_CONTAINER(toolitem_downolad), button_download);
	gtk_widget_set_size_request(button_download, 60, -1);
	gtk_tooltips_set_tip(tooltips, button_download,
			     _("\344\273\216\346\234\254\346\234\272\344"
			     "\270\213\350\275\275\346\226\207\344\273\266"
			     "\345\210\260arm\347\263\273\347\273\237\344\270\212"), NULL);

	alignment4 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment4);
	gtk_container_add(GTK_CONTAINER(button_download), alignment4);

	vbox_tool_download = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_tool_download);
	gtk_container_add(GTK_CONTAINER(alignment4), vbox_tool_download);

	image_download = gtk_image_new_from_stock("gtk-goto-bottom", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image_download);
	gtk_box_pack_start(GTK_BOX(vbox_tool_download), image_download, TRUE, TRUE, 0);

	label_download = gtk_label_new(_("\344\270\213\350\275\275"));
	gtk_widget_show(label_download);
	gtk_box_pack_start(GTK_BOX(vbox_tool_download), label_download, FALSE, FALSE, 0);

	toolitem_upload =(GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_upload);
	gtk_container_add(GTK_CONTAINER(toolbar), toolitem_upload);

	button_upload = gtk_button_new();
	gtk_widget_show(button_upload);
	gtk_container_add(GTK_CONTAINER(toolitem_upload), button_upload);
	gtk_widget_set_size_request(button_upload, 60, -1);
	gtk_tooltips_set_tip(tooltips, button_upload,
			     _("\344\273\216arm\347\263\273\347"
			     "\273\237\344\270\212\344\274\240"
			     "\346\226\207\344\273\266\345\210\260"
			     "\346\234\254\346\234\272\344\270\212"), NULL);

	alignment5 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment5);
	gtk_container_add(GTK_CONTAINER(button_upload), alignment5);

	vbox8 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox8);
	gtk_container_add(GTK_CONTAINER(alignment5), vbox8);

	image_upload = gtk_image_new_from_stock("gtk-goto-top", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image_upload);
	gtk_box_pack_start(GTK_BOX(vbox8), image_upload, TRUE, TRUE, 0);

	label_upload = gtk_label_new(_("\344\270\212\344\274\240"));
	gtk_widget_show(label_upload);
	gtk_box_pack_start(GTK_BOX(vbox8), label_upload, FALSE, FALSE, 0);
	
	
	g_signal_connect((gpointer) button_back, "clicked",
			 G_CALLBACK(cb_fview_back_clicked),
			 (gpointer)linuxarms);
	g_signal_connect((gpointer) button_up, "clicked",
			 G_CALLBACK(cb_fview_up_clicked),
			 (gpointer)linuxarms);
	g_signal_connect((gpointer) button_download, "clicked",
			 G_CALLBACK(cb_download_clicked),
			 (gpointer)linuxarms);
	g_signal_connect((gpointer) button_upload, "clicked",
			 G_CALLBACK(cb_upload_clicked),
			 (gpointer)linuxarms);
	hfview->widget.up = button_up;
	hfview->widget.back = button_back;
	debug_print("为控件指针赋值\n");
	hftrans->widget.toolbar_upload = button_upload;
	hftrans->widget.toolbar_download = button_download;

	return toolbar;
}
