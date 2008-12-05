#include <stdio.h>
#include <linuxarm.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "error.h"
#include "log.h"

static struct log_struct log;

static void down_log_lock()
{
	while (log->lock == TRUE) {
		sleep(1);
	} 
	log->lock = TRUE;
}
static void up_log_lock()
{
	log->lock = FALSE;
}
boolean armserver_init_log(char *usename)
{
	/* 初始化log结构体 */
	/* 创建配置文件 */
	char log_conf_dir[30] = "_log.conf";
	char log_dir[30] = "_log";
	char use_dir[50] = '\0';
	char temp_dir[50] = '\0';
	char *conf_sentence = "file_max_size:";
	int fd;
	FILE *fp;
	struct stat buf;
	
	/* 初始化log_struct 结构体 */
	strcpy(log->dir, LOGHOME_DIR);
	strcpy(log->usename, usename);
	log->lock = FALSE;
	log->file_size = FILE_SIZE;
	
	/* 创建用户日志主目录 */
	strcpy(use_dir, log->dir);
	use_dir[strlen(use_dir)] = '/';
	strcpy(use_dir, log->usename);
	if ((fd = open(use_dir, O_RDONLY)) == -1) { 
		/* 不存在用户日志目录，创建 */
		mkdir(use_dir,0700);
	} else if {
		fstat(fd, &buf);
		if (buf.st_mode != S_IFDIR)
			print_error(ENOINIT, "It is a file\n");
		close(fd);
	}
	
	strcpy(temp_dir, use_dir);

	/* 创建用户日志配置文件 */
	log_conf_dir = strcat(usename, log_conf_dir);
	temp_dir[strlen(temp_dir)] = '/';
	strcpy(temp_dir, log_conf_dir);
	if ((fd = open(temp_dir, O_RDONLY)) == -1) {
		/* 打开失败，不存在用户日志配置文件 */
		fp = fopen(temp_dir, "w");
		fprintf(fp,"%s%d", conf_sentence, log->file_size);
		fclose(fp);
	} else if {
		lseek(fd, strlen(conf_sentence), SEEK_SET);
		read(fd, log->file_size, sizeof(log->file_size));
		close(fd);
	}
	/* 创建用户日志文件 */
	strcpy(temp_dir, use_dir);
	temp_dir[strlen(temp_dir)] = '/';
	log_dir = strcat(usename, log_dir);
	strcpy(temp_dir, log_dir);
	if (fd = open(temp_dir, O_RDONLY) == -1) {
		fp = fopen(temp_dir, "w+");
		log->fp=fp;
	} else if {
		fstat(fd, &buf);
		if (buf.st_size >= log->file_size*1024) {
			fp = fopen(temp_dir, "w+");			
		}
		log->fp = fp; 
}
boolean write_log(char *log_data);
{
	/* 写入信息 */
	time_t timep;
	
	time(&timep);
	down_log_lock();
	if (fprintf(log.fp, "%s : %s\n", ctime(&timep), log_data) == -1) {
		print_error(EWARNING, "无法保存此日志信息");
		up_log_lock();
		return FALSE;
	}
	up_log_lock();
	return TRUE;
}
boolean close_log()
{
	if(fclose(log->fp) == -1)
		return FALSE;
	return TRUE;
}
