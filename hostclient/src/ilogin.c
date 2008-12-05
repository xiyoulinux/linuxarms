#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "login.h"
#include "support.h"

GtkWidget* create_window_login(void)
{
	GtkWidget *window_login;
	GtkWidget *vbox_login;
	GtkWidget *image_login;
	GtkWidget *table_login;
	GtkWidget *label_ip;
	GtkWidget *label_name;
	GtkWidget *label_passwd;
	GtkWidget *comboboxentry_name;
	GtkWidget *comboboxentry_ip;
	GtkWidget *hbox5;
	GtkWidget *entry_passwd;
	GtkWidget *fixed5;
	GtkWidget *checkbutton_rem;
	GtkWidget *hbox2;
	GtkWidget *button_help;
	GtkWidget *alignment8;
	GtkWidget *hbox6;
	GtkWidget *image324;
	GtkWidget *label6;
	GtkWidget *fixed4;
	GtkWidget *button_cancel;
	GtkWidget *alignment6;
	GtkWidget *hbox3;
	GtkWidget *image322;
	GtkWidget *label_cancel;
	GtkWidget *button_ok;
	GtkWidget *alignment7;
	GtkWidget *hbox4;
	GtkWidget *image323;
	GtkWidget *label5;

	window_login = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_login, 291, 290);
	gtk_window_set_title(GTK_WINDOW(window_login),
			_("Linuxarms\357\274\210arm\345\256\236\346\227\266\347\233\221"
			"\346\216\247\347\263\273\347\273\237\347\231\273\345\275\225\357\274\211"));
	gtk_window_set_position(GTK_WINDOW(window_login), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window_login), 330, 262);
	gtk_window_set_resizable(GTK_WINDOW(window_login), FALSE);
	gtk_window_set_decorated(GTK_WINDOW(window_login),TRUE);
	

	vbox_login = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_login);
	gtk_container_add(GTK_CONTAINER(window_login), vbox_login);
	gtk_container_set_border_width(GTK_CONTAINER(vbox_login), 4);

	image_login = create_pixmap(window_login, "login.png");
	gtk_widget_show(image_login);
	gtk_box_pack_start(GTK_BOX(vbox_login), image_login, FALSE, TRUE, 0);
	gtk_widget_set_size_request(image_login, 313, 130);

	table_login = gtk_table_new(3, 2, FALSE);
	gtk_widget_show(table_login);
	gtk_box_pack_start(GTK_BOX(vbox_login), table_login, FALSE, FALSE, 0);
	gtk_widget_set_size_request(table_login, -1, 89);

	label_ip = gtk_label_new(_("arm\347\263\273\347\273\237IP:"));
	gtk_widget_show(label_ip);
	gtk_table_attach(GTK_TABLE(table_login), label_ip, 0, 1, 0, 1,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label_ip), 0, 0.5);

	label_name = gtk_label_new(_("\347\231\273\345\275\225\347\224\250\346\210\267\345\220\215:"));
	gtk_widget_show(label_name);
	gtk_table_attach(GTK_TABLE(table_login), label_name, 0, 1, 1, 2,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label_name), 0, 0.5);

	label_passwd = gtk_label_new(_("\347\224\250\346\210\267\345\257\206\347\240\201:"));
	gtk_widget_show(label_passwd);
	gtk_table_attach(GTK_TABLE(table_login), label_passwd, 0, 1, 2, 3,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label_passwd), 0, 0.5);

