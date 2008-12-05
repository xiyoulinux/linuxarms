#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "support.h"
#include "help.h"

GtkWidget* create_window_help()
{
	GtkWidget *window_help;
	GtkWidget *vbox_main;
	GtkWidget *scrolledwindow;
	GtkWidget *textview;
	GtkWidget *hseparator;
	GtkWidget *hbox;
	GtkWidget *button_ok;

	window_help = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_help, 335,330);
	gtk_window_set_title(GTK_WINDOW(window_help), 
			_("\345\270\256\345\212\251\344\270\273\351\242\230"));
	gtk_window_set_position(GTK_WINDOW(window_help), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window_help), 335,330);
	gtk_window_set_resizable(GTK_WINDOW(window_help), FALSE);
	gtk_window_set_type_hint(GTK_WINDOW(window_help), GDK_WINDOW_TYPE_HINT_DIALOG);

	vbox_main = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_main);
	gtk_container_add(GTK_CONTAINER(window_help), vbox_main);

	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow);
	gtk_box_pack_start(GTK_BOX(vbox_main), scrolledwindow, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
				       GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow), 
								   GTK_SHADOW_IN);

	textview = gtk_text_view_new();
	gtk_widget_show(textview);
	gtk_container_add(GTK_CONTAINER(scrolledwindow), textview);
	GTK_WIDGET_UNSET_FLAGS(textview, GTK_CAN_FOCUS);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview), FALSE);

	hseparator = gtk_hseparator_new();
	gtk_widget_show(hseparator);
	gtk_box_pack_start(GTK_BOX(vbox_main), hseparator, FALSE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox);
	gtk_box_pack_start(GTK_BOX(vbox_main), hbox, FALSE, TRUE, 0);

	button_ok = gtk_button_new_from_stock("gtk-close");
	gtk_widget_show(button_ok);
	gtk_box_pack_end(GTK_BOX(hbox), button_ok, FALSE, FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(button_ok), 7);

	g_signal_connect((gpointer) window_help, "destroy",
	 		 G_CALLBACK(on_window_help_destroy),
			 (gpointer)window_help);
	g_signal_connect((gpointer) button_ok, "clicked",
			 G_CALLBACK(on_button_ok_clicked),
			 (gpointer)window_help);
	
	gtk_widget_show(window_help);
	return textview;
}
