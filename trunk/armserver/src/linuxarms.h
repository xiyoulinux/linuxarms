#ifndef _LINUXARMS_H
#define _LINUXARMS_H

//#define _HOST_CLIENT_

#ifdef _HOST_CLIENT_
#include <gtk/gtk.h>
typedef gboolean boolean;
#else
/* typedef a boolean type */
typedef enum _boolean {
	FALSE = 0,
	TRUE
} boolean;
#endif
/*
typedef enum _trans_state {
	STOP,
	CONTINUE,
}trans_state;
*/
#endif
