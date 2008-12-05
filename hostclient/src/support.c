#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gtk/gtk.h>
#include "support.h"

static GList *pixmaps_directories = NULL;

void add_file_directory(const gchar *directory)
{
	pixmaps_directories = g_list_prepend(pixmaps_directories,
			g_strdup(directory));
}
/*
 * 查找一个文件
 * @filename:   要查找的文件名
 * @return:     如果没有找到，则返回NULL,否则返回文件所在路径
 */
gchar *find_file(const gchar *filename)
{
	GList *elem;

	elem = pixmaps_directories;
	while(elem){
		gchar *pathname = g_strdup_printf("%s%s%s",(gchar*)elem->data,
						 G_DIR_SEPARATOR_S, filename);
		if(g_file_test(pathname, G_FILE_TEST_EXISTS))
			return pathname;
		g_free(pathname);
		elem = elem->next;
	}
	return NULL;
}

GtkWidget* create_pixmap(GtkWidget *widget, const gchar *filename)
{
	gchar *pathname = NULL;
	GtkWidget *pixmap;

	if(!filename || !filename[0])
		return gtk_image_new();

	pathname = find_file(filename);

	if(!pathname){
		g_warning(_("Couldn't find pixmap file: %s"), filename);
		return gtk_image_new();
	}

	pixmap = gtk_image_new_from_file(pathname);
	g_free(pathname);
	return pixmap;
}

GdkPixbuf* create_pixbuf(const gchar *filename)
{
	gchar *pathname = NULL;
	GdkPixbuf *pixbuf;
	GError *error = NULL;

	if(!filename || !filename[0])
		return NULL;

	pathname = find_file(filename);

	if(!pathname){
		g_warning(_("Couldn't find pixmap file: %s"), filename);
		return NULL;
	}

	pixbuf = gdk_pixbuf_new_from_file(pathname, &error);
	if(!pixbuf){
		fprintf(stderr, "Failed to load pixbuf file: %s: %s\n",
						 pathname, error->message);
		g_error_free(error);
	}
	g_free(pathname);
	return pixbuf;
}

void glade_set_atk_action_description(AtkAction *action,
		const gchar *action_name, const gchar *description)
{
	gint n_actions, i;

	n_actions = atk_action_get_n_actions(action);
	for(i = 0; i < n_actions; i++){
		if(!strcmp(atk_action_get_name(action, i), action_name))
			atk_action_set_description(action, i, description);
	}
}

