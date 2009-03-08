#ifndef _LINUXARMS_THREAD_H
#define _LINUXARMS_THREAD_H
#include "linuxarms.h"

//#define GTK_THREAD

typedef void *(*THREADFUNC)(void *);

#ifdef GTK_THREAD
	#include <glib.h>
	typedef GThread linuxarms_thread_t;
	typedef GMutex linuxarms_thread_lock_t;
#else
	#include <pthread.h>
	typedef pthread_t linuxarms_thread_t;
	typedef pthread_mutex_t linuxarms_thread_lock_t;
#endif

struct linuxarms_thread {
	boolean state;
	linuxarms_thread_lock_t *lock;
	linuxarms_thread_t *id;
};

void linuxarms_thread_init(struct linuxarms_thread *thread);
boolean linuxarms_thread_create(THREADFUNC func, void *arg);
void linuxarms_thread_exit(struct linuxarms_thread *thread);
linuxarms_thread_t *linuxarms_thread_self();

linuxarms_thread_lock_t *linuxarms_thread_lock_create(void);
void linuxarms_thread_lock_free(struct linuxarms_thread *thread);

void linuxarms_thread_lock(struct linuxarms_thread *thread);
void linuxarms_thread_unlock(struct linuxarms_thread *thread);
boolean linuxarms_thread_trylock(struct linuxarms_thread *thread);

#endif
