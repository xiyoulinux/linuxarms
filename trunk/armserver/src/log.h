#ifndef _LOG_H
#define _LOG_H
#include "linuxarms.h"
#include "thread.h"
#include "stdio.h"

#define FILE_SIZE 2
#define LOGHOME_DIR "/root/linuxarms-armserver"

struct log_struct {
	struct linuxarms_thread log_thread;
	FILE *log_fp;
	int log_file_size;
	boolean log_inited;
};


boolean armserver_init_log(const char *usename);
boolean write_log(const char *log_data); 
boolean close_log(void);
#endif
