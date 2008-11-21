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
 GtkWidget *create_page_fview()
 {
 	GtkWidget *vbox9;
  GtkWidget *vbox10;
  GtkWidget *hbox1;
  GtkWidget *label_fpath;
  GtkWidget *entry_fpath;
  GtkWidget *scrolledwindow_fview;
  GtkWidget *clist_fview;
  GtkWidget *label_fname;
  GtkWidget *label_fsize;
  GtkWidget *label_ftype;
  GtkWidget *label_fmdate;
  GtkWidget *label_fview;
 vbox9 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox9);
  gtk_container_add (GTK_CONTAINER (notebook_main), vbox9);

  vbox10 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox10);
  gtk_box_pack_start (GTK_BOX (vbox9), vbox10, FALSE, FALSE, 0);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox10), hbox1, TRUE, TRUE, 0);

  label_fpath = gtk_label_new (_("\346\226\207\344\273\266\350\267\257\345\276\204"));
  gtk_widget_show (label_fpath);
  gtk_box_pack_start (GTK_BOX (hbox1), label_fpath, FALSE, FALSE, 0);
  gtk_widget_set_size_request (label_fpath, 60, -1);

  entry_fpath = gtk_entry_new ();
  gtk_widget_show (entry_fpath);
  gtk_box_pack_start (GTK_BOX (hbox1), entry_fpath, TRUE, TRUE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry_fpath), 256);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry_fpath), 9679);

  scrolledwindow_fview = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow_fview);
  gtk_box_pack_start (GTK_BOX (vbox9), scrolledwindow_fview, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow_fview), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow_fview), GTK_SHADOW_IN);

  clist_fview = gtk_clist_new (4);
  gtk_widget_show (clist_fview);
  gtk_container_add (GTK_CONTAINER (scrolledwindow_fview), clist_fview);
  gtk_clist_set_column_width (GTK_CLIST (clist_fview), 0, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist_fview), 1, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist_fview), 2, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist_fview), 3, 80);
  gtk_clist_column_titles_show (GTK_CLIST (clist_fview));

  label_fname = gtk_label_new (_("\346\226\207\344\273\266\345\220\215"));
  gtk_widget_show (label_fname);
  gtk_clist_set_column_widget (GTK_CLIST (clist_fview), 0, label_fname);

  label_fsize = gtk_label_new (_("\345\244\247\345\260\217"));
  gtk_widget_show (label_fsize);
  gtk_clist_set_column_widget (GTK_CLIST (clist_fview), 1, label_fsize);

  label_ftype = gtk_label_new (_("\347\261\273\345\236\213"));
  gtk_widget_show (label_ftype);
  gtk_clist_set_column_widget (GTK_CLIST (clist_fview), 2, label_ftype);

  label_fmdate = gtk_label_new (_("\344\277\256\346\224\271\346\227\266\351\227\264"));
  gtk_widget_show (label_fmdate);
  gtk_clist_set_column_widget (GTK_CLIST (clist_fview), 3, label_fmdate);

  label_fview = gtk_label_new (_("\346\265\217\350\247\210\346\226\207\344\273\266"));
  gtk_widget_show (label_fview);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_main), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook_main), 2), label_fview);
  GTK_WIDGET_SET_FLAGS (label_fview, GTK_CAN_FOCUS);
  
    g_signal_connect ((gpointer) clist_fview, "click_column",
                    G_CALLBACK (cb_clist_fview_click_column),
                    NULL);
  g_signal_connect ((gpointer) clist_fview, "button_press_event",
                    G_CALLBACK (cb_clist_fview_button_press_event),
                    NULL);
                    
  GLADE_HOOKUP_OBJECT (window_main, vbox9, "vbox9");
  GLADE_HOOKUP_OBJECT (window_main, vbox10, "vbox10");
  GLADE_HOOKUP_OBJECT (window_main, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (window_main, label_fpath, "label_fpath");
  GLADE_HOOKUP_OBJECT (window_main, entry_fpath, "entry_fpath");
  GLADE_HOOKUP_OBJECT (window_main, scrolledwindow_fview, "scrolledwindow_fview");
  GLADE_HOOKUP_OBJECT (window_main, clist_fview, "clist_fview");
  GLADE_HOOKUP_OBJECT (window_main, label_fname, "label_fname");
  GLADE_HOOKUP_OBJECT (window_main, label_fsize, "label_fsize");
  GLADE_HOOKUP_OBJECT (window_main, label_ftype, "label_ftype");
  GLADE_HOOKUP_OBJECT (window_main, label_fmdate, "label_fmdate");
  GLADE_HOOKUP_OBJECT (window_main, label_fview, "label_fview");
}


