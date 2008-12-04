/*
 * linuxarms/armserver/src/config.c
 * 读取配置文件(文件位置见PORT_CONFIG_FILE)
 * Niu Tao<niutao0602@gmail.com>
 */
#include "config.h"
#include "error.h"
#include "linuxarms.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
static struct config_struct config;

static boolean config_exist(char *config_file);
static boolean config_read(char *config_file);
/*
 * 初始化配置文件
 * @return:  初始化成功，则为TRUE，否则为FALSE
 */
boolean config_init(char *config_file)
{
	if (!config_file)
		goto out;
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
	ret = fscanf(fp, "%s %d\n", tmp, &config.mthread_port);
	ret = fscanf(fp, "%s %d\n", tmp, &config.sthread_port);
	ret = fscanf(fp, "%s %d\n", tmp, &config.fthread_port);
	ret = fscanf(fp, "%s %d\n", tmp, &config.tthread_port);
	ret = fscanf(fp, "%s %d\n", tmp, &config.cthread_port);
	fclose(fp);
	
	return ret == -1 ? FALSE : TRUE;
}
/*
 * 获取主控连接的端口
 */
int get_mthread_port()
{
	return config.init ? config.mthread_port : -1;
}
/*
 * 获取文件浏览连接的端口
 */
int get_fthread_port()
{
	return config.init ? config.fthread_port : -1;
}
/*
 * 获取实时控制连接的端口
 */
int get_cthread_port()
{
	return config.init ? config.cthread_port : -1;
}
/*
 * 获取系统信息和进程信息连接的端口
 */
int get_sthread_port()
{
	return config.init ? config.sthread_port : -1;
}
/*
 * 获取文件传输连接的端口
 */
int get_tthread_port()
{
	return config.init ? config.tthread_port : -1;
}
