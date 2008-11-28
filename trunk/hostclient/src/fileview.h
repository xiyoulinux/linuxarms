#ifndef _FILEVIEW_H
#define _FILEVIEW_H

#include "linuxarms.h"
#include "protocol.h"
#include "hnet.h"
#include <gtk/gtk.h>
#define FILE_INFO_COLUMNS 4
#define FILE_NAME_LEN 256
enum {
	COL_FPIXBUF,
	COL_FNAME,
	COL_FSIZE,
	COL_FTYPE,
	COL_FMTIME
};
/*
 * hfview_recv   文件浏览功能中接收的文件信息
 * @ctrl:     控制状态
 * @name:     文件名
 * @size:     文件大小
 * @type:     文件类型
 * @mtime:    文件修改时间
 */
struct hfview_recv {
	protocol_fthread ctrl;
	char name[FILE_NAME_LEN];
	long long size;
	unsigned int type;
	time_t mtime;

	
};
/*
 * hfview_send 文件浏览功能中发送给armserver的反馈信息
 * @ctrl:      发送给armserver的反馈信息
 */
struct hfview_send {
	protocol_fthread ctrl;
};
/*
 * hfview_widget  与文件浏览相关的控件
 * @fpath:     文件路径控件
 * @treeview:  文件显示控件
 */
struct hfview_widget {
	GtkWidget *fpath;
	GtkWidget *treeview;
	GtkWidget *back;
	GtkWidget *up;
	GtkWidget *rename;
	GtkWidget *del;
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

static inline boolean hfview_recv(struct hfview_struct *hfview)
{
	if (!hfview)
		return FALSE;
	return hnet_recv(hfview->socket->tcp,
			 (void *)&hfview->frecv,
			 sizeof(struct hfview_recv));
}
static inline boolean hfview_send(struct hfview_struct *hfview)
{
	if (!hfview)
		return FALSE;
	return hnet_send(hfview->socket->tcp,
			 (void *)&hfview->fsend,
			 sizeof(struct hfview_send));
}
GtkListStore *create_page_fview(GtkWidget *notebook_main);
void cb_fview_selection_changed(GtkWidget *widget, gpointer user_data);

gboolean cb_fview_button_press(GtkWidget *widget,
	                 GdkEventButton *event, gpointer user_data);


#endif
