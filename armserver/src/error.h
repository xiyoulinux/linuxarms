#ifndef _ERROR_H
#define _ERROR_H
/*
 * 函数执行过程中的错误信息
 */
enum error {
	ESUCCESS, /* 执行成功 */
	ESYSERR,  /* 执行系统函数出错 */
	ENOINIT,  /* 初始化错误 */
	ENOSOCKET,/* 没有建立网络连接 */ 
	ENULL,    /* 无效的数据 */
	EWARNING  /* 警告 */
};

void print_error(int error, const char *format, ...)
		__attribute__ ((format(printf, 2, 3)));
#endif
