#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

extern void bsp_InitTimer(void);
extern void Delay_us(__IO u32 nTime);

#define Delay_ms(x) Delay_us(1000*x)	 //��λms

#endif /* __SYSTICK_H */
