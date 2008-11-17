#include "error.h"

void print_error(int error, const char *text)
{
	switch (error) {
	case ESUCCESS:
		break;
	case ESYSERR:
		break;
	case ENOUSER:
		break;
	case ENOINIT:
		break;
	case ENOSOCKET:
		break;
	case ENULL:
		break;
	default:
	break;
	}
}
