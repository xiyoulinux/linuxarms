#ifndef _CONFIG_H
#define _CONFIG_H

#include "linuxarms.h"
#include "login.h"
/*
 * config_struct 配置文件读取相关的数据结构
 * @init:         本数据结构是否已经初始化
 */
struct config_struct {
	int armserver_port;
	boolean init;
};
/* 初始化配置文件 */
boolean config_init();
int get_armserver_port();


#define CONFIG_FILE_PATH_LEN 128
#define USER_CONFIG_DIR_NAME "linuxarms-hostclient"

struct list_ip {
	char ip[IP_LEN];
	struct list_ip *next;
};
struct list_user {
	char user_name[USER_NAME_LEN];
	boolean remember;
	char passwd[PASSWD_LEN];
	struct list_user *next;
};
struct login_config_struct {
	boolean inited;
	char path[CONFIG_FILE_PATH_LEN];
	int ip_num;
	struct list_ip ip_list;
	int user_num;
	struct list_user user_list;
};
struct login_struct;
char *get_user_home(void);
boolean login_config_init(struct login_config_struct *config);
boolean login_config_read(struct login_config_struct *config);
boolean login_config_write(struct login_struct *login);
boolean login_config_free(struct login_config_struct *config);
#endif
