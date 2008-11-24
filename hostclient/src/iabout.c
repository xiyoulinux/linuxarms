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


#define GLADE_HOOKUP_OBJECT(component,widget,name)\
	g_object_set_data_full(G_OBJECT(component), name, \
	gtk_widget_ref(widget),(GDestroyNotify)gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name)\
	g_object_set_data(G_OBJECT(component), name, widget)


GtkWidget* create_window_about(void)
{
	GtkWidget *window_about;
	GdkPixbuf *window_about_icon_pixbuf;
	GtkWidget *vbox3;
	GtkWidget *image_linuxarms;
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
	GtkWidget *fixed12;
	GtkWidget *button_close;
	GtkWidget *alignment12;
	GtkWidget *hbox20;
	GtkWidget *image14;
	GtkWidget *label8;

	window_about = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_about, 290, 239);
	gtk_window_set_title(GTK_WINDOW(window_about), 
			_("\345\205\263\344\272\216\346\210\221"));
	gtk_window_set_position(GTK_WINDOW(window_about), GTK_WIN_POS_CENTER);
	gtk_window_set_modal(GTK_WINDOW(window_about), TRUE);
	gtk_window_set_resizable(GTK_WINDOW(window_about), FALSE);
	window_about_icon_pixbuf = create_pixbuf("ico-window.png");
	if(window_about_icon_pixbuf){
		gtk_window_set_icon(GTK_WINDOW(window_about),
				window_about_icon_pixbuf);
		gdk_pixbuf_unref(window_about_icon_pixbuf);
	}
	gtk_window_set_type_hint(GTK_WINDOW(window_about),
			GDK_WINDOW_TYPE_HINT_DIALOG);

	vbox3 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox3);
	gtk_container_add(GTK_CONTAINER(window_about), vbox3);

	image_linuxarms = create_pixmap(window_about, "login.png");
	gtk_widget_show(image_linuxarms);
	gtk_box_pack_start(GTK_BOX(vbox3), image_linuxarms, FALSE, FALSE, 2);

	label_text = gtk_label_new(
		_("Linuxarms是一个为基于arm上做开发的开发人员\n"
		  "提供的方便的开发平台。"));
	gtk_widget_show(label_text);
	gtk_box_pack_start(GTK_BOX(vbox3), label_text, FALSE, FALSE, 0);

	hbox17 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox17);
	gtk_box_pack_start(GTK_BOX(vbox3), hbox17, FALSE, FALSE, 0);

	fixed6 = gtk_fixed_new();
	gtk_widget_show(fixed6);
	gtk_box_pack_start(GTK_BOX(hbox17), fixed6, TRUE, TRUE, 0);
	gtk_widget_set_sensitive(fixed6, FALSE);

	button_link = gtk_button_new_with_mnemonic(_("http://xiyoulinux.cn"));
	gtk_widget_show(button_link);
	gtk_box_pack_start(GTK_BOX(hbox17), button_link, FALSE, FALSE, 0);

	fixed7 = gtk_fixed_new();
	gtk_widget_show(fixed7);
	gtk_box_pack_start(GTK_BOX(hbox17), fixed7, TRUE, TRUE, 0);
	gtk_widget_set_sensitive(fixed7, FALSE);

	hbox15 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox15);
	gtk_box_pack_start(GTK_BOX(vbox3), hbox15, TRUE, FALSE, 12);

	button_credits = gtk_button_new();
	gtk_widget_show(button_credits);
	gtk_box_pack_start(GTK_BOX(hbox15), button_credits, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_credits, 85, -1);

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
	gtk_box_pack_start(GTK_BOX(hbox15), button_licence, TRUE, TRUE, 0);
	gtk_widget_set_size_request(button_licence, 69, -1);

	fixed12 = gtk_fixed_new();
	gtk_widget_show(fixed12);
	gtk_box_pack_start(GTK_BOX(hbox15), fixed12, TRUE, FALSE, 0);
	gtk_widget_set_size_request(fixed12, 35, -1);

	button_close = gtk_button_new();
	gtk_widget_show(button_close);
	gtk_box_pack_start(GTK_BOX(hbox15), button_close, TRUE, TRUE, 0);
	gtk_widget_set_size_request(button_close, 85, -1);

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

	g_signal_connect((gpointer)window_about, "destroy",
			 G_CALLBACK(cb_window_destroy),
			 NULL);
	g_signal_connect((gpointer)button_link, "clicked",
			 G_CALLBACK(cb_button_link_clicked),
			 NULL);
	g_signal_connect((gpointer)button_credits, "clicked",
			 G_CALLBACK(cb_button_credits_clicked),
			 NULL);
	g_signal_connect((gpointer)button_close, "clicked",
			 G_CALLBACK(cb_button_close_clicked),
			 NULL);

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF(window_about, window_about, "window_about");
	GLADE_HOOKUP_OBJECT(window_about, vbox3, "vbox3");
	GLADE_HOOKUP_OBJECT(window_about, image_linuxarms, "image_linuxarms");
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
	GLADE_HOOKUP_OBJECT(window_about, fixed12, "fixed12");
	GLADE_HOOKUP_OBJECT(window_about, button_close, "button_close");
	GLADE_HOOKUP_OBJECT(window_about, alignment12, "alignment12");
	GLADE_HOOKUP_OBJECT(window_about, hbox20, "hbox20");
	GLADE_HOOKUP_OBJECT(window_about, image14, "image14");
	GLADE_HOOKUP_OBJECT(window_about, label8, "label8");


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
	gtk_window_set_title(GTK_WINDOW(window_credits),
			_("\350\207\264\350\260\242"));
	gtk_window_set_resizable(GTK_WINDOW(window_credits), FALSE);
	gtk_window_set_icon_name(GTK_WINDOW(window_credits), "gtk-about");

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
	GTK_WIDGET_UNSET_FLAGS (textview_author, GTK_CAN_FOCUS);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_author), FALSE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview_author), FALSE);
	gtk_container_add(GTK_CONTAINER(scrolledwindow2), textview_author);
	gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW (textview_author)),
	_("项目指导老师：\n"
	 "	陈莉君: 西安邮电学院教授\n"
	 "	   技术博客：http://www.lupaworld.com/26540\n"
	 "	   邮箱：cljcore@gmail.com\n"
	 "项目负责人：\n"
	 "   牛涛：西安邮电学院软件工程专业\n"
	 "	  博客：http://niutao.cublog.cn\n"
	 "	  邮箱：niutao0602@gmail.com\n"
	 "项目成员：\n"
	 "   李则良：西安邮电学院软件工程专业\n"
	 "	  博客：http://lizeliang.cublog.cn\n"
	 "	  邮箱：lizeliang.linux@gmail.com\n"
	 "   陈继峰：西安邮电学院软件工程专业\n"
	 "	  博客：http://chenjifeng.cublog.cn\n"
	 "	  邮箱：chengov@gmail.com\n"
	 "   吴刚：西安邮电学院计算机科学与技术专业\n"
	 "	  博客：http://kylinux.cublog.cn\n"
	 "	  邮箱：xuanfeng1104@gmail.com\n"
	 "   李宁：西安邮电学院电子科学与技术专业\n"
	 "	  博客：\n"
	 "	  邮箱：gforln9543@gmail.com"),-1);
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
	GTK_WIDGET_UNSET_FLAGS (textview_doc, GTK_CAN_FOCUS);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_doc), FALSE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview_doc), FALSE);
	gtk_container_add(GTK_CONTAINER(scrolledwindow3), textview_doc);
	gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview_doc)),
			_("牛涛<niutao0602@gmail.com>\n"
			  "李则良<lizeliang.linux@gmail.com>\n"
			  "陈继峰<chengov@gmail.com>\n"
			  "吴刚<xuanfeng1104@gmail.com>\n"
			  "李宁<gforln9543@gmail.com>"), -1);

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

	g_signal_connect((gpointer)window_credits, "destroy",
			 G_CALLBACK(cb_window_destroy),
			 NULL);
	g_signal_connect((gpointer)button_thank_close, "clicked",
			 G_CALLBACK(cb_button_close_clicked),
			 NULL);

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

GtkWidget* create_window_licence(void)
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

	g_signal_connect((gpointer)window_licence, "destroy",
			 G_CALLBACK(cb_window_destroy),
			 NULL);
	g_signal_connect((gpointer)button_licence, "clicked",
			 G_CALLBACK(cb_button_close_clicked),
			 (gpointer)window_licence);

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
