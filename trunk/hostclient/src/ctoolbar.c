#define __DEBUG__
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "message.h"
#include "filetrans.h"
#include "linuxarms.h"
#include "hfthread.h"
#include "mwindow.h"
#include "toolbar.h"
#include "fview.h"

struct list_head {
	struct list_head *prev;
	char path[PATH_LEN];
	boolean flag;
	struct list_head *next;
};

static struct list_head *list = NULL;

boolean list_head_new(int num)
{
	int i;
	struct list_head *head,*p, *r;
	if (num <= 2)
		return FALSE;
	head = (struct list_head *)malloc(sizeof(struct list_head));
	strcpy(head->path, "/");
	head->flag = FALSE;
	p = head;
	for (i = 1; i < num; i++) {
		r= (struct list_head *)malloc(sizeof(struct list_head));
		r->flag = FALSE;
		strcpy(r->path, "/");
		p->next = r;
		r->prev = p;
		p = r;
	}
	head->prev = p;
	p->next = head;
	list = head;
	return TRUE;
}

boolean list_head_set_path(const char *path)
{
	LINUXARMS_CHAR(path);
	list = list->prev;
	strncpy(list->path, path, PATH_LEN);
	list->flag = TRUE;
	return TRUE;
}
const char *list_head_get_path()
{
	list = list->next;
	if (list->prev->flag) {
		list->prev->flag = FALSE;
		return list->prev->path;
	}
	return NULL;
}

boolean list_head_test_next()
{
	LINUXARMS_POINTER(list);
	if (list->next->flag)
		return TRUE;
	return FALSE;
}
boolean list_head_free()
{
	struct list_head *p,*r;

	LINUXARMS_POINTER(list);
	for (p = list; p->next != list;) {
		r = p->next;
		p = r->next;
		free(r);
	}
	free(p);
	free(list);
	list = NULL;
	return TRUE;
}

void cb_fview_back_clicked(GtkButton *button, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	const char *path;
	debug_where();
	if ((path = list_head_get_path()) == NULL) {
		gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
		return;
	}
	debug_print("back path = %s\n", path);
	hfview_set_path(hfthread->hfview, path);
	hfthread_trans_set_path(&hfthread->trans, path);
	hfthread->set_protocol(hfthread, FVIEW);
	hfthread->send(hfthread);
	if (list_head_test_next() == FALSE)
		gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
	if (strlen(path) > 1)
		gtk_widget_set_sensitive(GTK_WIDGET(hfthread->hfview->widget.up), TRUE);
	else
		gtk_widget_set_sensitive(GTK_WIDGET(hfthread->hfview->widget.up), FALSE);
}


void cb_fview_up_clicked(GtkButton *button, gpointer user_data)
{
	struct linuxarms_struct *linuxarms = (struct linuxarms_struct *)user_data;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	char path[PATH_LEN];
	int i,len;
	debug_where();

	strcpy(path, hfview_get_path(hfthread->hfview));
	list_head_set_path(path);
	gtk_widget_set_sensitive(GTK_WIDGET(hfthread->hfview->widget.back), TRUE);
	len = strlen(path);
	if (len == 1) {
		gtk_widget_set_sensitive(GTK_WIDGET(hfthread->hfview->widget.up), FALSE);
back:
		hfview_set_path(hfthread->hfview, path);
		hfthread_trans_set_path(&hfthread->trans, (const char *)path);
		hfthread->set_protocol(hfthread, FVIEW);
		hfthread->send(hfthread);
		return;
	}
	for (i = len - 1; i >= 0 ; i--)
		if (path[i] == '/')
			break;
	if (i == 0) {
		gtk_widget_set_sensitive(GTK_WIDGET(hfthread->hfview->widget.up), FALSE);
		path[i + 1] = '\0';
	} else
		path[i] = '\0';
	goto back;
}


void cb_download_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();
	cb_fview_download_activate(NULL, user_data);
}


void cb_upload_clicked(GtkButton *button, gpointer user_data)
{
	debug_where();
	cb_fview_upload_activate(NULL, user_data);
}
