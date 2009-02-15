#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "support.h"
#include "about.h"

void cb_window_destroy(GtkObject *object, gpointer user_data)
{
	gtk_widget_destroy(GTK_WIDGET(object));
}

void cb_button_close_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *window = (GtkWidget *)user_data;
	gtk_widget_destroy(GTK_WIDGET(window));
}


void cb_button_link_clicked(GtkButton *button, gpointer user_data)
{
	gchar tmp[80];
	g_snprintf(tmp, 80, "firefox http://xiyoulinux.cn & >/dev/null");
	if (system(tmp) == -1)
		g_print("execute command error\n");
}


void cb_button_credits_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *window;
	window = (GtkWidget *)create_window_credits();
	gtk_widget_show(window);

}

gboolean gtk_textview_set_text(GtkWidget *textview, const gchar *text)
{
	GtkTextBuffer *buffer;
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));	
	gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), 
					 text, strlen(text));   
	return TRUE;
}


void cb_button_licence_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *textview;
	GtkTextBuffer *buffer;
	gint len;
	gchar tmp[80], *p = NULL;
	FILE *fp_help;
	textview = (GtkWidget *)create_window_licence();
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));	
	p = find_file((const gchar *)LICENCE_FILE);
	if (p == NULL) {
		gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), 
					"没有许可文件", strlen("没有许可文件"));
	} else {
		if ((fp_help  =  fopen(p, "r")) == NULL) {
			perror("fopen");
			return;
		}
		while(!feof(fp_help)) {
			p = fgets(tmp, 80, fp_help);
			len = strlen(tmp);
			gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), tmp, len);
			memset(tmp, '\0', len);
		}
		fclose(fp_help);
	}
}

