#ifndef _SPROCESS_H
#define _SPROCESS_H
#include <gtk/gtk.h>
#include "protocol.h"
#include "linuxarms.h"
#define PROCESS_INFO_LEN 256
/*
 * hsprocess_struct 接收armserver发送过来的进程信息
 * @num:   要接收的进程信息的总数
 * @state: 接收的状态。如果为STOP，则停止接收
 * @info:  接收到的信息
 */
struct hsprocess_trans {
	int num;
	trans_state state;
	char info[PROCESS_INFO_LEN];
};
/*
 * sprocess_widget 与实时监视(进程信息显示)相关的界面的控
 *                 件指针集合
 * @clist_process: 显示控件
 * @menu_kill:     主菜单杀死进程控件
 * @popup_kill:    右键菜单杀死进程控件
 */
struct sprocess_widget {
	GtkWidget *clist_process;
	GtkWidget *menu_kill;
	GtkWidget *popup_kill;
};
/*
 * hsprocess_struct 实时监视主数据结构
 * @trans:  要从armserver接收的数据
 * @clock:  进程信息更新频率(3秒/5秒)
 * @widget: 实时监视相关界面
 */
struct hsprocess_struct {
	struct hsprocess_trans trans;
	int clock;
	struct sprocess_widget widget;

	boolean (*recv)(struct hsprocess_struct *hsprocess);
};

void
cb_clist_process_click_column          (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data);

gboolean
cb_clist_process_button_press_event    (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
cb_popup_process_kill_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
#endif
