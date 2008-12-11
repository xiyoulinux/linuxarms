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
 * @is_record: 用户是否选择记住用户名和密码
 * @competence: 
 */
struct user_struct {
	char ip[16];
	char name[USER_NAME_LEN];
	char passwd[PASSWD_LEN];
	int is_record;
	boolean competence;
};

/*
 * user_config_struct 目录及文件安装配置相关数据结构和公用接口API
 * @buffer: 读文件存放临时数据的缓冲区
 * @create_file: 根据提供的文件名创建配置文件
 * @read_from_config_file: 根据提供的文件名读取数据到缓冲区
 * @write_to_config_file: 把提供的数据写入到提供文件名文件中
 */
struct user_config_struct {
	char buffer[CONFIG_BUF_SIZE];
	
	boolean (*create_file)(const char *filename);
	boolean (*read_from_config_file)(const char *filename, char buffer[]);
	boolean (*write_to_config_file)(const char *filename, const char *buffer);
};

/*
 * configure function definitions
 */
/*public interface*/
boolean create_config_file(const char *filename);
boolean read_from_config_file(const char *filename, char buffer[]);
boolean write_to_config_file(const char *filename, const char *buffer);

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
	struct user_struct *user;
	struct hnet_struct *socket;
	struct hmthread_struct *hmthread;
	char error_info[INFO_SIZE];
	Status status;
};

/*
 * login functions definitions
 */
char* get_armserver_ip();

/*
 * GTK signal connect callback functions definitions
 */ 
void cb_login_help_clicked(GtkButton *button, gpointer user_data); /*call login->show_help_dialog(struct login)*/
void cb_login_cancel_clicked(GtkButton *button, gpointer user_data);
void cb_login_ok_clicked(GtkButton *button, gpointer user_data);
void cb_login_rem_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void cb_comboboxentry_ip_changed(GtkComboBox *widget, gpointer user_data);
void cb_comboboxentry_name_changed(GtkComboBox *widget, gpointer user_data);
void cb_entry_passwd_activate(GtkEntry *widget, gpointer user_data);
void cb_entry_passwd_backspace(GtkEntry *widget, gpointer user_data);
gboolean cb_entry_passwd_focus_out_event(GtkWidget *widget, GdkEventFocus *event, gpointer user_data);
#endif
