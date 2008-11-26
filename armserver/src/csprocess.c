#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include "linuxarms.h"
#include "sprocess.h"
#include "hsthread.h"
/*
 * 选择的进程变化的时候，调用该回调函数
 */
void cb_process_selection_changed(GtkWidget *widget, gpointer user_data)
{
}
/*
 * 用户在treeview_process控件上按下鼠标的时候，调用该回调函数
 */
gboolean cb_process_button_press(GtkWidget *widget,
	                 GdkEventButton *event, gpointer user_data)
{
	
}
/*
 * 用户在treeview)process控件上按下鼠标右键弹出的右键菜单中
 * 选择杀死进程的时候，调用该回调函数
 */
void cb_popup_process_kill_activate(GtkMenuItem *menuitem,gpointer user_data)
{

}
/*
 * 接收进程信息函数
 * @hsthread:  
 * @process:
 */
boolean recv_process_info(struct hsthread_struct *hsthread, 
			  struct hsprocess_struct *process)
{
	/* 循环接收进程信息 */
	/* 每接收一条信息，就调用显示进程信息函数将信息
	 * 显示在图形界面上 
	 */
	/* 判断时候接收完毕，如果完毕，则退出循环 */
}
/*
 * 显示进程信息函数
 * 主要功能是将接收到的进程信息分割，然后组织的可显示
 * 的数据，然后显示在图形界面上
 * @process:  包含一条进程信息的数据结构
 */
boolean show_process_info(struct hsprocess_struct *process)
{
	/* 分割接收到的信息 */
	/* 将处理好的可显示的信息显示在图形界面上 */
}
/*
 * 杀死进程处理函数
 */
boolean kill_process(struct hsthread_struct *hsthread,
		     struct hsprocess_struct *process)
{
	/* 如果返回值为KILLSUC(hsthread->trans.ctrl = KILLSUC),
	 * 则表示成功杀死进程，则更新本地进程信息显示界面。
	 * 否则提示用户杀死进程失败
	 */
}
