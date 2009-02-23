#ifndef _LINUXARMS_H
#define _LINUXARMS_H
#include "debug.h"
#include "error.h"

#ifndef __GTK_H__
	#define FALSE (0)
	#define TRUE !(FALSE)
	typedef int boolean;
#else
	typedef gboolean boolean;
#endif

struct login_struct;
struct mwindow_struct;
struct hmthread_struct;
struct hfthread_struct;
struct hsthread_struct;
struct hcthread_struct;

struct linuxarms_struct {
	struct login_struct *login;
	struct mwindow_struct  *mwindow;
	struct hmthread_struct *hmthread;
	struct hfthread_struct *hfthread;
	struct hsthread_struct *hsthread;
	struct hcthread_struct *hcthread;
};

#define LINUXARMS_CHAR(p) do{                            \
	if (!(p)) {                                      \
		debug_where();                           \
		print_error(EWARNING, "无效的字符指针"); \
		return FALSE;                            \
	}                                                \
}while(0)

#define LINUXARMS_POINTER(p) do{                             \
	if (!(p)) {                                          \
		debug_where();                               \
		print_error(EWARNING, "%s : 无效的指针", #p);\
		return FALSE;                                \
	}                                                    \
}while(0)

#endif
