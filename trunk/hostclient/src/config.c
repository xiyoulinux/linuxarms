/*
 * linuxarms/hostclient/src/config.c
 * 读取端口配置文件(文件位置见PORT_CONFIG_FILE)
 * Niu Tao<niutao0602@gmail.com>
 */
#include "config.h"
#include "error.h"
#include "linuxarms.h"
#include "debug.h"
#include "support.h"
#include "login.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
static struct config_struct config;

static boolean config_exist(char *config_file);
static boolean config_read(char *config_file);
/*
 * 初始化配置文件
 * @return:  初始化成功，则为TRUE，否则为FALSE
 */
boolean config_init(char *config_file)
{
	if (!config_file) {
		print_error(ESYSERR, "没有端口配置文件");
		goto out;
	}
	if (!config_exist(config_file)) 
		goto out;
	if (!config_read(config_file))
		goto out;

	config.init = TRUE;
	return TRUE;
out:
	config.init = FALSE;
	return FALSE;
}
/*
 * 如果没有，则判断配置文件是否存在，
 * @return:  如果存在，则为TRUE。否则为FALSE
 */
static boolean config_exist(char *config_file)
{
	if (access(config_file,F_OK) == -1)
		return FALSE;
	return TRUE;
}
/*
 * 读取配置文件
 */
static boolean config_read(char *config_file)
{
	FILE *fp;
	char tmp[20];
	int ret;

	if ((fp = fopen(config_file, "r")) == NULL) {
		print_error(ESYSERR, "读取端口配置文件错误");
		return FALSE;
	}
	ret = fscanf(fp, "%s %d\n", tmp, &config.armserver_port);
	fclose(fp);
	
	return ret == -1 ? FALSE : TRUE;
}
int get_armserver_port()
{
	return config.init ? config.armserver_port : -1;
}


char *get_user_home();
static boolean login_config_set_path(struct login_config_struct *config);
static boolean login_config_error(struct login_config_struct *config);
static boolean login_config_create(void);

static boolean login_config_create(void)
{
	char *home, *p;
	char tmp[80];
	FILE *fp;
	home = get_user_home();
	if (!home)
		return FALSE;
	debug_where();
	snprintf(tmp, 80, "%s/."USER_CONFIG_DIR_NAME,home);
        if (access(tmp, F_OK) == -1) {
		if (mkdir(tmp, 0755) == -1)
			return FALSE;
	}
	strcat(tmp,"/config");
        if (access(tmp, F_OK) == -1) {
		if (mkdir(tmp, 0755) == -1)
			return FALSE;
	}
	strcat(tmp, "/user.conf");
        if (access(tmp,F_OK) == -1) {
		if ((fp = fopen(tmp, "w")) == NULL)
			return FALSE;
		fprintf(fp, "SERVERIP 0\nUSER 0\n");
		fclose(fp);
	}
	p = strrchr(tmp, '/');
	*p = '\0';
	add_file_directory(tmp);
	return TRUE;
}
/*
 * 判断参数是否为空，如果为空则返回FALSE
 */
static boolean login_config_error(struct login_config_struct *config)
{
	if (!config) {
		print_error(EWARNING, "无效参数");
		return FALSE;
	}
	return TRUE;
	
}
/*
 * 读取配置文件所在路径，如果正确读取，则返回TRUE
 */
static boolean login_config_set_path(struct login_config_struct *config)
{
	char *p;

	debug_where();
	login_config_error(config);

	p = get_user_home();
	if (!p) {
		print_error(ESYSERR, "读取环境变量错误");
		config->inited = FALSE;
		return FALSE;
	}
	if (!login_config_create()) {
		config->inited = FALSE;
		return TRUE;
	}
	snprintf(config->path, CONFIG_FILE_PATH_LEN, 
		 "%s/."USER_CONFIG_DIR_NAME"/config", p);
	add_file_directory(config->path);
	config->inited = TRUE;
	return TRUE;
}
char *get_user_home()
{
	return getenv("HOME");
}
boolean login_config_init(struct login_config_struct *config)
{
	if (!login_config_error(config))
		return FALSE;
	config->ip_num = 0;
	config->ip_list.next = NULL;
	config ->user_num = 0;
	config->user_list.next = NULL;
	memset(config->ip_list.ip, '\0',IP_LEN);
	memset(config->user_list.user_name, '\0',USER_NAME_LEN);
	memset(config->user_list.passwd, '\0',PASSWD_LEN);
	return login_config_set_path(config);
}
/*
 * 读取配置文件
 */
