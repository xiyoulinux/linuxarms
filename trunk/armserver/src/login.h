/*
 * linuxarms/armserver/src/login.h
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 *  
 * The file contains the private data structure and API definitions.
 */
#ifndef _LOGIN_H
#define _LOGIN_H

#include "linuxarms.h"
#include "anet.h"
#define USER_NAME_LEN 20
#define PASSWD_LEN 20
#define CONFIG_BUF_SIZE 4096
#define INFO_SIZE 80


/*
 * user_struct  用于存放获取的用户信息
 * @ip: 用户输入的IP地址
 * @name: 用户输入的用户名
 * @passwd: 用户输入的密码
 * @competence: 标记用户权限(root = TRUE)
 */
struct user_struct {
	char ip[IP_LEN];
	char name[USER_NAME_LEN];
	char passwd[PASSWD_LEN];
};
boolean user_struct_init(struct user_struct *user);
boolean user_competece(struct user_struct *user);
boolean user_struct_set(struct user_struct *user, char *ip, char *name, char *passwd);
/*
 * login_struct 用户登录处理及相关信息显示和出错处理
 * @user: 用户信息
 * @socket: 网络相关模块
 */
struct login_struct {
	struct user_struct user;
	struct anet_struct *socket;
	boolean competence;
};
/*
 * login functions definitions
 */
boolean login_init(struct login_struct *login, struct anet_struct *socket);
boolean login_check_user(struct login_struct *login);
boolean login_set_env(struct login_struct *login);
boolean login_user_competence(struct login_struct *login);

#endif
