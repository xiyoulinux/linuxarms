#include "fileview.h"
#include "hfthread.h"
#include "linuxarms.h"
#include <gtk/gtk.h>
#include "debug.h"
#include "error.h"
#include "filetrans.h"
#include "sprocess.h"
/*
 * 初始化hfview_struct结构
 */
boolean hfview_init(struct hfview_struct *hfview,
		    struct hnet_struct *socket,
		    struct hfview_widget *widget)
{

	debug_where();
	if (!hfview) {
		print_error(EWARNING,"初始化数据结构为空");
		return FALSE;
	}
}

/*
 * 接收文件信息
 */
boolean hfview_recv_info(struct hfview_struct *hfview)
{
	return hnet_recv(hfview->socket->tcp, 
			(void *)&hfview->frecv, sizeof(struct hfview_recv));
}

/*
 * 发送反馈信息
 */
boolean hfview_send_info(struct hfview_struct *hfview)
{
}
/*
 * 显示进程信息
 */
boolean hfview_show_info(struct hfview_struct *hfview)
{
	/*gtk_list_store_clear*/
}

/*
 * 重命名文件
 */
boolean hfview_rename(struct hfthread_struct *hfthread)
{
}
/*
 * 删除文件
 */
boolean hfview_delete(struct hfthread_struct *hfthread)
{
}
/*
 * hfview主调函数
 */
boolean do_file_view(struct hfview_struct *hfview)
{
	/*
	 * 调用其他模块，完成文件浏览功能
	 */
}
void cb_fview_selection_changed(GtkWidget *widget, gpointer user_data)
{
}

gboolean cb_fview_button_press(GtkWidget *widget,
	                 GdkEventButton *event, gpointer user_data)
{
        if (event->button == BUTTON_RIGHT) {
		GtkWidget *popup_menu = create_popup_menu_fview();
		gtk_menu_popup (GTK_MENU(popup_menu),
                                NULL, NULL, NULL, NULL,
                                event->button, event->time);
	}
	if (event->button == BUTTON_LEFT) {
		printf("哦哦\n");
		statusbar_set_text("按下鼠标左键");
	}
	//printf("button %d\n",event->button);
	if (event->type == 5 && event->button == BUTTON_LEFT)
		printf("click button %d\n",event->type);

}	                 



