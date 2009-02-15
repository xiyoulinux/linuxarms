/*
 * linuxarms/hostclient/
 */
#include <string.h>
#include <unistd.h>
#include "hfthread.h"
#include "hnet.h"
#include "fview.h"
#include "filetrans.h"
#include "error.h"
#include "config.h"
#include "login.h"
#include "statusbar.h"

static void hfthread_down_lock(struct hfthread_struct *hfthread);
static void hfthread_up_lock(struct hfthread_struct *hfthread);
static boolean hfthread_set_protocol(struct hfthread_struct *hfthread, protocol_fthread protocol);
static boolean hfthread_send(struct hfthread_struct *hfthread);
static boolean hfthread_recv(struct hfthread_struct *hfthread);
/*
 * 初始化文件浏览和文件传输控制主数据结构
 */
boolean hfthread_init(struct hfthread_struct *hfthread,
                      struct hfview_struct *hfview,
                      struct htthread_struct *hftrans)
{
	            
        if (!hfthread || !hfview || !hftrans) {
		debug_where();
                print_error(ESYSERR,"无效参数");
                return FALSE;
        }
	hfthread->thread = NULL;
	hfthread->hfview  =hfview;
	hfthread->hftrans = hftrans;

	hfthread->down_lock = hfthread_down_lock;
	hfthread->up_lock = hfthread_up_lock;
	hfthread->set_protocol = hfthread_set_protocol;
        hfthread->send = hfthread_send;
	hfthread->recv = hfthread_recv;
	hfthread_trans_init(&hfthread->trans);
	//hnet_init(&hfthread->socket, get_armserver_ip(), get_fthread_port());
	hfthread->up_lock(hfthread);
	return TRUE;
}
/*
 * 文件浏览和文件传输主线程执行体
 * @p:  struct hfthread_struct
 */
gboolean hfthread_thread(void *p)
{
	struct hfthread_struct *hfthread = (struct hfthread_struct *)p;
	struct hfview_struct *hfview = hfthread->hfview;
	linuxarms_print("create hfthread thread...\n");
	hfthread->thread = linuxarms_thread_self();
	hnet_init(&hfthread->socket, get_armserver_ip(), get_fthread_port());
	if (!create_tcp_client(&hfthread->socket)) {
		print_error(ESYSERR,"建立文件浏览和文件传输网络连接错误");
		return FALSE;
	}
	debug_print("hfthread socket ip : %s tcp: %d port: %d\n", hfthread->socket.ip,
				hfthread->socket.tcp, hfthread->socket.port);
	while (hfthread->thread) {
		if (!hfthread->recv(hfthread)) {
			linuxarms_print("hfthread recive data error\n");
			break;
		}
		hfthread->down_lock(hfthread);
		switch (hfthread->trans.protocol) {
		case FUP: /* 上传文件处理 */
			break;
		case FDOWN: /* 下载文件处理 */
			break;
		case FVIEW: /* 文件浏览处理 */
			do_file_view(hfthread->hfview);
			break;
		case FNOEXIST:
			statusbar_set_text("给出的路径不存在");
			break;
		case FRENAMESUC: /* 重命名文件成功 */
			hfthread_rename_success(hfthread);
			gtk_widget_set_sensitive(hfview->widget.treeview, TRUE);
			break;
		case FRENAMEERR: /* 重命名文件失败 */
			statusbar_set_text("重命名文件失败");
			gtk_widget_set_sensitive(hfview->widget.treeview, TRUE);
			break;
		case FDELETESUC: /* 删除文件成功 */
			hfthread_delete_success(hfthread);
			gtk_widget_set_sensitive(hfview->widget.treeview, TRUE);
			break;
		case FDELETEERR: /* 删除文件失败 */
			statusbar_set_text("删除文件失败");
			gtk_widget_set_sensitive(hfview->widget.treeview, TRUE);
			break;
		default:
			break;
		}
		hfthread->set_protocol(hfthread, FMAX);
		hfthread->up_lock(hfthread);
	}
	return TRUE;
}

static void hfthread_down_lock(struct hfthread_struct *hfthread)
{
	if (!hfthread)
		return ;
	while (hfthread->lock)
		sleep(1);
	hfthread->lock = TRUE;
}
static void hfthread_up_lock(struct hfthread_struct *hfthread)
{
	if (!hfthread)
		return;
	hfthread->lock = FALSE;
}

