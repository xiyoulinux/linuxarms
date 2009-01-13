#ifndef _LINUXARMS_H
#define _LINUXARMS_H

#if (TRUE == 1) && (FALSE == 0)
#include <gtk/gtk.h>
typedef gboolean boolean;
#else
typedef enum _boolean {
	FALSE = 0,
	TRUE
}boolean;
#endif

typedef void *(*THREADFUNC)(void *);

#define GTK_THREAD

#ifdef GTK_THREAD
#include <gtk/gtk.h>
#define linuxarms_thread_t GThread
static linuxarms_thread_t *linuxarms_thread_create(THREADFUNC func, void *arg)
{
	return g_thread_create(func, arg, TRUE, NULL);
}

static boolean linuxarms_thread_exit(linuxarms_thread_t *thread)
{
	g_thread_exit((gpointer)thread);
	return TRUE;
}
#else
#include <pthread.h>
#define linuxarms_thread_t pthread_t
static void *pthread_ret;
static linuxarms_thread_t *linuxarms_thread_create(THREADFUNC func, void *arg)
{
	static linuxarms_thread_t pthread;
	if (pthread_create(&pthread, NULL, func, arg) == -1) {
		return NULL;
	}
	return &pthread;
}
static boolean linuxarms_thread_exit(linuxarms_thread_t *thread)
{
	pthread_exit(pthread_ret);
	return TRUE;
}
#endif
#endif
