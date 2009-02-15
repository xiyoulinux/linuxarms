/*
 * linuxarms/armserver/src/arm.h
 * Niu Tao:niutao0602@gmail.com 
 */

#ifndef _EPC_ARM_H
#define _EPC_ARM_H
#include "beepdrv.h"
#include "leddrv.h"
/*
 * if the armserver run in EPC-8000(which is developed by
 * ZLG http://www.zlgmcu.com/),you can use the led or beep 
 * to clew user login or file transfer success.you should 
 * define EPC8000 to use it.In other arm syatem,you should
 * not define it.
 */ 
#define EPC8000
void init_led_beep();
void close_led_beep();
void led_clew_open();
void led_clew_close();
void beep_ring();
void beep_close();
#endif
