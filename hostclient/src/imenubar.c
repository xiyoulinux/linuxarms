#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "menubar.h"
#include "support.h"
#include "mwindow.h"
#include "debug.h"
#include "sprocess.h"
#include "hsthread.h"
#include "ssinfo.h"
#include "fileview.h"
#include "htthread.h"
#include "hmthread.h"
#include "linuxarms.h"
#include "hfthread.h"

GtkWidget *create_menubar(GtkWidget *vbox_main,
			  GtkTooltips *tooltips,
			  GtkAccelGroup *accel_group,
			  struct linuxarms_struct *linuxarms)
{
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hsprocess_struct *sprocess = hsthread->sprocess;
	struct hfview_struct *hfview = linuxarms->hfthread->hfview;
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;

	GtkWidget *menubar;
	GtkWidget *menubar_file;
	GtkWidget *menubar_file_menu;
	GtkWidget *login;
	GtkWidget *logout;
	GtkWidget *file_separator_one;
	GtkWidget *restart;
	GtkWidget *shutdown;
	GtkWidget *file_separator_two;
	GtkWidget *quit;
	GtkWidget *menubar_process;
	GtkWidget *menubar_process_menu;
	GSList *process_update_three_group = NULL;
	GtkWidget *process_update_three;
	GtkWidget *process_update_five;
	GtkWidget *separator_one;
	GtkWidget *process_kill;
	GtkWidget *image341;
	GtkWidget *menubar_fview;
	GtkWidget *menubar_fview_menu;
	GtkWidget *fview_rename;
	GtkWidget *image342;
	GtkWidget *fview_delete;
	GtkWidget *image343;
	GtkWidget *edit_separator_one;
	GtkWidget *fview_upload;
	GtkWidget *image344;
	GtkWidget *fview_download;
	GtkWidget *image345;
	GtkWidget *menubar_help;
	GtkWidget *menubar_help_menu;
	GtkWidget *help_topic;
	GtkWidget *image346;
	GtkWidget *help_separator_one;
	GtkWidget *help_about;
	GtkWidget *image347;

	GtkWidget *image309;
	GtkWidget *image310;
	GtkWidget *image311;
	GtkWidget *image312;
	GtkWidget *image313;

	/* 创建文件子菜单 */
	menubar = gtk_menu_bar_new();
	gtk_widget_show(menubar);
	gtk_box_pack_start(GTK_BOX(vbox_main), menubar, FALSE, FALSE, 0);

