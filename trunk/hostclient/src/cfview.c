#define __DEBUG__
#include <gtk/gtk.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "linuxarms.h"
#include "fview.h"
#include "hfthread.h"
#include "htthread.h"
#include "debug.h"
#include "error.h"
#include "filetrans.h"
#include "sprocess.h"
#include "statusbar.h"
#include "support.h"
#include "toolbar.h"
#include "message.h"
#include "mwindow.h"

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
	hfview->widget.popup = FALSE;
	
	hfview_recv_init(&hfview->frecv);
	hfview_send_init(&hfview->fsend);
	return TRUE;
}

/*
 * 接收文件信息
 */
static boolean hfview_recv_info(struct hfview_struct *hfview)
{
	LINUXARMS_POINTER(hfview);
	return hnet_recv(hfview->socket->tcp, (void *)&hfview->frecv,
			 sizeof(struct hfview_recv));
}

/*
 * 发送反馈信息
 */
static boolean hfview_send_info(struct hfview_struct *hfview)
{
	LINUXARMS_POINTER(hfview);
	return hnet_send(hfview->socket->tcp, (void *)&hfview->fsend,
			 sizeof(struct hfview_send));
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

char *get_file_size(off_t size)
{
	static char fsize[30];
	if (size < FSIZE_KB)
		snprintf(fsize, 30, "%ld.0 B", size);
	else if (size < FSIZE_MB)
		snprintf(fsize, 30, "%ld.%2ld KB", _fsize_kb(size), __fsize_kb(size));
	else if (size < FSIZE_GB)
		snprintf(fsize, 30, "%ld.%2ld MB", _fsize_mb(size), __fsize_mb(size));
	else
		snprintf(fsize, 30, "1 GB");
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
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	GdkPixbuf *directory;
	GdkPixbuf *file;
	int file_nums;
	boolean valid;

	debug_where();
	LINUXARMS_POINTER(hfview);
	widget = &hfview->widget;
	frecv = &hfview->frecv;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget->treeview));
	list_store = GTK_LIST_STORE(model);
	directory = gdk_pixbuf_new_from_file_at_size(find_file("gtk-directory.png"), 16, 16, NULL);
	file = gdk_pixbuf_new_from_file_at_size(find_file("gtk-file.png"), 16, 16, NULL);
	debug_where();
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget->treeview));
	valid = gtk_tree_selection_get_selected(selection, &model, &iter);
	if (valid) 
		gtk_tree_selection_unselect_iter(selection, &iter);
	
	valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store), &iter);
	file_nums = 0;
	while (TRUE) {
		if (!hfview->recv(hfview)) {
			debug_print("接收文件信息失败\n");
			goto out;
		}
		//debug_print("%-30s\t%-8ld\t%-4d\t%s\n", frecv->name, frecv->size, frecv->type, frecv->user);
		switch (frecv->protocol) {
		case FSENDALL:
			debug_print("接收文件信息结束\n");
			goto out;
		case FVIEW:
			if (!valid) {
				gtk_list_store_append(list_store, &iter);
			}
			file_nums++;
			stime = ctime(&frecv->mtime);
			stime[strlen(stime) - 1] = '\0';
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
	frecv->protocol = FMAX;
	if (file_nums < prev_file_nums) {
		int i = prev_file_nums - file_nums;
		while (i-- && valid) {
			gtk_list_store_set(GTK_LIST_STORE(list_store), &iter, 
				COL_FPIXBUF, NULL,
				COL_FNAME, "",
				COL_FSIZE, "",
				COL_FTYPE, "",
				COL_FUSER, "",
				COL_FMTIME,"",
				-1);
			valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
		}
	}
	prev_file_nums = file_nums;
	gtk_adjustment_set_value(GTK_ADJUSTMENT(widget->vadjustment), 0);
	//gtk_widget_queue_draw(widget->treeview);
	while (g_main_iteration(FALSE));  
	return TRUE;
}
void cb_fview_selection_changed(GtkWidget *widget, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct htthread_struct *htthread = hfthread->hftrans;
	struct hfview_struct *hfview = hfthread->hfview;
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	boolean rename, delete, upload, download;
	char home[PATH_LEN];
	const char *path;
	
	rename = delete = upload = download = FALSE;
	debug_where();
	
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(hfview->widget.treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		char *type, *user, *name;
		gtk_tree_model_get(list_store, &iter,
				COL_FNAME, &name,
				COL_FTYPE, &type,
				COL_FUSER, &user,
				-1);
		if (strlen(name) == 0) {
			gtk_tree_selection_unselect_iter(selection, &iter);
			goto out;
		}
		if (strcmp(type, "文件夹") == 0) {
			upload = FALSE;
			delete = TRUE; 
		} else {
			upload = TRUE;
		}
		if (strcmp(linuxarms->login->user.name, user) == 0) {
			rename = TRUE;
			delete = delete ? FALSE : TRUE;

		} else
			delete = FALSE;
		if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
			rename = FALSE;
		}
		hfview->widget.selection = iter;
out:
		g_free(name);
		g_free(type);
		g_free(user);
	}
	path = hfview_get_path(hfview);
	snprintf(home, PATH_LEN, "/home/%s",linuxarms->login->user.name);
	if (strstr(path, home)) {
		download = TRUE;
	}
	if (hfthread->competence) {
		download = TRUE;
	}
	if (hfview->widget.popup) {
		gtk_widget_set_sensitive(hfview->widget.popup_rename, rename);
		gtk_widget_set_sensitive(hfview->widget.popup_del, delete);
		gtk_widget_set_sensitive(hfview->widget.popup_upload, upload);
		gtk_widget_set_sensitive(hfview->widget.popup_download, download);
	}
	gtk_widget_set_sensitive(hfview->widget.rename, rename);
	gtk_widget_set_sensitive(hfview->widget.del, delete);
	gtk_widget_set_sensitive(htthread->widget.menubar_upload, upload);
	gtk_widget_set_sensitive(htthread->widget.menubar_download, download);
	gtk_widget_set_sensitive(htthread->widget.toolbar_upload, upload);
	gtk_widget_set_sensitive(htthread->widget.toolbar_download, download);
}

