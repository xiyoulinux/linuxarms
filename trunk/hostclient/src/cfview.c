#define __DEBUG__
#include <gtk/gtk.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "fview.h"
#include "hfthread.h"
#include "linuxarms.h"
#include "debug.h"
#include "error.h"
#include "filetrans.h"
#include "sprocess.h"
#include "statusbar.h"
#include "support.h"
#include "toolbar.h"
#include "message.h"
#include "mwindow.h"

static boolean hfview_send_info(struct hfview_struct *hfview);
static boolean hfview_recv_info(struct hfview_struct *hfview);
/*
 * 初始化hfview_struct结构
 */
boolean hfview_init(struct hfview_struct *hfview,char *path,
		    struct hnet_struct *socket)
{

	debug_where();
	if (!hfview || !path || !socket) {
		print_error(EWARNING,"初始化数据结构为空");
		return FALSE;
	}
	hfview->path = path;
	hfview->socket = socket;
	hfview->send = hfview_send_info;
	hfview->recv = hfview_recv_info;
	
	hfview_recv_init(&hfview->frecv);
	hfview_send_init(&hfview->fsend);
	return TRUE;
}

/*
 * 接收文件信息
 */
static boolean hfview_recv_info(struct hfview_struct *hfview)
{
	
	if (!hfview || hfview->socket->tcp == -1) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	return hnet_recv(hfview->socket->tcp, 
			(void *)&hfview->frecv, sizeof(struct hfview_recv));
}

/*
 * 发送反馈信息
 */
static boolean hfview_send_info(struct hfview_struct *hfview)
{
	if (!hfview || hfview->socket->tcp == -1) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		return FALSE;
	}
	return hnet_send(hfview->socket->tcp, 
			(void *)&hfview->fsend, sizeof(struct hfview_send));
}

boolean hfview_recv_init(struct hfview_recv *frecv)
{
	LINUXARMS_POINTER(frecv);
	frecv->protocol = FMAX;
	memset(frecv->name,'\0', FILE_NAME_LEN);
	memset(frecv->user,'\0', FILE_USER_LEN);
	frecv->size = 0;
	frecv->type = 0;
	return TRUE;
}
boolean hfview_send_init(struct hfview_send *fsend)
{
	LINUXARMS_POINTER(fsend);
	fsend->protocol = FMAX;
	return TRUE;
}
/*
 * hfview主调函数
 */
boolean do_file_view(struct hfview_struct *hfview)
{
	/*
	 * 调用其他模块，完成文件浏览功能
	 */
	struct hfview_widget *widget;
	struct hfview_recv *frecv;
	char *stime;
	GtkTreeModel *model;
	GtkListStore *list_store;

	GtkTreeIter iter;
	GdkPixbuf *directory;
	GdkPixbuf *file;

		debug_where();
	LINUXARMS_POINTER(hfview);
	widget = &hfview->widget;
	frecv = &hfview->frecv;
	gtk_widget_set_sensitive(widget->treeview, FALSE);
	debug_where();
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget->treeview));
	list_store = GTK_LIST_STORE(model);
/*
	if (gtk_tree_model_get_iter_first(model, &iter)) {
		gtk_list_store_clear(list_store);
		debug_where();
	}*/
	
	debug_where();
	directory = gdk_pixbuf_new_from_file_at_size(find_file("gtk-directory.png"), 16, 16, NULL);
	file = gdk_pixbuf_new_from_file_at_size(find_file("gtk-file.png"), 16, 16, NULL);
	debug_where();
	while (TRUE) {
		hfview->recv(hfview);
		switch (frecv->protocol) {
		case FSENDALL:
			goto out;
		case FVIEW:
			stime = ctime(&frecv->mtime);
			stime[strlen(stime) - 1] = '\0';
			gtk_list_store_append(list_store, &iter);
			gtk_list_store_set(list_store, &iter, 
			COL_FPIXBUF, frecv->type == TYPE_DIR ? directory : file,
			COL_FNAME, frecv->name,
			COL_FSIZE, frecv->size,
			COL_FTYPE, frecv->type == TYPE_DIR ? "文件夹" : "文件",
			COL_FUSER, frecv->user,
			COL_FMTIME,stime,
			-1);
			break;
		default:
			break;
		}
	}
