#include "uart3.h"
#include "string.h"

esp_dev wifi_dev;
void uart3_init(void){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//�������ڿ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 
}

void USART3_IRQHandler(void){
	uint8_t res;
	wifi_dev.flag=0;
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	  res=USART_ReceiveData(USART3);
		
		if(wifi_dev.count<256){
		  wifi_dev.esp_buf[wifi_dev.count++]=res;
		}
}
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET){
	  USART3->SR;
    USART3->DR;
	  wifi_dev.flag=1;
	}
}


void USART3_SendString(uint8_t *arr,uint16_t len)
{
	  uint16_t i=0;
    while(i < len) {
    USART_SendData(USART3, arr[i++]);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
  }
}
void uart3_clear(void){
  wifi_dev.count=0;
	wifi_dev.flag=0;
	memset(wifi_dev.esp_buf,0,256);
}