gboolean cb_fview_button_press(GtkWidget *widget,
		        GdkEventButton *event, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfview_struct *hfview = linuxarms->hfthread->hfview;
	//selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(hfview->widget.treeview));
//	select = gtk_tree_selection_get_selected(selection, &model, &iter);
//	if (event->type != GDK_BUTTON_PRESS && event->type != GDK_2BUTTON_PRESS)
//		return FALSE;
	debug_print("文件浏览界面鼠标按下\n");
	if (hfview->widget.popup)
		hfview->widget.popup = FALSE;
	if (event->type == GDK_BUTTON_PRESS && event->button == BUTTON_RIGHT) {
		GtkWidget *popup_menu = create_popup_menu_fview(linuxarms);
		gtk_menu_popup (GTK_MENU(popup_menu),
			     NULL, NULL, NULL, NULL,
			     event->button, event->time);
		hfview->widget.popup = TRUE;
		cb_fview_selection_changed(NULL, linuxarms);
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
void cb_treeview_fview_row_activated(GtkTreeView *treeview,
                GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hfview_struct *hfview = hfthread->hfview;
	boolean select;
	GtkTreeModel *model;
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
		p = path + len;
		if (*(p - 1) == '/')
			sprintf(p, "%s", name);
		else
			sprintf(p, "/%s", name);
		hfview_set_path(hfview, path);
		hfthread_trans_set_path(&hfthread->trans, path);
		gtk_widget_set_sensitive(hfview->widget.up, TRUE);
		gtk_widget_set_sensitive(hfview->widget.back, TRUE);
RETURN_TRUE:
		hfthread->set_protocol(hfthread, FVIEW);
		hfthread->send(hfthread);
RETURN_FALSE:
		debug_where();
		g_free(name);
		g_free(type);
	} 
}

void cb_fview_rename_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hfview_widget *widget = &hfthread->hfview->widget;
	GtkTreeModel *list_store;
	GtkTreeIter *iter = &widget->selection;
	boolean select;
	
	debug_where();
	list_store = gtk_tree_view_get_model(GTK_TREE_VIEW(widget->treeview));
	select = gtk_list_store_iter_is_valid(GTK_LIST_STORE(list_store), iter);
	if (select) {
		char *name, *user;
		gtk_tree_model_get(list_store, iter,
				COL_FNAME, &name,
				COL_FUSER, &user,
				-1);
		if (!hfthread->competence && strcmp(user, "root") == 0) {
			message_box_error(linuxarms->mwindow->window, "没有权限重命名文件");
			return;
		}
		create_window_rename(linuxarms);
		gtk_entry_set_text(GTK_ENTRY(hfthread->widget.oldname), name);
		g_free(name);
		g_free(user);
	}
}
boolean hfview_rename_success(struct hfview_struct *hfview, const char *newname)
{
	boolean select;
	GtkListStore *list_store;
	GtkTreeIter *iter = &hfview->widget.selection;
	
	debug_where();
	list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(hfview->widget.treeview)));
	select = gtk_list_store_iter_is_valid(list_store, iter);
	if (select) {
		gtk_list_store_set(GTK_LIST_STORE(list_store), iter,
				COL_FNAME, newname,
				-1);
		return TRUE;
	}
	return FALSE;
}

