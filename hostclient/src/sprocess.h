#ifndef _SPROCESS_H
#define _SPROCESS_H
#include <gtk/gtk.h>
#include "protocol.h"
#include "linuxarms.h"
#include "hsthread.h"

#define PROCESS_COLUMNS 5
#define PROCESS_INFO_LEN 256
enum {
	COL_PIXBUF = 0,
	COL_NAME,
	COL_ID,
	COL_STATUS,
	COL_CPU,
	COL_MEM
};

enum {
	BUTTON_LEFT = 0x1,
	BUTTON_MID =0x2,
	BUTTON_RIGHT =0x3
};
/*
 * hsprocess_struct 接收armserver发送过来的进程信息
 * @num:   要接收的进程信息的总数
 * @protocol: 接收的状态。
 * @info:  接收到的信息
 */
struct hsprocess_trans {
	int num;
	protocol_sthread protocol;
	char info[PROCESS_INFO_LEN];
};
/*
 * sprocess_widget 与实时监视(进程信息显示)相关的界面的控
 *                 件指针集合
 * @treeview: 显示控件
 * @menu_kill:     主菜单杀死进程控件
 * @popup_kill:    右键菜单杀死进程控件
 */
struct sprocess_widget {
	GtkWidget *treeview;
	GtkWidget *menu_kill;
	GtkWidget *menu_three;
	GtkWidget *menu_five;
	GtkWidget *popup_kill;
	GtkTreeIter *selected_row;
};
/*
 * hsprocess_struct 实时监视主数据结构
 * @trans:  要从armserver接收的数据
 * @clock:  进程信息更新频率(3秒/5秒)
 * @widget: 实时监视相关界面
 * @kill:   要杀死的进程号
 * @pnum:   当前显示进程信息的个数
 */
struct hsprocess_struct {
	struct hsprocess_trans trans;
	struct sprocess_widget widget;
	timer_time clock;
	int kill;
	int pnum;

	boolean (*send)(int tcp, struct hsprocess_struct *hsprocess);
	boolean (*recv)(int tcp,struct hsprocess_struct *hsprocess);
};
boolean process_show_info(struct hsprocess_struct *process);
boolean hsprocess_struct_init(struct hsprocess_struct *sprocess);
GtkListStore *create_page_sprocess(struct linuxarms_struct *linuxarms);
GtkWidget *create_popup_menu_process(struct hsthread_struct *hsthread);
void cb_process_selection_changed(GtkWidget *widget, gpointer user_data); 

gboolean cb_process_button_press(GtkWidget *widget,
	                 GdkEventButton *event, gpointer user_data);



void cb_process_kill_activate(GtkMenuItem*menuitem,
                                    gpointer user_data);
#endif