//	comboboxentry_name = gtk_combo_new();//gtk_combo_box_entry_new_text(); /* combo name */
/*	GList *items = NULL;
	items = g_list_append(items, "lizeliang");
	items = g_list_append(items, "niutao");
	gtk_combo_set_popdown_strings(GTK_COMBO(comboboxentry_name), items);
	*/
	comboboxentry_name = gtk_combo_box_entry_new_text();
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxentry_name), "lizeliang");
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxentry_name),"niutao");
	gtk_widget_show(comboboxentry_name);
	gtk_table_attach(GTK_TABLE(table_login), comboboxentry_name, 1, 2, 1, 2,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(GTK_FILL), 0, 0);
	
	comboboxentry_ip = gtk_combo_box_entry_new_text();/*combo ip*/
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxentry_ip), "192.168.200.67");
	gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxentry_ip), "192.168.200.68");
	gtk_widget_show(comboboxentry_ip);
	gtk_table_attach(GTK_TABLE(table_login), comboboxentry_ip, 1, 2, 0, 1,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(GTK_FILL), 0, 0);
	gtk_widget_set_size_request(comboboxentry_ip, 216, -1);

	hbox5 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox5);
	gtk_table_attach(GTK_TABLE(table_login), hbox5, 1, 2, 2, 3,
			(GtkAttachOptions)(GTK_FILL),
			(GtkAttachOptions)(GTK_FILL), 0, 0);

	entry_passwd = gtk_entry_new();
	gtk_widget_show(entry_passwd);
	gtk_box_pack_start(GTK_BOX(hbox5), entry_passwd, TRUE, TRUE, 0);
	gtk_widget_set_size_request(entry_passwd, 163, -1);
	gtk_entry_set_max_length(GTK_ENTRY(entry_passwd), 20);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_passwd), 9679);
	gtk_entry_set_visibility(GTK_ENTRY(entry_passwd),FALSE);

	fixed5 = gtk_fixed_new();
	gtk_widget_show(fixed5);
	gtk_box_pack_start(GTK_BOX(hbox5), fixed5, TRUE, TRUE, 0);

	checkbutton_rem =
		gtk_check_button_new_with_mnemonic(_("\350\256\260\344"
		"\275\217\345\257\206\347\240\201\357\274\237")); /*xuan ze kuang*/
	gtk_widget_show(checkbutton_rem);
	gtk_box_pack_start(GTK_BOX(vbox_login), checkbutton_rem, FALSE, FALSE, 0);
	gtk_widget_set_size_request(checkbutton_rem, -1, 29);
	gtk_container_set_border_width(GTK_CONTAINER(checkbutton_rem), 3);

	hbox2 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox2);
	gtk_box_pack_start(GTK_BOX(vbox_login), hbox2, TRUE, TRUE, 0);

	button_help = gtk_button_new();
	gtk_widget_show(button_help);
	gtk_box_pack_start(GTK_BOX(hbox2), button_help, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_help, 70, -1);

	alignment8 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment8);
	gtk_container_add(GTK_CONTAINER(button_help), alignment8);

	hbox6 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox6);
	gtk_container_add(GTK_CONTAINER(alignment8), hbox6);

	image324 = gtk_image_new_from_stock("gtk-help", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image324);
	gtk_box_pack_start(GTK_BOX(hbox6), image324, FALSE, FALSE, 0);

	label6 = gtk_label_new_with_mnemonic(_("\345\270\256\345\212\251"));
	gtk_widget_show(label6);
	gtk_box_pack_start(GTK_BOX(hbox6), label6, FALSE, FALSE, 0);

	fixed4 = gtk_fixed_new();
	gtk_widget_show(fixed4);
	gtk_box_pack_start(GTK_BOX(hbox2), fixed4, TRUE, TRUE, 0);

	button_cancel = gtk_button_new();
	gtk_widget_show(button_cancel);
	gtk_box_pack_start(GTK_BOX(hbox2), button_cancel, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_cancel, 70, -1);

	alignment6 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment6);
	gtk_container_add(GTK_CONTAINER(button_cancel), alignment6);

	hbox3 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox3);
	gtk_container_add(GTK_CONTAINER(alignment6), hbox3);

	image322 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image322);
	gtk_box_pack_start(GTK_BOX(hbox3), image322, FALSE, FALSE, 0);

	label_cancel = gtk_label_new_with_mnemonic(_("\345\217\226\346\266\210"));
	gtk_widget_show(label_cancel);
	gtk_box_pack_start(GTK_BOX(hbox3), label_cancel, FALSE, FALSE, 0);

	button_ok = gtk_button_new();
	gtk_widget_show(button_ok);
	gtk_box_pack_start(GTK_BOX(hbox2), button_ok, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_ok, 70, -1);
	gtk_widget_set_sensitive(button_ok,FALSE);

	alignment7 = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(alignment7);
	gtk_container_add(GTK_CONTAINER(button_ok), alignment7);

	hbox4 = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox4);
	gtk_container_add(GTK_CONTAINER(alignment7), hbox4);

	image323 = gtk_image_new_from_stock("gtk-ok", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(image323);
	gtk_box_pack_start(GTK_BOX(hbox4), image323, FALSE, FALSE, 0);

	label5 = gtk_label_new_with_mnemonic(_("\347\241\256\345\256\232"));
	gtk_widget_show(label5);
	gtk_box_pack_start(GTK_BOX(hbox4), label5, FALSE, FALSE, 0);

	g_signal_connect((gpointer)window_login, "destroy",
			G_CALLBACK(gtk_main_quit),
			NULL);
	g_signal_connect((gpointer)checkbutton_rem, "toggled",
			G_CALLBACK(cb_login_rem_toggled),
			checkbutton_rem);
	g_signal_connect((gpointer)button_help, "clicked",
			G_CALLBACK(cb_login_help_clicked),
			NULL);
	g_signal_connect((gpointer)button_cancel, "clicked",
			G_CALLBACK(gtk_main_quit),
			NULL);
	g_signal_connect((gpointer)button_ok, "clicked",
			G_CALLBACK(cb_login_ok_clicked),
			NULL);
	g_signal_connect((gpointer)comboboxentry_ip, "changed",
			G_CALLBACK(cb_comboboxentry_ip_changed),
			NULL);

	return window_login;
}
