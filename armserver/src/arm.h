#ifndef _EPC_ARM_H
#define _EPC_ARM_H
/*
 * if the armserver run in EPC-8000(which is developed by
 * ZLG http://www.zlgmcu.com/),you can use the led or beep 
 * to clew user login or file transfer success.you should 
 * define EPC8000 to use it.In other arm syatem,you should
 * not define it.
 */ 
#define EPC8000

#include <stdio.h>

#ifdef EPC8000
/*
 * open led
 */
static void led_clew_open()
{
	printf("led open\n");
}
/*
 * close led
 */ 
static void led_clew_close()
{
	printf("led close\n");
}
/*
 * open beep
 */
static void beep_ring()
{
	printf("beep ring\n");
}

#else
static void led_clew_open()
{
}

/*
 * close led
 */ 
static void led_clew_close()
{

}
/*
 * open beep
 */
static void beep_ring()
{

}
#endif
#endif
