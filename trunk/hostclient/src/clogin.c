/*
 * linuxarms/hostclient/src/clogin.c
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 */
#include <gtk/gtk.h>
#include "login.h"
#include "debug.h"

/*
 * callback functions
 */
void cb_login_rem_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	debug_where();
}


void cb_login_help_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();

}


void cb_login_ok_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();

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
