#include <stdlib.h>
#include "linuxarms.h"
#include "thread.h"
#include "error.h"

#define LINUXARMS_THREAD_POINTER(thread) do {              \
	if (!(thread)) {                                   \
		print_error(EWARNING, "invalid parameter");\
		return;                                    \
	}                                                  \
} while (0)

void linuxarms_thread_init(struct linuxarms_thread *thread)
{
	LINUXARMS_THREAD_POINTER(thread);
	thread->state = FALSE;
	thread->lock = linuxarms_thread_lock_create();
	thread->id = NULL;
}

#ifdef GTK_THREAD

boolean linuxarms_thread_create(THREADFUNC func, void *arg)
{
	if ( g_thread_create((void *)func, arg, TRUE, NULL) == NULL)
		return FALSE;
	return TRUE;
}

linuxarms_thread_t *linuxarms_thread_self()
{
	return g_thread_self();
}
void linuxarms_thread_exit(struct linuxarms_thread *thread)
{
	//g_thread_join(thread);
	LINUXARMS_THREAD_POINTER(thread);
	g_thread_join(thread->id);
	/*if (linuxarms_thread_trylock(thread))
		linuxarms_thread_unlock(thread);*/
	linuxarms_thread_lock_free(thread);
	thread->id = NULL;
}

linuxarms_thread_lock_t *linuxarms_thread_lock_create(void)
{
	return g_mutex_new(); 
}
void linuxarms_thread_lock(struct linuxarms_thread *thread)
{
	LINUXARMS_THREAD_POINTER(thread);
	thread->state = TRUE;
	g_mutex_lock(thread->lock);
}
void linuxarms_thread_unlock(struct linuxarms_thread *thread)
{
	LINUXARMS_THREAD_POINTER(thread);
	g_mutex_unlock(thread->lock);
	thread->state = FALSE;
}
boolean linuxarms_thread_trylock(struct linuxarms_thread *thread)
{
	if (!thread)
		return TRUE;
	return thread->state;
}
void linuxarms_thread_lock_free(struct linuxarms_thread *thread)
{
	LINUXARMS_THREAD_POINTER(thread);
	g_mutex_free(thread->lock);
}
#else
static void *pthread_ret;
boolean linuxarms_thread_create(THREADFUNC func, void *arg)
{
	pthread_t pthread;
	if (pthread_create(&pthread, NULL, func, arg) == -1) {
		return FALSE;
	}
	return TRUE;
}
linuxarms_thread_t *linuxarms_thread_self()
{
	pthread_t *pthread;
	pthread = (pthread_t *)malloc(sizeof(pthread_t));
	*pthread = pthread_self();
	return pthread;
}
void linuxarms_thread_exit(struct linuxarms_thread *thread)
{
	//pthread_join(thread, &pthread_ret);
	LINUXARMS_THREAD_POINTER(thread);
	if (linuxarms_thread_trylock(thread))
		linuxarms_thread_unlock(thread);
	linuxarms_thread_lock_free(thread);
	thread->id = NULL;
}
linuxarms_thread_lock_t *linuxarms_thread_lock_create(void)
{
	pthread_mutex_t *lock;
	lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(lock, NULL);
	return lock;
}
void linuxarms_thread_lock(struct linuxarms_thread *thread)
{
	LINUXARMS_THREAD_POINTER(thread);
	thread->state = TRUE;
	pthread_mutex_lock(thread->lock);
}
void linuxarms_thread_unlock(struct linuxarms_thread *thread)
{
	LINUXARMS_THREAD_POINTER(thread);
	pthread_mutex_unlock(thread->lock);
	thread->state = FALSE;
}
boolean linuxarms_thread_trylock(struct linuxarms_thread *thread)
{
	if (!thread)
		return TRUE;
	return thread->state;
}
void linuxarms_thread_lock_free(struct linuxarms_thread *thread)
{
	LINUXARMS_THREAD_POINTER(thread);
	free(thread->lock);
}
#endif
