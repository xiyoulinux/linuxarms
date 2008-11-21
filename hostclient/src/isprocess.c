#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "sprocess.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)
GtkWidget *create_page_sprocess()
{
	GtkWidget *scrolledwindow_process;
  GtkWidget *clist_process;
  GtkWidget *label_name;
  GtkWidget *label_pid;
  GtkWidget *label_status;
  GtkWidget *label_cpu;
  GtkWidget *label_mem;
  GtkWidget *label_sprocess;
scrolledwindow_process = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow_process);
  gtk_container_add (GTK_CONTAINER (notebook_main), scrolledwindow_process);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow_process), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow_process), GTK_SHADOW_IN);

  clist_process = gtk_clist_new (5);
  gtk_widget_show (clist_process);
  gtk_container_add (GTK_CONTAINER (scrolledwindow_process), clist_process);
  gtk_clist_set_column_width (GTK_CLIST (clist_process), 0, 196);
  gtk_clist_set_column_width (GTK_CLIST (clist_process), 1, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist_process), 2, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist_process), 3, 98);
  gtk_clist_set_column_width (GTK_CLIST (clist_process), 4, 80);
  gtk_clist_column_titles_show (GTK_CLIST (clist_process));

  label_name = gtk_label_new (_("\350\277\233\347\250\213\345\220\215"));
  gtk_widget_show (label_name);
  gtk_clist_set_column_widget (GTK_CLIST (clist_process), 0, label_name);

  label_pid = gtk_label_new (_("\350\277\233\347\250\213\345\217\267"));
  gtk_widget_show (label_pid);
  gtk_clist_set_column_widget (GTK_CLIST (clist_process), 1, label_pid);

  label_status = gtk_label_new (_("\350\277\233\347\250\213\347\212\266\346\200\201"));
  gtk_widget_show (label_status);
  gtk_clist_set_column_widget (GTK_CLIST (clist_process), 2, label_status);

  label_cpu = gtk_label_new (_("CPU\344\275\277\347\224\250\347\216\207"));
  gtk_widget_show (label_cpu);
  gtk_clist_set_column_widget (GTK_CLIST (clist_process), 3, label_cpu);

  label_mem = gtk_label_new (_("\345\206\205\345\255\230\344\275\277\347\224\250\351\207\217"));
  gtk_widget_show (label_mem);
  gtk_clist_set_column_widget (GTK_CLIST (clist_process), 4, label_mem);

  label_sprocess = gtk_label_new (_("\347\263\273\347\273\237\350\277\233\347\250\213"));
  gtk_widget_show (label_sprocess);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_main), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook_main), 1), label_sprocess);
  GTK_WIDGET_SET_FLAGS (label_sprocess, GTK_CAN_FOCUS);
    g_signal_connect ((gpointer) clist_process, "click_column",
                    G_CALLBACK (cb_clist_process_click_column),
                    NULL);
  g_signal_connect ((gpointer) clist_process, "button_press_event",
                    G_CALLBACK (cb_clist_process_button_press_event),
                    NULL);
                    
  GLADE_HOOKUP_OBJECT (window_main, scrolledwindow_process, "scrolledwindow_process");
  GLADE_HOOKUP_OBJECT (window_main, clist_process, "clist_process");
  GLADE_HOOKUP_OBJECT (window_main, label_name, "label_name");
  GLADE_HOOKUP_OBJECT (window_main, label_pid, "label_pid");
  GLADE_HOOKUP_OBJECT (window_main, label_status, "label_status");
  GLADE_HOOKUP_OBJECT (window_main, label_cpu, "label_cpu");
  GLADE_HOOKUP_OBJECT (window_main, label_mem, "label_mem");
  GLADE_HOOKUP_OBJECT (window_main, label_sprocess, "label_sprocess");
}

GtkWidget*
create_popup_menu_process (void)
{
  GtkWidget *popup_menu_process;
  GtkWidget *spopup_eparator;
  GtkWidget *popup_process_kill;
  GtkWidget *image209;
  GtkAccelGroup *accel_group;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  accel_group = gtk_accel_group_new ();

  popup_menu_process = gtk_menu_new ();

  spopup_eparator = gtk_separator_menu_item_new ();
  gtk_widget_show (spopup_eparator);
  gtk_container_add (GTK_CONTAINER (popup_menu_process), spopup_eparator);
  gtk_widget_set_sensitive (spopup_eparator, FALSE);

  popup_process_kill = gtk_image_menu_item_new_with_mnemonic (_("\346\235\200\346\255\273\350\277\233\347\250\213"));
  gtk_widget_show (popup_process_kill);
  gtk_container_add (GTK_CONTAINER (popup_menu_process), popup_process_kill);
  gtk_tooltips_set_tip (tooltips, popup_process_kill, _("\346\235\200\346\255\273\350\257\245\350\277\233\347\250\213"), NULL);
  gtk_widget_add_accelerator (popup_process_kill, "activate", accel_group,
                              GDK_K, (GdkModifierType) GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  image209 = gtk_image_new_from_stock ("gtk-cancel", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image209);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (popup_process_kill), image209);

  g_signal_connect ((gpointer) popup_process_kill, "activate",
                    G_CALLBACK (cb_popup_process_kill_activate),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (popup_menu_process, popup_menu_process, "popup_menu_process");
  GLADE_HOOKUP_OBJECT (popup_menu_process, spopup_eparator, "spopup_eparator");
  GLADE_HOOKUP_OBJECT (popup_menu_process, popup_process_kill, "popup_process_kill");
  GLADE_HOOKUP_OBJECT (popup_menu_process, image209, "image209");
  GLADE_HOOKUP_OBJECT_NO_REF (popup_menu_process, tooltips, "tooltips");

  gtk_menu_set_accel_group (GTK_MENU (popup_menu_process), accel_group);

  return popup_menu_process;
}
