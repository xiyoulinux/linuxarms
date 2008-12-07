#include <gtk/gtk.h>
#include "login.h"
#include "support.h"
#include "mwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
	GtkWidget *window_main;
	char *config_file;

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
	//window_main = create_window_main();
	window_main = create_window_login();
	gtk_widget_show(window_main);
	gtk_main();
	return 0;
}

