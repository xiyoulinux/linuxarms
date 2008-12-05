#ifndef __BEEPDRV_H
#define __BEEPDRV_H
#include <linux/ioctl.h>

#define BEEP_IOC_MAGIC 	'p'

#define BEEP_RESET		_IO(BEEP_IOC_MAGIC, 0)
#define BEEP_CTRL		_IO(BEEP_IOC_MAGIC, 1)

#define BEEP_IOC_MAXNR	2

#endif //__BEEPDRV_H

