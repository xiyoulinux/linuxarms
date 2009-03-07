#define __DEBUG__
#include <gtk/gtk.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <glib.h>

#include "filetrans.h"
#include "message.h"
#include "debug.h"
#include "error.h"
#include "linuxarms.h"
#include "hfthread.h"
#include "htthread.h"
#include "hmthread.h"
#include "mwindow.h"
#include "fview.h"
#include "protocol.h"

void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms =(struct linuxarms_struct *)user_data;
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;
	
	debug_where();
	htthread->select = FUP;
	GtkWidget *filechooserdialog = create_filechooserdialog(linuxarms);
	GtkFileFilter *filter = gtk_file_filter_new();
  	gtk_file_filter_set_name(filter, "All File(*)");
  	gtk_file_filter_add_pattern(filter,"*");
  	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooserdialog), filter);
  	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(filechooserdialog), filter);
  	gtk_widget_show(filechooserdialog);
  	gtk_dialog_run(GTK_DIALOG(filechooserdialog));
}


void cb_fview_download_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms =(struct linuxarms_struct *)user_data;
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;
	debug_where();

	htthread->select = FDOWN;
	GtkWidget *filechooserdialog = create_filechooserdialog(linuxarms);
	GtkFileFilter *filter = gtk_file_filter_new();
  	gtk_file_filter_set_name(filter, "All File(*)");
  	gtk_file_filter_add_pattern(filter,"*");
  	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooserdialog), filter);
  	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(filechooserdialog), filter);
  	gtk_widget_show(filechooserdialog);
  	gtk_dialog_run(GTK_DIALOG(filechooserdialog));
}

void cb_ftrans_button_cancel_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *filechooserdialog =(GtkWidget *)user_data;
	gtk_widget_destroy(filechooserdialog);
}

void cb_ftrans_window_close(GtkWidget *widget, gpointer user_data)
{
	gtk_widget_destroy(widget);
}

void cb_ftrans_button_ok_clicked(GtkButton *button, gpointer user_data)
{
	struct linuxarms_struct *linuxarms =(struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hfview_struct *hfview = hfthread->hfview;
	struct htthread_struct *htthread = hfthread->hftrans;
	GtkWidget *filechooserdialog = htthread->widget.choose;
	boolean select;
	GtkListStore *list_store;
	GtkTreeIter *iter = &hfview->widget.selection;
	char path[PATH_LEN], *name;
	char login_user[USER_NAME_LEN], *home;
	struct stat buf;
	char mode_size[20];
	char tooltips[512];

	switch (htthread->select) {
	case FDOWN:
		strcpy(path, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserdialog)));
		if (stat(path, &buf) == 0 &&  S_ISDIR(buf.st_mode)) {
			gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooserdialog), path);
			return;
		}
		strcpy(htthread->path, path);
		name = strrchr(path, '/');
		name = name + 1;
		snprintf(mode_size, 20, "%d %ld", buf.st_mode, buf.st_size);
		hfthread_trans_set_rename(&hfthread->trans, name, mode_size);
		hfthread_trans_set_path(&hfthread->trans,hfview_get_path(hfview));
		htthread->total_size = buf.st_size;
		debug_print("src path: %s\n", htthread->path);
		debug_print("dest path: %s/%s\n", hfthread->trans.path, name);
		*(name - 1) = '\0';
		snprintf(tooltips, 512, "传输类型：下载(从本机下载文件到arm系统)\n"
				 "文件名：%s MB\n"
				 "文件大小：%ld.%ld\n"
				 "源地址：localhost@%s\n"
				 "目的地址：%s@%s\n",
				 name, _fsize_mb(buf.st_size),__fsize_mb(buf.st_size),
				 path, linuxarms->login->user.ip, 
				 hfview_get_path(hfview));
		gtk_tooltips_set_tip(htthread->widget.tooltips, 
				     htthread->widget.progressbar, tooltips, NULL);
		hfthread->set_protocol(hfthread, FDOWN);
		hfthread->send(hfthread);
		break;
	case FUP:
		strcpy(path, gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(filechooserdialog)));
		strcpy(login_user, getlogin());
		home = getenv("HOME");
		/* 
		 * 如果当前用户不是root用户且要保存的路径不再用户目录中，则提示
		 * 用户没有权限将文件保存在选中的路径中 
		 */
		if (strcmp(login_user, "root") != 0 && strstr(path, home) == NULL) {
			message_box_error(linuxarms->mwindow->window,
					"没有权限将文件保存在\n"
					"当前路径中，请重新选择路径");
			return;
		}
		strcpy(htthread->path, path);
		list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(hfview->widget.treeview)));
		select = gtk_list_store_iter_is_valid(list_store, iter);
		if (select) {
			char *fname, *ssize;
			gtk_tree_model_get(GTK_TREE_MODEL(list_store), iter, 
					   COL_FNAME, &fname,
					   COL_FSIZE, &ssize, -1);
			snprintf(htthread->path, PATH_LEN, "%s/%s", path, fname);
			if (access(htthread->path, F_OK) == 0) {
				snprintf(tooltips, 512, "警告：文件 %s\n"
					 "已经存在，如果继续上传，将覆盖该文件，\n"
					 "确定继续上传？", htthread->path);
				if (!message_box_choose(linuxarms->mwindow->window, 
						GTK_MESSAGE_WARNING,tooltips))
					return;
			}
			snprintf(tooltips, 512, "传输类型：上传(从arm系统上传文件到本机)\n"
				 "文件名：%s\n"
				 "文件大小：%s\n"
				 "源地址：%s@%s\n"
				 "目的地址：localhost@%s\n",
				 fname, ssize, linuxarms->login->user.ip, 
				 hfview_get_path(hfview), path);
			gtk_tooltips_set_tip(htthread->widget.tooltips, 
					     htthread->widget.progressbar, tooltips, NULL);
			debug_print("src path: %s/%s\n", hfthread->trans.path, fname);
			debug_print("dest path: %s\n", htthread->path);
			hfthread_trans_set_path(&hfthread->trans, hfview_get_path(hfview));
			hfthread_trans_set_rename(&hfthread->trans, fname, "nomode");
			hfthread->set_protocol(hfthread, FUP);
			hfthread->send(hfthread);
			g_free(fname);
			g_free(ssize);
		}
		break;
	default:
		goto out;
	}
out:
  	gtk_widget_destroy(filechooserdialog);
}
