#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "support.h"
#include "statusbar.h"
#include "mwindow.h"
#include "linuxarms.h"
#include "hfthread.h"
#include "htthread.h"

GtkWidget *create_statusbar(GtkWidget *vbox_main, GtkTooltips *tooltips, struct linuxarms_struct *linuxarms)
{
	struct htthread_struct *htthread = linuxarms->hfthread->hftrans;
	GtkWidget *hbox18;
	GtkWidget *statusbar;
	GtkWidget *label_trans;
	GtkWidget *progressbar;
	GtkWidget *fixed13;	
	
	hbox18 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox18);
	gtk_box_pack_start(GTK_BOX(vbox_main), hbox18, FALSE, FALSE, 0);

	statusbar = gtk_statusbar_new();
	gtk_widget_show(statusbar);
	gtk_box_pack_start(GTK_BOX(hbox18), statusbar, TRUE, TRUE, 0);

	label_trans = gtk_label_new("");
	gtk_widget_show(label_trans);
	gtk_box_pack_start(GTK_BOX(hbox18), label_trans, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_trans, 80, -1);

	progressbar = gtk_progress_bar_new();
	gtk_widget_show(progressbar);
	gtk_box_pack_start(GTK_BOX(hbox18), progressbar, FALSE, FALSE, 0);
	gtk_widget_set_size_request(progressbar, 280, -1);
	gtk_tooltips_set_tip(tooltips, progressbar, _("文件传输进度条"), NULL);

	fixed13 = gtk_fixed_new();
	gtk_widget_show(fixed13);
	gtk_box_pack_start(GTK_BOX(hbox18), fixed13, FALSE, FALSE, 0);
	gtk_widget_set_size_request(fixed13, 24, -1);
	 
	htthread->widget.progressbar = progressbar;
	htthread->widget.tooltips = tooltips;
	htthread->widget.label_trans = label_trans;
	statusbar_init(statusbar);
	return statusbar;
}
