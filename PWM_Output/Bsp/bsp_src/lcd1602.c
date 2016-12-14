#include "Lcd1602.h"
#include "bsp_SysTick.h"
#include "myPidController.h"
/********************���˿ڶ���*********************************/
#define GPIO_EN       GPIOC                  //  ʹ�ܶ˿���
#define GPIO_EN_PIN   GPIO_Pin_10            //  ʹ�ܶ˿ں�
#define RCC_GPIO_EN   RCC_APB2Periph_GPIOC   //  ʹ��ʱ����

#define GPIO_RW       GPIOC                  //  ��/дѡ��˿���
#define GPIO_RW_PIN   GPIO_Pin_11            //  ��/дѡ��˿ں�
#define RCC_GPIO_RW   RCC_APB2Periph_GPIOC   //  ��/дʱ����

#define GPIO_RS       GPIOC                  //  ����/����˿���
#define GPIO_RS_PIN   GPIO_Pin_12            //  ����/����˿ں�
#define RCC_GPIO_RS   RCC_APB2Periph_GPIOC   //  ����/����ʱ����

#define GPIO_DATA_0       GPIOD         //  ������0_�˿���
#define GPIO_DATA_0_PIN   GPIO_Pin_0    //  ������0_�˿ں�
#define GPIO_DATA_1       GPIOD         //  ������1_�˿���
#define GPIO_DATA_1_PIN   GPIO_Pin_1    //  ������1_�˿ں�
#define GPIO_DATA_2       GPIOD         //  ������2_�˿���
#define GPIO_DATA_2_PIN   GPIO_Pin_2    //  ������2_�˿ں�
#define GPIO_DATA_3       GPIOD         //  ������3_�˿���
#define GPIO_DATA_3_PIN   GPIO_Pin_3    //  ������3_�˿ں�
#define GPIO_DATA_4       GPIOD         //  ������4_�˿���
#define GPIO_DATA_4_PIN   GPIO_Pin_4    //  ������4_�˿ں�
#define GPIO_DATA_5       GPIOD         //  ������5_�˿���
#define GPIO_DATA_5_PIN   GPIO_Pin_5    //  ������5_�˿ں�
#define GPIO_DATA_6       GPIOD         //  ������6_�˿���
#define GPIO_DATA_6_PIN   GPIO_Pin_6    //  ������6_�˿ں�
#define GPIO_DATA_7       GPIOD         //  ������7_�˿���
#define GPIO_DATA_7_PIN   GPIO_Pin_7    //  ������7_�˿ں�
#define RCC_GPIO_DATA   RCC_APB2Periph_GPIOD  //  ������ʱ����


/******************************************************************/


/***********************����ָ��***********************************/
#define SET_EN  GPIO_SetBits(GPIO_EN, GPIO_EN_PIN)		//EN ʹ��  ���1
#define CLE_EN  GPIO_ResetBits(GPIO_EN, GPIO_EN_PIN)	//      ���0 
#define SET_RW  GPIO_SetBits(GPIO_RW, GPIO_RW_PIN)      //RW ��д  ���1
#define CLE_RW  GPIO_ResetBits(GPIO_RW, GPIO_RW_PIN)	//      ���0
#define SET_RS  GPIO_SetBits(GPIO_RS, GPIO_RS_PIN)		//RS ָ��  ���1
#define CLE_RS  GPIO_ResetBits(GPIO_RS, GPIO_RS_PIN)	//      ���0
/******************************************************************/


/************************�˿ڳ�ʼ��*******************************/
void Lcd_GPIO_init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;   //�����ṹ��

    /********Data�˿�����*************/
	 RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE);  //�򿪶˿�Bʱ�� 
     GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_0_PIN|GPIO_DATA_1_PIN|GPIO_DATA_2_PIN|GPIO_DATA_3_PIN|GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB8~DB15
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //��׼���ģʽ
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
     GPIO_Init(GPIO_DATA_0, &GPIO_InitStructure);      //��ʼ���˿�

	 /********ʹ�ܶ˿�����**********/
	 RCC_APB2PeriphClockCmd(RCC_GPIO_EN, ENABLE);    //�򿪶˿�ʱ�� 
     GPIO_InitStructure.GPIO_Pin =  GPIO_EN_PIN;     // ʹ�ܶ˿�
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //��׼���ģʽ
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
     GPIO_Init(GPIO_EN, &GPIO_InitStructure);

	 /********��/д�˿�����**********/
	 RCC_APB2PeriphClockCmd(RCC_GPIO_RW, ENABLE);    //�򿪶˿�ʱ�� 
     GPIO_InitStructure.GPIO_Pin =  GPIO_RW_PIN;     // ʹ�ܶ˿�
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //��׼���ģʽ
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
     GPIO_Init(GPIO_RW, &GPIO_InitStructure);

	 /********ָ��/���ݶ˿�����**********/
	 RCC_APB2PeriphClockCmd(RCC_GPIO_RS, ENABLE);    //�򿪶˿�ʱ�� 
     GPIO_InitStructure.GPIO_Pin =  GPIO_RS_PIN;     // ʹ�ܶ˿�
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���츴�����
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
     GPIO_Init(GPIO_RS, &GPIO_InitStructure);


}
/******************************************************************/


