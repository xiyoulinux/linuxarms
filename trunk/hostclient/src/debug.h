#ifndef _DEBUG_H
#define _DEBUG_H
/*
 * if you want output some debug information in you function,you
 * can include debug.h in your code file.when you had finshed your
 * function and do not want output debug information any all, you
 * can udefine _DEBUG_.
 */ 
#define _DEBUG_

#define debug_where() \
	printf("\n###DEBUG###[file name]###[line]###[function name]\n"); \
	printf("########### %s ### %d ### %s\n", __FILE__, __LINE__, __FUNCTION__);
#include <stdio.h>
#include <stdarg.h>

static inline void debug_print(const char *format, ...) __attribute__ ((format(printf, 1, 2)));

#ifdef _DEBUG_
static inline void debug_print(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}
#else
static inline void debug_print(const char *format, ...)
{

}
#endif
#endif
