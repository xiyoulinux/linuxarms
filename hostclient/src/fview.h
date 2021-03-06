#ifndef _FILEVIEW_H
#define _FILEVIEW_H

#include <gtk/gtk.h>
#include <sys/types.h>
#include "linuxarms.h"
#include "protocol.h"
#include "hnet.h"

#define FSIZE_KB (1024)
#define _fsize_kb(kb) ((kb) / FSIZE_KB)
#define __fsize_kb(kb) ((kb) % FSIZE_KB / (int)(FSIZE_KB / 10))

#define FSIZE_MB (FSIZE_KB * 1024)
#define _fsize_mb(mb) ((mb) / FSIZE_MB)
#define __fsize_mb(mb) ((mb) % FSIZE_MB / (int)(FSIZE_MB / 10))

#define FSIZE_GB (FSIZE_MB * 1024)
#define _fsize_gb(gb) ((gb) / (float)FSIZE_GB)
#define __fsize_gb(gb) ((long long)(gb) % FSIZE_GB / (float)(FSIZE_GB / 10.0))

#define FILE_INFO_COLUMNS 5
#define FILE_USER_LEN 20
enum {
	TYPE_DIR = 4,
	TYPE_FILE = 8
};
enum {
	COL_FPIXBUF,
	COL_FNAME,
	COL_FTYPE,
	COL_FSIZE,	
	COL_FUSER,
	COL_FMTIME
};

/*
 * hfview_recv   文件浏览功能中接收的文件信息
 * @protocol:     控制状态
 * @name:     文件名
 * @size:     文件大小
 * @type:     文件类型
 * @mtime:    文件修改时间
 */
struct hfview_recv {
	protocol_fthread protocol;
	char name[FILE_NAME_LEN];
	off_t size;
	unsigned int type;
	char user[FILE_USER_LEN];
	time_t mtime;
};
boolean hfview_recv_init(struct hfview_recv *frecv);
/*
 * hfview_send 文件浏览功能中发送给armserver的反馈信息
 * @protocol:      发送给armserver的反馈信息
 */
struct hfview_send {
	protocol_fthread protocol;
};
boolean hfview_send_init(struct hfview_send *fsend);
/*
 * hfview_widget  与文件浏览相关的控件
 * @path:     文件路径控件
 * @treeview:  文件显示控件
 */
struct hfview_widget {
	GtkWidget *path;
	GtkWidget *treeview;
	GtkAdjustment *vadjustment;
	GtkWidget *back;
	GtkWidget *up;
	GtkWidget *rename;
	GtkWidget *del;
	GtkWidget *hide;
	GtkWidget *popup_rename;
	GtkWidget *popup_del;
	GtkWidget *popup_upload;
	GtkWidget *popup_download;
	GtkTreeIter selection;
	boolean popup;
};
/*
 * hfview_struct   文件浏览控制数据结构
 * @path:     文件路径
 * @frecv:    要接收的文件信息
 * @fsend:    要发送的反馈信息
 * @socket:   TCP连接
 * @widget:   文件浏览相关的控件
 */
struct hfview_struct {
	char *path;
	struct hfview_recv frecv;
	struct hfview_send fsend;
	struct hnet_struct *socket;
	struct hfview_widget widget;

	boolean (*recv)(struct hfview_struct *hfview);
	boolean (*send)(struct hfview_struct *hfview);
};

GtkListStore *create_page_fview(struct linuxarms_struct *linuxarms);
void cb_fview_selection_changed(GtkWidget *widget, gpointer user_data);
gboolean cb_fview_button_press(GtkWidget *widget,
	                 GdkEventButton *event, gpointer user_data);
void cb_treeview_fview_row_activated(GtkTreeView *treeview,
		GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data);

boolean hfview_init(struct hfview_struct *hfview,char *path,
		    struct hnet_struct *socket);
boolean hfview_rename(struct hfthread_struct *hfthread);
boolean hfview_delete(struct hfthread_struct *hfthread);
boolean do_file_view(struct hfview_struct *hfview);
boolean hfview_set_path(struct hfview_struct *hfview, const char *path);
const char *hfview_get_path(struct hfview_struct *hfview);

GtkWidget* create_popup_menu_fview(struct linuxarms_struct *linuxarms);
void cb_fview_rename_activate(GtkMenuItem *menuitem, gpointer user_data);
void cb_fview_delete_activate(GtkMenuItem *menuitem, gpointer user_data);
extern void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data);
extern void cb_fview_download_activate(GtkMenuItem *menuitem, gpointer user_data);

void create_window_rename(struct linuxarms_struct *linuxarms);
void cb_window_rename_destroy(GtkObject *object,gpointer user_data);
void cb_rename_ok_clicked(GtkButton *button, gpointer user_data);
void cb_rename_help_clicked(GtkButton *button, gpointer user_data);
void cb_rename_entry_changed(GtkEditable *editable, gpointer user_data);

boolean hfview_rename_success(struct hfview_struct *hfview, const char *newname);
boolean hfview_delete_success(struct hfview_struct *hfview);
#endif
