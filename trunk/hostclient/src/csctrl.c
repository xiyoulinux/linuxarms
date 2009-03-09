#define __DEBUG__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <unistd.h>
#include <string.h>
#include "sctrl.h"
#include "debug.h"
#include "hcthread.h"
#include "statusbar.h"
boolean handle_command(struct hcthread_struct *hcthread)
{
	/* 处理某些限制命令 */
	debug_where();
	char *command = hcthread->trans.buffer;
	
	if (strstr(command, "exit")) {
		debug_print("sorry~your command be limited\n");
		/* 显示到状态提示栏 */
		statusbar_set_text("sorry~!your command be limited");
		gtk_entry_set_text(GTK_ENTRY(hcthread->widget.entry_input), "");
		return FALSE;
	}
	if (strstr(command, "cd ./") || strstr(command, "cd .")) {
		gtk_entry_set_text(GTK_ENTRY(hcthread->widget.entry_input), "");
		return FALSE;
	}
	if (strstr(command, "clear")) {
		GtkTextBuffer *gtkbuffer;
		GtkTextIter start, end;
		
		gtkbuffer = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(hcthread->widget.textview_ctrl));
		gtk_text_view_set_buffer(GTK_TEXT_VIEW
				(hcthread->widget.textview_ctrl), gtkbuffer);
		gtk_text_buffer_get_bounds (gtkbuffer, &start, &end);
		gtk_text_buffer_delete (gtkbuffer, &start, &end);
		gtk_entry_set_text(GTK_ENTRY(hcthread->widget.entry_input), "");
		return FALSE;	
	}

	return TRUE;
}
gboolean cb_ctrl_input_key_press(GtkWidget *widget,
			GdkEventKey *event, gpointer user_data)
{
	debug_where();
	struct hcthread_struct *hcthread = 
		(struct hcthread_struct *)user_data;
	const gchar *input;
	const gchar *label;
	char info[128];

	if (event->keyval == GDK_Return) {
		input = gtk_entry_get_text(GTK_ENTRY(hcthread->widget.entry_input));
		hcthread_trans_set_buf(&hcthread->trans, input);
		
		if (! handle_command(hcthread))
			return FALSE;

		gtk_widget_set_sensitive(hcthread->widget.entry_input, FALSE);
		debug_print("it is enter:%s\n",input);
		hcthread_trans_set_protocol(&hcthread->trans, CSEND);
		hcthread->send(hcthread);

		label = gtk_label_get_text(GTK_LABEL(hcthread->widget.label_path));
		sprintf(info, "%s %s\n", label, input);
		hcthread_trans_set_buf(&hcthread->trans, info);
		showinfo(hcthread);
		return TRUE;
	}
	return FALSE;
}
