#ifndef __LEDDRV_H
#define __LEDDRV_H
#include <linux/ioctl.h>

#define LED_IOC_MAGIC 	'l'

#define LED_TURN_ON_ALL		_IO(LED_IOC_MAGIC, 0)
#define LED_TURN_OFF_ALL	_IO(LED_IOC_MAGIC, 1)

/* only for GPO */
#define LED_TURN_ON		_IO(LED_IOC_MAGIC, 2)
#define LED_TURN_OFF		_IO(LED_IOC_MAGIC, 3)

#define LED_IOC_MAXNR	4

#endif //__LEDDRV_H

