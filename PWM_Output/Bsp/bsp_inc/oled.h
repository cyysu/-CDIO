#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"
#include "sys.h"



#define OLED_ADDRESS	0x78  //DC ��ӦSA0       R/W��   Ĭ��д

void OLED_WriteOneByte(u8 WriteAddr,u8 DataToWrite);
void OLED_WriteCmd(unsigned char I2C_Command);//д����
void OLED_WriteDat(unsigned char I2C_Data);//д����
void OLED_Init(void);
void OLED_Fill(unsigned char fill_Data);//ȫ�����
void OLED_CLS(void);//����
void OLED_ON(void);
void OLED_OFF(void);
void OLED_SetPos(unsigned char x, unsigned char y); //������ʼ������
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);


#endif
