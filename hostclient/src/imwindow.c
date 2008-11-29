#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "mwindow.h"
#include "support.h"
#include "debug.h"
#include "menubar.h"
#include "toolbar.h"
#include "sprocess.h"
#include "fileview.h"
#include "sctrl.h"
#include "statusbar.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name)\
	g_object_set_data_full(G_OBJECT(component), name, \
	gtk_widget_ref(widget),(GDestroyNotify)gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name)\
	g_object_set_data(G_OBJECT(component), name, widget)

GtkWidget *window_main;
/*
 * 创建主窗口
 */
GtkWidget *create_window_main()
{
	
	GtkWidget *vbox_main;
	GtkWidget *menubar;
	GtkWidget *toolbar;
	GtkWidget *notebook_main;
	GtkWidget *sprocess;
	GtkAccelGroup *accel_group;
	GtkTooltips *tooltips;
	
	GtkListStore *list_store_process;
	GtkListStore *list_store_fview;

	tooltips = gtk_tooltips_new();

	accel_group = gtk_accel_group_new();
	window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_main, 800, 650);
	GTK_WIDGET_SET_FLAGS(window_main, GTK_CAN_FOCUS);
	gtk_window_set_title(GTK_WINDOW(window_main), _("Linux ARMS"));
	gtk_window_set_position(GTK_WINDOW(window_main), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window_main), 800, 650);
	
	vbox_main = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_main);
	gtk_container_add(GTK_CONTAINER(window_main), vbox_main);

	menubar = (GtkWidget *)create_menubar(vbox_main, tooltips,accel_group);
	toolbar = (GtkWidget *)create_toolbar(vbox_main,tooltips);

	notebook_main = gtk_notebook_new();
	gtk_widget_show(notebook_main);
	gtk_box_pack_start(GTK_BOX(vbox_main), notebook_main, TRUE, TRUE, 0);
	/* 创建系统信息显示界面 */
	create_page_ssinfo(notebook_main);
	/* 创建进程信息显示界面 */
	list_store_process = (GtkListStore *)create_page_sprocess(notebook_main);
	/* 创建文件浏览界面 */
	list_store_fview = (GtkListStore *)create_page_fview(notebook_main);
	/* 创建实时控制界面 */
	create_ctrl_page(vbox_main, tooltips);
	create_statusbar(vbox_main);

	g_signal_connect((gpointer)window_main, "destroy", 
			 G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect((gpointer)notebook_main, "switch_page", 
			 G_CALLBACK(cb_notebook_switch_page), NULL);
	
	GLADE_HOOKUP_OBJECT_NO_REF(window_main, window_main, "window_main");
	GLADE_HOOKUP_OBJECT(window_main, vbox_main, "vbox_main");
	GLADE_HOOKUP_OBJECT(window_main, notebook_main, "notebook_main");
	GLADE_HOOKUP_OBJECT_NO_REF(window_main, tooltips, "tooltips");
			 
	gtk_widget_grab_focus(notebook_main);
	gtk_window_add_accel_group(GTK_WINDOW(window_main), accel_group); 

	return (GtkWidget *)window_main;
}
