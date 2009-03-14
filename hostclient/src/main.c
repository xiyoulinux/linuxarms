//#define __DEBUG__
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <locale.h>
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

int main(int argc, char *argv[])
{
	struct linuxarms_struct linuxarms;
	struct mwindow_struct mwindow;
	struct login_struct login;
	struct hsthread_struct hsthread;
	struct hsprocess_struct hsprocess;
	struct hssinfo_struct hssinfo;
	struct hcthread_struct hcthread;
	struct hfthread_struct hfthread;
	struct hfview_struct hfview;
	struct htthread_struct hftrans;
	struct hmthread_struct hmthread;

	if (!g_thread_supported())
		g_thread_init(NULL);
	gtk_set_locale();
	gtk_init(&argc, &argv);

	hsthread_init(&hsthread, &hssinfo, &hsprocess);
	hfthread_init(&hfthread, &hfview, &hftrans);
	linuxarms.login    = &login;
	linuxarms.mwindow  = &mwindow;
	linuxarms.hmthread = &hmthread;
	linuxarms.hsthread = &hsthread;
	linuxarms.hcthread = &hcthread;
	linuxarms.hfthread = &hfthread;
	hostclient_init(&linuxarms);

	linuxarms_print("create login window...\n");
	create_window_login(&linuxarms);
	hmthread.timer = gtk_timeout_add(50, create_window_main_timeout, (gpointer)&linuxarms);

	gtk_main();
	return 0;
}
void hostclient_init(struct linuxarms_struct *linuxarms)
{
	struct hmthread_struct *hmthread = linuxarms->hmthread;
	struct hsthread_struct *hsthread = linuxarms->hsthread;
	struct hfthread_struct *hfthread = linuxarms->hfthread;
	struct hcthread_struct *hcthread = linuxarms->hcthread;
	struct login_struct *login = linuxarms->login;

	setlocale (LC_ALL, "");
	add_file_directory("/usr/share/linuxarms-hostclient/pixmaps");
	add_file_directory("/usr/share/linuxarms-hostclient/doc");
	add_file_directory("/etc/linuxarms-hostclient/config");
	add_file_directory("/etc/linuxarms-hostclient/config");
#ifdef _DEBUG_
	add_file_directory("./pixmaps");
	add_file_directory("../pixmaps");
	add_file_directory("./doc");
	add_file_directory("../doc");
	add_file_directory("./config");
	add_file_directory("../config");
#endif
	if (!config_init(find_file("port.conf"))) {
		linuxarms_print("read config file \"port.conf\" error...\n");
		exit(0);
	}
	login_init(login, &hmthread->socket);
	hmthread_init(hmthread, &login->user);
	hsprocess_init(hsthread->hsprocess, &hsthread->trans.kill, &hsthread->socket);
	hssinfo_init(hsthread->hssinfo, &hsthread->socket);
	hfview_init(hfthread->hfview, hfthread->trans.path, &hfthread->socket);
	htthread_init(hfthread->hftrans);
	hcthread_init(hcthread);
}
