#include <gtk/gtk.h>
#include "linuxarms.h"
#include "login.h"
#include "support.h"
#include "mwindow.h"
#include "config.h"
#include "hmthread.h"
#include "hsthread.h"
#include "hcthread.h"
#include "sprocess.h"
#include "ssinfo.h"
#include "hfthread.h"
#include "htthread.h"
#include "fview.h"
#include "debug.h"
#include "thread.h"
#ifdef GTK_THREAD
#include <glib.h>
#endif
#define _DEBUG_
//#define _CREATE_WINDOW_

int main(int argc, char *argv[])
{
	char *config_file;
	struct linuxarms_struct linuxarms;
	struct mwindow_struct mwindow;
	/*************login_struct********************************/
	struct login_struct login;
	struct user_struct user;
	struct login_config_struct login_config;
	/***************hsthread_struct***************************/
	struct hsthread_struct hsthread;
	struct hsprocess_struct hsprocess;
	struct hssinfo_struct hssinfo;
	/***************hcthread_struct***************************/
	struct hcthread_struct hcthread;
	/***************hfthread_struct***************************/
	struct hfthread_struct hfthread;
	struct hfview_struct hfview;
	struct htthread_struct hftrans;
	/***************hmthread_struct***************************/
	struct hmthread_struct hmthread;

#ifdef GTK_THREAD
	if (!g_thread_supported())
		g_thread_init(NULL);
	//gdk_threads_init();
#endif
	gtk_set_locale();
	gtk_init(&argc, &argv);
	add_file_directory("/usr/share/linuxarms-hostclient/pixmaps");
	add_file_directory("/usr/share/linuxarms-hostclient/doc");
	add_file_directory("/etc/linuxarms-hostclient/config");
#ifdef _DEBUG_
	add_file_directory("./pixmaps");
	add_file_directory("../pixmaps");
	add_file_directory("./doc");
	add_file_directory("../doc");
	add_file_directory("./config");
	add_file_directory("../config");
#endif

	config_file = find_file("port.conf");
	if (!config_init(config_file))
		return 1;
	/***************init_login*****************************/
	user_struct_init(&user);
	login_config_init(&login_config);
	login_config_read(&login_config);
	login_init(&login, &user, &login_config, &hmthread.socket);

	/***************init_hmthread**************************/
	hmthread_init(&hmthread, &user);
	/***************init hsthread**************************/
	hsprocess_init(&hsprocess, &hsthread.trans.kill, &hsthread.socket);
	/*对系统信息显示部分进行初始化*/
	hssinfo_init(&hssinfo, &hsthread.socket);
	hsthread_init(&hsthread, &hssinfo, &hsprocess);
	/***************init hfthread**************************/
	hfview_init(&hfview, hfthread.trans.path, &hfthread.socket);
	hfthread_init(&hfthread, &hfview, &hftrans);
	/**************init hcthread**************************/
	hcthread_init(&hcthread);

	linuxarms.login = &login;
	linuxarms.mwindow = &mwindow;
	linuxarms.hmthread = &hmthread;
	linuxarms.hsthread = &hsthread;
	linuxarms.hcthread = &hcthread;
	linuxarms.hfthread = &hfthread;
#ifdef _CREATE_WINDOW_
	create_window_main(&linuxarms);
	gtk_window_main_set_sensitive(&linuxarms);
	hsthread.set_protocol(&hsthread, SSYSINFO);
	hsthread_create_timer(&hsthread);
#else
	linuxarms_print("create login window...\n");
	create_window_login(&linuxarms);
	gtk_timeout_add(50, create_window_main_timeout, (gpointer)&linuxarms);
#endif
	gtk_main();
	return 0;
}

