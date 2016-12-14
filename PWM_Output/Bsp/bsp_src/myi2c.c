#include "myi2c.h"
#include "delay.h"

//I2C��ʼ��
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);	 //��PB6 7 �øߣ��൱��Ĭ������scl sda��������

	IIC_SDA = 1;
	IIC_SCL = 1;

}

//��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();				//��Ϊ�������
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(4);
	IIC_SDA = 0;				//sclΪ�ߵ�ƽʱ��SDA���ֵ͵�ƽ���䣬��ʾ���俪ʼ
	delay_us(5);
	IIC_SCL = 0;				   //ǯλSCL,����������������ݴ���

}
//��ֹ�ź�
void IIC_Stop(void)
{
	SDA_OUT();			   //��Ϊ�������
	IIC_SDA = 0;
	IIC_SCL = 1;
	delay_us(2);			  //sclΪ�ߵ�ƽ��SDA���ָߵ�ƽ���䣬��ʾ�������
	IIC_SDA = 1;
		
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u16 Errtime=0;
	SDA_IN();			//��ʱ����Ϊ����
	IIC_SCL = 1;
	IIC_SDA = 1;		//����Ĺ�ϵ
	if(READ_SDA)
	{
		Errtime++;
		if(Errtime > 250)
		{
			IIC_Stop();
			return 1;		
		}
	}
	else
	IIC_SCL = 0;	//ǯλ�������´δ���
	return 0;
}
//����Ӧ���ź�
void IIC_Ack(void)
{
	SDA_OUT();		 //��ʱ���൱�������ڽ������ݣ��Ǳ�����
	IIC_SCL = 0;
	IIC_SDA = 0;
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}
//������Ӧ���ź�
void IIC_NAck(void)
{
	SDA_OUT();		 //��ʱ���൱�������ڽ������ݣ��Ǳ�����
	IIC_SCL = 0;
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;	

}
//����һ���ֽ�����
void IIC_Send_Byte(u8 txd)
{
	u8 i;
	SDA_OUT();
	IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	for(i=0;i<8;i++)
	{
		if(txd & 0x80)
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
		txd <<= 1;
		delay_us(2);
		IIC_SCL = 1;
		delay_us(2);
		IIC_SCL = 0;
		delay_us(2);
	}
} 
//��һ���ֽ�����   ack = 1 ʱ������Ӧ��	   ack = 0  ,������Ӧ��
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL = 0;
		delay_us(2);
		IIC_SCL = 1;		  //�ߵ�ƽʱ�����Ѿ��ȶ����ʽ������ݵĶ�ȡ
		receive <<= 1;
		if(READ_SDA)
			receive++;	
	}
	if(ack)
		IIC_Ack();
	else
		IIC_NAck();
	return receive;

}





