#include "led.h"
#include "string.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
//RTOSͷ�ļ�
#include "FreeRTOS.h"
#include "task.h" 	
#include "limits.h"
//NET
#include "esp32.h"
#include "onenet.h"
#include "uart3.h"
 
 
#define START_TASK_PRIO        1                    //�������ȼ�
#define START_STK_SIZE         256              //����ջ��С    
TaskHandle_t StartTask_Handler;            //������
void start_task(void *pvParameters);//������

#define FRMAIN_TASK_PRIO        3                    //�������ȼ�
#define FRMAIN_STK_SIZE         256              //����ջ��С    
TaskHandle_t FRMAINTask_Handler;            //������
void FRMAIN_task(void *pvParameters);    //������

#define press_FR_TASK_PRIO        3                    //�������ȼ�
#define press_FR_STK_SIZE         256              //����ջ��С    
TaskHandle_t press_FRTask_Handler;            //������
void press_FR_task(void *pvParameters);    //������

#define FR_Fun_TASK_PRIO        2                    //�������ȼ�
#define FR_Fun_STK_SIZE         256              //����ջ��С    
TaskHandle_t FR_FunTask_Handler;            //������
void FR_Fun_task(void *pvParameters);    //������

//void Usart1_Sendata();
void Face(void); //����ʶ��
//u8 send_buff[] = {"Y\r\n"};

void BoardInit(void)
{
  delay_init();  	    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
	uart2_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
	uart3_init();
	LED_Init();
	KEY_Init();
	ESP32_Init();
}

int main(void)
{    
	 BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	//��ʼ������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
   
    BoardInit();
    printf("Welcome to FreeRTOS,CoreClock:%d\r\n",SystemCoreClock);
     
    //������ʼ����  ʹ�õĺ��� xTaskCreate()��������(��̬����)
 //�Ļ���ô�����ջ�ͻ��ɺ��� xTaskCreate()�Զ�����
    xReturn = xTaskCreate((TaskFunction_t )start_task,            //������       
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    if(pdPASS == xReturn)
    vTaskStartScheduler();   //����������� 
  else
    return -1;                                                     
}

void start_task(void *pvParameters)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    taskENTER_CRITICAL();           //�����ٽ���
    //����FRMAIN����ָ��������
		xReturn =xTaskCreate((TaskFunction_t )FRMAIN_task,         
												(const char*    )"FRMAIN_task",       
												(uint16_t       )FRMAIN_STK_SIZE, 
												(void*          )NULL,                
												(UBaseType_t    )FRMAIN_TASK_PRIO,    
												(TaskHandle_t*  )&FRMAINTask_Handler);   
		if(pdPASS == xReturn)
    printf("����FRMAIN����ɹ�!\r\n");
//								
//		//����press_FR����ˢָ��						
		xReturn =xTaskCreate((TaskFunction_t )press_FR_task,         
                (const char*    )"press_FR_task",       
                (uint16_t       )press_FR_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )press_FR_TASK_PRIO,    
                (TaskHandle_t*  )&press_FRTask_Handler); 
    if(pdPASS == xReturn)
    printf("����press_FR����ɹ�!\r\n");
		
		xReturn =xTaskCreate((TaskFunction_t )FR_Fun_task,         
                (const char*    )"FR_Fun_task",       
                (uint16_t       )FR_Fun_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )FR_Fun_TASK_PRIO,    
                (TaskHandle_t*  )&FR_FunTask_Handler); 
		if(pdPASS == xReturn)
    printf("����FR_Fun����ɹ�!\r\n");	
		
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

void FRMAIN_task(void *pvParameters){
	u8 key=0;
	u8 LED1_Status;
	u8 LED0_Status;
	u8 FAN0_Status;
	u8 FAN1_Status;
	LED1=1;
	LED0=1;
	FAN0=1;
	FAN1=0;
	while(1){
		key=KEY_Scan(0);
		switch(key){
			case KEY0_PRES:
			//LED0=1;
			LED0=!LED0;
			LED0_Status=get_LED0_status();
			break;
			
			case KEY1_PRES:
			//LED1=1;
			LED1=!LED1;
			LED1_Status=get_LED1_status();
			break;
			
			case KEY2_PRES:
			//LED1=1;
			FAN0=!FAN0;
			FAN0_Status=get_FAN0_status();
			break;
			
			case KEY3_PRES:
			//LED1=1;
			FAN1=!FAN1;
			FAN1_Status=get_FAN1_status();
			break;
			
		  case WKUP_PRES:
			LED1=!LED1;
			LED0=!LED0;
			LED1_Status=get_LED1_status();
			LED0_Status=get_LED0_status();
			break;
			
			default:
			delay_ms(10);	
		}
	}
}

void press_FR_task(void *pvParameters){
	while(1){
		Face();
		vTaskDelay(500);
	}
}

void FR_Fun_task(void *pvParameters){
	uint16_t timeCount = 0;
  uint8_t *dataPtr = NULL;
//����OneNet�������Ӳ��ɹ�������
  while(MqttLinkOneNet())
    delay_ms(500);
  while(1) {
  //���ͼ��5s	
    if(++timeCount >= 500) {
      printf("OneNet_SendData\r\n");
      MqttSendDataOneNet(); //��������
			
      timeCount = 0;
      uart3_clear();
    }
		
    dataPtr = ESP32_GetIPD(0);
    if(dataPtr != NULL)
      MqttRecvProOneNet(dataPtr);    
    delay_ms(10);
  }
}

//void Usart1_Sendata()
//{	
//	u8 i = 0;
//	
////	USART_SendData(USART1,0x0d);
////	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
//	
//		USART_SendData(USART1,send_buff[0]);
//		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
//	
//}

 //����ʶ��
void Face(void){
	if(USART2_RX_STA){
		printf("USART2_RX_BUF:%s\r\n",USART2_RX_BUF);
	if(USART2_RX_BUF[0]==/*send_buff[0]*/0x59) //ascii����ֵ 16���� 0x31==ʮ���Ƶ�1 
		{
			USART_SendData(USART2,0x59/*USART2_RX_BUF[0]*/);
			while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
			memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));
			count=0;
			USART2_RX_STA=0;
		}
		else
		{
			memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));
			count=0;
			USART2_RX_STA=0;
			printf("face fail\r\n");
		}
	}
}
