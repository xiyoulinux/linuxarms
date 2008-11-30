#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "sctrl.h"
#include "mwindow.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name)\
	g_object_set_data_full(G_OBJECT(component), name, \
	gtk_widget_ref(widget),(GDestroyNotify)gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name)\
	g_object_set_data(G_OBJECT(component), name, widget)

GtkWidget *create_ctrl_page(GtkWidget *vbox_main,
			    GtkTooltips *tooltips)
{
	GtkWidget *frame_main;
	GtkWidget *alignment_frame;
	GtkWidget *vbox_frame;
	GtkWidget *scrolledwindow_frame;
	GtkWidget *textview_ctrl;
	GtkWidget *entry_input;
	GtkWidget *hbox_input;
	GtkWidget *fixed6;
	GtkWidget *label_frame;

	frame_main = gtk_frame_new(NULL);
	gtk_widget_show(frame_main);
	gtk_box_pack_start(GTK_BOX(vbox_main), frame_main, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type(GTK_FRAME(frame_main), GTK_SHADOW_NONE);

	alignment_frame = gtk_alignment_new(0.5, 0.5, 1, 1);
	gtk_widget_show(alignment_frame);
	gtk_container_add(GTK_CONTAINER(frame_main), alignment_frame);
	gtk_alignment_set_padding(GTK_ALIGNMENT(alignment_frame), 0, 0, 12, 0);

	vbox_frame = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_frame);
	gtk_container_add(GTK_CONTAINER(alignment_frame), vbox_frame);

	scrolledwindow_frame = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow_frame);
	gtk_box_pack_start(GTK_BOX(vbox_frame), scrolledwindow_frame, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow_frame),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow_frame), GTK_SHADOW_IN);

	textview_ctrl = gtk_text_view_new();
	gtk_widget_show(textview_ctrl);
	gtk_container_add(GTK_CONTAINER(scrolledwindow_frame), textview_ctrl);
	GTK_WIDGET_UNSET_FLAGS(textview_ctrl, GTK_CAN_FOCUS);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_ctrl), FALSE);

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

	label_frame = gtk_label_new(_("<b>实时控制[root@EPC-8000:/root/$]</b>"));
	gtk_widget_show(label_frame);
	gtk_frame_set_label_widget(GTK_FRAME(frame_main), label_frame);
	gtk_label_set_use_markup(GTK_LABEL(label_frame), TRUE);
	g_signal_connect((gpointer)entry_input, "key_press_event",
			 G_CALLBACK(cb_ctrl_input_key_press),
			 NULL);

	GLADE_HOOKUP_OBJECT(window_main, frame_main, "frame_main");
	GLADE_HOOKUP_OBJECT(window_main, alignment_frame, "alignment_frame");
	GLADE_HOOKUP_OBJECT(window_main, vbox_frame, "vbox_frame");
	GLADE_HOOKUP_OBJECT(window_main, scrolledwindow_frame, "scrolledwindow_frame");
	GLADE_HOOKUP_OBJECT(window_main, textview_ctrl, "textview_ctrl");
	GLADE_HOOKUP_OBJECT(window_main, entry_input, "entry_input");
	GLADE_HOOKUP_OBJECT(window_main, label_frame, "label_frame");

	return NULL;
}
