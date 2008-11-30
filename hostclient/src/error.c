/*
 * linuxarms/hostclient/src/error.c
 * 打印错误信息
 * Niu Tao<niutao0602@gmail.com>
 */
#include "error.h"
#include "debug.h"
#include <stdio.h>
//#include "statusbar.h"
/*
 * print_error  打印错误信息。如果出现严重错误，比如调用系统函数
 *              出错，建立网络连接，程序将进入死循环。
 * @error:      错误类型码，详见error.h
 * @text:       要打印的错误信息
 */
void print_error(int error, const char *text)
{
	switch (error) {
	case ESUCCESS:
		printf("Success: ");
		break;
	case ESYSERR:
		printf("Call system function error: ");
		for(;;);
		break;
	case ENOINIT:
		printf("Uninitialized: ");
		break;
	case ENOSOCKET:
		printf("No socket: ");
		for(;;);
		break;
	case ENULL:
		break;
	case EWARNING:
		printf("Warning: ");
	default:
		printf("The wrong type of error");
		break;
	}
	if (text)
		printf(text);
	printf("\n");
}