boolean login_config_read(struct login_config_struct *config)
{
	FILE *fp;
	char buf[20];
	int i, ret;
	char *config_file;
	struct list_ip *ip, *ip_head;
	struct list_user *user, *user_head;
	debug_where();
	if (!config->inited)
		goto out;
	config_file = find_file("user.conf");
	if (!config_file) {
		print_error(EWARNING, "配置文件不存在");
		goto out;
	}
	if ((fp = fopen(config_file, "r")) == NULL) {
		debug_where();
		print_error(EWARNING, "读取配置文件错误");
		goto out;
	}
	ret = fscanf(fp , "%s %d\n", buf, &config->ip_num);
	debug_print("%s %d\n",buf,config->ip_num);
	ip_head = &config->ip_list;
	for (i = 0; i < config->ip_num; i++) {
		ip = (struct list_ip *)malloc(sizeof(struct list_ip));
		ret = fscanf(fp, "%s\n", ip->ip);
		ip_head->next = ip;
		ip_head = ip_head->next;
		debug_print("%s\n",ip->ip);
	}
	ip_head = NULL;

	ret = fscanf(fp , "%s %d\n", buf, &config->user_num);
	debug_print("%s %d\n",buf,config->user_num);
	user_head = &config->user_list;
	for (i = 0; i < config->ip_num; i++) {
		user = (struct list_user *)malloc(sizeof(struct list_user));
		ret = fscanf(fp, "%s %d",user->user_name, &user->remember);
		if (user->remember)
			ret = fscanf(fp, "%s\n", user->passwd);
		else
			ret = fscanf(fp, "\n");
		user_head->next = user;
		user_head = user_head->next;
		debug_print("%s\n",user->user_name);
	}
	user_head = NULL;
	fclose(fp);
	if (ret < 0)
		goto out;

	config->inited = TRUE;
	return TRUE;
out:
	config->inited = FALSE;
	return FALSE;
}
/*
 * 写配置文件
 */
boolean login_config_write(struct login_struct *login)
{
	struct login_config_struct *config = login->config;
	struct user_struct *suser = &login->user;
	FILE *fp;
	char *config_file;
	int i, ret;
	struct list_ip *ip, *ip_tmp;
	struct list_user *user, *user_tmp;
	debug_where();
	config_file = find_file("user.conf");
	debug_print("配置文件:%s\n",config_file);
	if (!config_file) {
		print_error(ESYSERR, "没有配置文件");
		return FALSE;
	}
	if ((fp = fopen(config_file, "w")) == NULL) {
		print_error(ESYSERR, "打开配置文件错误");
		return FALSE;
	}
	debug_where();
	if (config->ip_num)
		ip = config->ip_list.next;
	else
		ip = &config->ip_list;

	debug_where();
	for (i = 0;i < config->ip_num; i++) {
		if (strcmp(ip->ip, suser->ip) == 0) 
			break;
		ip = ip->next;
	}
	debug_print("i = %d  %d\n", i ,config->ip_num);
	if (i == config->ip_num) {
		ip_tmp = (struct list_ip *)malloc(sizeof(struct list_ip));
		strncpy(ip_tmp->ip,suser->ip,IP_LEN);
		ip->next = ip_tmp;
		ip_tmp->next = NULL;
		config->ip_num += 1;
	}
	debug_where();
	debug_print("i = %d  %d\n", i ,config->ip_num);
	ret = fprintf(fp, "SERVERIP %d\n",config->ip_num);
	ip = config->ip_list.next;
	for (i = 0; i < config->ip_num; i++) {
		ret = fprintf(fp, "%s\n",ip->ip);
		ip = ip->next;
	}
	if (config->user_num)
		user = config->user_list.next;
	else
		user = &config->user_list;

	for (i = 0; i < config->user_num; i++) {
		if (strcmp(user->user_name,suser->name) == 0) {
			user->remember = login->remember;
			strncpy(user->passwd, suser->passwd, PASSWD_LEN);
			break;
		}
		user = user->next;
	}
	debug_print("i = %d  %d\n", i ,config->user_num);
	if (i == config->user_num) {
		user_tmp = (struct list_user *)malloc(sizeof(struct list_user));
		strncpy(user_tmp->passwd, suser->passwd, PASSWD_LEN);
		strncpy(user_tmp->user_name, suser->name, USER_NAME_LEN);
		user_tmp->remember = login->remember;
		user->next = user_tmp;
		user_tmp->next = NULL;
		config->user_num += 1;
	}
	debug_print("i = %d  %d\n", i ,config->user_num);
	debug_where();
	ret = fprintf(fp, "USER %d\n",config->user_num);
	user = config->user_list.next;
	for (i = 0; i < config->user_num; i++) {
		ret = fprintf(fp, "%s %d",user->user_name, user->remember);
		if (user->remember) 
			ret = fprintf(fp, " %s\n",user->passwd);
		else
			ret = fprintf(fp, "\n");
		user = user->next;
	}

	fclose(fp);
	debug_where();
	return TRUE;
}
boolean login_config_free(struct login_config_struct *config)
{
	
	int i;
	struct list_ip *ip,*ip_tmp;
	struct list_user *user, *user_tmp;
	
	if (!login_config_error(config))
		return FALSE;
	ip = &config->ip_list;
	ip_tmp = ip->next;
	for (i = 0; i < config->ip_num; i++) {
		ip = ip_tmp->next;
		free(ip_tmp);
		ip_tmp = ip;
	}
	user = &config->user_list;
	user_tmp = user->next;
	for (i = 0; i < config->user_num; i++) {
		user = user_tmp->next;
		free(user_tmp);
		user_tmp = user;
	}
	debug_where();
	return TRUE;
}
