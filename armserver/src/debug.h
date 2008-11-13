#ifndef _DEBUG_H
#define _DEBUG_H
/*
 * if you want output some debug information in you function,you
 * can include debug.h in your code file.when you had finshed your
 * function and do not want output debug information any all, you
 * can udefine _DEBUG_.
 */ 
#define _DEBUG_

#include <stdio.h>
#include <stdarg.h>

inline void debug_print(const char *format, ...) __attribute__ ((format(printf, 1, 2)));

#ifdef _DEBUG_
inline void debug_print(const char *format, ...)
{
	va_list args;
	char buffer[256];
	va_start(args, format);
	vsnprintf(buffer, 256, format, args);
	va_end(args);
	printf(buffer);
}
#else
inline void debug_print(const char *format, ...)
{

}
#endif
#endif