	menubar_file = gtk_menu_item_new_with_mnemonic(_("\346\226\207\344\273\266(_F)"));
	gtk_widget_show(menubar_file);
	gtk_container_add(GTK_CONTAINER(menubar), menubar_file);
	menubar_file_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menubar_file), menubar_file_menu);

	login = gtk_image_menu_item_new_with_mnemonic(_("\347\231\273\345\275\225arm"
			"\347\263\273\347\273\237"));
	gtk_widget_show(login);
	gtk_container_add(GTK_CONTAINER(menubar_file_menu), login);
	gtk_tooltips_set_tip(tooltips, login, 
			     _("\347\231\273\345\275\225arm\347\263\273\347\273\237"), NULL);

	image309 = gtk_image_new_from_stock("gtk-jump-to", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image309);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(login), image309);

	logout = gtk_image_menu_item_new_with_mnemonic(_("\346\263\250\351\224\200\347"
					"\224\250\346\210\267\347\231\273\345\275\225"));
	gtk_widget_show(logout);
	gtk_container_add(GTK_CONTAINER(menubar_file_menu), logout);
	gtk_tooltips_set_tip(tooltips, logout,_("\346\263\250\351\224\200\345\275\223"
					"\345\211\215\347\224\250\346\210\267"), NULL);

	image310 = gtk_image_new_from_stock("gtk-justify-left", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image310);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(logout), image310);

	file_separator_one = gtk_separator_menu_item_new();
	gtk_widget_show(file_separator_one);
	gtk_container_add(GTK_CONTAINER(menubar_file_menu), file_separator_one);
	gtk_widget_set_sensitive(file_separator_one, FALSE);

	restart = gtk_image_menu_item_new_with_mnemonic(_("\351\207\215\345\220\257arm"
			  "\347\263\273\347\273\237"));
	gtk_widget_show(restart);
	gtk_container_add(GTK_CONTAINER(menubar_file_menu), restart);

	image311 = gtk_image_new_from_stock("gtk-refresh", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image311);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(restart), image311);

	shutdown = gtk_image_menu_item_new_with_mnemonic(_("\345\205\263\346\257\225arm"
			   "\347\263\273\347\273\237"));
	gtk_widget_show(shutdown);
	gtk_container_add(GTK_CONTAINER(menubar_file_menu), shutdown);
	gtk_tooltips_set_tip(tooltips, shutdown, 
		_("\345\205\263\351\227\255arm\347\263\273\347\273\237"), NULL);

	image312 = gtk_image_new_from_stock("gtk-stop", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image312);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(shutdown), image312);

	file_separator_two = gtk_separator_menu_item_new();
	gtk_widget_show(file_separator_two);
	gtk_container_add(GTK_CONTAINER(menubar_file_menu), file_separator_two);
	gtk_widget_set_sensitive(file_separator_two, FALSE);

	quit = gtk_image_menu_item_new_with_mnemonic(_("\351\200\200\345\207\272"));
	gtk_widget_show(quit);
	gtk_container_add(GTK_CONTAINER(menubar_file_menu), quit);
	gtk_widget_add_accelerator(quit, "activate", accel_group,
			GDK_Q, (GdkModifierType)GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	image313 = gtk_image_new_from_stock("gtk-quit", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image313);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(quit), image313);
	 /* 创建进程子菜单 */
	menubar_process = gtk_menu_item_new_with_mnemonic(_("进程(_P)"));
	gtk_widget_show(menubar_process);
	gtk_container_add(GTK_CONTAINER(menubar), menubar_process);
	menubar_process_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menubar_process), menubar_process_menu);

	process_update_three = gtk_radio_menu_item_new_with_mnemonic(process_update_three_group, 
				_("\346\233\264\346\226\260(3\347\247\222)"));
	process_update_three_group = 
		gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(process_update_three));
	gtk_widget_show(process_update_three);
	gtk_container_add(GTK_CONTAINER(menubar_process_menu), process_update_three);
	gtk_tooltips_set_tip(tooltips, process_update_three, _("\346\257\217\351"
				"\232\2243\347\247\222\351\222\237\346"
				"\233\264\346\226\260\344\270\200\346\254\241"), NULL);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(process_update_three), TRUE);

	process_update_five = gtk_radio_menu_item_new_with_mnemonic(
				process_update_three_group, 
				_("\346\233\264\346\226\260(5\347\247\222)"));
	process_update_three_group = 
		gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(process_update_five));
	gtk_widget_show(process_update_five);
	gtk_container_add(GTK_CONTAINER(menubar_process_menu), process_update_five);
	gtk_tooltips_set_tip(tooltips, process_update_five,_("\346\257\217\351\232\224"
					"5\347\247\222\351\222\237\346\233\264\346\226"
					"\260\344\270\200\346\254\241"), NULL);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(process_update_five), FALSE);

	separator_one = gtk_separator_menu_item_new();
	gtk_widget_show(separator_one);
	gtk_container_add(GTK_CONTAINER(menubar_process_menu), separator_one);
	gtk_widget_set_sensitive(separator_one, FALSE);

	process_kill = gtk_image_menu_item_new_with_mnemonic(_("\346\235\200\346\255\273"));
	gtk_widget_show(process_kill);
	gtk_container_add(GTK_CONTAINER(menubar_process_menu), process_kill);
	gtk_tooltips_set_tip(tooltips, process_kill,
			_("\346\235\200\346\255\273\351\200\211\344\270"
			"\255\347\232\204\350\277\233\347\250\213"), NULL);
	gtk_widget_add_accelerator(process_kill, "activate", accel_group,
			GDK_K, (GdkModifierType)GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	image341 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image341);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(process_kill), image341);
	/* 创建浏览子菜单 */
	menubar_fview = gtk_menu_item_new_with_mnemonic(_("\346\265\217\350\247\210(_V)"));
	gtk_widget_show(menubar_fview);
	gtk_container_add(GTK_CONTAINER(menubar), menubar_fview);

	menubar_fview_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menubar_fview), menubar_fview_menu);

	fview_rename = gtk_image_menu_item_new_with_mnemonic(_("\351\207\215\345"
				"\221\275\345\220\215\346\226\207\344\273\266"));
	gtk_widget_show(fview_rename);
	gtk_container_add(GTK_CONTAINER(menubar_fview_menu), fview_rename);
	gtk_tooltips_set_tip(tooltips, fview_rename, _("\351\207\215\345\221\275\345\220"
			"\215\351\200\211\344\270\255\347\232\204\346\226\207\344\273\266"), NULL);
	gtk_widget_add_accelerator(fview_rename, "activate", accel_group,
			GDK_F2, (GdkModifierType)0, GTK_ACCEL_VISIBLE);

	image342 = gtk_image_new_from_stock("gtk-refresh", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image342);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(fview_rename), image342);

	fview_delete = gtk_image_menu_item_new_with_mnemonic(_(
			"\345\210\240\351\231\244\346\226\207\344\273\266"));
	gtk_widget_show(fview_delete);
	gtk_container_add(GTK_CONTAINER(menubar_fview_menu), fview_delete);
	gtk_tooltips_set_tip(tooltips, fview_delete, _(
			"\345\210\240\351\231\244\351\200\211"
			"\344\270\255\347\232\204\346\226\207\344\273\266"), NULL);

	image343 = gtk_image_new_from_stock("gtk-delete", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image343);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(fview_delete), image343);

	edit_separator_one = gtk_separator_menu_item_new();
	gtk_widget_show(edit_separator_one);
	gtk_container_add(GTK_CONTAINER(menubar_fview_menu), edit_separator_one);
	gtk_widget_set_sensitive(edit_separator_one, FALSE);

	fview_upload = gtk_image_menu_item_new_with_mnemonic(_(
				"\344\270\212\344\274\240\346\226\207\344\273\266"));
	gtk_widget_show(fview_upload);
	gtk_container_add(GTK_CONTAINER(menubar_fview_menu), fview_upload);
	gtk_tooltips_set_tip(tooltips, fview_upload, _("\344\273\216arm\347\263\273\347\273"
				"\237\344\270\212\344\274\240\346\226\207"
				"\344\273\266\345\210\260\346\234\254\346\234\272"), NULL);
	gtk_widget_add_accelerator(fview_upload, "activate", accel_group,
			GDK_U, (GdkModifierType)GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	image344 = gtk_image_new_from_stock("gtk-goto-top", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image344);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(fview_upload), image344);

	fview_download = gtk_image_menu_item_new_with_mnemonic(
			_("\344\270\213\350\275\275\346\226\207\344\273\266"));
	gtk_widget_show(fview_download);
	gtk_container_add(GTK_CONTAINER(menubar_fview_menu), fview_download);
	gtk_tooltips_set_tip(tooltips, fview_download,
			     _("\344\273\216\346\234\254\346\234\272\344"
			     "\270\213\350\275\275\346\226\207\344\273\266"
			     "\345\210\260arm\347\263\273\347\273\237\344\270\212"), NULL);
	gtk_widget_add_accelerator(fview_download, "activate", accel_group,
			GDK_D, (GdkModifierType)GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	image345 = gtk_image_new_from_stock("gtk-goto-bottom", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image345);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(fview_download), image345);
	/* 创建帮助子菜单 */
	menubar_help = gtk_menu_item_new_with_mnemonic(_("\345\270\256\345\212\251(_H)"));
	gtk_widget_show(menubar_help);
	gtk_container_add(GTK_CONTAINER(menubar), menubar_help);

	menubar_help_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menubar_help), menubar_help_menu);

	help_topic = gtk_image_menu_item_new_with_mnemonic(
			_("\345\270\256\345\212\251\344\270\273\351\242\230"));
	gtk_widget_show(help_topic);
	gtk_container_add(GTK_CONTAINER(menubar_help_menu), help_topic);
	gtk_tooltips_set_tip(tooltips, help_topic, 
			_("\345\270\256\345\212\251\346\226\207\346\241\243"), NULL);
	gtk_widget_add_accelerator(help_topic, "activate", accel_group,
			GDK_F1, (GdkModifierType)0, GTK_ACCEL_VISIBLE);

	image346 = gtk_image_new_from_stock("gtk-help", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image346);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(help_topic), image346);

	help_separator_one = gtk_separator_menu_item_new();
	gtk_widget_show(help_separator_one);
	gtk_container_add(GTK_CONTAINER(menubar_help_menu), help_separator_one);
	gtk_widget_set_sensitive(help_separator_one, FALSE);

	help_about = gtk_image_menu_item_new_with_mnemonic(_("\345\205\263\344\272\216"));
	gtk_widget_show(help_about);
	gtk_container_add(GTK_CONTAINER(menubar_help_menu), help_about);
	gtk_tooltips_set_tip(tooltips, help_about, _(
			"\345\205\263\344\272\216\346\234\254\350\275\257\344\273\266"), NULL);
	gtk_widget_add_accelerator(help_about, "activate", accel_group,
			GDK_A, (GdkModifierType)GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	image347 = gtk_image_new_from_stock("gtk-about", GTK_ICON_SIZE_MENU);
	gtk_widget_show(image347);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(help_about), image347);
	
	
	g_signal_connect((gpointer)login, "activate",
			 G_CALLBACK(cb_login_activate), NULL);
	g_signal_connect((gpointer)logout, "activate",
			 G_CALLBACK(cb_logout_activate), NULL);
	g_signal_connect((gpointer)restart, "activate",
			 G_CALLBACK(cb_restart_activate), NULL);
	g_signal_connect((gpointer)shutdown, "activate",
			 G_CALLBACK(cb_shutdown_activate), NULL);
	g_signal_connect((gpointer)quit, "activate",
			 G_CALLBACK(cb_quit_activate), NULL);
	g_signal_connect((gpointer)process_update_three, "activate",
			 G_CALLBACK(cb_process_update_three_activate), (gpointer)sprocess);
	g_signal_connect((gpointer)process_update_five, "activate",
			 G_CALLBACK(cb_process_update_five_activate), (gpointer)sprocess);
	g_signal_connect((gpointer)process_kill, "activate",
			 G_CALLBACK(cb_process_kill_activate), NULL);
	g_signal_connect((gpointer)fview_rename, "activate",
			 G_CALLBACK(cb_fview_rename_activate), NULL);
	g_signal_connect((gpointer)fview_delete, "activate",
			 G_CALLBACK(cb_fview_delete_activate), NULL);
	g_signal_connect((gpointer)fview_upload, "activate", 
			 G_CALLBACK(cb_fview_upload_activate), NULL);
	g_signal_connect((gpointer)fview_download, "activate",
			 G_CALLBACK(cb_fview_download_activate), NULL);
	g_signal_connect((gpointer)help_topic, "activate",
			 G_CALLBACK(cb_help_topic_activate), NULL);
	g_signal_connect((gpointer)help_about, "activate",
			 G_CALLBACK(cb_help_about_activate), NULL);  
	
	hmthread->widget.login = login;
	hmthread->widget.logout = logout;
	hmthread->widget.restart = restart;
	hmthread->widget.shutdown = shutdown;

	sprocess->widget.menu_kill = process_kill;
	sprocess->widget.menu_three = process_update_three;
	sprocess->widget.menu_five = process_update_five;

	hfview->widget.rename = fview_rename;
	hfview->widget.del = fview_delete;
	htthread->widget.menubar_upload = fview_upload;
	htthread->widget.menubar_download = fview_download;

	return menubar;
}
