//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103ZET6,����ԭ��ELITE STM32������,��Ƶ72MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��        DC5V/3.3V      //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI����
//     LCDģ��                STM32��Ƭ��    
//       SDA         ��          PB15         //Һ����SPI��������д�ź�
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//       LED         ��          PB9          //Һ������������źţ��������Ҫ���ƣ���5V��3.3V
//       SCK         ��          PB13         //Һ����SPI����ʱ���ź�
//       A0          ��          PB10         //Һ��������/��������ź�
//       RESET       ��          PB12         //Һ������λ�����ź�
//       CS          ��          PB11         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//      T_IRQ        ��          PC10         //�����������ж��ź�
//      T_DO         ��          PC2          //������SPI���߶��ź�
//      T_DIN        ��          PC3          //������SPI����д�ź�
//      T_CS         ��          PC13         //������Ƭѡ�����ź�
//      T_CLK        ��          PC0          //������SPI����ʱ���ź�
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#ifndef __LCD180_H
#define __LCD180_H		
#include "sys.h"	 
#include "stdlib.h"

//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				  //LCD ID
	u8  dir;			  //���������������ƣ�0��������1��������	
	u16	 wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��	 
}_180lcd_dev; 	

//LCD����
extern _180lcd_dev lcd180dev;	//����LCD��Ҫ����
/////////////////////////////////////�û�������///////////////////////////////////	 
#define USE_HORIZONTAL  	 0 //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת

//////////////////////////////////////////////////////////////////////////////////	  
//����LCD�ĳߴ�
#define LCD180_W 130
#define LCD180_H 161

//TFTLCD������Ҫ���õĺ���		   
extern u16  POINT180_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK180_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 
#define GPIO_TYPE  GPIOF  //GPIO������
#define LED      0        //�����������        PB9
#define LCD180_CS   3       //Ƭѡ����            PB11
#define LCD180_RS   1       //�Ĵ���/����ѡ������ PB10 
#define LCD180_RST  2       //��λ����            PB12


//QDtechȫϵ��ģ������������ܿ��Ʊ��������û�Ҳ���Խ�PWM���ڱ�������
#define	LCD180_LED PFout(LED) //LCD����    		 PB9
//���ʹ�ùٷ��⺯���������еײ㣬�ٶȽ����½���14֡ÿ�룬���������˾�Ƽ�����
//����IO����ֱ�Ӳ����Ĵ���������IO������ˢ�����ʿ��Դﵽ28֡ÿ�룡 

//GPIO��λ�����ߣ�
#define	LCD180_CS_SET  GPIO_TYPE->BSRR=1<<LCD180_CS    //Ƭѡ�˿�  	PB11
#define	LCD180_RS_SET	GPIO_TYPE->BSRR=1<<LCD180_RS    //����/����  PB10	  
#define	LCD180_RST_SET	GPIO_TYPE->BSRR=1<<LCD180_RST   //��λ			  PB12

//GPIO��λ�����ͣ�							    
#define	LCD180_CS_CLR  GPIO_TYPE->BRR=1<<LCD180_CS     //Ƭѡ�˿�  	PB11
#define	LCD180_RS_CLR	GPIO_TYPE->BRR=1<<LCD180_RS     //����/����  PB10	 
#define	LCD180_RST_CLR	GPIO_TYPE->BRR=1<<LCD180_RST    //��λ			  PB12

//������ɫ
#define WHITE180       0xFFFF
#define BLACK180      	0x0000	  
#define BLUE180       	0x001F  
#define BRED180        0XF81F
#define GRED180 			 	0XFFE0
#define GBLUE180			 	0X07FF
#define RED180         0xF800
#define MAGENTA180     0xF81F
#define GREEN180       0x07E0
#define CYAN180        0x7FFF
#define YELLOW180      0xFFE0
#define BROWN180 			0XBC40 //��ɫ
#define BRRED180 			0XFC07 //�غ�ɫ
#define GRAY180  			0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE180      	 0X01CF	//����ɫ
#define LIGHTBLUE180      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE180       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN180     	0X841F //ǳ��ɫ
#define LIGHTGRAY180     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY180 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE180      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE180          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
void LCD180_Init(void);
void LCD180_DisplayOn(void);
void LCD180_DisplayOff(void);
void LCD180_Clear(u16 Color);	 
void LCD180_SetCursor(u16 Xpos, u16 Ypos);
void LCD180_DrawPoint(u16 x,u16 y);//����
u16  LCD180_ReadPoint(u16 x,u16 y); //����
void LCD180_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD180_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD180_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);

u16 LCD180_RD_DATA(void);//��ȡLCD����									    
void LCD180_WriteReg(u8 LCD180_Reg, u16 LCD180_RegValue);
void LCD180_WR_DATA(u8 data);
u16 LCD180_ReadReg(u8 LCD180_Reg);
void LCD180_WriteRAM_Prepare(void);
void LCD180_WriteRAM(u16 RGB_Code);
u16 LCD180_ReadRAM(void);		   
u16 LCD180_BGR2RGB(u16 c);
void LCD180_SetParam(void);
void LCD180_WriteData_16Bit(u16 Data);
void LCD180_direction(u8 direction );

//�����Ȼ�����ٶȲ����죬����ʹ������ĺ궨��,����ٶ�.
//ע��Ҫȥ��lcd.c��void LCD_WR_DATA(u16 data)��������Ŷ
/*
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//ʹ��16λ������������ģʽ
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
#endif
*/
				  		 
#endif  
	 
	 



