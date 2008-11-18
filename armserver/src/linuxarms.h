#ifndef _LINUXARMS_H
#define _LINUXARMS_H

#include <string.h>
/* typedef a boolean type */
typedef enum _boolean{FALSE,TRUE}boolean;

enum error {
	ESUCESS,
	ENOUSER,
	ENOINIT,
	ENOSOCKET,
	ENULL,
};
struct armip_struct {
	char ip[15];
	int port;
	int tcp_fd;
};

#endif
