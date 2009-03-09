#ifndef _CONFIG_H
#define _CONFIG_H

#include "linuxarms.h"
/*
 * config_struct 配置文件读取相关的数据结构
 * @mthread_port:  主控连接的端口
 * @fthread_port:  文件浏览连接的端口
 * @cthread_port:  实时控制连接的端口
 * @sthread_port:  系统信息和进程信息显示连接的端口
 * @tthread_port:  文件传输连接的端口
 * @path:          普通用户的环境变量
 * @init:          本数据结构是否已经初始化
 */
struct config_struct {
	int mthread_port;
	int fthread_port;
	int cthread_port;
	int sthread_port;
	int tthread_port;
	char *path;
	boolean init;
};
/* 初始化配置文件 */
boolean config_init();
/* 读取主线程TCP端口 */
int get_mthread_port();
/* 读取文件浏览线程TCP端口 */
int get_fthread_port();
/* 读取实时控制线程TCP端口 */
int get_cthread_port();
/* 读取系统信息显示和进程信息显示线程TCP端口 */
int get_sthread_port();
/* 读取文件传输线程TCP端口 */
int get_tthread_port();
char *get_path_env();
#endif
