//#define __DEBUG__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "sctrl.h"
#include "hcthread.h"
#include "mwindow.h"
#include "support.h"
#include "debug.h"
#include "error.h"

GtkWidget *create_ctrl_page(GtkWidget *vbox_main,
			    GtkTooltips *tooltips,
			    GtkWidget *frame_main,
			    struct hcthread_struct *hcthread)
{
	GtkWidget *alignment_frame;
	GtkWidget *vbox_frame;
	GtkWidget *scrolledwindow_frame;
	GtkAdjustment *adjustment;
	GtkWidget *textview_ctrl;
	GtkWidget *entry_input;
	GtkWidget *hbox_input;
	GtkWidget *fixed6;
	GtkWidget *label_frame;

	alignment_frame = gtk_alignment_new(0.5, 0.5, 1, 1);
	gtk_widget_show(alignment_frame);
	gtk_container_add(GTK_CONTAINER(frame_main), alignment_frame);
	gtk_alignment_set_padding(GTK_ALIGNMENT(alignment_frame), 0, 0, 12, 0);

	debug_where();
	vbox_frame = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_frame);
	gtk_container_add(GTK_CONTAINER(alignment_frame), vbox_frame);

	debug_where();
	scrolledwindow_frame = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow_frame);
	gtk_box_pack_start(GTK_BOX(vbox_frame), scrolledwindow_frame, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow_frame),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow_frame), 
						GTK_SHADOW_IN);

	adjustment = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, (double)G_MAXINT, 0.0, 0.0, 0.0));
	gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(scrolledwindow_frame),
			adjustment);
	debug_where();
	
	textview_ctrl = gtk_text_view_new();
	debug_where();
	gtk_widget_show(textview_ctrl);
	debug_where();
	gtk_container_add(GTK_CONTAINER(scrolledwindow_frame), textview_ctrl);
	debug_where();
	GTK_WIDGET_UNSET_FLAGS(textview_ctrl, GTK_CAN_FOCUS);
	debug_where();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_ctrl), FALSE);

	debug_where();
	hbox_input = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_input);
	gtk_box_pack_start (GTK_BOX (vbox_frame), hbox_input, FALSE, FALSE, 0);
  
	entry_input = gtk_entry_new();
	gtk_widget_show(entry_input);
	gtk_box_pack_start(GTK_BOX(hbox_input), entry_input, TRUE, TRUE, 0);
	gtk_tooltips_set_tip(tooltips, entry_input, 
			_("\350\276\223\345\205\245\345\221\275\344\273\244"), NULL);
	gtk_entry_set_max_length(GTK_ENTRY(entry_input), 256);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_input), 9679);
	
	fixed6 = gtk_fixed_new ();
	gtk_widget_show (fixed6);
	gtk_box_pack_start (GTK_BOX (hbox_input), fixed6, FALSE, FALSE, 0);
	gtk_widget_set_size_request (fixed6, 21, -1);

	label_frame = gtk_label_new(_("<b>实时控制[root@EPC-8000:/$]</b>"));
	gtk_widget_show(label_frame);
	gtk_frame_set_label_widget(GTK_FRAME(frame_main), label_frame);
	gtk_label_set_use_markup(GTK_LABEL(label_frame), TRUE);
	
	debug_where();
	hcthread->widget.textview_ctrl = textview_ctrl;
	debug_where();
	hcthread->widget.entry_input   = entry_input;
	hcthread->widget.label_path    = label_frame;
	hcthread->widget.adjustment    = adjustment;
	
	debug_where();
	g_signal_connect((gpointer)entry_input, "key_press_event",
			 G_CALLBACK(cb_ctrl_input_key_press),
			 (gpointer)hcthread);
	debug_where();
	return frame_main;
}