out:
	gtk_widget_set_sensitive(widget->treeview, TRUE);
	return TRUE;
}
void cb_fview_selection_changed(GtkWidget *widget, gpointer user_data)
{
	debug_where();
	debug_print("fview selection");
}

gboolean cb_fview_button_press(GtkWidget *widget,
		        GdkEventButton *event, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hfview_struct *hfview = hfthread->hfview;
	//if (event->type == GDK_BUTTON_RELEASE)
	//	return FALSE;
	debug_print("文件浏览界面鼠标按下\n");
	if (event->button == BUTTON_RIGHT) {
		GtkWidget *popup_menu = create_popup_menu_fview(linuxarms);
		gtk_menu_popup (GTK_MENU(popup_menu),
			     NULL, NULL, NULL, NULL,
			     event->button, event->time);
		return TRUE;
	}
	/*if (event->button == BUTTON_LEFT) {
		printf("哦哦\n");
		statusbar_set_text("按下鼠标左键");
		return TRUE;
	}*/
	if (event->type == GDK_2BUTTON_PRESS && event->button == BUTTON_LEFT) {
		debug_print("双击鼠标左键\n");
		boolean select;
		GtkTreeModel *model;
		GtkListStore *list_store;
		GtkTreeIter iter;
		GtkTreeSelection *selection;
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(hfview->widget.treeview));
		select = gtk_tree_selection_get_selected(selection, &model, &iter);
		if (select) {
			char *type,*name;
			gtk_tree_model_get(model, &iter,
					COL_FNAME, &name,
					COL_FTYPE, &type,
					-1);
			if (strcmp(type, "文件") == 0)
				goto RETURN_FALSE;
			if (strcmp(name, ".") == 0)
				goto RETURN_FALSE;
			if (strcmp(name, "..") == 0) {
				cb_fview_up_clicked(NULL, (gpointer)linuxarms);
				goto RETURN_TRUE;
			}
			char path[PATH_LEN],*p;
			int len;
			strcpy(path, hfview_get_path(hfview));
			len = strlen(path);
			if (len + strlen(name) + 1 >= PATH_LEN) {
				message_box_error(linuxarms->mwindow->window, "路径过深，无法访问");
				goto RETURN_FALSE;
			}
			list_head_set_path(path);
			debug_print("path = %s\n", path);
			p = path + len;
			if (*(p - 1) == '/')
				sprintf(p, "%s", name);
			else
				sprintf(p, "/%s", name);

			debug_print("path = %s\n", path);
			hfview_set_path(hfview, path);
			hfthread_trans_set_path(&hfthread->trans, path);
			gtk_widget_set_sensitive(hfview->widget.up, TRUE);
			gtk_widget_set_sensitive(hfview->widget.back, TRUE);

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(hfview->widget.treeview));
	list_store = GTK_LIST_STORE(model);

	if (gtk_tree_model_get_iter_first(model, &iter)) {
		 g_signal_handler_block(hfview->widget.treeview,button_pressed_signal_id);
		gtk_list_store_clear(list_store);
		 g_signal_handler_unblock(hfview->widget.treeview,button_pressed_signal_id);

		debug_where();
	}
RETURN_TRUE:
			g_free(name);
			g_free(type);
		debug_where();
			hfthread->set_protocol(hfthread, FVIEW);
			hfthread->send(hfthread);
		debug_where();
			return TRUE;
RETURN_FALSE:
			g_free(name);
			g_free(type);
			return TRUE;
		} 
		return FALSE;
	}
	return FALSE;

}

boolean hfview_set_path(struct hfview_struct *hfview, const char *path)
{
	LINUXARMS_POINTER(hfview);
	LINUXARMS_CHAR(path);
	gtk_entry_set_text(GTK_ENTRY(hfview->widget.path), path);
	return TRUE;
}
const char *hfview_get_path(struct hfview_struct *hfview)
{
	if (!hfview || !hfview->widget.path)
		return NULL;
	return gtk_entry_get_text(GTK_ENTRY(hfview->widget.path));
}
