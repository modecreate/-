#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PAout(8)// PB5
#define FAN0 PBout(9)// PB5
#define FAN1 PBout(5)// PB5
#define LED1 PDout(2)// PE5	

void LED_Init(void);//��ʼ��
int get_LED0_status();
int get_LED1_status();
int get_FAN0_status();
int get_FAN1_status();
		 				    
#endif
