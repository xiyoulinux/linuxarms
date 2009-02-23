#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "support.h"
#include "sprocess.h"
#include "error.h"

static GList *pixmaps_directories = NULL;

struct default_process_icon {
	const char *command;
	const char *icon;
};

/* The current table is only a test */
static const struct default_process_icon default_icon_table[] = {
/* "0", "gnome-run", */
	{"X", "gnome-mdi"},
	{"Xorg", "gnome-mdi"},

	{"aio"G_DIR_SEPARATOR_S"0", "gnome-run"},

	{"apache", "other"G_DIR_SEPARATOR_S"SMB-Server-alt"},
	{"apache2", "other"G_DIR_SEPARATOR_S"SMB-Server-alt"},

	{"atd", "gnome-set-time"},

	{"bash", "gnome-terminal"},

	{"cron", "gnome-set-time"},
	{"CRON", "gnome-set-time"},

	{"cupsd", "other"G_DIR_SEPARATOR_S"Printer"},
	{"cvsd",  "other"G_DIR_SEPARATOR_S"SMB-Server-alt"},
	{"dbus-daemon-1", "other"G_DIR_SEPARATOR_S"Reading"},

	{"emacs", "gnome-emacs"},
	{"emacsserver", "gnome-emacs"},
	{"emacs21", "gnome-emacs"},

	{"events"G_DIR_SEPARATOR_S"0", "other"G_DIR_SEPARATOR_S"Dialog-Warning5"},

	{"evolution-alarm-notify", "other"G_DIR_SEPARATOR_S"Button-Apps"},
	{"evolution-wombat", "other"G_DIR_SEPARATOR_S"Button-Apps"},

	{"exim4", "other"G_DIR_SEPARATOR_S"Envelope"},

	{"famd", "other"G_DIR_SEPARATOR_S"Find-Files2"},
	{"gam_server", "other"G_DIR_SEPARATOR_S "Find-Files2"},

	{"gconfd-2", "other"G_DIR_SEPARATOR_S"Control-Center2"},
	{"gdm", "gdm"},
	{"getty", "gksu-icon"},
	{"gnome-session", "gnome-logo-icon-transparent"},
	{"inetd", "other"G_DIR_SEPARATOR_S"Networking"},

	{"logger", "gnome-log"},

	{"kblockd"G_DIR_SEPARATOR_S"0", "other"G_DIR_SEPARATOR_S"Disks"},

	/* "khelper", "other"G_DIR_SEPARATOR_S "Help", */
	{"kirqd", "other"G_DIR_SEPARATOR_S"Connection-Ethernet"},
	{"klogd", "gnome-log"},

	{"ksoftirqd"G_DIR_SEPARATOR_S"0", "other"G_DIR_SEPARATOR_S"Connection-Ethernet"},

	/* "kswapd0", "other"G_DIR_SEPARATOR_S"Harddisk", */

	{"mc", "mc"},

	{"metacity", "metacity-properties"},

	{"migration"G_DIR_SEPARATOR_S"0" ,"other"G_DIR_SEPARATOR_S"Bird"},

	{"mysqld", "other"G_DIR_SEPARATOR_S"MySQL"},
	{"mutt", "mutt"},
	{"pdflush", "gnome-run"},
	{"portmap","other"G_DIR_SEPARATOR_S"Connection-Ethernet"},
	{"powernowd", "battstat"},

	{"pppoe", "other"G_DIR_SEPARATOR_S"Modem"},

	{"reiserfs"G_DIR_SEPARATOR_S"0","other"G_DIR_SEPARATOR_S"Disks"},

	{"sendmail", "other"G_DIR_SEPARATOR_S"Envelope"},
	{"setiathome", "other"G_DIR_SEPARATOR_S"WPicon2"},
	{"sh", "gnome-term"},
	{"squid", "other"G_DIR_SEPARATOR_S"Proxy-Config"},

	{"sshd", "ssh-askpass-gnome"},
	{"ssh", "ssh-askpass-gnome"},
	{"ssh-agent", "ssh-askpass-gnome"},

	{"syslogd", "gnome-log"},

	{"tail", "other"G_DIR_SEPARATOR_S"Tail"},

	{"top", "gnome-ccperiph"},

	{"xfs", "other"G_DIR_SEPARATOR_S"Font-Capplet"},
	{"xscreensaver", "xscreensaver"},
	{"xterm", "gnome-xterm"},

	{"vim", "vim"},
	{"gedit", "text-editor"},
	{"kedit", "kedit"},

	{"default", "klinkstatus"}
	/* GNOME applets */
};


void add_file_directory(const gchar *directory)
{
	/*
	if (access(directory, F_OK) == -1) {	
		print_error(EWARNING, "路径 %s 不存在!\n", directory);
		return;
	}
	*/
	pixmaps_directories = g_list_prepend(pixmaps_directories,
			g_strdup(directory));
}
/*
 * 查找一个文件
 * @filename:   要查找的文件名
 * @return:     如果没有找到，则返回NULL,否则返回文件所在路径
 */
gchar *find_file(const gchar *filename)
{
	GList *elem;

	elem = pixmaps_directories;
	while(elem){
		gchar *pathname = g_strdup_printf("%s%s%s",(gchar*)elem->data,
						 G_DIR_SEPARATOR_S, filename);
		if(g_file_test(pathname, G_FILE_TEST_EXISTS))
			return pathname;
		g_free(pathname);
		elem = elem->next;
	}
	return NULL;
}

GtkWidget* create_pixmap(GtkWidget *widget, const gchar *filename)
{
	gchar *pathname = NULL;
	GtkWidget *pixmap;

	if(!filename || !filename[0])
		return gtk_image_new();

	pathname = find_file(filename);

	if(!pathname){
		g_warning(_("Couldn't find pixmap file: %s"), filename);
		return gtk_image_new();
	}

	pixmap = gtk_image_new_from_file(pathname);
	g_free(pathname);
	return pixmap;
}

GdkPixbuf* create_pixbuf(const gchar *filename)
{
	gchar *pathname = NULL;
	GdkPixbuf *pixbuf;
	GError *error = NULL;

	if(!filename || !filename[0])
		return NULL;

	pathname = find_file(filename);

	if(!pathname){
		g_warning(_("Couldn't find pixmap file: %s"), filename);
		return NULL;
	}

	pixbuf = gdk_pixbuf_new_from_file_at_size(pathname, APP_ICON_SIZE, APP_ICON_SIZE, &error);
	if(!pixbuf){
		//fprintf(stderr, "Failed to load pixbuf file: %s: %s\n",
		//				 pathname, error->message);
		g_error_free(error);
	}
	g_free(pathname);
	return pixbuf;
}
/*
 * get 
 */
const char *get_default_icon_name(const char *cmd)
{
	int i;
	for (i = 0; i < G_N_ELEMENTS(default_icon_table) - 1; i++) {
		if (strstr(default_icon_table[i].command, cmd) != NULL)
			return default_icon_table[i].icon;
	}
	return default_icon_table[i].icon;
}

