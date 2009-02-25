#define __DEBUG__
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "linuxarms.h"
#include "thread.h"
#include "error.h"
#include "log.h"
#include "debug.h"

static struct log_struct log;

boolean armserver_init_log(const char *usename)
{
	/* 初始化log结构体 */
	/* 创建配置文件 */
	char log_conf_dir[30] = ".conf";
	const char log_dir[30] = ".log";
	char use_dir[128] = "\0";
	char temp_dir[256] = "\0";
	char *conf_sentence = "file_max_size:";
	int fd;
	FILE *fp;
	struct stat buf;
	
	/* 初始化log_struct 结构体 */
	linuxarms_thread_init(&log.log_thread);
	log.log_file_size = FILE_SIZE;
	log.log_inited = FALSE;
	
	/* 创建用户日志主目录 */
	snprintf(use_dir, 128, "%s/%s", LOGHOME_DIR, usename);
	debug_print("dir = %s\n", use_dir);
	if ((fd = open(use_dir, O_RDONLY)) == -1) { 
		/* 不存在用户日志目录，创建 */
		mkdir(use_dir,0644);
	} else {
		fstat(fd, &buf);
		if (!S_ISDIR(buf.st_mode)) {
			print_error(ENOINIT, "It is a file\n");
			return FALSE;
		}
		close(fd);
	}
	
	/* 创建用户日志配置文件 */
	snprintf(temp_dir, 256, "%s/%s%s", use_dir, usename, log_conf_dir);
	debug_print("dir = %s\n", temp_dir);

	if ((fp = fopen(temp_dir, "r")) == NULL) {
		/* 打开失败，不存在用户日志配置文件 */
		fp = fopen(temp_dir, "w");
		fprintf(fp,"%s %d", conf_sentence, log.log_file_size);
		fclose(fp);
	} else {
		fscanf(fp, "%s %d\n",log_conf_dir, &log.log_file_size);
		debug_print("size = %d\n", log.log_file_size);
		close(fd);
	}
	/* 创建用户日志文件 */
	snprintf(temp_dir, 256, "%s/%s%s", use_dir, usename,  log_dir);
	debug_print("log_dir = %s\n", temp_dir);
	if ((fp = fopen(temp_dir, "a+")) == NULL) {
		/* 日志文件创建失败 */
		print_error(ESYSERR, "function error");
	} else {
		stat(temp_dir,  &buf);
		if (buf.st_size >= (log.log_file_size*1024*1024)) {
			fclose(fp);
			log.log_fp = fopen(temp_dir, "w");
		} else {
			log.log_fp = fopen(temp_dir, "a+"); 
		}
	}

	log.log_inited = TRUE;
	return TRUE;
}
boolean write_log(const char *log_data)
{
	/* 写入信息 */
	time_t timep;
	char buf[80];
	
	if (!log.log_inited)
		return FALSE;
	time(&timep);
	strcpy(buf, ctime(&timep));
	buf[strlen(buf)-1] = '\0';
	linuxarms_thread_lock(&log.log_thread);
	if (fprintf(log.log_fp, "%s linuxarms-armserver : %s\n", 
				buf, log_data) == -1) {
		print_error(EWARNING, "无法保存此日志信息");
		linuxarms_thread_unlock(&log.log_thread);
		return FALSE;
	}
	linuxarms_thread_unlock(&log.log_thread);
	return TRUE;
}
boolean close_log()
{
	if (!log.log_inited)
		return FALSE;
	linuxarms_thread_lock_free(&log.log_thread);
	if(fclose(log.log_fp) == -1)
		return FALSE;
	return TRUE;
}
