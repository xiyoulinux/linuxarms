/*
 * linuxarms/armserver/src/debug.h
 * Niu Tao:niutao0602@gmail.com
 */
#ifndef _DEBUG_H
#define _DEBUG_H
/*
 * if you want output some debug information in you function,you
 * can include debug.h in your code file.when you had finshed your
 * function and do not want output debug information any all, you
 * can udefine _DEBUG_.
 */ 

#include <stdio.h>
#include <stdarg.h>

static inline void debug_print(const char *format, ...) 
			__attribute__ ((format(printf, 1, 2)));
static inline void linuxarms_print(const char *format, ...) 
			__attribute__ ((format(printf, 1, 2)));

static inline void linuxarms_print(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	printf("linuxarms-armserver: ");
	vprintf(format, args);
	va_end(args);
}
#ifdef __DEBUG__
#define debug_where() \
		printf("\n###FILE: %s ###LINE: %d ###FUC: %s\n", \
		                  __FILE__, __LINE__, __FUNCTION__);
static inline void debug_print(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}
#else
#define debug_where() 
static inline void debug_print(const char *format, ...)
{

}
#endif
#endif