/*
 * 设置传输结构体
 */
boolean hfthread_set_trans(struct hfthread_struct *hfthread, 
			  protocol_fthread protocol,
			  char *path, char *oldname, char *newname)
{
	if (!hfthread || !path || !oldname || !newname) {
		debug_where();
		print_error(EWARNING, "无效的参数");
		goto out;
	}
	if (!PROTOCOL_IS_FTHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		goto out;
	}
	hfthread->trans.protocol = protocol;
	strncpy(hfthread->trans.path, path , PATH_LEN);
	strncpy(hfthread->trans.rename[OLDNAME], oldname, FNAME_LEN);
	strncpy(hfthread->trans.rename[NEWNAME], newname, FNAME_LEN);
	return TRUE;
out:
	return FALSE;
}
/*
 * 发送数据到armserver上
 */
static boolean hfthread_send(struct hfthread_struct *hfthread)
{
	LINUXARMS_POINTER(hfthread);
	return hnet_send(hfthread->socket.tcp, (void *)&hfthread->trans,
			 sizeof(struct hfthread_struct));
}
/*
 * 接收armserver发送过来的数据
 */
static boolean hfthread_recv(struct hfthread_struct *hfthread)
{
	LINUXARMS_POINTER(hfthread);
	return hnet_recv(hfthread->socket.tcp, (void *)&hfthread->trans,
			 sizeof(struct hfthread_struct));
}

static boolean hfthread_set_protocol(struct hfthread_struct *hfthread, protocol_fthread protocol)
{
	LINUXARMS_POINTER(hfthread);
	return hfthread_trans_set_protocol(&hfthread->trans, protocol);
}
boolean hfthread_trans_init(struct hfthread_trans *hftrans)
{
	LINUXARMS_POINTER(hftrans);
	hftrans->protocol = FMAX;
	memset(hftrans->path, '\0', PATH_LEN);
	hftrans->path[0] = '/';
	memset(hftrans->rename, '\0', FNAME_LEN * 2);
	return TRUE;
}
boolean hfthread_trans_set_protocol(struct hfthread_trans *hftrans, protocol_fthread protocol)
{
	LINUXARMS_POINTER(hftrans);
	if (!PROTOCOL_IS_FTHREAD(protocol)) {
		hftrans->protocol = FMAX;
		return FALSE;
	}
	hftrans->protocol = protocol;
	return TRUE;
}
boolean hfthread_trans_set_path(struct hfthread_trans *hftrans, const char *path)
{
	LINUXARMS_POINTER(hftrans);
	LINUXARMS_CHAR(path);

	if (strlen(path) > PATH_LEN) {
		print_error(EWARNING, "路径过长");
		strcpy(hftrans->path, "/");
		return FALSE;
	}
	strcpy(hftrans->path, path);
	return TRUE;
}
const char *hfthread_trans_get_path(struct hfthread_trans *hftrans)
{
	if (!hftrans)
		return NULL;
	return (const char *)hftrans->path;
}
boolean hfthread_trans_set_rename(struct hfthread_trans *hftrans, char *oldname, char *newname)
{
	LINUXARMS_POINTER(hftrans);
	LINUXARMS_CHAR(oldname);
	LINUXARMS_CHAR(newname);
	if (strlen(oldname) > FNAME_LEN || strlen(newname) > FNAME_LEN) {
		print_error(EWARNING, "文件名过长");
		return FALSE;
	}
	strcpy(hftrans->rename[OLDNAME], oldname);
	strcpy(hftrans->rename[NEWNAME], newname);
	return TRUE;
}

boolean hfthread_rename_success(struct hfthread_struct *hfthread)
{
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	
	debug_where();
	selection = gtk_tree_view_get_selection(
		GTK_TREE_VIEW(hfthread->hfview->widget.treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		gtk_list_store_set(GTK_LIST_STORE(list_store), &iter,
				COL_FNAME, hfthread->trans.rename[NEWNAME],
				-1);
		return TRUE;
	}
	return FALSE;
}

boolean hfthread_delete_success(struct hfthread_struct *hfthread)
{
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	
	debug_where();
	selection = gtk_tree_view_get_selection(
		GTK_TREE_VIEW(hfthread->hfview->widget.treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		gtk_list_store_remove(GTK_LIST_STORE(list_store),&iter);
		return TRUE;
	}
	return FALSE;
}