void LCD_Init(void)
{  
 	Lcd_GPIO_init();
	Delay_us(1500);                     //��ʱ15ms
	Lcd_Write_Command( 0x38,0);       // дָ��38H �����æ�ź�
	Delay_us(500);				      //��ʱ5ms
    Lcd_Write_Command( 0x38,0);       // дָ��38H �����æ�ź�
	Delay_us(500);					  //��ʱ5ms
    Lcd_Write_Command( 0x38,0);       // дָ��38H �����æ�ź�
   									  //�Ժ�ÿ��дָ���/д���ݲ���֮ǰ����æ�ź�
	Lcd_Write_Command( 0x38,1);       //��ʾģʽ���� 
    Lcd_Write_Command( 0x08,1);		  //��ʾ�ر�
	Lcd_Write_Command( 0x01,1);       //��ʾ����
    Lcd_Write_Command( 0x06,1);       //��ʾ����ƶ����� 
    Lcd_Write_Command( 0x0C,1); 	  //��ʾ������겻��ʾ
} 
/******************************************************/

void Lcd_En_Toggle(void) //��ʹ������
{
	SET_EN;        //ʹ��1
	Delay_us(5);   //��ʱ160us
	CLE_EN;
}

void Lcd_Busy(void)//�ж�æ
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE);    //��DATA�˿�ʱ�� 

     GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_0_PIN|GPIO_DATA_1_PIN|GPIO_DATA_2_PIN|GPIO_DATA_3_PIN|GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB8~DB15
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //����ģʽ = ��������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
     GPIO_Init(GPIO_DATA_7, &GPIO_InitStructure);      //��æ���˿�

 	 CLE_RS; //RS = 0
	 //Delay_us(1);   //��ʱ10us
	 SET_RW; //RW = 1
	 //Delay_us(1);   //��ʱ10us
	 SET_EN; //EN = 1
	 //Delay_us(2);   //��ʱ20us
     while (GPIO_ReadInputDataBit(GPIO_DATA_7,GPIO_DATA_7_PIN)); //ѭ���ȴ�æ���˿� = 0
	 CLE_EN; //EN = 0

     //�ָ��˿�Ϊ���״̬ 
   	 RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE);    //��DATA�˿�ʱ�� 
	 GPIO_InitStructure.GPIO_Pin  = GPIO_DATA_0_PIN|GPIO_DATA_1_PIN|GPIO_DATA_2_PIN|GPIO_DATA_3_PIN|GPIO_DATA_4_PIN|GPIO_DATA_5_PIN|GPIO_DATA_6_PIN|GPIO_DATA_7_PIN; //  DB8~DB15
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
     GPIO_Init(GPIO_DATA_7, &GPIO_InitStructure);

} 

void Gpio_data(unsigned char x)  //�˿���������
{  
	GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_0_PIN);  //DB0
	GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_1_PIN);  //DB1
	GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_2_PIN);  //DB2
	GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_3_PIN);  //DB3
	GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_4_PIN);  //DB4
	GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_5_PIN);  //DB5
	GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_6_PIN);  //DB6
	GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_7_PIN);  //DB7
	if(x&0X01)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_0_PIN);//DB0
	if(x&0X02)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_1_PIN);//DB1
	if(x&0X04)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_2_PIN);//DB2
	if(x&0X08)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_3_PIN);//DB3
	if(x&0X10)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_4_PIN);//DB4
	if(x&0X20)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_5_PIN);//DB5
	if(x&0X40)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_6_PIN);//DB6
	if(x&0X80)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_7_PIN);//DB7
}


//��Һ������д��ָ��  ʱ��RS=L,RW=L,Data0-Data7=ָ���룬E=������
void Lcd_Write_Command(unsigned char x,char Busy) 
{ 
    if(Busy) Lcd_Busy();
    //Delay_us(1);   //��ʱ10us
	CLE_RS;  //RS = 0 
    //Delay_us(1);   //��ʱ10us
	CLE_RW;  //RW = 0 
    //Delay_us(4);   //��ʱ40us
    Gpio_data(x);  //�˿���������
    //Delay_us(4);   //��ʱ40us
	Lcd_En_Toggle();  //��ʹ������
    //Delay_us(1);   //��ʱ100us
	Lcd_Busy(); //��æ

}
//��Һ������д������  ʱ��RS=H,RW=L,Data0-Data7=ָ���룬E=������
void Lcd_Write_Data( unsigned char x) //��Һ������д������ 
{ 
	Lcd_Busy(); //��æ
    //Delay_us(1);   //��ʱ10us
	SET_RS;   //RS = 1 
    //Delay_us(1);   //��ʱ10us
    CLE_RW;   //RW = 0
    //Delay_us(4);   //��ʱ40us
    Gpio_data(x);
    //Delay_us(4);   //��ʱ40us
	Lcd_En_Toggle();  //��ʹ������
    //Delay_us(1);   //��ʱ100us
	Lcd_Busy(); //��æ

} 

