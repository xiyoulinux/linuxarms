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
#include "support.h"
#include "about.h"


#define GLADE_HOOKUP_OBJECT(component,widget,name) \
	g_object_set_data_full(G_OBJECT(component), name, \
	gtk_widget_ref(widget),(GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
	g_object_set_data(G_OBJECT(component), name, widget)


GtkWidget* create_window_about(void)
{
	GtkWidget *window_about;
	GdkPixbuf *window_about_icon_pixbuf;
	GtkWidget *vbox3;
	GtkWidget *hbox16;
	GtkWidget *fixed8;
	GtkWidget *vbox4;
	GtkWidget *image_help;
	GtkWidget *label_help;
	GtkWidget *fixed9;
	GtkWidget *label_text;
	GtkWidget *hbox17;
	GtkWidget *fixed6;
	GtkWidget *button_link;
	GtkWidget *fixed7;
	GtkWidget *hbox15;
	GtkWidget *button_credits;
	GtkWidget *alignment10;
	GtkWidget *hbox18;
	GtkWidget *image12;
	GtkWidget *label_credits;
	GtkWidget *button_licence;
	GtkWidget *fixed5;
	GtkWidget *button_close;
	GtkWidget *alignment12;
	GtkWidget *hbox20;
	GtkWidget *image14;
	GtkWidget *label8;
	GtkAccelGroup *accel_group;

	accel_group = gtk_accel_group_new();

	window_about = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_widget_set_size_request(window_about, 359, 260);
	gtk_window_set_title(GTK_WINDOW(window_about), _("\345\205\263\344\272\216\346\210\221"));
	gtk_window_set_position(GTK_WINDOW(window_about), GTK_WIN_POS_CENTER);
	gtk_window_set_modal(GTK_WINDOW(window_about), TRUE);
	gtk_window_set_resizable(GTK_WINDOW(window_about), FALSE);
	window_about_icon_pixbuf = create_pixbuf("ico-window.png");
	if (window_about_icon_pixbuf) {
			gtk_window_set_icon(GTK_WINDOW(window_about), window_about_icon_pixbuf);
			gdk_pixbuf_unref(window_about_icon_pixbuf);
	}
	gtk_window_set_type_hint(GTK_WINDOW(window_about), GDK_WINDOW_TYPE_HINT_DIALOG);

	vbox3 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox3);
	gtk_container_add(GTK_CONTAINER(window_about), vbox3);

	hbox16 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox16);
	gtk_box_pack_start(GTK_BOX(vbox3), hbox16, TRUE, TRUE, 0);

	fixed8 = gtk_fixed_new();
	gtk_widget_show(fixed8);
	gtk_box_pack_start(GTK_BOX(hbox16), fixed8, TRUE, TRUE, 0);
	gtk_widget_set_sensitive(fixed8, FALSE);

	vbox4 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox4);
	gtk_box_pack_start(GTK_BOX(hbox16), vbox4, TRUE, TRUE, 0);

	image_help = create_pixmap(window_about, "help.png");
	gtk_widget_show(image_help);
	gtk_box_pack_start(GTK_BOX(vbox4), image_help, FALSE, FALSE, 0);

	label_help = gtk_label_new_with_mnemonic(Q_("xyeasyblog\357\274\2100.1\357\274\211"));
	gtk_widget_show(label_help);
	gtk_box_pack_start(GTK_BOX(vbox4), label_help, FALSE, FALSE, 0);
	gtk_label_set_ellipsize(GTK_LABEL(label_help), PANGO_ELLIPSIZE_MIDDLE);

	fixed9 = gtk_fixed_new();
	gtk_widget_show(fixed9);
	gtk_box_pack_start(GTK_BOX(hbox16), fixed9, TRUE, TRUE, 0);
	gtk_widget_set_sensitive(fixed9, FALSE);

	label_text = gtk_label_new(_("LINUARMS"));
	gtk_widget_show(label_text);
	gtk_box_pack_start(GTK_BOX(vbox3), label_text, FALSE, FALSE, 0);

	hbox17 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox17);
	gtk_box_pack_start(GTK_BOX(vbox3), hbox17, FALSE, FALSE, 0);

	fixed6 = gtk_fixed_new();
	gtk_widget_show(fixed6);
	gtk_box_pack_start(GTK_BOX(hbox17), fixed6, TRUE, TRUE, 0);
	gtk_widget_set_sensitive(fixed6, FALSE);

	/*button_link = gtk_button_new_with_mnemonic(_("http://niutao.cublog.cn"));*/
	button_link=gtk_link_button_new_with_label(_("htp://xiyoulinux.cn"),_("http://xiyoulinux.cn"));
	gtk_widget_show(button_link);
	gtk_box_pack_start(GTK_BOX(hbox17), button_link, FALSE, FALSE, 0);

	fixed7 = gtk_fixed_new();
	gtk_widget_show(fixed7);
	gtk_box_pack_start(GTK_BOX(hbox17), fixed7, TRUE, TRUE, 0);
	gtk_widget_set_sensitive(fixed7, FALSE);

	hbox15 = gtk_hbox_new(TRUE, 0);
	gtk_widget_show(hbox15);
	gtk_box_pack_start(GTK_BOX(vbox3), hbox15, FALSE, FALSE, 6);
	gtk_widget_set_size_request(hbox15, -1, 35);

	button_credits = gtk_button_new();
	gtk_widget_show(button_credits);
	gtk_box_pack_start(GTK_BOX(hbox15), button_credits, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_credits, 85, -1);
	gtk_widget_add_accelerator(button_credits, "clicked", 
			accel_group, GDK_R,(GdkModifierType)GDK_MOD1_MASK,
			GTK_ACCEL_VISIBLE);

	alignment10 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment10);
	gtk_container_add(GTK_CONTAINER(button_credits), alignment10);

	hbox18 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox18);
	gtk_container_add(GTK_CONTAINER(alignment10), hbox18);

	image12 = gtk_image_new_from_stock("gtk-about", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image12);
	gtk_box_pack_start(GTK_BOX(hbox18), image12, FALSE, FALSE, 0);
	gtk_misc_set_padding(GTK_MISC(image12), 4, 0);

	label_credits = gtk_label_new_with_mnemonic(_("\350\207\264\350\260\242(_R)"));
	gtk_widget_show(label_credits);
	gtk_box_pack_start(GTK_BOX(hbox18), label_credits, FALSE, FALSE, 0);

	button_licence = gtk_button_new_with_mnemonic(_("\350\256\270\345\217\257(_L)"));
	gtk_widget_show(button_licence);
	gtk_box_pack_start(GTK_BOX(hbox15), button_licence, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_licence, 80, -1);
	gtk_widget_add_accelerator(button_licence, "clicked", accel_group, 
			GDK_L,(GdkModifierType)GDK_MOD1_MASK, 
			GTK_ACCEL_VISIBLE);

	fixed5 = gtk_fixed_new();
	gtk_widget_show(fixed5);
	gtk_box_pack_start(GTK_BOX(hbox15), fixed5, TRUE, TRUE, 0);
	gtk_widget_set_sensitive(fixed5, FALSE);

	button_close = gtk_button_new();
	gtk_widget_show(button_close);
	gtk_box_pack_start(GTK_BOX(hbox15), button_close, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_close, 85, -1);
	gtk_widget_add_accelerator(button_close, "clicked", accel_group,
			GDK_C,(GdkModifierType)GDK_MOD1_MASK, 
			GTK_ACCEL_VISIBLE);

	alignment12 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment12);
	gtk_container_add(GTK_CONTAINER(button_close), alignment12);

	hbox20 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox20);
	gtk_container_add(GTK_CONTAINER(alignment12), hbox20);

	image14 = gtk_image_new_from_stock("gtk-close", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image14);
	gtk_box_pack_start(GTK_BOX(hbox20), image14, FALSE, FALSE, 0);
	gtk_misc_set_padding(GTK_MISC(image14), 4, 0);

	label8 = gtk_label_new_with_mnemonic(_("\345\205\263\351\227\255(_C)"));
	gtk_widget_show(label8);
	gtk_box_pack_start(GTK_BOX(hbox20), label8, FALSE, FALSE, 0);

	g_signal_connect((gpointer) window_about, "destroy",
			 G_CALLBACK(on_window_destroy), NULL);
	g_signal_connect((gpointer) button_link, "clicked", 
			 G_CALLBACK(on_button_link_clicked), NULL);
	g_signal_connect((gpointer) button_credits, "clicked",
			 G_CALLBACK(on_button_credits_clicked),	NULL);
	g_signal_connect((gpointer) button_licence, "clicked",
			 G_CALLBACK(on_button_licence_clicked), NULL);										
	g_signal_connect((gpointer) button_close, "clicked",
			 G_CALLBACK(on_button_close_clicked),(gpointer)window_about);

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF(window_about, window_about, "window_about");
	GLADE_HOOKUP_OBJECT(window_about, vbox3, "vbox3");
	GLADE_HOOKUP_OBJECT(window_about, hbox16, "hbox16");
	GLADE_HOOKUP_OBJECT(window_about, fixed8, "fixed8");
	GLADE_HOOKUP_OBJECT(window_about, vbox4, "vbox4");
	GLADE_HOOKUP_OBJECT(window_about, image_help, "image_help");
	GLADE_HOOKUP_OBJECT(window_about, label_help, "label_help");
	GLADE_HOOKUP_OBJECT(window_about, fixed9, "fixed9");
	GLADE_HOOKUP_OBJECT(window_about, label_text, "label_text");
	GLADE_HOOKUP_OBJECT(window_about, hbox17, "hbox17");
	GLADE_HOOKUP_OBJECT(window_about, fixed6, "fixed6");
	GLADE_HOOKUP_OBJECT(window_about, button_link, "button_link");
	GLADE_HOOKUP_OBJECT(window_about, fixed7, "fixed7");
	GLADE_HOOKUP_OBJECT(window_about, hbox15, "hbox15");
	GLADE_HOOKUP_OBJECT(window_about, button_credits, "button_credits");
	GLADE_HOOKUP_OBJECT(window_about, alignment10, "alignment10");
	GLADE_HOOKUP_OBJECT(window_about, hbox18, "hbox18");
	GLADE_HOOKUP_OBJECT(window_about, image12, "image12");
	GLADE_HOOKUP_OBJECT(window_about, label_credits, "label_credits");
	GLADE_HOOKUP_OBJECT(window_about, button_licence, "button_licence");
	GLADE_HOOKUP_OBJECT(window_about, fixed5, "fixed5");
	GLADE_HOOKUP_OBJECT(window_about, button_close, "button_close");
	GLADE_HOOKUP_OBJECT(window_about, alignment12, "alignment12");
	GLADE_HOOKUP_OBJECT(window_about, hbox20, "hbox20");
	GLADE_HOOKUP_OBJECT(window_about, image14, "image14");
	GLADE_HOOKUP_OBJECT(window_about, label8, "label8");

	gtk_window_add_accel_group(GTK_WINDOW(window_about), accel_group);

	return window_about;
}

