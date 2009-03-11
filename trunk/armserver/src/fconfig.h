/*
 * Real-Time Management System Based on Arm Linux
 *
 * Copyright (c) 2009-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: fconfig.h
 * modify date: 2009-3-10
 * Author: niutao0602@gmail.com
 *
 * Program definitions: read file config/port.conf  to get the port of armserver,
 * and get ordinary user's environment $PATH.
 */
#ifndef _FCONFIG_H
#define _FCONFIG_H

#include "linuxarms.h"
/*
 * @name:           config_struct 
 * @armserver_port: the port of server.
 * @path:           ordinary user's environment $PATH
 * @init:           correctly read the port.conf?
 */
struct config_struct {
	int armserver_port;
	int afthread_port;
	char *path;
	boolean init;
};
/* 初始化配置文件 */
boolean config_init();
int get_armserver_port();
int get_afthread_port();
char *get_path_env();
#endif
