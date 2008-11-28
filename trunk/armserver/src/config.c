#include "config.h"
#include <stdio.h>
#include <string.h>

static struct config_struct config;

static boolean config_create();
static boolean config_read();
/*
 * 初始化配置文件
 * @return:  初始化成功，则为TRUE，否则为FALSE
 */
boolean config_init()
{
	if (!config_create()) 
		goto out;
	if (!config_read())
		goto out;

	config.init = TRUE;
	return TRUE;
out:
	config.init = FALSE;
	return FALSE;
}
/*
 * 如果没有，则创建配置文件，
 * @return:  创建成功，则为TRUE。否则为FALSE
 */
static boolean config_create()
{
}
/*
 * 读取配置文件
 */
static boolean config_read()
{

}
/*
 * 获取主控连接的端口
 */
int get_mthread_port()
{

}
/*
 * 获取文件浏览连接的端口
 */
int get_fthread_port()
{
}
/*
 * 获取实时控制连接的端口
 */
int get_cthread_port()
{
}
/*
 * 获取系统信息和进程信息连接的端口
 */
int get_sthread_port()
{
}
/*
 * 获取文件传输连接的端口
 */
int get_tthread_port()
{
}
