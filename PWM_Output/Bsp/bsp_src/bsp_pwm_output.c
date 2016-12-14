#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_pwm_output.h" 
#include "myPidController.h"
#include "bsp_irq_priority.h"

static void TIM3_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	/*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIO_ResetBits(GPIOB, GPIO_Pin_0 |GPIO_Pin_1);
}

static void TIM3_Mode_Config(uint16_t CCR3_Val, uint16_t CCR4_Val)//3��4ͨ����ռ�ձ�
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	/* ----------------------------------------------------------------------- 
	TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR+1)* 100% = %
	TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR+1)* 100% = %
	----------------------------------------------------------------------- */

	/* Time base configuration */		 
	TIM_TimeBaseStructure.TIM_Period = 100-1;   
	TIM_TimeBaseStructure.TIM_Prescaler = 72;	  //����Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	                 //��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //����ʱ������ֵС��CCRx_ValʱΪ�ߵ�ƽ
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	                 //ʹ��ͨ��3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);/* CH3Ԥװ��ʹ�� */

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);/* CH4Ԥװ��ʹ�� */

	TIM_ARRPreloadConfig(TIM3, ENABLE);			  //ʹ��TIM3��ARR�ϵ�Ԥװ�ؼĴ���
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

/**
  * @brief  TIM3 ���PWM�źų�ʼ��
  * @param  none
  * @retval none
  */
void TIM3_PWM_Init(void)
{
	TIM3_GPIO_Config();
	TIM3_Mode_Config(0, 0);	//10*1000Hz  ch3��ch4
}

static void TIM3_pwm_duty(TIM_TypeDef* TIMx, TIM_CHn_e ch, uint16_t duty)
{ 
	/* Check the parameters */
    assert_param(IS_TIM_LIST6_PERIPH(TIMx));
    /* Set the Capture Comparex Register value */
	switch(ch)
	{
		/*
		case TIM_CH1:
			TIMx->CCR1 = duty;
			break;
		case TIM_CH2:
			TIMx->CCR2 = duty;
			break;
		*/
		case TIM_CH3:
			TIMx->CCR3 = duty;
			break;
		case TIM_CH4:
			TIMx->CCR4 = duty;
			break;
		default:
			break;			
	}
}

void motor_go(short motor_pwm_out)//�������
{

	 if (motor_pwm_out > 0)
		{ 
				TIM3_pwm_duty(TIM3,TIM_CH3,(uint32_t)motor_pwm_out);
				TIM3_pwm_duty(TIM3,TIM_CH4,0);  
    } 
	  else 
		{
				TIM3_pwm_duty(TIM3,TIM_CH3,0); 
				TIM3_pwm_duty(TIM3,TIM_CH4,(uint32_t)(fabs(motor_pwm_out)));  
    }
		
}

/*********************************************************************************
�������ܣ�bangbang_control
��ڲ�����none
���ڲ�����none
ʱ�䣺2016_01_29
*********************************************************************************/
void BangBangGo(short x) 
{ 
    motor_go(x);
}

void BangBangBack(short y)  
{        
    motor_go(-y);  
}

/*
 *�������ܣ��ƶ�
 *ʱ�䣺2016_01_21
 */
void motor_stop(void)
{
    DisableInterrupts   
	DesiredSpeed = 0;
    //GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1); //û�г�ʼ��֮ǰ�ǲ���ʹ�õ�
}

/**************5ms�����Ӻ���***************/
void velocity_measurement_printf(void)
{
			short pwm = 0;
			Motor.backpulse = -(short)TIM2->CNT;//���������岶��
    	TIM_SetCounter(TIM2,0);
			if (0 != Motor.backpulse) 
			{//���С���𲽣��ͷ�������
        (void)printf("�յ� %d ��pulse��\n",(uint32_t)Motor.backpulse);
			}

			pwm = PIDCalc(&Motor);
 			//printf("pwm = %d",pwm);
      motor_go(pwm);//����

}
/*********************************************END OF FILE**********************/
