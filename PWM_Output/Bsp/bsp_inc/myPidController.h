#ifndef __MYPIDCONTROLLER_H
#define __MYPIDCONTROLLER_H

#include "math.h" 
#include "stm32f10x.h"
/*******************************************************************************
//����ֵ�ʵ�ַ���(�ǻ��ַ���ĸĽ�)��ƫ��󣬻��ּ�������ȫ�ޣ�ƫ��С�����ּ�ǿ��
��Ӧ����ƫ���С���ı�����ٶ�
*******************************************************************************/
typedef struct
{
    short P;
    short I;
    short D;
    //�ɽ���������΢������ϵ������ͬ���Ŵ���λ��ԭ��Ҳ���Ա��⸡������
    short I_limit;        //��������������
    short D_limit;        
    short setpulse;       
    short backpulse;      
    short error;          
    short last_error;     //��һ���ٶ����
    short prev_error;     //���ϴ����
    short d_error;        //�ٶ����֮��
    short i_error;        //�ٶ����֮��
    short out;            //����������ռ�ձ�(0~990)���ѹֵ���ֲ��������Թ�ϵ
}PID_Motor;              
extern PID_Motor Motor;

//�ջ��ٶȲ���
#pragma anon_unions
typedef union MySpeed
{
	struct
	{
		float zhangai;//�ϰ��ٶ�
		float qiao;   //�ŵ��ٶ�
		float zhidao; //ֱ���ٶ�
		float shizi;  //ʮ���ٶ�
		float ruwan;  //�����ٶ�
		float wandao; //����ٶ�
		float zuixiao;//��С�ٶ�
	};
	float a[7];
}MySpeed;
extern MySpeed speed0,speed1,speed2,speed3,speed4;
extern float DesiredSpeed;//Ŀ���ٶ�
extern uint8_t Stop_Flag;

extern short MotorPIDCalc(PID_Motor *pp);
extern short PIDCalc(PID_Motor *pp);

#endif
