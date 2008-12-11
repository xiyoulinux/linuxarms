/*
 * linuxarms/hostclient/src/csprocess.c
 * 实时监视(显示进程信息)模块处理函数
 * Niu Tao<niutao0602@gmail.com>
 */
#include <gtk/gtk.h>
#include <stdlib.h>
#include "linuxarms.h"
#include "sprocess.h"
#include "hsthread.h"
#include "protocol.h"
#include "debug.h"
#include "error.h"

boolean hsprocess_struct_init(struct hsprocess_struct *sprocess)
{
	if (!sprocess) {
		debug_where();
		print_error(EWARNING,"无效的参数");
		return FALSE;
	}
	sprocess->clock = TM_THREE;
	sprocess->kill = -1;
	sprocess->pnum = 0;
	return TRUE;
}

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
	struct hsthread_struct *hsthread = (struct hsthread_struct *)user_data;
	if (event->button == BUTTON_RIGHT) {
		GtkWidget *popup_menu = create_popup_menu_process(hsthread);
		gtk_menu_popup (GTK_MENU(popup_menu),
				NULL, NULL, NULL, NULL,
				event->button, event->time);
	}
	debug_where();
}
/*
 * 用户在treeview)process控件上按下鼠标右键弹出的右键菜单中
 * 选择杀死进程的时候，调用该回调函数
 */
void cb_process_kill_activate(GtkMenuItem *menuitem,gpointer user_data)
{
	struct hsthread_struct *hsthread = (struct hsthread_struct *)user_data;
	struct hsprocess_struct *sprocess = hsthread->sprocess;
	GtkWidget *treeview = sprocess->widget.treeview;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	char *spid;
	int pid;
	if (sprocess->pnum == 0)
		return;
	gtk_tree_selection_get_selected(
		gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
		&list_store, &iter);
	sprocess->widget.selected_row = &iter;
	gtk_tree_model_get (list_store, &iter, COL_ID, &spid,-1);
	pid = atoi(spid);
	sprocess->kill = pid;
	debug_where();
	debug_print("选中要杀死的进程的进程号为 %d\n",sprocess->kill);
}

/*
 * 主调其他模块完成实时监视显示功能
 */
boolean do_show_process(int tcp, struct hsprocess_struct *process)
{
	/* 循环接收进程信息 */
	/* 每接收一条信息，就调用显示进程信息函数将信息
	 * 显示在图形界面上 ,发送反馈信息
	 */
	/* 判断时候接收完毕，如果完毕，则退出循环 */
	GtkTreeModel *list_store;
	list_store = gtk_tree_view_get_model(GTK_TREE_VIEW(process->widget.treeview));

	gtk_list_store_clear(GTK_LIST_STORE(list_store));
	while (TRUE) {
		process->recv(tcp, process);
		if (process->trans.protocol = SSENDALL)
			return TRUE;
		process_show_info(process);
		process->trans.protocol = SPROCESS;
		process->send(tcp, process);

	}
}

/*
 * 接收进程信息函数
 * @hsthread:  
 * @process:
 */
boolean process_recv_info(int tcp, struct hsprocess_struct *process)
{
	return hnet_recv(tcp, &process->trans,
			 sizeof(struct hsprocess_trans));
}
/*
 * 发送反馈信息
 */
boolean process_send_info(int tcp, struct hsprocess_struct *process)
{
	return hnet_send(tcp, &process->trans,
			 sizeof(struct hsprocess_struct));
}
/*
 * 显示进程信息函数
 * 主要功能是将接收到的进程信息分割，然后组织为可显示
 * 的数据，然后显示在图形界面上
 * @process:  包含一条进程信息的数据结构
 */
boolean process_show_info(struct hsprocess_struct *process)
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
	GtkTreeModel *list_store;
	list_store = gtk_tree_view_get_model(process->widget.treeview);

	gtk_list_store_remove(list_store, &process->widget.selected_row);

}
