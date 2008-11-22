#ifndef _ABOUT_H
#define _ABOUT_H

void on_window_destroy(GtkObject *object, gpointer user_data);
void on_button_close_clicked(GtkButton *button, gpointer user_data);                                        

GtkWidget* create_window_about(void);
void on_button_link_clicked(GtkButton *button, gpointer user_data);

void on_button_credits_clicked(GtkButton *button, gpointer user_data);

void on_button_licence_clicked(GtkButton *button, gpointer user_data);

GtkWidget* create_window_credits(void); 

gboolean gtk_textview_set_text(GtkWidget *textview, const gchar *text);                                                                 
GtkWidget* create_window_licence(void);  
                                 
#endif //_ABOUT_H
