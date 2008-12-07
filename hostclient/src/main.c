#include <gtk/gtk.h>
#include "login.h"
#include "support.h"
#include "mwindow.h"
#include "config.h"
#include "hsthread.h"
#include "hcthread.h"

int main(int argc, char *argv[])
{
	//GtkWidget *window_main;
	char *config_file;
	
	struct hsthread_struct hsthread;
	struct hsthread_timer timer;
	/***************hcthread_struct***************************************/
	struct hcthread_struct hcthread;
	gtk_set_locale();
	gtk_init(&argc, &argv);
	add_file_directory("/usr/share/linuxarms-hostclient/pixmaps");
	add_file_directory("./pixmaps");
	add_file_directory("../pixmaps");
	add_file_directory("/usr/share/linuxarms-hostclient/doc");
	add_file_directory("./doc");
	add_file_directory("../doc");
	add_file_directory("/etc/linuxarms-hostclient/config");
	add_file_directory("./config");
	add_file_directory("../config");

	config_file = find_file("port.conf");
	if (!config_init(config_file))
		return 1;
	//hsthread_set_timer_time(&hsthread, TM_THREE);
	timer.time = TM_THREE * 1000;
	timer.timer = -1;
	hsthread.timer = timer;
	window_main = create_window_main(&hsthread, &hcthread);
	//window_main = create_window_login();
	gtk_widget_show(window_main);
	hsthread_set_trans(&hsthread, SYSINFO, -1);
	hsthread_create_timer(&hsthread);
	gtk_main();
	return 0;
}

