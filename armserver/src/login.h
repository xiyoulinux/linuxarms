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
#endif
