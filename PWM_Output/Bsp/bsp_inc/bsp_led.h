#ifndef __BSP_LED_H
#define	__BSP_LED_H

#include "stm32f10x.h"

typedef enum
{
	LED0,
	LED1,
	LED2,
	LED3,
	LED_MAX,
}LED_e;

typedef enum LED_status
{
  LED_ON  =   0,            //����(��Ӧ�͵�ƽ)
  LED_OFF =   1             //�ư�(��Ӧ�ߵ�ƽ)
}LED_status; 

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define LED_GPIO                GPIOB
#define	digitalHi(p,i)			{p->BSRR=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BRR=i; }			//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR^=i;}			//�����ת״̬


extern void LED_GPIO_Config(void);
extern void led_set(LED_e ledn, LED_status status);
extern void led_turn(LED_e ledn);
#endif /* __LED_H */
