#define __DEBUG__
#include <gtk/gtk.h>
#include "linuxarms.h"
#include "mwindow.h"
#include "debug.h"
#include "error.h"
#include "hmthread.h"
#include "hsthread.h"
#include "hfthread.h"
#include "hcthread.h"
#include "sprocess.h"
#include "fview.h"
#include "htthread.h"
#include "toolbar.h"
#include "thread.h"

typedef enum _Page_num {
	PSYSINFO,
	PPROCESS,
	PFILEVIEW,
}page_num;
static page_num old_num = PSYSINFO;
/*
 * 系统信息显示，进程信息显示，文件浏览页面切换的时候，调用该函数
 * @page_num:  当前页面的id.
 */
void cb_notebook_switch_page(GtkNotebook *notebook,
			     GtkNotebookPage *page, 
			     guint page_num, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hsprocess_struct *hsprocess = hsthread->hsprocess;
	struct hfview_struct *hfview = linuxarms->hfthread->hfview;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct htthread_struct *htthread = hfthread->hftrans;
	
	switch (old_num) {
	case PSYSINFO:
		hmthread->set_protocol(hmthread, CSTHREAD);
		hmthread->send(hmthread);
		break;
	case PPROCESS:
		gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_kill), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_three), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_five), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_user), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_all), FALSE);
		hmthread->set_protocol(hmthread, CSTHREAD);
		hmthread->send(hmthread);
		break;
	case PFILEVIEW:
		gtk_widget_set_sensitive(GTK_WIDGET(linuxarms->mwindow->toolbar), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.rename), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.del), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.hide), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_upload), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_download), FALSE);
		gtk_widget_set_sensitive(hfview->widget.hide, FALSE);
		debug_print("前一个面板为文件浏览\n");
		break;
	}
	debug_print("当前页面的页面号为 = %d\n",page_num);
	/* 如果有定时器存在，则去掉 */
	if (hsthread->timer.timer != -1) {
		hsthread_close_timer(hsthread);
		debug_where();
		debug_print("删除定时器\n");
	}
	/* 如果当前页面焦点为'系统信息'或者'进程信息'，也就是说用户切换页面到这
	 * 两个页面上，则启动定时器
	 */
	if (page_num != PFILEVIEW) {
		if (page_num == PSYSINFO) {
			hsthread_set_timer_time(hsthread, TM_THREE);
			hsthread->set_protocol(hsthread, SSYSINFO);
			old_num = PSYSINFO;
		} else {
			hsthread_set_timer_time(hsthread, hsprocess->clock);
			hsthread->set_protocol(hsthread, SPROCESS);
			//gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_kill), TRUE);
			gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_three), TRUE);
			gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_five), TRUE);
			gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_user), TRUE);
			gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_all), TRUE);
			old_num = PPROCESS;
		}
		hsthread_create_timer(hsthread);
		hsthread->send(hsthread);
		debug_print("启动定时器\n");
	} else {
		gtk_widget_set_sensitive(GTK_WIDGET(linuxarms->mwindow->toolbar), TRUE);
		cb_fview_selection_changed(NULL, linuxarms);
		gtk_widget_set_sensitive(hfview->widget.hide, TRUE);
		old_num = PFILEVIEW;
		hfthread_trans_set_path(&hfthread->trans, hfview_get_path(hfview));
		hfthread->set_protocol(hfthread, FVIEW);
		hfthread->send(hfthread);
	}
}

boolean gtk_window_main_set_sensitive(struct linuxarms_struct *linuxarms)
{
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hsprocess_struct *hsprocess = hsthread->hsprocess;
	struct hfview_struct *hfview = linuxarms->hfthread->hfview;
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;
	struct hmthread_struct *hmthread = linuxarms->hmthread;

	gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_kill), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_three), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_five), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_user), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hsprocess->widget.menu_all), FALSE);
	
	//gtk_widget_set_sensitive(GTK_WIDGET(linuxarms->mwindow->toolbar), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.rename), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.del), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.hide), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_upload), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_download), FALSE);

	gtk_widget_set_sensitive(GTK_WIDGET(hmthread->widget.login), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hmthread->widget.logout), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hmthread->widget.restart), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hmthread->widget.shutdown), FALSE);
	return TRUE;
}

void cb_linuxarms_window_main_close(GtkObject *object, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	
	debug_where();
	if (hsthread->timer.timer != -1) {
		hsthread_close_timer(hsthread);
		debug_where();
		debug_print("删除定时器\n");
	}
	debug_where();
	hmthread->set_protocol(hmthread, CLOSECLIENT);
	hmthread->send(hmthread);
	//hostclient_close_all_thread(linuxarms);
	list_head_free();
	hostclient_close_all_thread(linuxarms);
	gtk_main_quit();
}

gboolean cb_window_main_button_press(GtkWidget *widget,
		        GdkEventButton *event, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	debug_print("window_main press\n");
	if (linuxarms->hfthread->hfview->widget.popup)
		linuxarms->hfthread->hfview->widget.popup = FALSE;
	return TRUE;
}
