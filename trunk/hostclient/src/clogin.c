#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"
#include "debug.h"
#include "error.h"
#include "linuxarms.h"
#include "config.h"
#include "message.h"
#include "mwindow.h"
#include "hmthread.h"
#include "thread.h"

static void set_button_ok_state(struct login_struct *login)
{
	struct user_struct *user = login->user;
	if(strlen(user->ip)     > 0 && 
	   strlen(user->name)   > 0 &&
	   strlen(user->passwd) > 0)
		gtk_widget_set_sensitive(GTK_WIDGET(login->widget.button_ok), TRUE);
	else
		gtk_widget_set_sensitive(GTK_WIDGET(login->widget.button_ok), FALSE);
}

gboolean cb_window_login_delete_event(GtkWidget *widget,
			GdkEvent  *event,gpointer user_data)
{
	cb_login_button_cancel_clicked(NULL,user_data);
	return TRUE;
}

void cb_login_button_cancel_clicked(GtkWidget *widget, gpointer user_data)
{
	struct login_struct *login = (struct login_struct *)user_data;
	login_config_free(login->config);
	gtk_main_quit();
}

void cb_login_rem_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	struct login_struct *login = (struct login_struct *)user_data;
	
	debug_where();
	debug_print("login->rmember = %d\n",login->remember);
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)) == TRUE) {
		login->remember = TRUE;
		debug_print("选中记住密码\n");
	} else {
		login->remember = FALSE;
		debug_print("没有选中记住密码\n");
	}
}

void cb_login_help_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog;
	gchar help_msg[512];
	
	snprintf(help_msg, 512, 
		"首先取保您输入的数据库服务器ip，\n"
		"的格式是正确的，确保用户名和密码\n"
		"有效。其中:\n"
		"1.数据库服务器ip：符合ip写法规范\n"
		"  ,并且长度小于 %d\n"
		"2.用户名：长度要小于 %d\n"
		"3.用户密码长度要小于 %d\n"
		"如果这些数据都有效，则请您检查您\n"
		"的网线是否插好，确保有正确的网络\n"
		"环境来链接服务器!!",
		IP_LEN, USER_NAME_LEN, PASSWD_LEN);

	dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION,
                                  GTK_BUTTONS_CLOSE,
                                  "%s",
                                 (const gchar *)help_msg);
	gtk_widget_show(dialog);
 	g_signal_connect_swapped(dialog, "response",
                           G_CALLBACK(gtk_widget_destroy),
                           dialog);  

}

void cb_login_ok_clicked(GtkButton *button, gpointer user_data)
{
	struct linuxarms_struct *linuxarms =(struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct login_struct *login = linuxarms->login;
	struct user_struct *user = login->user;
	struct hnet_struct *socket = login->socket;
	char buf[80];

	if (socket->tcp != -1) {
		user_struct_set(&hmthread->trans.user, user->ip, user->name, user->passwd);
		hmthread->set_protocol(hmthread, LOGIN);
		hmthread->send(hmthread);
		debug_where();
		return;
	}

	strcpy(socket->ip, user->ip);
	if (create_tcp_client(socket) == FALSE) {
		snprintf(buf, 80, "无法连接服务器 %s :请\n确保ip地址格式正确并且有效！", socket->ip);
		message_box_error(login->widget.window_login, buf);
		socket->tcp = -1;
		return;
	}
	strcpy(armserver_ip, user->ip);
	debug_where();
	if (hmthread->thread == NULL)
		linuxarms_thread_create(hmthread_thread, linuxarms);
	debug_where();
}

gboolean cb_server_ip_changed(GtkWidget *widget, gpointer user_data)
{
	struct login_struct *login =(struct login_struct *)user_data;
	char *user_ip;

	user_ip = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
	if(strlen(user_ip) >= IP_LEN) {
		message_box_error(login->widget.window_login, "输入的ip地址过长");
		return FALSE;
	}
	strncpy(login->user->ip, user_ip, IP_LEN);
	set_button_ok_state(login);
	return TRUE;
}

gboolean cb_user_name_changed(GtkWidget *widget, gpointer user_data)
{
	struct login_struct *login =(struct login_struct *)user_data;
	char *user_name;
	int i;
	struct list_user *user;
	
	user_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
	if(strlen(user_name) >= USER_NAME_LEN) {
		message_box_error(login->widget.window_login, "输入的用户名过长");
		return FALSE;
	}
	strncpy(login->user->name, user_name, USER_NAME_LEN);
	user = login->config->user_list.next;
	for(i = 0; i < login->config->user_num; i++) {
		if(strcmp(user->user_name,login->user->name) == 0) {
			if(user->remember) {
				gtk_entry_set_text(GTK_ENTRY(login->widget.passwd), user->passwd);
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(login->widget.remember),TRUE);
				//set_button_ok_state(login);
				login->remember = TRUE;
			}
			break;
		}
		user = user->next;
	}
	set_button_ok_state(login);
	return TRUE;
}

