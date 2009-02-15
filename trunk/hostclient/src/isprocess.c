#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib-object.h>

#include "sprocess.h"
#include "mwindow.h"
#include "support.h"
#include "hsthread.h"
#include "linuxarms.h"
#include "hmthread.h"

 struct process_info {
	GType type;
	char stype[20];
	char name[20];
};

struct process_info columns[ ] = {
	{G_TYPE_STRING , "", ""},
	{G_TYPE_STRING,  "text", "进程名"},
	{G_TYPE_LONG,  "text", "进程号"},
	{G_TYPE_STRING,  "text", "用户"},
	{G_TYPE_STRING,  "text", "进程状态"},
	{G_TYPE_STRING,  "text", "CPU使用率"},
	{G_TYPE_STRING,  "text", "内存使用"}
};

GtkListStore  *create_page_sprocess(struct linuxarms_struct *linuxarms)
{
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hsprocess_struct *hsprocess = hsthread->hsprocess;
	GtkWidget *notebook_main = linuxarms->mwindow->notebook;
	GtkWidget *label_sprocess;
	GtkWidget *scrolledwindow_process;
	GtkWidget *treeview_process;
	GtkCellRenderer *cell_renderer;
	GtkListStore *list_store;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection;

	int i;

	/* 创建滚动条 */
	scrolledwindow_process = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow_process);
	gtk_container_add(GTK_CONTAINER(notebook_main), scrolledwindow_process);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow_process),
				       GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow_process),
					    GTK_SHADOW_IN);
	list_store = gtk_list_store_new(PROCESS_COLUMNS + 1,
					GDK_TYPE_PIXBUF,                       
					columns[COL_SNAME].type, /*进程名*/        
					columns[COL_SID].type,  /* 进程号 */            
					columns[COL_SUSER].type,  /* 用户 */           
					columns[COL_SSTATE].type,  /* 进程状态 */ 
					columns[COL_SCPU].type,  /* CPU使用率 */  
					columns[COL_SMEM].type  /* 内存使用 */ 
					);
	treeview_process = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
	gtk_widget_show(treeview_process);
	gtk_container_add(GTK_CONTAINER(scrolledwindow_process), treeview_process);
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview_process), TRUE);
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(treeview_process), TRUE);
	
	/* 创建进程名列 */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, columns[COL_SNAME].name);
	gtk_tree_view_column_set_sort_column_id(column, COL_SNAME);
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	//gtk_tree_view_column_set_fixed_width(column, 80);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_process), column);
	
	/* 进程图标 */
	cell_renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, cell_renderer, TRUE);
	gtk_tree_view_column_set_attributes(column, cell_renderer, 
					    "pixbuf",COL_SPIXBUF ,NULL);

	/* 显示进程名 */
	cell_renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, cell_renderer, TRUE);
	gtk_tree_view_column_set_attributes(column, cell_renderer,
				columns[COL_SNAME].stype, COL_SNAME, NULL);

		
	for(i = COL_SNAME + 1; i <= PROCESS_COLUMNS; i++) {
		column = gtk_tree_view_column_new();
		gtk_tree_view_column_set_title(column, columns[i].name);
		gtk_tree_view_column_set_sort_column_id(column, i);
		gtk_tree_view_column_set_resizable(column, TRUE);
		gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
		//gtk_tree_view_column_set_fixed_width(column, 80);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_process), column);
	
		cell_renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_column_pack_start(column, cell_renderer, TRUE);
		gtk_tree_view_column_set_attributes(column,cell_renderer,
						    columns[i].stype, i,NULL);
	}
	
	label_sprocess = gtk_label_new(_("进程信息"));
	gtk_widget_show(label_sprocess);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook_main), 
		   gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook_main), 1), label_sprocess);
	GTK_WIDGET_SET_FLAGS(label_sprocess, GTK_CAN_FOCUS);
	
	g_signal_connect(G_OBJECT(treeview_process), "button_press_event",
                         G_CALLBACK(cb_process_button_press), 
			 (gpointer)linuxarms);

	hsprocess->widget.treeview = treeview_process;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_process));
  	g_signal_connect(selection, "changed", 
		         G_CALLBACK(cb_process_selection_changed), (gpointer)linuxarms);
	return list_store;
}

GtkWidget *create_popup_menu_process(struct linuxarms_struct *linuxarms)
{
	GtkWidget *popup_menu_process;
	GtkWidget *spopup_eparator;
	GtkWidget *popup_process_kill;
	GtkWidget *image209;
	GtkTooltips *tooltips;

	tooltips = gtk_tooltips_new();

	popup_menu_process = gtk_menu_new();
	gtk_widget_show(popup_menu_process);
	spopup_eparator = gtk_separator_menu_item_new();
	gtk_widget_show(spopup_eparator);
	gtk_container_add(GTK_CONTAINER(popup_menu_process), spopup_eparator);
	gtk_widget_set_sensitive(spopup_eparator, FALSE);

	popup_process_kill = gtk_image_menu_item_new_with_mnemonic(
			     _("\346\235\200\346\255\273\350\277\233\347\250\213"));
	gtk_widget_show(popup_process_kill);
	gtk_container_add(GTK_CONTAINER(popup_menu_process), popup_process_kill);
	gtk_tooltips_set_tip(tooltips, popup_process_kill,
		_("\346\235\200\346\255\273\350\257\245\350\277\233\347\250\213"), NULL);

	image209 = gtk_image_new_from_stock("gtk-close", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image209);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(popup_process_kill), image209);

	linuxarms->hsthread->hsprocess->widget.popup_kill = popup_process_kill;
	linuxarms->hsthread->hsprocess->widget.popup_menu = TRUE;
	g_signal_connect((gpointer) popup_process_kill, "activate",
			 G_CALLBACK(cb_process_kill_activate), 
			 (gpointer)linuxarms);

	return popup_menu_process;
}
