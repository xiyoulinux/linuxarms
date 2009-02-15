#include <gtk/gtk.h>
#include <string.h>

#include "filetrans.h"
#include "message.h"
#include "debug.h"
#include "error.h"
#include "linuxarms.h"
#include "hfthread.h"
#include "hmthread.h"
#include "mwindow.h"
#include "fview.h"

void cb_fview_rename_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	
	debug_where();
	selection = gtk_tree_view_get_selection(
		GTK_TREE_VIEW(hfthread->hfview->widget.treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		char *name, *user;
		gtk_tree_model_get(list_store, &iter,
				COL_FTYPE, &name,
				COL_FUSER, &user,
				-1);
		if (hmthread->competence(hmthread) && strcmp(user, "root") != 0) {
			message_box_error(linuxarms->mwindow->window, "没有权限重命名文件");
			return;
		}
		create_window_rename(linuxarms);
		gtk_entry_set_text(GTK_ENTRY(hfthread->widget.oldname), name);
		g_free(name);
		g_free(user);
	}
}


void cb_fview_delete_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	
	debug_where();
	selection = gtk_tree_view_get_selection(
		GTK_TREE_VIEW(hfthread->hfview->widget.treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		char *name, *user;
		gtk_tree_model_get(list_store, &iter,
				COL_FTYPE, &name,
				COL_FUSER, &user,
				-1);
		if (hmthread->competence(hmthread) && strcmp(user, "root") != 0) {
			message_box_error(linuxarms->mwindow->window, "没有权限删除文件");
			return;
		}
		hfthread_trans_set_path(&hfthread->trans, hfview_get_path(hfthread->hfview));
		strcpy(hfthread->trans.rename[OLDNAME],name);
		hfthread->set_protocol(hfthread, FDELETE);
		if (hfthread->send(hfthread))
			gtk_widget_set_sensitive(hfthread->hfview->widget.treeview, FALSE);
		g_free(name);
		g_free(user);
	}
}


void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	debug_where();
	GtkWidget *filechooserdialog = create_filechooserdialog(linuxarms);
	GtkFileFilter *filter = gtk_file_filter_new ();
  	gtk_file_filter_set_name (filter, "All File(*)");
  	gtk_file_filter_add_pattern(filter,"*");
  	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (filechooserdialog), filter);
  	gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (filechooserdialog), filter);
  	gtk_widget_show(filechooserdialog);
  	gtk_dialog_run (GTK_DIALOG (filechooserdialog));

}


void cb_fview_download_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	debug_where();
	GtkWidget *filechooserdialog = create_filechooserdialog(linuxarms);
	GtkFileFilter *filter = gtk_file_filter_new ();
  	gtk_file_filter_set_name (filter, "All File(*)");
  	gtk_file_filter_add_pattern(filter,"*");
  	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (filechooserdialog), filter);
  	gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (filechooserdialog), filter);
  	gtk_widget_show(filechooserdialog);
  	gtk_dialog_run (GTK_DIALOG (filechooserdialog));
}

void cb_button_cancel_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *filechooserdialog = (GtkWidget *)user_data;
	gtk_widget_destroy (filechooserdialog);
}


void cb_button_open_clicked(GtkButton *button, gpointer user_data)
{

	GtkWidget *filechooserdialog = (GtkWidget *)user_data;
	char *file_name;
	file_name=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (filechooserdialog));
	debug_print("get file name: %s\n", file_name);
  	gtk_widget_destroy (filechooserdialog);
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
	if (hfthread->send(hfthread))
		gtk_widget_set_sensitive(hfthread->hfview->widget.treeview, FALSE);
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