gboolean cb_entry_passwd_changed(GtkWidget *widget, gpointer user_data)
{
	struct login_struct *login =(struct login_struct *)user_data;
	char *passwd;
	int len;
	passwd =(char *)gtk_entry_get_text(GTK_ENTRY(login->widget.passwd));
	//debug_print("%s\n",passwd);
	len = strlen(passwd);
	if (!len)
		gtk_widget_set_sensitive(login->widget.remember,FALSE);
	else
		gtk_widget_set_sensitive(login->widget.remember,TRUE);
	if(len >= PASSWD_LEN) {
		message_box_error(login->widget.window_login, "输入用户密码过长");
		return FALSE;
	}
	strncpy(login->user->passwd, passwd, PASSWD_LEN);
	set_button_ok_state(login);
	return FALSE;

}
boolean login_init(struct login_struct *login, 
		   struct user_struct *user,
		   struct login_config_struct *config,
		   struct hnet_struct *socket)
{
	login->user = user;
	login->config = config;
	login->socket = socket;
	login->remember = FALSE;
	return TRUE;
}


boolean user_struct_init(struct user_struct *user)
{
	LINUXARMS_POINTER(user);
	memset(user->ip, '\0', IP_LEN);
	memset(user->name, '\0', USER_NAME_LEN);
	memset(user->passwd, '\0', PASSWD_LEN);
	user->competence = FALSE;
	return TRUE;
}
boolean user_competece(struct user_struct *user)
{
	LINUXARMS_POINTER(user);
	return user->competence;
}
boolean user_struct_set(struct user_struct *user, char *ip, char *name, char *passwd)
{
	LINUXARMS_POINTER(user);
	strncpy(user->ip, !ip ? "" : ip, IP_LEN);
	strncpy(user->name, !name ? "" : name, USER_NAME_LEN);
	strncpy(user->passwd, !passwd ? "" : passwd, PASSWD_LEN);
	if (strcmp(user->name, "root") == 0)
		user->competence = TRUE;
	else
		user->competence = FALSE;
	return TRUE;
}

boolean login_add_default_data(struct login_struct *login)
{
	int i;
	struct login_config_struct *config = login->config;
	struct login_widget *widget = &login->widget;
	struct list_ip *ip = config->ip_list.next;
	struct list_user *user = config->user_list.next;
	debug_print("SERVERIP: %d\n",config->ip_num);
	for(i = 0; i < config->ip_num; i++) {
		gtk_combo_box_append_text(GTK_COMBO_BOX(widget->server_ip),
					  ip->ip);
		debug_print("%s\n",ip->ip);
		ip = ip->next;
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(widget->server_ip),0);
	cb_server_ip_changed(login->widget.server_ip,(gpointer)login);
	debug_print("USER: %d\n",config->user_num);
	for(i = 0; i < config->user_num; i++) {
		gtk_combo_box_append_text(GTK_COMBO_BOX(widget->user_name),
					  user->user_name);
		debug_print("%s\n",user->user_name);
		user = user->next;
	}
	return TRUE;
}

