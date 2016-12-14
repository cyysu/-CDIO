#include "bsp_irq_priority.h"

static void TIM6_NVIC_Configuration(void);//���������岶��
static void TIM7_NVIC_Configuration(void);//���ڰ���ɨ��
void SetIrqAndPriority(void)
{
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;	//�ں��쳣
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//�����ж�����
	TIM6_NVIC_Configuration();//����
	TIM7_NVIC_Configuration();//����ɨ��
}

static void TIM6_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
 													
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	TIM6_1msIRQ_Config(5);
}
static void TIM7_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
 													
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	TIM7_1msIRQ_Config(3);
}


