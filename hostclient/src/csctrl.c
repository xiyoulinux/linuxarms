#define _DEBUG__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "sctrl.h"
#include "debug.h"
#include "hcthread.h"
#include "statusbar.h"
void handle_command(char *command)
{
	/* 处理某些限制命令 */
	debug_where();
	if (strstr(command, "exit")) memset(command, '\0', strlen(command));
}
gboolean cb_ctrl_input_key_press(GtkWidget *widget,
			GdkEventKey *event, gpointer user_data)
{
	debug_where();
	struct hcthread_struct *hcthread = 
		(struct hcthread_struct *)user_data;
	const gchar *input;
	if (event->keyval == GDK_Return) {
		char command[256];
		input = gtk_entry_get_text(GTK_ENTRY(hcthread->widget.entry_input));
		strcpy(command, input);
		handle_command(command);
		if (command[0] == '\0') {
			debug_print("sorry~your command be limited\n");
			/* 显示到状态提示栏 */
			statusbar_set_text("sorry~!your command be limited");
			gtk_entry_set_text(GTK_ENTRY(hcthread->widget.entry_input), "");
			return FALSE;
		}
		gtk_widget_set_sensitive(hcthread->widget.entry_input, FALSE);
		debug_print("it is enter:%s\n",input);
		hcthread_trans_set_protocol(&hcthread->trans, CSEND);
		hcthread_trans_set_buf(&hcthread->trans, input);
		hcthread->send(hcthread);
		return TRUE;
	}
	return FALSE;
}
