/*
 * linuxarms/armserver/src/clogin.c
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 */
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <shadow.h>

#include "login.h"
#include "debug.h"
#include "error.h"
#include "config.h"
#include "anet.h"

boolean login_init(struct login_struct *login,
		   struct anet_struct *socket)
{
	if (!login || !socket) {
		print_error(ENOINIT, "login_init");
		return FALSE;
	}
	login->socket = socket;
	return TRUE;
}

boolean user_struct_init(struct user_struct *user)
{
	LINUXARMS_POINTER(user);
	memset(user->ip, '\0', IP_LEN);
	memset(user->name, '\0', USER_NAME_LEN);
	memset(user->passwd, '\0', PASSWD_LEN);
	user->competence = FALSE;
	return TRUE;
}
boolean user_competece(struct user_struct *user)
{
	LINUXARMS_POINTER(user);
	return user->competence;
}
boolean user_struct_set(struct user_struct *user, char *ip, char *name, char *passwd)
{
	LINUXARMS_POINTER(user);
	strncpy(user->ip, !ip ? "" : ip, IP_LEN);
	strncpy(user->name, !name ? "" : name, USER_NAME_LEN);
	strncpy(user->passwd, !passwd ? "" : passwd, PASSWD_LEN);
	if (strcmp(user->name, "root") == 0)
		user->competence = TRUE;
	else
		user->competence = FALSE;
	return TRUE;
}
/*
 * 验证用户
 */
boolean login_check_user(struct login_struct *login)
{
	struct user_struct *user;
	struct passwd *pwd;
	struct spwd *spwd;
	char *current, *encrypted, *unencrypted;
	
	LINUXARMS_POINTER(login);
	user = &login->user;
	unencrypted = login->user.passwd;
	if (strcmp(user->name, "root") == 0)
		user->competence = TRUE;
	else
		user->competence = FALSE;			
	/*pwd = getpwnam(user->name);
	if (!(pwd && pwd->pw_name && pwd->pw_name[0] && pwd->pw_dir && pwd->pw_dir[0] 
				           && pwd->pw_passwd)){
		print_error(EWARNING, "the user is not found");
		return FALSE;
	}*/
	spwd = getspnam(user->name);
	if (!spwd) {
		print_error(EWARNING, "the user is not found");
		return FALSE;
	}
	endspent ();
	current = spwd->sp_pwdp;
	encrypted = crypt(unencrypted, current);
	if (strcmp(encrypted, current) != 0)
		return FALSE;
	return TRUE;
}
/*
 * 设置用户环境变量
 */
boolean login_set_env(struct login_struct *login)
{
	struct user_struct *user;
	struct passwd *pwd;
	
	LINUXARMS_POINTER(login);
	user = &login->user;
	if (user->competence == TRUE)
		return TRUE;
	pwd = getpwnam(user->name);
	setenv("USER", user->name, 1);
	setenv("LOGNAME", user->name, 1);
	setenv("PATH", get_path_env(), 1);
	setenv("SHELL", pwd->pw_shell, 1);
	if (access(pwd->pw_dir,F_OK) == -1) {
		print_error(EWARNING, "home directory %s isn't exist,"
				"so ew change it to \"/\"",pwd->pw_dir);
		setenv("HOME", "/", 1);
	} else {
		setenv("HOME", pwd->pw_dir, 1);
	}
	if (chdir(getenv("HOME")) == -1 ) {
		debug_where();
		print_error(EWARNING, "chdir: error");
	}
	setgid(pwd->pw_gid);
	setuid(pwd->pw_uid);
	
	return TRUE;
}
