/*
 * linuxarms/hostclient/src/clogin.c
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 */
#include <gtk/gtk.h>
#include <stdlib.h>
#include "login.h"
#include "debug.h"
#include "linuxarms.h"

#define _DEBUG_LZEL

struct user_struct user;
int ok_on_flag[3] = {0};

/*
 * static function definitions
 */
/*inward interface*/
static boolean set_ip(struct user_struct *user, const char *ip);
static boolean set_name(struct user_struct *user, const char *name);
static boolean set_passwd(struct user_struct *user, const char *passwd);
static boolean set_is_record(struct user_struct *user, int is_record);
static boolean get_is_record(struct user_struct *user);
static boolean print_info(char *text);
/*
 * configure function definitions
 */
/*inward interface*/
static boolean is_config_exist(void);
static boolean is_path_exist(const char *pathname);
static boolean is_file_exist(const char *path);
static boolean config_create(); /*create $HOME/.linuxarms*/
static boolean create_path(const char *path);
static boolean create_file(const char *path, const char *filename);
static const char *search_file(const char *filename); /*return file path*/
static const char *read_file(const char *filename);
static const  char *write_file(const char *filename);
/*
 * login functions definitions
 */
/*inward interface*/
static boolean is_ip_right(const char *ip, char *error_info);
static inline int get_domain(const char *ip, const int n);
static boolean is_name_right(const char *name, char *error_info);
static boolean set_error_info(struct login_struct *login, const char *error_info);
static const char *get_error(struct login_struct *login);
static void show_error_dialog(const char *error); /*display the error dialog*/
static void show_ip_tooltips(GtkWidget *login);
static void show_name_tooltips(GtkWidget *login);
static void show_passwd_toolstips(GtkWidget *login);
static void show_help_dialog(const char *help_info);
/*
 * callback functions
 */
void cb_login_rem_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(user_data)) == TRUE) {
		user.is_record = 1;
	} else {
		user.is_record = 0;
	}
#ifdef _DEBUG_LZEL
		printf("user.is_record = %d\n", user.is_record);
#endif
}


void cb_login_help_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog;
	const gchar *help =
		"\t\t###登录帮助###\n"
		"\t如果您是第一次使用本软件，那么请您务必使用之前阅读本帮助,"
		"以减少您登录过程中可能遇到的麻烦。"
		"\n\tIP选项："
		"\n\t请严格按照ipv4标准的格式填写此项，即整体分为4个域，域之间由\".\"分隔;"
		"每个域由固定的3个数字组成，并且数字的大小必须在0~255之间\(包括0和255\)."
		"\n\t用户名选项："
		"\n\t用户名是由字符长度不超过20个字符组成。字符种类为0-9、a-z或A-Z、_这三种。其它为无效字符。"
		"\n\t密码选项："
		"\n\t密码是由字符长度不超过20个字符组成。字符种类为0-9、a-z或A-Z、_这三种。其它为无效字符。"
		"\n\t记住密码选项框:"
		"\n\t选中则下次登录时会自动输入密码,无需用户再次手动输入。"
		;

	dialog = gtk_message_dialog_new (GTK_WIDGET(user_data),
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION,
                                  GTK_BUTTONS_CLOSE,
                                  "%s",
                                  help);
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
	char *user_ip;
	//have not fix
	char buf[20];
	GtkTreeIter iter;
	
	user_ip = gtk_combo_box_get_active_text(widget); //get user input ip
	if (strcmp(user.ip, user_ip) != 0)
		strcpy(user.ip, user_ip);
		
#ifdef _DEBUG_LZEL_
	printf("user.ip = %s\n", user.ip);
#endif
	
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
	ok_on_flag[0] = 1;
	ok_on_flag[2] = 1;
	if ((ok_on_flag[0] == 1) && (ok_on_flag[1] == 1) && (ok_on_flag[2] == 1))
		gtk_widget_set_sensitive(user_data, TRUE);
}
void cb_comboboxentry_name_changed(GtkComboBox *widget, gpointer user_data)
{
	char *user_name;
	//have not fix
	char buf[20];
	GtkTreeIter iter;
	
	user_name = gtk_combo_box_get_active_text(widget); //get user input ip
	if (strcmp(user.name, user_name) != 0)
		strcpy(user.name, user_name);
#ifdef _DEBUG_LZEL_
	printf("user.ip = %s\n", user.name);
#endif

#if 0
	if (gtk_combo_box_get_focus_on_click(widget) == TRUE) {
		printf("focus on\n");
	}
	if (gtk_combo_box_get_focus_on_click(widget) == FALSE) {
		printf("focus off\n");
	}
#endif
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
	ok_on_flag[1] = 1;
	if ((ok_on_flag[0] == 1) && (ok_on_flag[1] == 1) && (ok_on_flag[2] == 1))
		gtk_widget_set_sensitive(user_data, TRUE);
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
static boolean is_ip_right(const char *ip, char *error_info)
{
	boolean is_right = FALSE;
	int ip_len;
	int ip_domain;
	int i;
	
	ip_len = strlen(ip);
	if (ip_len != 15) {
#ifdef _DEBUG_LZEL
		printf("the lenght of ip is %d not 15 \n", ip_len);
#endif
		sprintf(error_info,"the totle lenght of ip is not right");
		goto out;
	}
	if ( !((ip[3] == '.') && (ip[7] == '.') && (ip[11] == '.')) ) {
#ifdef _DEBUG_LZEL
		printf("the format of ip is not right\n");
#endif
		sprintf(error_info,"the format of ip is not right");
		goto out;
	}
	for ( i = 0; i <= 4; i++) {
		if ((ip_domain = get_domain(ip, i)) < 0) {
#ifdef _DEBUG_LZEL
		debug_where();
#endif				
			goto out;
		}
		if ( !((ip_domain >= 0) && (ip_domain <= 255)) ) {
#ifdef _DEBUG_LZEL
		printf("IP domain in the %dth is wrong\n", i);
#endif				
		sprintf(error_info, "the format of some domain is not right\n");
		goto out;
		}
	}
	is_right = TRUE;
out:
	return is_right;
}
static inline int get_domain(const char *ip, const int n)
{
	int result;
	char tmp[3];
	int i, j = 0;
	if ((n < 0) || (n >3))
		goto out;
	for (i = n*4; i <= (n*4 + 3); i++, j++) {
		tmp[j] = ip[i];
	}
	result = atoi(tmp);
out:
	return result;
}
static boolean is_name_right(const char *name, char *error_info)
{
	boolean is_right = FALSE;
	int name_len;
	
	name_len = strlen(name);
	if ((name_len < 4) || (name_len > 20)) {
#ifdef _DEBUG_LZEL
		printf("name length should between 4 and 20\n");
#endif		
		sprintf(error_info, "name length should between 4 and 20\n");
		goto out;
	}
	is_right = TRUE;
out:
	return is_right;
}
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

char *get_armserver_ip()
{
}
