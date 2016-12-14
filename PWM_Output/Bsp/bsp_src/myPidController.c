#include "myPidController.h"
#include "bsp_usart.h"

uint8_t Stop_Flag = 0;
float global_test = 0;
PID_Motor Motor =
{      
    6,  //P   46
    1,   //I   10
    0,   //D   1
    500,    //�����������
    800,    //΢���������
    0,0,0,0,0,0,0,0
};
//�ջ�
MySpeed speed0 = {
    120,//zhangai
    90,//qiao
    160,//zhidao
    160,//shizi
    115,//ruwan
    105,//wandao
    90 //minspeed 
};
float DesiredSpeed = 0; //��ʼ��ϣ�����Ƶ��ٶ�


/*
 *�������ܣ�����������ռ�ձȣ�������
 *ʱ�䣺2016_08_03
 */
short PIDCalc(PID_Motor *pp)
{	
	 static float Bias,Last_bias;
	 //Bias = Encoder - Target;                //����ƫ��
	 //pp->setpulse = 500;
	 Bias = pp->setpulse - pp->backpulse;
	 pp->out += pp->P * (Bias-Last_bias) + pp->I * Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return pp->out;                         //�������
}         
