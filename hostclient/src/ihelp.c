#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "support.h"
#include "help.h"

GtkWidget* create_dialog_help(GtkWidget *parent)
{
	GtkWidget *dialog_help;
	GtkWidget *dialog_vbox1;
	GtkWidget *notebook1;
	GtkWidget *scrolledwindow1;
	GtkWidget *textview_ssinfo;
	GtkWidget *label_ssinfo;
	GtkWidget *scrolledwindow2;
	GtkWidget *textview_process;
	GtkWidget *label_process;
	GtkWidget *scrolledwindow3;
	GtkWidget *textview_fview;
	GtkWidget *label_fview;
	GtkWidget *scrolledwindow4;
	GtkWidget *textview_sctrl;
	GtkWidget *label_sctrl;
	GtkWidget *dialog_action_area1;
	GtkWidget *okbutton;
	GtkWidget *alignment1;
	GtkWidget *hbox1;
	GtkWidget *image1;
	GtkWidget *label5;
	
	dialog_help = gtk_dialog_new_with_buttons("帮助", GTK_WINDOW(parent),
				GTK_DIALOG_DESTROY_WITH_PARENT,NULL);
	gtk_widget_set_size_request(dialog_help, 350, 300);
	gtk_window_set_position(GTK_WINDOW(dialog_help), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(dialog_help), FALSE);
	gtk_window_set_type_hint(GTK_WINDOW(dialog_help), GDK_WINDOW_TYPE_HINT_DIALOG);

	dialog_vbox1 = GTK_DIALOG(dialog_help)->vbox;
	gtk_widget_show(dialog_vbox1);

	notebook1 = gtk_notebook_new();
	gtk_widget_show(notebook1);
	gtk_box_pack_start(GTK_BOX(dialog_vbox1), notebook1, TRUE, TRUE, 0);

	scrolledwindow1 = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow1);
	gtk_container_add(GTK_CONTAINER(notebook1), scrolledwindow1);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow1), 
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow1), GTK_SHADOW_IN);

	textview_ssinfo = gtk_text_view_new();
	gtk_widget_show(textview_ssinfo);
	gtk_container_add(GTK_CONTAINER(scrolledwindow1), textview_ssinfo);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_ssinfo), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview_ssinfo), FALSE);

	label_ssinfo = gtk_label_new(_("系统信息"));
	gtk_widget_show(label_ssinfo);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook1),
			gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook1), 0), label_ssinfo);

	scrolledwindow2 = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow2);
	gtk_container_add(GTK_CONTAINER(notebook1), scrolledwindow2);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow2), 
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow2), GTK_SHADOW_IN);

	textview_process = gtk_text_view_new();
	gtk_widget_show(textview_process);
	gtk_container_add(GTK_CONTAINER(scrolledwindow2), textview_process);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_process), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview_process), FALSE);

	label_process = gtk_label_new(_("进程信息"));
	gtk_widget_show(label_process);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook1),
			gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook1), 1), label_process);

	scrolledwindow3 = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow3);
	gtk_container_add(GTK_CONTAINER(notebook1), scrolledwindow3);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow3), 
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow3), GTK_SHADOW_IN);

	textview_fview = gtk_text_view_new();
	gtk_widget_show(textview_fview);
	gtk_container_add(GTK_CONTAINER(scrolledwindow3), textview_fview);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_fview), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview_fview), FALSE);

	label_fview = gtk_label_new(_("文件浏览"));
	gtk_widget_show(label_fview);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook1),
			gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook1), 2), label_fview);

	scrolledwindow4 = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow4);
	gtk_container_add(GTK_CONTAINER(notebook1), scrolledwindow4);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow4), 
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow4), GTK_SHADOW_IN);

	textview_sctrl = gtk_text_view_new();
	gtk_widget_show(textview_sctrl);
	gtk_container_add(GTK_CONTAINER(scrolledwindow4), textview_sctrl);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_sctrl), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview_sctrl), FALSE);

	label_sctrl = gtk_label_new(_("实时控制"));
	gtk_widget_show(label_sctrl);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook1),
			gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook1), 3), label_sctrl);

	dialog_action_area1 = GTK_DIALOG(dialog_help)->action_area;
	gtk_widget_show(dialog_action_area1);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area1), GTK_BUTTONBOX_END);

	okbutton = gtk_button_new();
	gtk_widget_show(okbutton);
	gtk_dialog_add_action_widget(GTK_DIALOG(dialog_help), okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS(okbutton, GTK_CAN_DEFAULT);

	alignment1 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment1);
	gtk_container_add(GTK_CONTAINER(okbutton), alignment1);

	hbox1 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox1);
	gtk_container_add(GTK_CONTAINER(alignment1), hbox1);

	image1 = gtk_image_new_from_stock("gtk-close", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image1);
	gtk_box_pack_start(GTK_BOX(hbox1), image1, FALSE, FALSE, 0);

	label5 = gtk_label_new_with_mnemonic(_("\345\205\263\351\227\255"));
	gtk_widget_show(label5);
	gtk_box_pack_start(GTK_BOX(hbox1), label5, FALSE, FALSE, 0);

	g_signal_connect((gpointer)dialog_help, "destroy",
	 		 G_CALLBACK(cb_dialog_help_destroy),
			(gpointer)dialog_help);
	g_signal_connect((gpointer)okbutton, "clicked",
			 G_CALLBACK(cb_help_button_clicked),
			(gpointer)dialog_help);
	gtk_widget_show(dialog_help);
	return dialog_help;
}

