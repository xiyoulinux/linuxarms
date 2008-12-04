/*
 * linuxarms/hostclient/src/clogin.c
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 */
#include <gtk/gtk.h>
#include "login.h"
#include "debug.h"
#include "linuxarms.h"

struct user_struct user;
/*
 * callback functions
 */
void cb_login_rem_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(user_data)) == TRUE) {
		user.is_record = 1;
#if 1
		printf("is_recode = 1 \n");

	} else {
		printf("is_recorde = 0 \n");	
#endif
	}
}


void cb_login_help_clicked(GtkButton *button, gpointer user_data)
{
	//debug_where();
	GtkWidget *dialog;
	const gchar *license =
		"This library is free software; you can redistribute it and/or\n"
		"modify it under the terms of the GNU Library \n"
		"General Public License as published by the \n"
		"Free Software Foundation; either version 2 of the License, \n"
		"or (at your option) any later version.\n"
		"This library is distributed in the hope that it will be useful,\n"
		"but WITHOUT ANY WARRANTY; without even the implied \n"
		"warranty of MERCHANTABILITY or \n"
		"FITNESS FOR A PARTICULAR PURPOSE.\n"
		"See the GNU Library General Public License for more details.\n"
		"\n"
		"You should have received a copy of the GNU Library General Public\n"
		"License along with the Gnome Library; see the file COPYING.LIB.  \n"
		"If not,write to the Free Software Foundation, Inc., \n"
		"59 Temple Place - Suite 330,Boston, MA 02111-1307, USA.\n";

	dialog = gtk_message_dialog_new (GTK_WIDGET(user_data),
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION,
                                  GTK_BUTTONS_CLOSE,
                                  "%s",
                                  license);
	gtk_widget_show(dialog);
 /* Destroy the dialog when the user responds to it (e.g. clicks a button) */
 	g_signal_connect_swapped (dialog, "response",
                           G_CALLBACK (gtk_widget_destroy),
                           dialog);  

}


void cb_login_ok_clicked(GtkButton *button, gpointer user_data)
{
	//debug_where();
	
}

void cb_comboboxentry_ip_changed(GtkComboBox *widget, gpointer user_data)
{
	char buf[20];
	char *point;
	GtkTreeIter iter;
	point = gtk_combo_box_get_active_text(widget);
	//if (gtk_combo_box_get_focus_on_click(widget) == TRUE)
	//	printf("mouse is move off\n");
	//printf("%s\n",point);
	//if (gtk_combo_box_get_active_iter(widget, &iter) == FALSE) 
	//	printf("%s\n",point);
	//gtk_combo_box_append_text(widget, point);
	switch (gtk_combo_box_get_active(widget)) {
	case 0:
		printf("chioce 0\n");
		break;
	case 1:
		printf("chioce 1\n");
		break;
	default:
		break;
	}
	
}

/*
 * some handling functions for login using
 */
boolean set_ip(struct user_struct *user, const char *ip)
{
	
}
boolean set_name(struct user_struct *user, const char *name)
{
	
}
boolean set_passwd(struct user_struct *user, const char *passwd)
{
	
}
boolean set_is_record(struct user_struct *user, int is_record)
{
	
}
boolean get_is_record(struct user_struct *user)
{
	
}
boolean print_info(char *text)
{
	
}

/*
 * some handling functions for configure using
 */
boolean is_config_exist(void)
{
	
}
boolean is_path_exist(const char *pathname)
{
	
}
boolean is_file_exist(const char *path)
{
	
}
boolean config_create() /*create $HOME/.linuxarms*/
{
	
}
boolean create_path(const char *path)
{
	
}
boolean create_file(const char *path, const char *filename)
{
	
}
const char *search_file(const char *filename) /*return file path*/
{
	
}
const char *read_file(const char *filename)
{
	
}
const  char *write_file(const char *filename)
{
	
}

/*
 * some handling functions for login
 */
boolean set_error_info(struct login_struct *login, const char *error_info)
{
	
}
const char *get_error(struct login_struct *login)
{
	
}
void show_error_dialog(const char *error) /*display the error dialog*/
{
	
}
void show_ip_tooltips(GtkWidget *login)
{
	
}
void show_name_tooltips(GtkWidget *login)
{
	
}
void show_passwd_toolstips(GtkWidget *login)
{
	
}
void show_help_dialog(const char *help_info)
{
	
}
