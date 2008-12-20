#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "sprocess.h"
#include "mwindow.h"
#include "support.h"
#include "linuxarms.h"

GtkWidget *create_page_ssinfo(struct linuxarms_struct *linuxarms)
{
	GtkWidget *notebook_main = linuxarms->mwindow->notebook;
	GtkWidget *window_main = linuxarms->mwindow->window;
	GtkWidget *hbox_notebook;
	GtkWidget *image_sinfo;
	GtkWidget *vbox_sinfo;
	GtkWidget *label_hostname;
	GtkWidget *hseparator_one;
	GtkWidget *label_release;
	GtkWidget *label_kernel;
	GtkWidget *label_memory;
	GtkWidget *hseparator_two;
	GtkWidget *label_net;
	GtkWidget *hbox_net;
	GtkWidget *label_netrecv;
	GtkWidget *label_netsend;
	GtkWidget *hseparator_three;
	GtkWidget *label_cpuinfo;
	GtkWidget *label_cpuused;
	GtkWidget *label_cpuload;
	GtkWidget *hseparator_four;
	GtkWidget *label_disk;
	GtkWidget *label_ssinfo;
	GtkWidget *fixed_ssinfo;
	
	hbox_notebook = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox_notebook);
	gtk_container_add(GTK_CONTAINER(notebook_main), hbox_notebook);
	gtk_container_set_border_width(GTK_CONTAINER(hbox_notebook), 8);
	GTK_WIDGET_SET_FLAGS(hbox_notebook, GTK_CAN_FOCUS);
	
	image_sinfo = create_pixmap(window_main, "linuxarms.png");
	gtk_widget_show(image_sinfo);
	gtk_box_pack_start(GTK_BOX(hbox_notebook), image_sinfo, FALSE, TRUE, 0);
	gtk_widget_set_size_request(image_sinfo, 247, 230);

	fixed_ssinfo = gtk_fixed_new();
	gtk_widget_show(fixed_ssinfo);
	gtk_box_pack_start(GTK_BOX(hbox_notebook), fixed_ssinfo, FALSE, FALSE, 0);
	gtk_widget_set_size_request(fixed_ssinfo, 45, -1); 

	vbox_sinfo = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_sinfo);
	gtk_box_pack_start(GTK_BOX(hbox_notebook), vbox_sinfo, TRUE, TRUE, 0);

	label_hostname = gtk_label_new(_("<b>\344\270\273\346\234\272\345\220\215\357\274\232</b>"));
	gtk_widget_show(label_hostname);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_hostname, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_hostname, -1, 25);
	gtk_label_set_use_markup(GTK_LABEL(label_hostname), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label_hostname), 0, 0.5);

	hseparator_one = gtk_hseparator_new();
	gtk_widget_show(hseparator_one);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), hseparator_one, FALSE, TRUE, 0);

	label_release = gtk_label_new(_("<b>		\345\217\221\350"
				"\241\214\347\211\210\346\234\254	</b>"));
	gtk_widget_show(label_release);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_release, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_release, -1, 25);
	gtk_label_set_use_markup(GTK_LABEL(label_release), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label_release), 0, 0.5);

	label_kernel = gtk_label_new(_("<b>		\345\206\205\346\240"
				"\270\347\211\210\346\234\254 </b>"));
	gtk_widget_show(label_kernel);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_kernel, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_kernel, -1, 25);
	gtk_label_set_use_markup(GTK_LABEL(label_kernel), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label_kernel), 0, 0.5);

	label_memory = gtk_label_new(_(" \345\206\205\345\255\230\357\274"
			"\232\346\200\273\351\207\217(M) | \344\275\277\347\224\250(M)"));
	gtk_widget_show(label_memory);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_memory, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_memory, -1, 25);
	gtk_misc_set_alignment(GTK_MISC(label_memory), 0, 0.5);

	hseparator_two = gtk_hseparator_new();
	gtk_widget_show(hseparator_two);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), hseparator_two, FALSE, TRUE, 0);

	label_net = gtk_label_new(_("<b>\347\275\221\347\273\234\344\275\277\347\224\250</b>"));
	gtk_widget_show(label_net);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_net, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_net, -1, 25);
	gtk_label_set_use_markup(GTK_LABEL(label_net), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label_net), 0, 0.5);

	hbox_net = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox_net);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), hbox_net, FALSE, TRUE, 0);

	label_netrecv = gtk_label_new(_("\346\216\245\346\224\266: kb"));
	gtk_widget_show(label_netrecv);
	gtk_box_pack_start(GTK_BOX(hbox_net), label_netrecv, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_netrecv, 164, -1);
	gtk_misc_set_alignment(GTK_MISC(label_netrecv), 0.15, 0.5);

	label_netsend = gtk_label_new(_("\345\217\221\351\200\201: kb"));
	gtk_widget_show(label_netsend);
	gtk_box_pack_start(GTK_BOX(hbox_net), label_netsend, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_netsend, 163, 25);
	gtk_misc_set_alignment(GTK_MISC(label_netsend), 0, 0.5);

	hseparator_three = gtk_hseparator_new();
	gtk_widget_show(hseparator_three);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), hseparator_three, FALSE, TRUE, 0);

	label_cpuinfo = gtk_label_new(_("<b>CPU\345\236\213\345\217\267 </b>"));
	gtk_widget_show(label_cpuinfo);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_cpuinfo, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_cpuinfo, -1, 25);
	gtk_label_set_use_markup(GTK_LABEL(label_cpuinfo), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label_cpuinfo), 0, 0.5);

	label_cpuused = gtk_label_new(_("\344\275\277\347\224\250\347\216\207\357\274\232"));
	gtk_widget_show(label_cpuused);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_cpuused, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_cpuused, -1, 25);
	gtk_misc_set_alignment(GTK_MISC(label_cpuused), 0.03, 0.5);

	label_cpuload = gtk_label_new(_("CPU\350\264\237\350\275\275\357\274\232"));
	gtk_widget_show(label_cpuload);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_cpuload, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_cpuload, -1, 25);
	gtk_misc_set_alignment(GTK_MISC(label_cpuload), 0.03, 0.5);

	hseparator_four = gtk_hseparator_new();
	gtk_widget_show(hseparator_four);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), hseparator_four, FALSE, TRUE, 0);

	label_disk = gtk_label_new(_(
			"\347\241\254\347\233\230\357\274\232\346"
			"\200\273\345\256\271\351\207\217(M) | \344"
			"\275\277\347\224\250(M)"));
	gtk_widget_show(label_disk);
	gtk_box_pack_start(GTK_BOX(vbox_sinfo), label_disk, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_disk, -1, 25);
	gtk_misc_set_alignment(GTK_MISC(label_disk), 0, 0.5);

	label_ssinfo = gtk_label_new(_("系统信息"));
	gtk_widget_show(label_ssinfo);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook_main),
			gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook_main),
			0), label_ssinfo);
	GTK_WIDGET_SET_FLAGS(label_ssinfo, GTK_CAN_FOCUS);
	
	return NULL;
}
