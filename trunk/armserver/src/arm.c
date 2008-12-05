#include "arm.h"
#include "linuxarms.h"
#include "support.h"
#include <stdlib.h>

#define DEV_LED "/dev/led"
#define DEV_BEEP "/dev/beep"

#define DEV_NAME_LEN 256

struct epc8000_struct {
       int led;
       int beep;
       boolean inited;
};

static struct epc8000_struct epc8000;

static boolean load_drive()
{
	char *drv_led,drv_beep;
	int ret;
	char buf[256];

	drv_led = find_file("leddrv.ko");
	drv_beep = find_file("beepdrv.ko");
	if (!drv_led || !drv_beep) {
		debug_where();
		print_error(EWARNING, "没有发现驱动");
		epc8000.inited =FALSE;
		return FALSE;
	}
	snprintf(buf, 256, "insmod %s",drv_led);
	if (system(buf) ==-1) {
		epc8000.led = -1;
	}
	snprintf(buf, 256, "insmod %s",drv_beep);
	if (system(buf) ==-1) {
		epc8000.beep = -1;
	}
	epc8000.inited = TRUE;
	return TRUE;
}

#ifdef EPC8000
void init_led_beep()
{
	if (!load_drive()) {
		debug_where();
		print_eror(EWARNING, "无法使用led灯和蜂鸣器");
		return;
	}
	if ((epc8000.led = open(DEV_LED, O_RDWR)) == -1) {
		debug_where();
		print_error(EWARNING, "无法使用led灯");
		return;
	}
	if ((epc8000.beep = open(DEV_BEEP, O_RDWR)) == -1) {
		debug_where();
		print_error(EWARNING, "无法使用蜂鸣器");
		return;
	}
	epc8000.inited = TRUE;
}

void close_led_beep()
{
	if (epc8000.inited) {
		if (epc8000.led != -1)
			close(epc8000.led);
		if (epc8000.beep != -1)
			close(epc8000.beep);
	}
}
/*
 * 打开led灯
 */
void led_clew_open()
{
	if (epc8000.inited && epc8000.led != -1) {
		write(epc8000.led, "3", 1);
	}
}
/*
 * 熄灭led灯
 */ 
void led_clew_close()
{
	if (epc8000.inited && epc8000.led != -1) {
		write(epc8000.led, "0", 1);
	}
}
/*
 *  蜂鸣器鸣叫
 */
void beep_ring()
{
	if (ecp8000.inited && epc8000.beep != -1) {
		ioctl(fd, BEEP_CTRL, 1);
	}
}
/*
 * 蜂鸣器停止鸣叫
 */
void beep_close()
{
	if (ecp8000.inited && epc8000.beep != -1) {
		ioctl(fd, BEEP_CTRL, 0);
	}
}
#else
void init_led_beep() {}
void close_led_beep() {}
void led_clew_open() {}
void led_clew_close() {}
void beep_ring() {}
void beep_close() {}
#endif

