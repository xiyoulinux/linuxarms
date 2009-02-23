#include <gtk/gtk.h>
#include <string.h>
#include "linuxarms.h"
#include "mwindow.h"
#include "filetrans.h"
#include "hsthread.h"
#include "hmthread.h"
#include "hfthread.h"
#include "hcthread.h"
#include "sprocess.h"
#include "toolbar.h"
#include "support.h"
#include "message.h"
#include "debug.h"
#include "about.h"
#include "help.h"
#include "error.h"
#include "login.h"
#include "hnet.h"
#include "config.h"
#include "fview.h"

void cb_login_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct login_struct *login = linuxarms->login;
	debug_where();
	debug_print("cb_login_activate\n");
	
	/***************init_login*****************************/
	login_config_init(login->config);
	login_config_read(login->config);
	hnet_init(login->socket, NULL, get_mthread_port());
	login_init(login, login->config, login->socket);

	/***************init_hmthread**************************/
	hmthread_init(hmthread, &login->user);
	/***************init hsthread**************************/
	hsthread->timer.time = TM_THREE * 1000;
	hsthread->timer.timer = -1;

	hsprocess_init(hsthread->hsprocess, &hsthread->trans.kill, &hsthread->socket);
	
	/***************init hfthread**************************/
	hfview_init(hfthread->hfview, hfthread->trans.path, &hfthread->socket);

	gtk_widget_destroy(linuxarms->mwindow->window);
	create_window_login(linuxarms);
}


void cb_logout_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	
	hmthread->set_protocol(hmthread, LOGOUT);
	hmthread->send(hmthread);
	gtk_widget_set_sensitive(linuxarms->mwindow->window, FALSE);
	debug_where();
	debug_print("cb_logout_activate");

}


void cb_restart_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	debug_where();

	if (!hmthread->competence) {
		message_box_error(linuxarms->mwindow->window, 
				"非root用户，没有权限执行重启arm系统，"
				"如果要执行，请以root身份登录");
		return;
	}
	hmthread->set_protocol(hmthread, RESTART);
	hmthread->send(hmthread);
	gtk_widget_set_sensitive(linuxarms->mwindow->window, FALSE);
}


void cb_shutdown_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	debug_where();

	if (!hmthread->competence) {
		message_box_error(linuxarms->mwindow->window, 
				"非root用户，没有权限执行关闭arm系统，"
				"如果要执行，请以root身份登录");
		return;
	}
	hmthread->set_protocol(hmthread, SHUTDOWN);
	hmthread->send(hmthread);
	gtk_widget_set_sensitive(linuxarms->mwindow->window, FALSE);
}


void cb_quit_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	cb_linuxarms_window_main_close(NULL, user_data);
}


void cb_process_update_three_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct hsprocess_struct *hsprocess = (struct hsprocess_struct *)user_data;

	debug_where();
	hsprocess->clock = TM_THREE;
	debug_print("修改定时时间为3秒钟\n");
}


void cb_process_update_five_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct hsprocess_struct *hsprocess = (struct hsprocess_struct *)user_data;

	debug_where();
	hsprocess->clock = TM_FIVE;
	debug_print("修改定时时间为5秒钟\n");

}

void cb_help_topic_activate(GtkMenuItem *menuitem, gpointer user_data)
{
        GtkWidget *textview;
        GtkTextBuffer *buffer;
        gint len;
        gchar tmp[80],*p=NULL;
        FILE *fp_help;

        textview=(GtkWidget *)create_window_help();
	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
	p=find_file("helpmain");
	if(p== NULL) {
		gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer),
		                                "没有帮助文件",strlen("没有帮助文件"));
	} else {
		if((fp_help=fopen(p,"r"))==NULL) {
			print_error(EWARNING, "fopen");
			return;
		}   
		while(!feof(fp_help)) {
			if (fgets(tmp,80,fp_help) == NULL)
				break;
			len=strlen(tmp);
			gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer),tmp,len);
			memset(tmp,'\0',len);
		}   
		fclose(fp_help);
	}   
	debug_where();

}


void cb_help_about_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget *window;
	window=(GtkWidget *)create_window_about();
	gtk_widget_show(window);
	debug_where();

}

