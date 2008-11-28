#ifndef _LINUXARMS_H
#define _LINUXARMS_H

//#define _HOST_CLIENT_

#ifdef _HOST_CLIENT_
typedef gboolean boolean;
#else
/* typedef a boolean type */
typedef enum _boolean {
	FALSE = 0,
	TRUE
} boolean;
#endif
#endif
