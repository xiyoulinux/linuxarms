#include <gtk/gtk.h>
#include "debug.h"
#include "about.h"
#include "help.h"
#include "filetrans.h"
#include "support.h"
#include "error.h"

void cb_login_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("cb_login_activate\n");
}


void cb_logout_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("cb_logout_activate");
}


void cb_restart_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_shutdown_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_quit_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_process_update_three_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_process_update_five_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}


void cb_process_kill_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	debug_where();
	debug_print("");

}

extern void cb_fview_upload_activate(GtkMenuItem *menuitem, gpointer user_data);

extern void cb_fview_downooad_activate(GtkMenuItem *menuitem, gpointer user_data);

extern void cb_fview_rename_activate(GtkMenuItem *menuitem, gpointer user_data);

extern void cb_fview_delete_activate(GtkMenuItem *menuitem, gpointer user_data);

void cb_help_topic_activate(GtkMenuItem *menuitem, gpointer user_data)
{
        GtkWidget *textview;
        GtkTextBuffer *buffer;
        gint i,len;
        gchar tmp[80],*p=NULL;
        FILE *fp_help;

        textview=(GtkWidget *)create_window_help();
	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
	p=find_file("helpmain");
	if(p== NULL) {
		gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer),
		                                "没有帮助文件",strlen("没有帮助文件"));
	} else {
		if((fp_help=fopen(p,"r"))==NULL) {
			print_error(EWARNING, "fopen");
			return;
		}   
		while(!feof(fp_help)) {
			fgets(tmp,80,fp_help);
			len=strlen(tmp);
			gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer),tmp,len);
			memset(tmp,'\0',len);
		}   
		fclose(fp_help);
	}   
	debug_where();
	debug_print("");

}


void cb_help_about_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget *window;
	window=(GtkWidget *)create_window_about();
	gtk_widget_show(window);
	debug_where();
	debug_print("");

}