GtkWidget*
create_window_credits(void)
{
	GtkWidget *window_credits;
	GtkWidget *vbox5;
	GtkWidget *notebook1;
	GtkWidget *scrolledwindow2;
	GtkWidget *textview_author;
	GtkWidget *label_author;
	GtkWidget *scrolledwindow3;
	GtkWidget *textview_doc;
	GtkWidget *label_document;
	GtkWidget *hbox21;
	GtkWidget *fixed10;
	GtkWidget *button_thank_close;
	GtkWidget *alignment13;
	GtkWidget *hbox23;
	GtkWidget *image15;
	GtkWidget *label11;
	GtkAccelGroup *accel_group;

	accel_group = gtk_accel_group_new();

	window_credits = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_credits, 360, 260);
	gtk_window_set_title(GTK_WINDOW(window_credits), _("\350\207\264\350\260\242"));
	gtk_window_set_resizable(GTK_WINDOW(window_credits), FALSE);
	gtk_window_set_modal(GTK_WINDOW(window_credits), TRUE);	
	gtk_window_set_icon_name(GTK_WINDOW(window_credits), "gtk-about");
	gtk_window_set_type_hint(GTK_WINDOW(window_credits), GDK_WINDOW_TYPE_HINT_DIALOG);

	vbox5 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox5);
	gtk_container_add(GTK_CONTAINER(window_credits), vbox5);
	gtk_widget_set_size_request(vbox5, -1, 122);

	notebook1 = gtk_notebook_new();
	gtk_widget_show(notebook1);
	gtk_box_pack_start(GTK_BOX(vbox5), notebook1, TRUE, TRUE, 0);

	scrolledwindow2 = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow2);
	gtk_container_add(GTK_CONTAINER(notebook1), scrolledwindow2);
	gtk_container_set_border_width(GTK_CONTAINER(scrolledwindow2), 5);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow2), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow2), GTK_SHADOW_IN);

	textview_author = gtk_text_view_new();
	gtk_widget_show(textview_author);
	gtk_container_add(GTK_CONTAINER(scrolledwindow2), textview_author);
	GTK_WIDGET_UNSET_FLAGS(textview_author, GTK_CAN_FOCUS);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_author), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview_author), FALSE);

	label_author = gtk_label_new(_("\347\274\226\345\206\231\350\200\205"));
	gtk_widget_show(label_author);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook1), 
			gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook1), 0), label_author);

	scrolledwindow3 = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow3);
	gtk_container_add(GTK_CONTAINER(notebook1), scrolledwindow3);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow3),
				       GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow3), GTK_SHADOW_IN);

	textview_doc = gtk_text_view_new();
	gtk_widget_show(textview_doc);
	gtk_container_add(GTK_CONTAINER(scrolledwindow3), textview_doc);
	GTK_WIDGET_UNSET_FLAGS(textview_doc, GTK_CAN_FOCUS);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_doc), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview_doc), FALSE);

	label_document = gtk_label_new(_("\346\226\207\346\241\243\346\224\245\345\206\231\350\200\205"));
	gtk_widget_show(label_document);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook1),
			           gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook1), 1), label_document);

	hbox21 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox21);
	gtk_box_pack_start(GTK_BOX(vbox5), hbox21, FALSE, FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(hbox21), 8);

	fixed10 = gtk_fixed_new();
	gtk_widget_show(fixed10);
	gtk_box_pack_start(GTK_BOX(hbox21), fixed10, TRUE, TRUE, 0);

	button_thank_close = gtk_button_new();
	gtk_widget_show(button_thank_close);
	gtk_box_pack_start(GTK_BOX(hbox21), button_thank_close, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_thank_close, 86, -1);
	gtk_widget_add_accelerator(button_thank_close, "clicked", accel_group,
			           GDK_C,(GdkModifierType) GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);

	alignment13 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment13);
	gtk_container_add(GTK_CONTAINER(button_thank_close), alignment13);

	hbox23 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox23);
	gtk_container_add(GTK_CONTAINER(alignment13), hbox23);

	image15 = gtk_image_new_from_stock("gtk-close", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image15);
	gtk_box_pack_start(GTK_BOX(hbox23), image15, FALSE, FALSE, 0);

	label11 = gtk_label_new_with_mnemonic(_("\345\205\263\351\227\255(_C)"));
	gtk_widget_show(label11);
	gtk_box_pack_start(GTK_BOX(hbox23), label11, FALSE, FALSE, 0);

	g_signal_connect((gpointer) window_credits, "destroy", 
			 G_CALLBACK(on_window_destroy), NULL);
	g_signal_connect((gpointer) button_thank_close, "clicked", 
			 G_CALLBACK(on_button_close_clicked), (gpointer)window_credits);
 
	gtk_textview_set_text(textview_author,"牛涛 niutao0602@gmail.com\n") ;
	gtk_textview_set_text(textview_doc,"文档攥写者：牛涛\n");					

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF(window_credits, window_credits, "window_credits");
	GLADE_HOOKUP_OBJECT(window_credits, vbox5, "vbox5");
	GLADE_HOOKUP_OBJECT(window_credits, notebook1, "notebook1");
	GLADE_HOOKUP_OBJECT(window_credits, scrolledwindow2, "scrolledwindow2");
	GLADE_HOOKUP_OBJECT(window_credits, textview_author, "textview_author");
	GLADE_HOOKUP_OBJECT(window_credits, label_author, "label_author");
	GLADE_HOOKUP_OBJECT(window_credits, scrolledwindow3, "scrolledwindow3");
	GLADE_HOOKUP_OBJECT(window_credits, textview_doc, "textview_doc");
	GLADE_HOOKUP_OBJECT(window_credits, label_document, "label_document");
	GLADE_HOOKUP_OBJECT(window_credits, hbox21, "hbox21");
	GLADE_HOOKUP_OBJECT(window_credits, fixed10, "fixed10");
	GLADE_HOOKUP_OBJECT(window_credits, button_thank_close, "button_thank_close");
	GLADE_HOOKUP_OBJECT(window_credits, alignment13, "alignment13");
	GLADE_HOOKUP_OBJECT(window_credits, hbox23, "hbox23");
	GLADE_HOOKUP_OBJECT(window_credits, image15, "image15");
	GLADE_HOOKUP_OBJECT(window_credits, label11, "label11");

	gtk_window_add_accel_group(GTK_WINDOW(window_credits), accel_group);

	return window_credits;
}

