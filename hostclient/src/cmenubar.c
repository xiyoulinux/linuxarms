#define __DEBUG__
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

static void cb_do_logout(struct linuxarms_struct *linuxarms);

void cb_logout_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	
	hmthread->set_protocol(hmthread, LOGOUT);
	hmthread->send(hmthread);
	cb_do_logout(linuxarms);
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
	cb_do_logout(linuxarms);
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
	cb_do_logout(linuxarms);
}

void cb_quit_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	cb_linuxarms_window_main_close(NULL, user_data);
}

void cb_process_update_three_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct hsthread_struct *hsthread = (struct hsthread_struct *)user_data;
	struct hsprocess_struct *hsprocess = hsthread->hsprocess;

	debug_where();
	hsprocess->clock = TM_THREE;
	debug_print("修改定时时间为3秒钟\n");
	if (hsthread->timer.timer != -1) {
		hsthread_close_timer(hsthread);
		debug_where();
		debug_print("删除定时器\n");
	}
	hsthread_set_timer_time(hsthread, hsprocess->clock);
	hsthread->set_protocol(hsthread, SPROCESS);
	hsthread_create_timer(hsthread);
}

void cb_process_update_five_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct hsthread_struct *hsthread = (struct hsthread_struct *)user_data;
	struct hsprocess_struct *hsprocess = hsthread->hsprocess;

	debug_where();
	hsprocess->clock = TM_FIVE;
	debug_print("修改定时时间为5秒钟\n");
	if (hsthread->timer.timer != -1) {
		hsthread_close_timer(hsthread);
		debug_where();
		debug_print("删除定时器\n");
	}
	hsthread_set_timer_time(hsthread, hsprocess->clock);
	hsthread->set_protocol(hsthread, SPROCESS);
	hsthread_create_timer(hsthread);

}

void cb_process_show_user_activate(GtkMenuItem  *menuitem, gpointer user_data)
{
	struct hsthread_struct *hsthread = (struct hsthread_struct *)user_data;

	hsthread->trans.all = FALSE;
	hsthread->set_protocol(hsthread, SPROCESS);
	hsthread->send(hsthread);
}
void cb_process_show_all_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct hsthread_struct *hsthread = (struct hsthread_struct *)user_data;

	hsthread->trans.all = TRUE;
	hsthread->set_protocol(hsthread, SPROCESS);
	hsthread->send(hsthread);
}

void cb_help_topic_activate(GtkMenuItem *menuitem, gpointer user_data)
{
 	GtkWidget *help;
 	if (help_window_created)
 		return;
 	help = (GtkWidget *)create_dialog_help(user_data);
	help_window_created = TRUE;
 	/*
        GtkWidget *textview;
        GtkTextBuffer *buffer;
        gint len;
        gchar tmp[80],*p=NULL;
        FILE *fp_help;

        textview=(GtkWidget *)create_window_help(user_data);
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
	*/
}


void cb_help_about_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget *window;
	window=(GtkWidget *)create_window_about();
	gtk_widget_show(window);
	debug_where();

}

static void cb_do_logout(struct linuxarms_struct *linuxarms)
{
	struct hmthread_struct *hmthread = linuxarms->hmthread;

	hostclient_user_logout(linuxarms);
	hostclient_close_all_thread(linuxarms);
	debug_where();
	gtk_widget_destroy(linuxarms->mwindow->window);
	hostclient_init(linuxarms);
	create_window_login(linuxarms);
	hmthread->timer = gtk_timeout_add(50, create_window_main_timeout, (gpointer)linuxarms);
}
