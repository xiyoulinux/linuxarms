#define __DEBUG__
#include <gtk/gtk.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

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
	char mode[10];

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
		snprintf(mode, 10, "%d", buf.st_mode);
		hfthread_trans_set_rename(&hfthread->trans, name, mode);
		hfthread_trans_set_path(&hfthread->trans,hfview_get_path(hfview));
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
			char *name;
			gtk_tree_model_get(GTK_TREE_MODEL(list_store), iter, COL_FNAME, &name, -1);
			hfthread_trans_set_path(&hfthread->trans, hfview_get_path(hfview));
			hfthread_trans_set_rename(&hfthread->trans, name, "nomode");
			snprintf(htthread->path, PATH_LEN, "%s/%s", path, name);
			hfthread->set_protocol(hfthread, FUP);
			hfthread->send(hfthread);
		}
		break;
	default:
		goto out;
	}
	debug_print("get file name: %s\n", path);
out:
  	gtk_widget_destroy(filechooserdialog);
}
