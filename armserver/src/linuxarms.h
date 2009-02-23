#ifndef _LINUXARMS_H
#define _LINUXARMS_H

#ifndef __GTK_H__
	#define FALSE (0)
	#define TRUE !(FALSE)
	typedef int boolean;
#else
	typedef gboolean boolean;
#endif

struct login_struct;
struct amthread_struct;
struct afthread_struct;
struct asthread_struct;
struct acthread_struct;

struct linuxarms_struct {
	struct login_struct *login;
	struct amthread_struct *amthread;
	struct asthread_struct *asthread;
	struct afthread_struct *afthread;
	struct acthread_struct *acthread;
	
};

#define LINUXARMS_CHAR(p) do{                           \
	if (!(p)) {                                     \
		debug_where();                          \
		print_error(EWARNING, "无效的字符指针");\
		return FALSE;                           \
	}                                               \
}while(0)

#define LINUXARMS_POINTER(p) do{                             \
	if (!(p)) {                                          \
		debug_where();                               \
		print_error(EWARNING, "%s : 无效的指针", #p);\
		return FALSE;                                \
	}                                                    \
}while(0)
#endif
