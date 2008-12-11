#include <gtk/gtk.h>
#include "login.h"
#include "support.h"
#include "mwindow.h"
#include "config.h"
#include "hmthread.h"
#include "hsthread.h"
#include "hcthread.h"
#include "sprocess.h"
#include "hfthread.h"
#include "htthread.h"
#include "fileview.h"

int main(int argc, char *argv[])
{
	//GtkWidget *window_main;
	char *config_file;
	struct main_struct hmain;

	struct hsthread_struct hsthread;
	struct hsthread_timer timer;
	struct hsprocess_struct sprocess;
	/***************hcthread_struct****************************/
	struct hcthread_struct hcthread;
	/***************hfthread_struct****************************/
	struct hfthread_struct hfthread;
	struct hfview_struct hfview;
	struct htthread_struct hftrans;
	/***************hmthread_struct*******************************/
	struct hmthread_struct hmthread;

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
	/***************init hsthread**************************/
	timer.time = TM_THREE * 1000;
	timer.timer = -1;
	hsthread.timer = timer;

	hsprocess_struct_init(&sprocess);
	hsthread.sprocess = &sprocess;
	
	/***************init hfthread**************************/
	hfthread.hfview = &hfview;
	hfthread.hftrans = &hftrans;

	hmain.hmthread = &hmthread;
	hmain.hsthread = &hsthread;
	hmain.hcthread = &hcthread;
	hmain.hfthread = &hfthread;
	
	window_main = create_window_main(&hmain);
	gtk_window_main_set_sensitive(&hmain);

	//window_main = create_window_login();
	gtk_widget_show(window_main);
	hsthread_set_trans(&hsthread, SYSINFO, -1);
	hsthread_create_timer(&hsthread);
	gtk_main();
	return 0;
}

