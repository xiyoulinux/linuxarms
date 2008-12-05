#ifndef _LOG_H
#define _LOG_H
#include "linuxarms.h"

#define USE_NAME_LEN 20
#define DIR_LEN 30	
#define FILE_SIZE 2
#define LOGHOME_DIR "/root/linuxarms-armserver"

struct log_struct {
	boolean lock;
	char dir[DIR_LEN];
	char usename[USE_NAME_LEN];
	FILE *fp;
	int file_size;
};

boolean armserver_init_log(char *usename);
boolean write_log(char *log_data); 
boolean close_log(struct log_struct *log);
#endif
