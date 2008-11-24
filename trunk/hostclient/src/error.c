/*
 * linuxarms/hostclient/src/error.c
 * 打印错误信息
 * Niu Tao<niutao0602@gmail.com>
 */
#include "error.h"
#include "debug.h"
//#include "statusbar.h"
void print_error(int error, const char *text)
{
	switch (error) {
	case ESUCCESS:
		debug_print("success\n");
		break;
	case ESYSERR:
		printf("调用系统函数出错\n");
		for(;;);
		break;
	case ENOINIT:
		printf("数据没有初始化\n");
		break;
	case ENOSOCKET:
		printf("没有建立网络连接\n");
		for(;;);
		break;
	case ENULL:
		break;
	case EWARNING:

//		statusbar_set_text(text);
	default:
	break;
	}
}
