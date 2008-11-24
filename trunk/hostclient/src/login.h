#ifndef _LOGIN_H
#define _LOGIN_H
#include "linuxarms.h"
#define USER_NAME_LEN 20
#define PASSWD_LEN 20
struct user_struct {
	char name[USER_NAME_LEN];
	char passwd[PASSWD_LEN];
	boolean competence;
};
void cb_login_help_clicked(GtkButton *button, gpointer user_data);

void cb_login_cancel_clicked(GtkButton *button, gpointer user_data);

void cb_login_ok_clicked(GtkButton *button, gpointer user_data);

void cb_login_rem_toggled(GtkToggleButton *togglebutton, gpointer user_data);
#endif
