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
#include "lcd180.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "key.h" 
//#include "pic.h"

//========================variable==========================//
u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//������ɫ����
//=====================end of variable======================//

/*****************************************************************************
 * @name       :void DrawTestPage(u8 *str)
 * @date       :2018-08-09 
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/ 
void DrawTestPage(u8 *str)
{
//���ƹ̶���up
LCD180_Clear(WHITE);
LCD180_Fill(0,0,lcd180dev.width,20,BLUE);
//���ƹ̶���down
LCD180_Fill(0,lcd180dev.height-20,lcd180dev.width,lcd180dev.height,BLUE);
POINT180_COLOR=WHITE;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//������ʾ
Gui_StrCenter(0,lcd180dev.height-18,WHITE,BLUE,"www.lcdwiki.com",16,1);//������ʾ
//���Ʋ�������
//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

/*****************************************************************************
 * @name       :void main_test(void)
 * @date       :2018-08-09 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void main_test(void)
{
	DrawTestPage("�ۺϲ��Գ���");	
	Gui_StrCenter(0,23,RED,BLUE,"ȫ������",16,1);//������ʾ
	Gui_StrCenter(0,40,RED,BLUE,"�ۺϲ��Գ���",16,1);//������ʾ	
	Gui_StrCenter(0,57,GREEN,BLUE,"1.8\" ST7735S",16,1);//������ʾ
	Gui_StrCenter(0,74,GREEN,BLUE,"128X160",16,1);//������ʾ
	Gui_StrCenter(0,91,BLUE,BLUE,"2018-08-22",16,1);//������ʾ
	delay_ms(1500);		
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-09 
 * @function   :Color fill test(white,black,red,green,blue)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	//DrawTestPage("����1:��ɫ������");
	LCD180_Fill(0,0,lcd180dev.width,lcd180dev.height,WHITE);
	Show_Str1(20,30,BLUE,YELLOW,"BL Test",16,1);delay_ms(800);
	LCD180_Fill(0,0,lcd180dev.width,lcd180dev.height,RED);
	Show_Str1(20,30,BLUE,YELLOW,"RED ",16,1);delay_ms(800);
	LCD180_Fill(0,0,lcd180dev.width,lcd180dev.height,GREEN);
	Show_Str1(20,30,BLUE,YELLOW,"GREEN ",16,1);delay_ms(800);
	LCD180_Fill(0,0,lcd180dev.width,lcd180dev.height,BLUE);
	Show_Str1(20,30,RED,YELLOW,"BLUE ",16,1);delay_ms(800);
}

/*****************************************************************************
 * @name       :void Test_FillRec(void)
 * @date       :2018-08-09 
 * @function   :Rectangular display and fill test
								Display red,green,blue,yellow,pink rectangular boxes in turn,
								1500 milliseconds later,
								Fill the rectangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("GUI����������");
	LCD180_Fill(0,20,lcd180dev.width,lcd180dev.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		POINT180_COLOR=ColorTab[i];
		LCD180_DrawRectangle(lcd180dev.width/2-40+(i*16),lcd180dev.height/2-40+(i*13),lcd180dev.width/2-40+(i*16)+30,lcd180dev.height/2-40+(i*13)+30); 
	}
	delay_ms(1500);	
	LCD180_Fill(0,20,lcd180dev.width,lcd180dev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		POINT180_COLOR=ColorTab[i];
		LCD180_DrawFillRectangle(lcd180dev.width/2-40+(i*16),lcd180dev.height/2-40+(i*13),lcd180dev.width/2-40+(i*16)+30,lcd180dev.height/2-40+(i*13)+30); 
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-09 
 * @function   :circular display and fill test
								Display red,green,blue,yellow,pink circular boxes in turn,
								1500 milliseconds later,
								Fill the circular in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("GUI��Բ������");
	LCD180_Fill(0,20,lcd180dev.width,lcd180dev.height-20,WHITE);
	for (i=0; i<5; i++)  
		gui_circle(lcd180dev.width/2-40+(i*15),lcd180dev.height/2-25+(i*13),ColorTab[i],15,0);
	delay_ms(1500);	
	LCD180_Fill(0,20,lcd180dev.width,lcd180dev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(lcd180dev.width/2-40+(i*15),lcd180dev.height/2-25+(i*13),ColorTab[i],15,1);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void English_Font_test(void)
 * @date       :2018-08-09 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void English_Font_test(void)
{
	DrawTestPage("Ӣ����ʾ����");
	Show_Str(10,22,BLUE,YELLOW,"6X12:abcdefgh01234567",12,0);
	Show_Str(10,34,BLUE,YELLOW,"6X12:ABCDEFGH01234567",12,1);
	Show_Str(10,47,BLUE,YELLOW,"6X12:~!@#$%^&*()_+{}:",12,0);
	Show_Str(10,60,BLUE,YELLOW,"8X16:abcde01234",16,0);
	Show_Str(10,76,BLUE,YELLOW,"8X16:ABCDE01234",16,1);
	Show_Str(10,92,BLUE,YELLOW,"8X16:~!@#$%^&*()",16,0); 
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-09 
 * @function   :triangle display and fill test
								Display red,green,blue,yellow,pink triangle boxes in turn,
								1500 milliseconds later,
								Fill the triangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	u8 i=0;
	DrawTestPage("GUI Tri������");
	LCD180_Fill(0,20,lcd180dev.width,lcd180dev.height-20,WHITE);
	for(i=0;i<5;i++)
	{
		POINT180_COLOR=ColorTab[i];
		Draw_Triangel(lcd180dev.width/2-40+(i*15),lcd180dev.height/2-12+(i*11),lcd180dev.width/2-25-1+(i*15),lcd180dev.height/2-12-26-1+(i*11),lcd180dev.width/2-10-1+(i*15),lcd180dev.height/2-12+(i*11));
	}
	delay_ms(1500);	
	LCD180_Fill(0,20,lcd180dev.width,lcd180dev.height-20,WHITE); 
	for(i=0;i<5;i++)
	{
		POINT180_COLOR=ColorTab[i];
		Fill_Triangel(lcd180dev.width/2-40+(i*15),lcd180dev.height/2-12+(i*11),lcd180dev.width/2-25-1+(i*15),lcd180dev.height/2-12-26-1+(i*11),lcd180dev.width/2-10-1+(i*15),lcd180dev.height/2-12+(i*11));
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Chinese_Font_test(void)
 * @date       :2018-08-09 
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Chinese_Font_test(void)
{	
	DrawTestPage("������ʾ����");
	Show_Str(10,25,BLUE,YELLOW,"16X16:ȫ�����ӻ�ӭ",16,0);
	Show_Str(10,45,BLUE,YELLOW,"24X24:���Ĳ���",24,1);
	Show_Str(10,70,BLUE,YELLOW,"32X32:�������",32,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Pic_test(void)
 * @date       :2018-08-09 
 * @function   :picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
//void Pic_test(void)
//{
//	DrawTestPage("ͼƬ��ʾ����");
//	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
//	Gui_Drawbmp16(20,30,gImage_qq);
//	Show_Str(20+12,75,BLUE,YELLOW,"QQ",16,1);
//	Gui_Drawbmp16(70,30,gImage_qq);
//	Show_Str(70+12,75,BLUE,YELLOW,"QQ",16,1);
////	Gui_Drawbmp16(150,30,gImage_qq);
////	Show_Str(150+12,75,BLUE,YELLOW,"QQ",16,1);
//	delay_ms(1200);
//}

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09 
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
//void Rotate_Test(void)
//{
//	u8 i=0;
//	u8 *Direction[4]={"Rotation:0","Rotation:90","Rotation:180","Rotation:270"};
//	
//	for(i=0;i<4;i++)
//	{
//	LCD180_direction(i);
//	DrawTestPage("��Ļ��ת����");
//	Show_Str(20,30,BLUE,YELLOW,Direction[i],16,1);
//	Gui_Drawbmp16(30,50,gImage_qq);
//	delay_ms(1000);delay_ms(1000);
//	}
//	LCD180_direction(USE_HORIZONTAL);
//}
void Num_Test(void){
	int i=0;
	for(i=0;i<=10;i++){
	LCD180_Clear(WHITE);
	LCD180_ShowNum(1,1,i,2,16);
	delay_ms(1000);
	}
}

/*****************************************************************************
 * @name       :void Touch_Test(void)
 * @date       :2018-08-09 
 * @function   :touch test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
//void Touch_Test(void)
//{
//	u8 key;
//	u16 i=0;
//	u16 j=0;
//	u16 colorTemp=0;
//	TP_Init();
//	KEY_Init();
//	LED_Init();
//	DrawTestPage("����9:Touch(��KEY0У׼)      ");
//	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
//	POINT_COLOR=RED;
//	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,RED); 
//		while(1)
//	{
//	 	key=KEY_Scan();
//		tp_dev.scan(0); 		 
//		if(tp_dev.sta&TP_PRES_DOWN)			//������������
//		{	
//		 	if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
//			{	
//				if(tp_dev.x>(lcddev.width-24)&&tp_dev.y<16)
//				{
//					DrawTestPage("����9:Touch(��KEY0У׼)      ");//���
//					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
//					POINT_COLOR=colorTemp;
//					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
//				}
//				else if((tp_dev.x>(lcddev.width-60)&&tp_dev.x<(lcddev.width-50+20))&&tp_dev.y<20)
//				{
//				LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,ColorTab[j%5]); 
//				POINT_COLOR=ColorTab[(j++)%5];
//				colorTemp=POINT_COLOR;
//				delay_ms(10);
//				}

//				else TP_Draw_Big_Point(tp_dev.x,tp_dev.y,POINT_COLOR);		//��ͼ	  			   
//			}
//		}else delay_ms(10);	//û�а������µ�ʱ�� 	    
//		if(key==1)	//KEY_RIGHT����,��ִ��У׼����
//		{

//			LCD_Clear(WHITE);//����
//		    TP_Adjust();  //��ĻУ׼ 
//			TP_Save_Adjdata();	 
//			DrawTestPage("����9:Touch(��KEY0У׼)      ");
//			LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
//			POINT_COLOR=colorTemp;
//			LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
//		}
//		i++;
//		if(i==30)
//		{
//			i=0;
//			//break;
//		}
//	}   
//}




