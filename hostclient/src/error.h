#ifndef _ERROR_H
#define _ERROR_H

enum error {
	ESUCCESS,
	ESYSERR,
	ENOUSER,
	ENOINIT,
	ENOSOCKET,
	ENULL,
};

void print_error(int error, const char *text);
#endif
