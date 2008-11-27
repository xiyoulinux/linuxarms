#include "log.h"
#include <stdio.h>
#include <unstd.h>
#include <string.h>

static struct log_struct log;
void down_log_lock(struct log_struct *log)
{
}
void up_log_lock(struct log_struct *log)
{
}
boolean armserver_init_log(char *usename)
{
	/* 初始化log结构体 */
	/* 是否有主日志目录，没有则创建 */
	/* 创建配置文件 */
}
boolean write_log(char *log_data);
{
	/* 写入信息 */
}
