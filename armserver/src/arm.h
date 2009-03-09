/*
 * linuxarms/armserver/src/arm.h
 * Niu Tao:niutao0602@gmail.com 
 */

#ifndef _EPC_ARM_H
#define _EPC_ARM_H
#include "config.h"
#include "beepdrv.h"
#include "leddrv.h"

#ifdef HAS_EPC8000
#  define EPC8000
#endif
void init_led_beep();
void close_led_beep();
void led_clew_open();
void led_clew_close();
void beep_ring();
void beep_close();
#endif
