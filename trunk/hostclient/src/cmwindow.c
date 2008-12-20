#include <gtk/gtk.h>
#include "linuxarms.h"
#include "mwindow.h"
#include "debug.h"
#include "error.h"
#include "hmthread.h"
#include "hsthread.h"
#include "hfthread.h"
#include "sprocess.h"
#include "fileview.h"
#include "htthread.h"

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
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hsprocess_struct *sprocess = hsthread->sprocess;
	struct hfview_struct *hfview = linuxarms->hfthread->hfview;
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;
	
	switch (old_num) {
	case PSYSINFO:
		break;
	case PPROCESS:
		gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_kill), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_three), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_five), FALSE);
		break;
	case PFILEVIEW:
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.back), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.up), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.rename), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.del), FALSE);
		debug_print("前一个面板为文件浏览\n");
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_upload), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_download), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.toolbar_upload), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.toolbar_download), FALSE);
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
			hsthread_set_trans(hsthread, SYSINFO, -1);
			old_num = PSYSINFO;
		} else {
			hsthread_set_timer_time(hsthread, sprocess->clock);
			hsthread_set_trans(hsthread, SPROCESS, -1);
			gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_kill), TRUE);
			gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_three), TRUE);
			gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_five), TRUE);
			old_num = PPROCESS;
		}
		hsthread_create_timer(hsthread);
		debug_print("启动定时器\n");
	} else {
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.back), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.up), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.rename), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.del), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_upload), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_download), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.toolbar_upload), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.toolbar_download), TRUE);
		old_num = PFILEVIEW;
	}
}

boolean gtk_window_main_set_sensitive(struct linuxarms_struct *linuxarms)
{
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hsprocess_struct *sprocess = hsthread->sprocess;
	struct hfview_struct *hfview = linuxarms->hfthread->hfview;
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;
	struct hmthread_struct *hmthread = linuxarms->hmthread;

	gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_kill), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_three), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(sprocess->widget.menu_five), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.back), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.up), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.rename), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hfview->widget.del), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_upload), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.menubar_download), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.toolbar_upload), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(htthread->widget.toolbar_download), FALSE);

	gtk_widget_set_sensitive(GTK_WIDGET(hmthread->widget.login), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hmthread->widget.logout), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hmthread->widget.restart), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(hmthread->widget.shutdown), FALSE);
	return TRUE;
}
