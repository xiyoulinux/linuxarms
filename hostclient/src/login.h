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
#include "hnet.h"
#include <gtk/gtk.h>
#define USER_NAME_LEN 20
#define PASSWD_LEN 20
#define CONFIG_BUF_SIZE 256
#define INFO_SIZE 80

typedef enum {
	FAILURE = -1, /* -1 */
	SUCCESS /* 0 */
}Status;

/*
 * user_struct  用于存放获取的用户信息
 * @ip: 用户输入的IP地址
 * @name: 用户输入的用户名
 * @passwd: 用户输入的密码
 * @competence: 
 */
struct user_struct {
	char ip[IP_LEN];
	char name[USER_NAME_LEN];
	char passwd[PASSWD_LEN];
	boolean competence;
};

boolean user_struct_init(struct user_struct *user);
boolean user_competece(struct user_struct *user);
boolean user_struct_set(struct user_struct *user, char *ip, char *name, char *passwd);
/* 登录界面的控件指针 */
struct login_widget {
	GtkWidget *window_login;
	GtkWidget *server_ip;
	GtkWidget *user_name;
	GtkWidget *passwd;
	GtkWidget *remember;
	GtkWidget *button_ok;
};

struct login_config_struct;
/*
 * login_struct 用户登录处理及相关信息显示和出错处理
 * @user: 用户信息
 * @socket: 网络相关模块
 * @hmthread: 主线程控制相关
 * @error_info: 出错信息缓冲
 * @help_info: 提示、帮助信息缓冲
 * @status: 网络结果状态信息
 */
struct login_struct {
	boolean remember;
	struct user_struct *user;
	struct hnet_struct *socket;
	struct login_config_struct *config;
	struct login_widget widget;
	Status status;
};

boolean login_init(struct login_struct *login,
		   struct user_struct *user,
		   struct login_config_struct *config,
		   struct hnet_struct *socket);
void create_window_login(struct linuxarms_struct *linuxarms);
boolean login_add_default_data(struct login_struct *login);
gboolean cb_window_login_delete_event(GtkWidget *widget,
			GdkEvent  *event,gpointer user_data);
void cb_login_button_cancel_clicked(GtkWidget *widget, gpointer user_data);
void cb_login_rem_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void cb_login_help_clicked(GtkButton *button, gpointer user_data);
void cb_login_ok_clicked(GtkButton *button, gpointer user_data);
gboolean cb_server_ip_changed(GtkWidget *widget, gpointer user_data);
gboolean cb_user_name_changed(GtkWidget *widget, gpointer user_data);
gboolean cb_entry_passwd_changed(GtkWidget *widget, gpointer user_data);

#endif
