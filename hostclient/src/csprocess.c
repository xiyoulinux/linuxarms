/*
 * linuxarms/hostclient/src/csprocess.c
 * 实时监视(显示进程信息)模块处理函数
 * Niu Tao<niutao0602@gmail.com>
 */
#define __DEBUG__

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "linuxarms.h"
#include "sprocess.h"
#include "hsthread.h"
#include "hmthread.h"
#include "protocol.h"
#include "support.h"
#include "debug.h"
#include "error.h"
#include "hnet.h"

static int prev_process_nums = 0;
static const char *task_state_array[] = {
        "Running",          /*  0  */
        "Sleeping",         /*  1  */
        "Disk sleep",       /*  2  */
        "(Tracing) stop",   /* 4 8 */
        "Zombie",           /* 16  */
        "Dead",             /* 32  */
	"Uknown"
};


static boolean hsprocess_send_info(struct hsprocess_struct *hsprocess);
static boolean hsprocess_recv_info(struct hsprocess_struct *hsprocess);
static boolean hsprocess_set_protocol(struct hsprocess_struct *hsprocess, 
					protocol_sthread protocol);
boolean hsprocess_init(struct hsprocess_struct *hsprocess,
			int *kill, struct hnet_struct *socket)
{
	LINUXARMS_POINTER(hsprocess);
	LINUXARMS_POINTER(kill);
	LINUXARMS_POINTER(socket);
	hsprocess_trans_init(&hsprocess->trans);
	hsprocess->socket = socket;
	hsprocess->set_protocol = hsprocess_set_protocol;
	hsprocess->send = hsprocess_send_info;
	hsprocess->recv = hsprocess_recv_info;
	hsprocess->clock = TM_FIVE;
	hsprocess->kill = kill;
	hsprocess->widget.popup = FALSE;
	return TRUE;
}
/*
 * 选择的进程变化的时候，调用该回调函数
 */
void cb_process_selection_changed(GtkWidget *widget, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hsprocess_struct *hsprocess = linuxarms->hsthread->hsprocess;
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(hsprocess->widget.treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		char *user;
		gtk_tree_model_get(list_store, &iter, COL_SUSER, &user, -1);
		list_store = gtk_tree_view_get_model(GTK_TREE_VIEW(hsprocess->widget.treeview));
		if (strcmp(user, linuxarms->hmthread->user->name) != 0)
			select = FALSE;
		else
			select = TRUE;
		debug_where();
		g_free(user);
	} 
	if (hsprocess->widget.popup)
		gtk_widget_set_sensitive(hsprocess->widget.popup_kill, select);
	gtk_widget_set_sensitive(hsprocess->widget.menu_kill, select);
	/*boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget->treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		char *user;
		gtk_tree_model_get(list_store, &iter,
				COL_SUSER, &user,
				-1);
		if (!hsthread->competence && strcmp(user, "root") == 0) {
			gtk_widget_set_sensitive(widget->process_kill, FALSE);
			gtk_widget_set_sensitive(widget->menu_kill, FALSE);
		} else
			gtk_widget_set_sensitive(widget->menu_kill, TRUE);
		widget->selection = iter;
		g_free(user);
	} else {
		gtk_widget_set_sensitive(widget->menu_kill, FALSE);
	}*/
	debug_where();
}
/*
 * 用户在treeview_process控件上按下鼠标的时候，调用该回调函数
 */
gboolean cb_process_button_press(GtkWidget *widget,
	                 GdkEventButton *event, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	
	if (linuxarms->hsthread->hsprocess->widget.popup)
		linuxarms->hsthread->hsprocess->widget.popup = FALSE;
	if (event->type != GDK_BUTTON_PRESS)
		return TRUE;
	if (event->button == BUTTON_RIGHT) {
		GtkWidget *popup_menu = create_popup_menu_process(linuxarms);
		gtk_menu_popup (GTK_MENU(popup_menu),
				NULL, NULL, NULL, NULL,
				event->button, event->time);
		linuxarms->hsthread->hsprocess->widget.popup = TRUE;
	} else {
		linuxarms->hsthread->hsprocess->widget.popup_kill = NULL;
	}
	debug_where();
	return FALSE;
}
/*
 * 用户在treeview)process控件上按下鼠标右键弹出的右键菜单中
 * 选择杀死进程的时候，调用该回调函数
 */
void cb_process_kill_activate(GtkMenuItem *menuitem,gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hsprocess_struct *hsprocess = hsthread->hsprocess;
	GtkWidget *treeview = hsprocess->widget.treeview;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	char *pid;
	boolean select;

	select = gtk_tree_selection_get_selected(
		 gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
		 &list_store, &iter);
	if (select) {
		gtk_tree_model_get(list_store, &iter, COL_SID, &pid,-1);
		*hsprocess->kill = atoi(pid);
		if (hsthread->timer.timer != -1)
			hsthread_close_timer(hsthread);
		hmthread->set_protocol(hmthread, CSTHREAD);
		hmthread->send(hmthread);
		hsthread_set_trans(hsthread, SKILL, atoi(pid));
		hsthread->send(hsthread);
		hsprocess->widget.selection = iter;
		g_free(pid);
	}
	debug_print("选中要杀死的进程的进程号为 %d\n",*hsprocess->kill);
}


