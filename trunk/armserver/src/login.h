/*
 * linuxarms/armserver/src/login.h
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 *  
 * The file contains the private data structure and API definitions.
 */
#ifndef _LOGIN_H
#define _LOGIN_H

#include "linuxarms.h"
#include "amthread.h"
#include "anet.h"

#define USER_NAME_LEN 20
#define PASSWD_LEN 20
#define CONFIG_BUF_SIZE 4096
#define INFO_SIZE 80

typedef enum {
	FAILURE_ = -1, /* -1 */
	SUCCESS_ /* 0 */
}Status;

/*
 * user_struct  用于存放获取的用户信息
 * @ip: 用户输入的IP地址
 * @name: 用户输入的用户名
 * @passwd: 用户输入的密码
 * @is_record: 用户是否选择记住用户名和密码
 * @competence: 
 */
struct user_struct {
	char ip[15];
	char name[USER_NAME_LEN];
	char passwd[PASSWD_LEN];
	int is_record;
	boolean competence;
};
/*
 * login handling function definitions
 */
/*inward interface*/
static boolean set_ip(struct user_struct *user, const char *ip);
static boolean set_name(struct user_struct *user, const char *name);
static boolean set_passwd(struct user_struct *user, const char *passwd);
static boolean set_is_record(struct user_struct *user, int is_record);
static boolean get_is_record(struct user_struct *user);

/*
 * user_config_struct 目录及文件安装配置相关数据结构和公用接口API
 * @buffer: 读文件存放临时数据的缓冲区
 * @create_file: 根据提供的文件名创建配置文件
 * @read_from_config_file: 根据提供的文件名读取数据到缓冲区
 * @write_to_config_file: 把提供的数据写入到提供文件名文件中
 */
struct user_config_struct {
	char buffer[CONFIG_BUF_SIZE];
	
	boolean (*create_file)(const char *filename);
	boolean (*read_from_config_file)(const char *filename, char buffer[]);
	boolean (*write_to_config_file)(const char *filename, const char *buffer);
};
/*
 * configure function definitions
 */
/*public interface*/
boolean create_config_file(const char *filename);
boolean read_from_config_file(const char *filename, char buffer[]);
boolean write_to_config_file(const char *filename, const char *buffer);
/*inward interface*/
static boolean is_config_exist(void);
static boolean is_path_exist(const char *pathname);
static boolean is_file_exist(const char *path);
static boolean config_create(); /*create $HOME/.linuxarms*/
static boolean create_path(const char *path);
static boolean create_file(const char *path, const char *filename);
static const char *search_file(const char *filename); /*return file path*/
static const char *read_file(const char *filename);
static const  char *write_file(const char *filename);

/*
 * login_struct 用户登录处理及相关信息显示和出错处理
 * @user: 用户信息
 * @socket: 网络相关模块
 * @hmthread: 主线程控制相关
 * @status: 网络结果状态信息
 */
struct login_struct {
	struct user_struct *user;
	struct anet_struct *socket;
	struct amthread_struct *hmthread;
	Status status;
};
/*
 * login functions definitions
 */
/*inward interface*/
boolean set_error_info(struct login_struct *login, const char *error_info);
const char *get_error(struct login_struct *login);

#endif
