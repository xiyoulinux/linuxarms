#ifndef _LOG_H
#define _LOG_H

#define USE_NAME_LEN 20
#define DIR_LEN 30	
#define FILE_SIZE 2
struct log_struct {
	int lock;
	char dir[DIR_LEN];
	char usename[USE_NAME_LEN];
	int fd;
	int file_size;
	void (*down_log_lock)(struct log_struct *); 
	void (*up_log_lock)(struct log_struct *);
};

boolean armserver_init_log(char *usename);
boolean write_log(char *log_data); 
boolean clock_log();
#endif
