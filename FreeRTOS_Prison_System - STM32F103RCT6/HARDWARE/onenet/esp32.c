#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "esp32.h"
#include "delay.h"
#include "usart.h"
#include "uart3.h"


#define delay_ms delay_ms

#define ESP32_WIFI_INFO		"AT+CWJAP=\"12345\",\"12345678\"\r\n"

#define ESP32_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"


//==========================================================
//	�������ƣ�	ESP32_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP32_Clear(void)
{
	uart3_clear();
}

//==========================================================
//	�������ƣ�	ESP32_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP32_WaitRecive(void)
{

	if(wifi_dev.flag == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
	if(wifi_dev.flag)
		return REV_OK;
}

//==========================================================
//	�������ƣ�	ESP32_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP32_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	USART3_SendString((uint8_t*)cmd,strlen(cmd));
	
	while(timeOut--)
	{
		if(ESP32_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)wifi_dev.esp_buf, res) != NULL)		//����������ؼ���
			{
				ESP32_Clear();									//��ջ���
				printf("cmd:%s  res:%s\r\n",cmd,res);
				return 0;
			}
		}
		ESP32_Clear();
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	ESP32_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP32_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP32_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!ESP32_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		USART3_SendString((uint8_t*)data,len);		//�����豸������������
	}

}

//==========================================================
//	�������ƣ�	ESP32_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP32�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP32_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP32_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)wifi_dev.esp_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}

//==========================================================
//	�������ƣ�	ESP32_Init
//
//	�������ܣ�	��ʼ��ESP32
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP32_Init(void)
{
	
	GPIO_InitTypeDef GPIO_Initure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//ESP32��λ����
	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initure.GPIO_Pin = GPIO_Pin_4;					//GPIOC14-��λ
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_Initure);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
	delay_ms(250);
	GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
	delay_ms(500);
	
	ESP32_Clear();
	
	while(ESP32_SendCmd("AT\r\n", "OK"));
		delay_ms(500);
	
	while(ESP32_SendCmd("AT+CWMODE=1\r\n", "OK"));
		delay_ms(500);
	
	while(ESP32_SendCmd(ESP32_WIFI_INFO, "GOT IP"));
		delay_ms(500);
	
	while(ESP32_SendCmd(ESP32_ONENET_INFO, "CONNECT"));
		delay_ms(500);

}