static inline const char *get_task_state(char state)
{
	switch (state) {
	case 'R':
		return task_state_array[0];
	case 'S':
		return task_state_array[1];
	case 'D':
		return task_state_array[2];
	case 'T':
		return task_state_array[3];
	case 'Z':
		return task_state_array[4];
	case 'X':
		return task_state_array[5];
	default:
		return task_state_array[6];
	}
	return "Ukown";
}
/*
 * 主调其他模块完成实时监视显示功能
 */
boolean do_show_process(struct hsprocess_struct *hsprocess)
{
	/* 循环接收进程信息 */
	GtkListStore *list_store;
	GtkTreeIter iter;
	GdkPixbuf *theme_icon;
	GtkIconTheme *theme;
	struct hsprocess_trans *hstrans;
	char pid[10], cpu[10], mem[10];
	int pid_old = 0;
	int process_nums = 0;
	LINUXARMS_POINTER(hsprocess);
	hstrans = &hsprocess->trans;
	theme = gtk_icon_theme_get_default();
	list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(hsprocess->widget.treeview)));
	//g_object_ref(list_store);
	//gtk_tree_view_set_model(GTK_TREE_VIEW(hsprocess->widget.treeview), NULL); 

	boolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store), &iter);
	while (TRUE) {
		if (!hsprocess->recv(hsprocess)) {
			print_error(EWARNING, "接收进程信息失败\n");
			goto out;
		}
		switch (hstrans->protocol) {
		case SSENDALL:
			goto out;
		case SPROCESS:
			if (pid_old == hstrans->pid)
				break;
			pid_old = hstrans->pid;
			if (!valid)
				gtk_list_store_append(list_store, &iter);
			theme_icon = gtk_icon_theme_load_icon(theme, 
					get_default_icon_name(hstrans->name),
					APP_ICON_SIZE, GTK_ICON_LOOKUP_USE_BUILTIN, NULL);
			if (!theme_icon)
				theme_icon = create_pixbuf("gtk-exec.png");
			snprintf(cpu, 10, "%2d%s", (int)hstrans->cpu * 100, "%");
			snprintf(mem, 10, "%.1fKB", hstrans->mem);
			snprintf(pid, 10, "%d", hstrans->pid);
			gtk_list_store_set(GTK_LIST_STORE(list_store), &iter, 
				COL_SPIXBUF, theme_icon,
				COL_SNAME,   hstrans->name,
				COL_SID,     pid,
				COL_SUSER,   hstrans->user,
				COL_SSTATE,  get_task_state(hstrans->state),
				COL_SCPU,    cpu,
				COL_SMEM,    mem,
				-1);
			process_nums++;
			if (valid)
				valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
			break;
		default:
			debug_print("sprocess default\n");
			break;
		}
		//hstrans->protocol = SMAX;
	}
out:
	hstrans->protocol = SMAX;
	if (process_nums < prev_process_nums) {
		int i = prev_process_nums - process_nums;
		while (i--) {
			gtk_list_store_set(GTK_LIST_STORE(list_store), &iter, 
				COL_SPIXBUF, NULL,
				COL_SNAME,   "",
				COL_SID,     "",
				COL_SUSER,   "",
				COL_SSTATE,  "",
				COL_SCPU,    "",
				COL_SMEM,    "",
				-1);
			gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
		}
	}
	//gtk_tree_view_set_model(GTK_TREE_VIEW(hsprocess->widget.treeview), GTK_TREE_MODEL(list_store));
	//g_object_unref(list_store);		
	prev_process_nums = process_nums;
	/*
	hsprocess->trans.protocol = SPROCESS;
	hsprocess->send(hsprocess);
	*/
	return TRUE;
}

/*
 * 接收进程信息函数
 * @hsthread:  
 * @process:
 */
static boolean hsprocess_recv_info(struct hsprocess_struct *hsprocess)
{
	LINUXARMS_POINTER(hsprocess);
	return hnet_recv(hsprocess->socket->tcp, (void *)&hsprocess->trans,
			 sizeof(struct hsprocess_trans));
}
/*
 * 发送反馈信息
 */
static boolean hsprocess_send_info(struct hsprocess_struct *hsprocess)
{
	LINUXARMS_POINTER(hsprocess);
	return hnet_send(hsprocess->socket->tcp, (void *)&hsprocess->trans,
			 sizeof(struct hsprocess_trans));
}
boolean hsprocess_trans_init(struct hsprocess_trans *hstrans)
{
	LINUXARMS_POINTER(hstrans);
	hstrans->protocol = SMAX;
	memset(hstrans->name, '\0', PROCESS_NAME_LEN);
	hstrans->pid = -1;
	memset(hstrans->user, '\0', PROCESS_USER_LEN);
	hstrans->state = 'S';
	hstrans->cpu = 0.0;
	hstrans->mem = 0.0 ;
	return TRUE;
}
boolean hsprocess_trans_set_protocol(struct hsprocess_trans *hstrans, 
					protocol_sthread protocol)
{
	LINUXARMS_POINTER(hstrans);
	if (!PROTOCOL_IS_STHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		hstrans->protocol = SMAX;
		return FALSE;
	}
	hstrans->protocol = protocol;
	return TRUE;
}					
static boolean hsprocess_set_protocol(struct hsprocess_struct *hsprocess, 
					protocol_sthread protocol)
{
	LINUXARMS_POINTER(hsprocess);
	return hsprocess_trans_set_protocol(&hsprocess->trans, protocol);
}					
