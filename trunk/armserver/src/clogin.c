/*
 * linuxarms/armserver/src/clogin.c
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 */
#include "config.h"
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_SHADOW
# include <shadow.h>
#endif

#include "login.h"
#include "debug.h"
#include "error.h"
#include "anet.h"
#include "fconfig.h"

boolean login_init(struct login_struct *login,
		   struct anet_struct *socket)
{
	if (!login || !socket) {
		print_error(ENOINIT, "login_init");
		return FALSE;
	}
	login->socket = socket;
	login->permit = FALSE;
	return TRUE;
}

boolean user_struct_init(struct user_struct *user)
{
	LINUXARMS_POINTER(user);
	memset(user->ip, '\0', IP_LEN);
	memset(user->name, '\0', USER_NAME_LEN);
	memset(user->passwd, '\0', PASSWD_LEN);
	return TRUE;
}
boolean login_user_permit(struct login_struct *login)
{
	LINUXARMS_POINTER(login);
	return login->permit;
}
boolean user_struct_set(struct user_struct *user, char *ip, char *name, char *passwd)
{
	LINUXARMS_POINTER(user);
	strncpy(user->ip, !ip ? "" : ip, IP_LEN);
	strncpy(user->name, !name ? "" : name, USER_NAME_LEN);
	strncpy(user->passwd, !passwd ? "" : passwd, PASSWD_LEN);
	return TRUE;
}
/*
 * 验证用户
 */
boolean login_check_user(struct login_struct *login)
{
	struct user_struct *user;
#ifdef HAVE_SHADOW
	struct spwd *pwd;
#else
	struct passwd *pwd;
#endif
	char *current, *encrypted, *unencrypted;
	
	LINUXARMS_POINTER(login);
	user = &login->user;
	unencrypted = login->user.passwd;
	if (strcmp(user->name, "root") == 0)
		login->permit = TRUE;
	else
		login->permit = FALSE;
#ifdef HAVE_SHADOW
	pwd = getspnam(user->name);
	if (!pwd) {
		print_error(EWARNING, "the user is not found");
		return FALSE;
	}
	endspent ();
	current = pwd->sp_pwdp;
#else
	pwd = getpwnam(user->name);
	if (!pwd) {
		print_error(EWARNING, "the user is not found");
		return FALSE;
	}
	current = pwd->pw_passwd;
#endif

	encrypted = (char *)crypt(unencrypted, current);
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
	if (login->permit)
		return TRUE;
	pwd = getpwnam(user->name);
	setenv("USER", user->name, 1);
	setenv("LOGNAME", user->name, 1);
	setenv("PATH", get_path_env(), 1);
	setenv("SHELL", pwd->pw_shell, 1);
	setenv("PWD", "/", 1);
	chdir("/");
	if (access(pwd->pw_dir,F_OK) == -1) {
		print_error(EWARNING, "home directory %s isn't exist,"
				"so ew change it to \"/\"",pwd->pw_dir);
		setenv("HOME", "/", 1);
	} else {
		setenv("HOME", pwd->pw_dir, 1);
	}
	/*if (chdir(getenv("HOME")) == -1 ) {
		debug_where();
		print_error(EWARNING, "chdir: error");
	}*/
	setgid(pwd->pw_gid);
	setuid(pwd->pw_uid);
	
	return TRUE;
}

