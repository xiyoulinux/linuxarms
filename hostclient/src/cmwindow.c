#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include "mwindow.h"
#include "debug.h"
#include "error.h"
#include "hsthread.h"
enum _Page_num {
	PSYSINFO,
	PPROCESS,
	PFILEVIEW,
};
/*
 * 系统信息显示，进程信息显示，文件浏览页面切换的时候，调用该函数
 * @page_num:  当前页面的id.
 */
void cb_notebook_switch_page(GtkNotebook *notebook,
			     GtkNotebookPage *page, 
			     guint page_num, gpointer user_data)
{
	struct hsthread_struct *hsthread = (struct hsthread_struct *)user_data;
	
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
			hsthread_set_trans(hsthread, SYSINFO, -1);
		} else {
			hsthread_set_trans(hsthread, SPROCESS, -1);
		}
		hsthread_create_timer(hsthread);
		debug_print("启动定时器\n");
	}
}