GtkWidget*
create_window_licence(void)
{
	GtkWidget *window_licence;
	GtkWidget *vbox6;
	GtkWidget *scrolledwindow4;
	GtkWidget *textview_licence;
	GtkWidget *hbox22;
	GtkWidget *fixed11;
	GtkWidget *button_licence;
	GtkWidget *alignment14;
	GtkWidget *hbox24;
	GtkWidget *image16;
	GtkWidget *label12;
	GtkAccelGroup *accel_group;

	accel_group = gtk_accel_group_new();

	window_licence = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_licence, 360, 260);
	gtk_window_set_title(GTK_WINDOW(window_licence), _("\350\256\270\345\217\257"));
	gtk_window_set_resizable(GTK_WINDOW(window_licence), FALSE);
	gtk_window_set_modal(GTK_WINDOW(window_licence), TRUE);	
	gtk_window_set_icon_name(GTK_WINDOW(window_licence), "gtk-select-color");
	gtk_window_set_type_hint(GTK_WINDOW(window_licence), GDK_WINDOW_TYPE_HINT_DIALOG);

	vbox6 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox6);
	gtk_container_add(GTK_CONTAINER(window_licence), vbox6);

	scrolledwindow4 = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow4);
	gtk_box_pack_start(GTK_BOX(vbox6), scrolledwindow4, FALSE, TRUE, 0);
	gtk_widget_set_size_request(scrolledwindow4, -1, 210);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow4),
				       GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow4), GTK_SHADOW_IN);

	textview_licence = gtk_text_view_new();
	gtk_widget_show(textview_licence);
	gtk_container_add(GTK_CONTAINER(scrolledwindow4), textview_licence);
	gtk_widget_set_size_request(textview_licence, 20, 44);
	gtk_container_set_border_width(GTK_CONTAINER(textview_licence), 10);
	GTK_WIDGET_UNSET_FLAGS(textview_licence, GTK_CAN_FOCUS);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_licence), FALSE);

	hbox22 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox22);
	gtk_box_pack_start(GTK_BOX(vbox6), hbox22, FALSE, TRUE, 0);
	gtk_widget_set_size_request(hbox22, -1, 49);
	gtk_container_set_border_width(GTK_CONTAINER(hbox22), 6);

	fixed11 = gtk_fixed_new();
	gtk_widget_show(fixed11);
	gtk_box_pack_start(GTK_BOX(hbox22), fixed11, TRUE, TRUE, 0);

	button_licence = gtk_button_new();
	gtk_widget_show(button_licence);
	gtk_box_pack_start(GTK_BOX(hbox22), button_licence, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_licence, 85, 25);
	gtk_widget_add_accelerator(button_licence, "clicked", accel_group,
			           GDK_C,(GdkModifierType)GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);

	alignment14 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment14);
	gtk_container_add(GTK_CONTAINER(button_licence), alignment14);

	hbox24 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox24);
	gtk_container_add(GTK_CONTAINER(alignment14), hbox24);

	image16 = gtk_image_new_from_stock("gtk-close", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image16);
	gtk_box_pack_start(GTK_BOX(hbox24), image16, FALSE, FALSE, 0);

	label12 = gtk_label_new_with_mnemonic(_("\345\205\263\351\227\255(_C)"));
	gtk_widget_show(label12);
	gtk_box_pack_start(GTK_BOX(hbox24), label12, FALSE, FALSE, 0);

	g_signal_connect((gpointer) window_licence, "destroy",
			 G_CALLBACK(on_window_destroy), NULL);
	g_signal_connect((gpointer) button_licence, "clicked",
			 G_CALLBACK(on_button_close_clicked), (gpointer)window_licence);

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF(window_licence, window_licence, "window_licence");
	GLADE_HOOKUP_OBJECT(window_licence, vbox6, "vbox6");
	GLADE_HOOKUP_OBJECT(window_licence, scrolledwindow4, "scrolledwindow4");
	GLADE_HOOKUP_OBJECT(window_licence, textview_licence, "textview_licence");
	GLADE_HOOKUP_OBJECT(window_licence, hbox22, "hbox22");
	GLADE_HOOKUP_OBJECT(window_licence, fixed11, "fixed11");
	GLADE_HOOKUP_OBJECT(window_licence, button_licence, "button_licence");
	GLADE_HOOKUP_OBJECT(window_licence, alignment14, "alignment14");
	GLADE_HOOKUP_OBJECT(window_licence, hbox24, "hbox24");
	GLADE_HOOKUP_OBJECT(window_licence, image16, "image16");
	GLADE_HOOKUP_OBJECT(window_licence, label12, "label12");

	gtk_window_add_accel_group(GTK_WINDOW(window_licence), accel_group);

	gtk_widget_show(window_licence);
	return textview_licence;
}
