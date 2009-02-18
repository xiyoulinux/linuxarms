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

#define FSIZE_KB (1024)
#define FSIZE_MB (FSIZE_KB * 1024)
#define FSIZE_GB (FSIZE_MB * 1024)
#define FSIZE_TB (FSIZE_GB * 1024)

static int prev_file_nums = 0;
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
void cb_treeview_fview_columns_changed(GtkTreeView *treeview,
		                                gpointer user_data)
{
	//GtkListStore *list_store = (GtkListStore *)user_data;
	//gtk_list_store_clear(list_store);

}

char *get_file_size(off_t size)
{
	static char fsize[30];
	if (size < FSIZE_KB)
		snprintf(fsize, 30, "%ld B", size);
	else if (size < FSIZE_MB)
		snprintf(fsize, 30, "%ld.%2ld KB", size / FSIZE_KB, size % FSIZE_KB);
	else if (size < FSIZE_GB)
		snprintf(fsize, 30, "%ld.%2ld MB", size / FSIZE_MB, size % FSIZE_MB);
	else if (size < FSIZE_TB)
		snprintf(fsize, 30, "%ld.%2ld GB", size / FSIZE_GB, size % FSIZE_GB);
	else
		snprintf(fsize, 30, "%s", "大于1TB");
	return fsize;
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
	GtkListStore *list_store;
	//GtkTreeModel *list_store;

	GtkTreeIter iter;
	GdkPixbuf *directory;
	GdkPixbuf *file;

	debug_where();
	LINUXARMS_POINTER(hfview);
	debug_where();
	widget = &hfview->widget;
	debug_where();
	frecv = &hfview->frecv;
	//gtk_widget_set_sensitive(widget->treeview, FALSE);
	debug_where();
	list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget->treeview)));
	debug_where();
	directory = gdk_pixbuf_new_from_file_at_size(find_file("gtk-directory.png"), 16, 16, NULL);
	file = gdk_pixbuf_new_from_file_at_size(find_file("gtk-file.png"), 16, 16, NULL);
	debug_where();
	int file_nums = 0;

	boolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store), &iter);
	while (TRUE) {
		if (!hfview->recv(hfview)) {
			debug_print("接收文件信息失败\n");
			goto out;
		}
		//debug_print("%-30s\t%-8ld\t%-4d\t%s\n", frecv->name, frecv->size, frecv->type, frecv->user);
		switch (frecv->protocol) {
		case FSENDALL:
			debug_print("接收进程信息结束\n");
			goto out;
		case FVIEW:
			if (!valid) {
				gtk_list_store_append(list_store, &iter);
				debug_print("新建 %d\n", file_nums);
			}
			file_nums++;
			stime = ctime(&frecv->mtime);
			stime[strlen(stime) - 1] = '\0';
			//gtk_tree_store_append(GTK_TREE_STORE(list_store), &tree_iter, NULL);
			gtk_list_store_set(GTK_LIST_STORE(list_store), &iter, 
				COL_FPIXBUF, frecv->type == TYPE_DIR ? directory : file,
				COL_FNAME, frecv->name,
				COL_FSIZE, get_file_size(frecv->size),
				COL_FTYPE, frecv->type == TYPE_DIR ? "文件夹" : "文件",
				COL_FUSER, frecv->user,
				COL_FMTIME,stime,
				-1);
			if (valid)
				valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
			break;
		default:
			break;
		}
	}
	debug_where();
out:
	//gtk_widget_set_sensitive(widget->treeview, TRUE);
	if (file_nums < prev_file_nums) {
		int i = prev_file_nums - file_nums;
		while (i--) {
			gtk_list_store_set(GTK_LIST_STORE(list_store), &iter, 
				COL_FPIXBUF, NULL,
				COL_FNAME, "",
				COL_FSIZE, "",
				COL_FTYPE, "",
				COL_FUSER, "",
				COL_FMTIME,"",
				-1);
			gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
		}
		/*while (valid && i--) {
			GtkTreePath *path;
			if (!gtk_list_store_iter_is_valid(list_store, &iter))
				break;
			path = gtk_tree_model_get_path(GTK_TREE_MODEL(list_store), &iter);
			if (gtk_tree_model_get_iter(GTK_TREE_MODEL(list_store), &iter, path)) {
				 valid = gtk_list_store_remove(GTK_LIST_STORE(list_store), &iter);
				debug_print("free......................\n");
			}
		}*/
	}
	prev_file_nums = file_nums;
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
	boolean select;
	GtkTreeSelection *selection;
	//selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(hfview->widget.treeview));
//	select = gtk_tree_selection_get_selected(selection, &model, &iter);
//	if (event->type != GDK_BUTTON_PRESS && event->type != GDK_2BUTTON_PRESS)
//		return FALSE;
	debug_print("文件浏览界面鼠标按下\n");
	if (event->type == GDK_BUTTON_PRESS && event->button == BUTTON_RIGHT) {
		GtkWidget *popup_menu = create_popup_menu_fview(linuxarms);
		gtk_menu_popup (GTK_MENU(popup_menu),
			     NULL, NULL, NULL, NULL,
			     event->button, event->time);
		return FALSE;
	}
	/*if (event->button == BUTTON_LEFT) {
		printf("哦哦\n");
		statusbar_set_text("按下鼠标左键");
		return TRUE;
	}*/
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
void cb_treeview_fview_row_activated(GtkTreeView *treeview,
                GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hfview_struct *hfview = hfthread->hfview;
	debug_print("选中一行\n");
	boolean select;
	GtkTreeModel *model;
	//GtkListStore *list_store;
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
		debug_where();
		debug_print("path = %s\n", path);
		hfview_set_path(hfview, path);
		debug_where();
		hfthread_trans_set_path(&hfthread->trans, path);
		debug_where();
		gtk_widget_set_sensitive(hfview->widget.up, TRUE);
		debug_where();
		gtk_widget_set_sensitive(hfview->widget.back, TRUE);
		debug_where();

RETURN_TRUE:
		hfthread->set_protocol(hfthread, FVIEW);
		hfthread->send(hfthread);
		debug_where();
RETURN_FALSE:
		debug_where();
		g_free(name);
		g_free(type);
	} 
}