GtkWidget*
create_popup_menu_fview (void)
{
  GtkWidget *popup_menu_fview;
  GtkWidget *popup_separator;
  GtkWidget *fview_rename;
  GtkWidget *image203;
  GtkWidget *fview_delete;
  GtkWidget *image204;
  GtkWidget *edit_separator_one;
  GtkWidget *edit_upload;
  GtkWidget *image205;
  GtkWidget *edit_downolad;
  GtkWidget *image206;
  GtkAccelGroup *accel_group;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  accel_group = gtk_accel_group_new ();

  popup_menu_fview = gtk_menu_new ();

  popup_separator = gtk_separator_menu_item_new ();
  gtk_widget_show (popup_separator);
  gtk_container_add (GTK_CONTAINER (popup_menu_fview), popup_separator);
  gtk_widget_set_sensitive (popup_separator, FALSE);

  fview_rename = gtk_image_menu_item_new_with_mnemonic (_("\351\207\215\345\221\275\345\220\215\346\226\207\344\273\266"));
  gtk_widget_show (fview_rename);
  gtk_container_add (GTK_CONTAINER (popup_menu_fview), fview_rename);
  gtk_tooltips_set_tip (tooltips, fview_rename, _("\351\207\215\345\221\275\345\220\215\351\200\211\344\270\255\347\232\204\346\226\207\344\273\266"), NULL);
  gtk_widget_add_accelerator (fview_rename, "activate", accel_group,
                              GDK_F2, (GdkModifierType) 0,
                              GTK_ACCEL_VISIBLE);

  image203 = gtk_image_new_from_stock ("gtk-refresh", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image203);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (fview_rename), image203);

  fview_delete = gtk_image_menu_item_new_with_mnemonic (_("\345\210\240\351\231\244\346\226\207\344\273\266"));
  gtk_widget_show (fview_delete);
  gtk_container_add (GTK_CONTAINER (popup_menu_fview), fview_delete);
  gtk_tooltips_set_tip (tooltips, fview_delete, _("\345\210\240\351\231\244\351\200\211\344\270\255\347\232\204\346\226\207\344\273\266"), NULL);

  image204 = gtk_image_new_from_stock ("gtk-delete", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image204);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (fview_delete), image204);

  edit_separator_one = gtk_separator_menu_item_new ();
  gtk_widget_show (edit_separator_one);
  gtk_container_add (GTK_CONTAINER (popup_menu_fview), edit_separator_one);
  gtk_widget_set_sensitive (edit_separator_one, FALSE);

  edit_upload = gtk_image_menu_item_new_with_mnemonic (_("\344\270\212\344\274\240\346\226\207\344\273\266"));
  gtk_widget_show (edit_upload);
  gtk_container_add (GTK_CONTAINER (popup_menu_fview), edit_upload);
  gtk_tooltips_set_tip (tooltips, edit_upload, _("\344\273\216arm\347\263\273\347\273\237\344\270\212\344\274\240\346\226\207\344\273\266\345\210\260\346\234\254\346\234\272"), NULL);
  gtk_widget_add_accelerator (edit_upload, "activate", accel_group,
                              GDK_U, (GdkModifierType) GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  image205 = gtk_image_new_from_stock ("gtk-goto-top", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image205);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (edit_upload), image205);

  edit_downolad = gtk_image_menu_item_new_with_mnemonic (_("\344\270\213\350\275\275\346\226\207\344\273\266"));
  gtk_widget_show (edit_downolad);
  gtk_container_add (GTK_CONTAINER (popup_menu_fview), edit_downolad);
  gtk_tooltips_set_tip (tooltips, edit_downolad, _("\344\273\216\346\234\254\346\234\272\344\270\213\350\275\275\346\226\207\344\273\266\345\210\260arm\347\263\273\347\273\237\344\270\212"), NULL);
  gtk_widget_add_accelerator (edit_downolad, "activate", accel_group,
                              GDK_D, (GdkModifierType) GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  image206 = gtk_image_new_from_stock ("gtk-goto-bottom", GTK_ICON_SIZE_MENU);
  gtk_widget_show (image206);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (edit_downolad), image206);

  g_signal_connect ((gpointer) fview_rename, "activate",
                    G_CALLBACK (cb_fview_rename_activate),
                    NULL);
  g_signal_connect ((gpointer) fview_delete, "activate",
                    G_CALLBACK (cb_fview_delete_activate),
                    NULL);
  g_signal_connect ((gpointer) edit_upload, "activate",
                    G_CALLBACK (cb_fview_upload_activate),
                    NULL);
  g_signal_connect ((gpointer) edit_downolad, "activate",
                    G_CALLBACK (cb_fview_downolad_activate),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (popup_menu_fview, popup_menu_fview, "popup_menu_fview");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, popup_separator, "popup_separator");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, fview_rename, "fview_rename");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, image203, "image203");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, fview_delete, "fview_delete");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, image204, "image204");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, edit_separator_one, "edit_separator_one");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, edit_upload, "edit_upload");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, image205, "image205");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, edit_downolad, "edit_downolad");
  GLADE_HOOKUP_OBJECT (popup_menu_fview, image206, "image206");
  GLADE_HOOKUP_OBJECT_NO_REF (popup_menu_fview, tooltips, "tooltips");

  gtk_menu_set_accel_group (GTK_MENU (popup_menu_fview), accel_group);

  return popup_menu_fview;
}