void cb_fview_delete_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hfview_struct *hfview = hfthread->hfview;
	//struct hmthread_struct *hmthread = linuxarms->hmthread;
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter *iter = &hfview->widget.selection;
	
	debug_where();
	list_store = gtk_tree_view_get_model(GTK_TREE_VIEW(hfview->widget.treeview));
	select = gtk_list_store_iter_is_valid(GTK_LIST_STORE(list_store), iter);
	if (select) {
		char *name, *user, *type;
		int sure;
		char msg[300];
		GtkWidget *dialog;
		gtk_tree_model_get(list_store, iter,
				COL_FTYPE, &type,
				COL_FNAME, &name,
				COL_FUSER, &user,
				-1);
		if (strcmp(type, "文件夹") == 0) {
			message_box_error(linuxarms->mwindow->window, "不能删除文件夹");
			return;
		}
		if (!hfthread->competence && strcmp(user, linuxarms->login->user.name) != 0) {
			message_box_error(linuxarms->mwindow->window, "没有权限删除文件");
			return;
		}
		snprintf(msg, 300, "确定删除文件 %s ?", name);
		dialog = gtk_message_dialog_new(GTK_WINDOW(linuxarms->mwindow->window), 0,
			         GTK_MESSAGE_INFO, GTK_BUTTONS_OK_CANCEL, msg, -1);
		sure = gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		if (sure == GTK_RESPONSE_CANCEL)
			return;
		hfthread_trans_set_path(&hfthread->trans, hfview_get_path(hfthread->hfview));
		strcpy(hfthread->trans.rename[OLDNAME],name);
		hfthread->set_protocol(hfthread, FDELETE);
		hfthread->send(hfthread);
		g_free(name);
		g_free(user);
	}
}

boolean hfview_delete_success(struct hfview_struct *hfview)
{
	boolean select;
	GtkListStore *list_store;
	GtkTreeIter *iter = &hfview->widget.selection;
	
	debug_where();
	list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(hfview->widget.treeview)));
	select = gtk_list_store_iter_is_valid(list_store, iter);
	if (select) {
		gtk_list_store_remove(list_store, iter);
		return TRUE;
	}
	return FALSE;
}

void cb_fview_hide_file_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hfview_struct *hfview = hfthread->hfview;
	
	hfthread->trans.hide = !(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(hfview->widget.hide)));
	hfthread->set_protocol(hfthread, FVIEW);
	hfthread->send(hfthread);
}

void cb_window_rename_destroy(GtkObject *object,gpointer user_data)
{
	gtk_widget_destroy(GTK_WIDGET(user_data));
}                                        

void cb_rename_ok_clicked(GtkButton *button, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	const char *oldname, *newname;

	oldname = gtk_entry_get_text(GTK_ENTRY(hfthread->widget.oldname));
	newname = gtk_entry_get_text(GTK_ENTRY(hfthread->widget.newname));
	hfthread_trans_set_path(&hfthread->trans, hfview_get_path(hfthread->hfview));
	hfthread_trans_set_rename(&hfthread->trans, (char *)oldname, (char *)newname);
	hfthread->set_protocol(hfthread, FRENAME);
	hfthread->send(hfthread);
	//if (hfthread->send(hfthread))
	//	gtk_widget_set_sensitive(hfthread->hfview->widget.treeview, FALSE);
	gtk_widget_destroy(hfthread->widget.window);
}                                        

void cb_rename_help_clicked(GtkButton *button, gpointer user_data)
{
	message_box_info((GtkWidget *)user_data, 
			"请确保输入的新文件名"
			"符合文件名命名规范，"
			"并且长度小于256个字符");
}                              

void cb_rename_entry_changed(GtkEditable *editable, gpointer user_data)
{
	int i, len;
	const char *newname;
	newname = gtk_entry_get_text(GTK_ENTRY(editable));
	len = strlen(newname); 
	if (len == 0)
		return;
	for (i = 0; i < len; i ++)
		if (newname[i] != ' ')
			break;
	if (i == len)
		return;
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
}
