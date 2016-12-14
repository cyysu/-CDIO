#include "bsp_keyboard.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "myPidController.h"
#include "lcd1602.h"

#if 1
uint8_t MatrixKey_value = 0;
void MatrixKeyboard_Init(void) 
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;

    GPIO_Init(GPIOE,&GPIO_InitStructure);		
}

static uint8_t MatrixKey_scan(void) 
{
	uint8_t KeyVal;	
	GPIO_Write(GPIOE,(GPIOE->ODR & 0xf0ff | 0x0f00)); 
	if((GPIOE->IDR & 0xf000)==0x0000) 
		return 0;
	else
	{	
	    Delay_ms(5);    
	    if((GPIOE->IDR & 0xf000)==0x0000)
	    return 0;
	}

	GPIO_Write(GPIOE,(GPIOE->ODR & 0xf0ff | 0x0100));
	switch(GPIOE->IDR & 0xf000)
	{
		case 0x1000: KeyVal='D';break;
		case 0x2000: KeyVal='C';break;
		case 0x4000: KeyVal='B';break;
		case 0x8000: KeyVal='A';break;
	}	   
	GPIO_Write(GPIOE,(GPIOE->ODR & 0xf0ff | 0x0200));
	switch(GPIOE->IDR & 0xf000)		     
	{
		case 0x1000: KeyVal='#';break;
		case 0x2000: KeyVal='9';break;
		case 0x4000: KeyVal='6';break;
		case 0x8000: KeyVal='3';break;
	}

	GPIO_Write(GPIOE,(GPIOE->ODR & 0xf0ff | 0x0400));	
	switch(GPIOE->IDR & 0xf000)		     
	{
		case 0x1000: KeyVal='0';break;
		case 0x2000: KeyVal='8';break;
		case 0x4000: KeyVal='5';break;
		case 0x8000: KeyVal='2';break;
	}
	GPIO_Write(GPIOE,(GPIOE->ODR & 0xf0ff | 0x0800));	
	switch(GPIOE->IDR & 0xf000)		     
	{
		case 0x1000: KeyVal='*';break;
		case 0x2000: KeyVal='7';break;
		case 0x4000: KeyVal='4';break;
		case 0x8000: KeyVal='1';break;
	}									  
	return KeyVal;		
}

void MatrixKeyDealPro(void)
{
	  static unsigned int set_Speed = 0;
		if(MatrixKey_scan() >= '0' && MatrixKey_scan() <= '9')
		{
				set_Speed = set_Speed * 10 + (MatrixKey_scan() - '0');
				if(set_Speed >= 600)
						set_Speed = 600;
				InfoDisplay((int32_t)set_Speed,(int32_t)Motor.backpulse);
				
		}
		else
		{
			 switch(MatrixKey_scan())
			 {
					 case 'A':
							printf("set_Speed = %d",set_Speed);
							Motor.setpulse = set_Speed ;
							set_Speed = 0;
							break;
					 
					 case 'B':
							printf("������B��");
							break;
					 
					 case 'C':
							printf("������C��");
							break;
					 
					 case 'D':
							printf("������D��");
							break;
						
					 case '#':
							printf("������#��");
							break;
					 
					 case '*':
							printf("������*��");
							break;
			 }
		}
}

#elif 0
void MatrixKey_Clock_Enable(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIOE->CRH &= 0X00000000;
    GPIOE->CRH |= 0X33333333;//���ø�8λ�������
    //��ֵ:��4λΪ��,�ε�4λΪ��
    GPIOE->ODR &= 0X00FF;
    GPIOE->ODR |= 0XF0FF;
	
    GPIOE->CRH &= 0X0000FFFF;//���12-15λ
    GPIOE->CRH |= 0X88880000;//��������
}

uint8_t scan_MatrixKey(void)
{
    uint8_t column, row;
    uint8_t tmp;
	uint8_t MatrixKey_value = 20;//��ʼֵ����Ϊ0~15 
	static uint8_t key_count = 0;

    tmp = KEYPORT;
    if (tmp != 0XF0)//�м�����
    {   
        //��ֹ����ʱ,�����������±������
        if (key_count <= 2)
        {
            key_count++;
        }
    }
    //����������������̧���������0 
    else
    {
        key_count = 0;
    }
    //����������2��ɨ������ڰ���״̬,����Ϊ����ȷʵ��������
    if (key_count == 2)
    {          
        column = tmp & 0XF000;//��ȡ������ 
        
        GPIOE->CRH &= 0X00000000;
        GPIOE->CRH |= 0X33333333;
		
        GPIOE->ODR &= 0X00FF;
        GPIOE->ODR |= 0X0F00;//��4λΪ��,�ε�4λΪ��

		GPIOE->CRH &= 0X0000FFFF;
		GPIOE->CRH |= 0X88880000;
								
        row = KEYPORT & 0X0F00;//��ȡ������                                       
        switch (column | row)  //��������
        {   
            case 0XEE: MatrixKey_value = '1'; break;
            case 0XDE: MatrixKey_value = '2'; break;
            case 0XBE: MatrixKey_value = '3'; break;                
            case 0X7E: MatrixKey_value = 'A'; break;                                              
            case 0XED: MatrixKey_value = '4'; break;
            case 0XDD: MatrixKey_value = '5'; break;
            case 0XBD: MatrixKey_value = '6'; break;
            case 0X7D: MatrixKey_value = 'B'; break;                                  
            case 0XEB: MatrixKey_value = '7'; break;
            case 0XDB: MatrixKey_value = '8'; break;
            case 0XBB: MatrixKey_value = '9'; break;
            case 0X7B: MatrixKey_value = 'C'; break;                                    
            case 0XE7: MatrixKey_value = '*'; break;
            case 0XD7: MatrixKey_value = '0'; break;
            case 0XB7: MatrixKey_value = '#'; break;
            case 0X77: MatrixKey_value = 'D'; break;
            default:   break;     
        }
			printf("��ֵ��%c\r\n",MatrixKey_value);
    } 
    //��û�а���������(������)��ɨ�������0
    if ((KEYPORT & 0XFF00) == 0xF000)
    {
        key_count = 0;  
    }
    return MatrixKey_value;
}
#endif

