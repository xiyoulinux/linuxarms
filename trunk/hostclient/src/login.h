/*
 * linuxarms/hostclient/src/login.h
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 * 
 * The file contains the private data structure and API definitions.
 */
#ifndef _LOGIN_H
#define _LOGIN_H
#include "linuxarms.h"
#include "error.h"
#include "debug.h"
#define USER_NAME_LEN 20
#define PASSWD_LEN 20
/*
 * 
 * 
 */
struct user_struct {
	char ip[15];
	char name[USER_NAME_LEN];
	char passwd[PASSWD_LEN];
	int is_record;
	boolean competence;
	
	boolean (*set_ip)(struct user_struct *user, const char *ip);
	boolean (*set_name)(struct user_struct *user, const char *name);
	boolean (*set_passwd)(struct user_struct *user, const char *passwd);
	boolean (*set_is_record)(struct user_struct *user, int is_record);
	boolean (*get_is_record)(struct user_struct *user);
	boolean (*print_info)(char *text);
	void (*print_error)(int error,const char *text);
};
/*
 * login handling function definitions
 */
boolean set_ip(struct user_struct *user, const char *ip);
boolean set_name(struct user_struct *user, const char *name);
boolean set_passwd(struct user_struct *user, const char *passwd);
boolean set_is_record(struct user_struct *user, int is_record);
boolean get_is_record(struct user_struct *user);
boolean print_info(char *text);
/*
 * 
 * 
 */
struct config_struct {
	
	boolean (*is_config_exist)(void);
	boolean (*is_path_exist)(const char *pathname);
	boolean (*is_file_exist)(const char *path);
	boolean (*config_create)(); /*create $HOME/.linuxarms*/
	boolean (*create_path)(const char *path);
	boolean (*create_file)(const char *path, const char *filename);
	const char *(*search_file)(const char *filename); /*return file path*/
	const char *(*read_file)(const char *filename);
	const  char *(*write_file)(const char *filename);
};
/*
 * configure function definitions
 */
boolean is_config_exist(void);
boolean is_path_exist(const char *pathname);
boolean is_file_exist(const char *path);
boolean config_create(); /*create $HOME/.linuxarms*/
boolean create_path(const char *path);
boolean create_file(const char *path, const char *filename);
const char *search_file(const char *filename); /*return file path*/
const char *read_file(const char *filename);
const  char *write_file(const char *filename);
/*
 * 
 * 
 */
struct login_struct {
	char error_info[80];
	char help_info[80];
	
	boolean (*set_error_info)(struct login_struct *login, const char *error_info);
	const char *(*get_error)(struct login_struct *login);
	void (*show_error_dialog)(const char *error); /*display the error dialog*/
	void (*show_ip_tooltips)(GtkWidget *login);
	void (*show_name_tooltips)(GtkWidget *login);
	void (*show_passwd_toolstips)(GtkWidget *login);
	void (*show_help_dialog)(const char *help_info);
};
/*
 * login functions definitions
 */
boolean set_error_info(struct login_struct *login, const char *error_info);
const char *get_error(struct login_struct *login);
void show_error_dialog(const char *error); /*display the error dialog*/
void show_ip_tooltips(GtkWidget *login);
void show_name_tooltips(GtkWidget *login);
void show_passwd_toolstips(GtkWidget *login);
void show_help_dialog(const char *help_info);
/*
 * callback functions definitions
 */ 
void cb_login_help_clicked(GtkButton *button, gpointer user_data); /*call login->show_help_dialog(struct login)*/

void cb_login_cancel_clicked(GtkButton *button, gpointer user_data);

void cb_login_ok_clicked(GtkButton *button, gpointer user_data);

void cb_login_rem_toggled(GtkToggleButton *togglebutton, gpointer user_data);
#endif