void Lcd_SetXY(unsigned char x,unsigned char y)   //�ַ���ʼλ���趨��x��ʾ�У�y��ʾ�� 
{ 
     unsigned char addr; 
     if(y==0) 
          addr=0x80+x; 
     else if(y==1)
          addr=0xC0+x; 
     Lcd_Write_Command(addr,1) ; 
} 
/******************************************************/

void Lcd_Puts(unsigned char x,unsigned char y, unsigned char *string) //��1602дһ���ַ��� 
{ 
   //unsigned char i=0;
   Lcd_SetXY(x,y); 
	while(*string) 
	  { 
	   Lcd_Write_Data(*string); 
       string++; 
      } 
}


unsigned char strTemp2[2];
unsigned char strTemp3[3];
unsigned char strTemp4[4];
unsigned char strTemp5[5];
unsigned char strTemp6[6];
unsigned char strTemp7[7];

unsigned char *tr_2(unsigned int outData2)	
{
	strTemp2[0]=(unsigned char)(((outData2/10) % 10) + 0x30);
	strTemp2[1]=(unsigned char)((outData2%10) + 0x30);
	return strTemp2;
}

unsigned char *tr_3(unsigned int outData3)
{
	strTemp3[0]=(unsigned char)(((outData3/100) % 10) + 0x30);
	strTemp3[1]=(unsigned char)(((outData3/10) % 10) + 0x30);
	strTemp3[2]=(unsigned char)((outData3%10) + 0x30);
	return strTemp3;
}

unsigned char *tr_4(unsigned int outData4)
{
	strTemp4[0]=(unsigned char)(((outData4/1000)% 10) + 0x30);
	strTemp4[1]=(unsigned char)(((outData4/100)% 10) + 0x30);
	strTemp4[2]=(unsigned char)(((outData4/10)% 10) + 0x30);
	strTemp4[3]=(unsigned char)((outData4%10) + 0x30);
	return strTemp4;
}


unsigned char *tr_5(unsigned int outData5)
{
	strTemp5[0]=(unsigned char)(((outData5/10000)% 10) + 0x30);
	strTemp5[1]=(unsigned char)(((outData5/1000)% 10) + 0x30);
	strTemp5[2]=(unsigned char)(((outData5/100)% 10) + 0x30);
	strTemp5[3]=(unsigned char)(((outData5/10)% 10) + 0x30);
	strTemp5[4]=(unsigned char)((outData5%10) + 0x30);
	return strTemp5;
}

unsigned char *tr_6(unsigned int outData6)
{
	strTemp6[0]=(unsigned char)(((outData6/100000)% 10) + 0x30);
	strTemp6[1]=(unsigned char)(((outData6/10000)% 10) + 0x30);
	strTemp6[2]=(unsigned char)(((outData6/1000)% 10) + 0x30);
	strTemp6[3]=(unsigned char)(((outData6/100)% 10) + 0x30);
	strTemp6[4]=(unsigned char)(((outData6/10)% 10) + 0x30);
	strTemp6[5]=(unsigned char)((outData6%10) + 0x30);
	return strTemp6;
}

unsigned char *tr_7(unsigned int outData7)
{
	strTemp7[0]=(unsigned char)(((outData7/1000000)% 10) + 0x30);
	strTemp7[1]=(unsigned char)(((outData7/100000)% 10) + 0x30);
	strTemp7[2]=(unsigned char)(((outData7/10000)% 10) + 0x30);
	strTemp7[3]=(unsigned char)(((outData7/1000)% 10) + 0x30);
	strTemp7[4]=(unsigned char)(((outData7/100)% 10) + 0x30);
	strTemp7[5]=(unsigned char)(((outData7/10)% 10) + 0x30);
	strTemp7[6]=(unsigned char)((outData7%10) + 0x30);
	return strTemp7;
}

void InfoDisplay(int32_t a,int32_t b)
{
	//Lcd_Puts(0,0,"Set:");Lcd_Puts(4,0,tr_3((int32_t)Motor.setpulse));
	//Lcd_Puts(8,0,"Real:");Lcd_Puts(13,0,tr_3((int32_t)Motor.backpulse));
	Lcd_Write_Command( 0x01,1);       
	Lcd_Puts(0,0,"Set:");Lcd_Puts(4,0,tr_3(a));
	Lcd_Puts(8,0,"Real:");Lcd_Puts(13,0,tr_3(b));
	Delay_ms(200);
}
/******************************************************/
