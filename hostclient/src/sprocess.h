#ifndef _SPROCESS_H
#define _SPROCESS_H
#include <gtk/gtk.h>
#include "protocol.h"
#include "linuxarms.h"
#include "hsthread.h"
#include "hnet.h"

#define PROCESS_COLUMNS 6
#define PROCESS_NAME_LEN 16
#define PROCESS_USER_LEN 20
#define APP_ICON_SIZE 16
enum {
	COL_SPIXBUF = 0,
	COL_SNAME,
	COL_SID,
	COL_SUSER,
	COL_SSTATE,
	COL_SCPU,
	COL_SMEM
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
	protocol_sthread protocol;
	char name[PROCESS_NAME_LEN];
	int pid;
	char user[PROCESS_USER_LEN];
	char state;
	float cpu;
	float mem;
};

boolean hsprocess_trans_init(struct hsprocess_trans *hstrans);
boolean hsprocess_trans_set_protocol(struct hsprocess_trans *hstrans, 
					protocol_sthread protocol);
/*
 * hsprocess_widget 与实时监视(进程信息显示)相关的界面的控
 *                 件指针集合
 * @treeview: 显示控件
 * @menu_kill:     主菜单杀死进程控件
 * @popup_kill:    右键菜单杀死进程控件
 */
struct hsprocess_widget {
	GtkWidget *treeview;
	GtkWidget *menu_kill;
	GtkWidget *menu_three;
	GtkWidget *menu_five;
	GtkWidget *menu_user;
	GtkWidget *menu_all;
	GtkWidget *popup_kill;
	boolean popup;
	GtkTreeIter selection;
};
/*
 * hsprocess_struct 实时监视主数据结构
 * @trans:  要从armserver接收的数据
 * @clock:  进程信息更新频率(3秒/5秒)
 * @widget: 实时监视相关界面
 * @kill:   要杀死的进程号
 */
struct hsprocess_struct {
	struct hsprocess_trans trans;
	struct hsprocess_widget widget;
	struct hnet_struct *socket;
	timer_time clock;
	int *kill;

	boolean (*set_protocol)(struct hsprocess_struct *hsprocess, 
				protocol_sthread protocol);
	boolean (*send)(struct hsprocess_struct *hsprocess);
	boolean (*recv)(struct hsprocess_struct *hsprocess);
};
boolean hsprocess_show_info(struct hsprocess_struct *hsprocess);
boolean hsprocess_init(struct hsprocess_struct *hsprocess, 
			int *kill,struct hnet_struct *socket);
boolean do_show_process(struct hsprocess_struct *hsprocess);
GtkListStore *create_page_sprocess(struct linuxarms_struct *linuxarms);
GtkWidget *create_popup_menu_process(struct linuxarms_struct *linuxarms);
void cb_process_selection_changed(GtkWidget *widget, gpointer user_data); 

gboolean cb_process_button_press(GtkWidget *widget,
	                 GdkEventButton *event, gpointer user_data);
void cb_process_kill_activate(GtkMenuItem*menuitem,
                                    gpointer user_data);
#endif
