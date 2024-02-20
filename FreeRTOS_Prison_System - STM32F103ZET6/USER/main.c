#include "sys.h"
#include "string.h"
#include "math.h"	
#include "delay.h"
#include "usart.h"
#include "usart3.h"
#include "usmart.h" 
#include "key.h"  
#include "malloc.h"  
#include "usart2.h"
#include "AS608.h"
#include "timer.h"
#include "math.h"	
#include "adc.h"
//#include "led.h"
//���������
#include "pwm.h"
#include "beep.h"
//LCD180
#include "lcd180.h"
#include "lcd.h"
#include "gui.h"
//DHT11
#include "dht11.h"
//RTOSͷ�ļ�
#include "FreeRTOS.h"
#include "task.h" 	
#include "limits.h"
//ov7725
#include "ov7725.h"
#include "sccb.h"
#include "tpad.h"
#include "exti.h"
//sd
#include "sdio_sdcard.h"  
//FATFS
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
//PITCURE
#include "piclib.h"	
//TEXT
#include "text.h"
//Mini STM32F103������ʵ��
//AS608ָ��ʶ��ģ�鴮��ͨѶʵ��-�⺯���汾   


#define usart2_baund  115200//����2�����ʣ�����ָ��ģ�鲨���ʸ��ģ�ע�⣺ָ��ģ��Ĭ��57600��   �����  ע�⣺ָ��ģ��Ĭ��57600 �ĳ�115200����
SysPara AS608Para;//ָ��ģ��AS608����
u16 ValidN;//ģ������Чģ�����
u8** kbd_tbl;

#define START_TASK_PRIO        1                    //�������ȼ�
#define START_STK_SIZE         256              //����ջ��С    
TaskHandle_t StartTask_Handler;            //������
void start_task(void *pvParameters);//������
 
#define FRMAIN_TASK_PRIO        2                    //�������ȼ�
#define FRMAIN_STK_SIZE         256              //����ջ��С    
TaskHandle_t FRMAINTask_Handler;            //������
void FRMAIN_task(void *pvParameters);    //������
 
#define FR_Fun_TASK_PRIO        4                    //�������ȼ�
#define FR_Fun_STK_SIZE         256              //����ջ��С    
TaskHandle_t FR_FunTask_Handler;            //������
void FR_Fun_task(void *pvParameters);    //������


#define press_FR_TASK_PRIO        3                    //�������ȼ�
#define press_FR_STK_SIZE         256              //����ջ��С    
TaskHandle_t press_FRTask_Handler;            //������
void press_FR_task(void *pvParameters);    //������

#define LCD180DHT11_TASK_PRIO        6                    //�������ȼ�
#define LCD180DHT11_STK_SIZE         256              //����ջ��С    
TaskHandle_t LCD180DHT11Task_Handler;            //������
void LCD180DHT11_task(void *pvParameters);    //������

#define LCD180MQ2_TASK_PRIO        7                    //�������ȼ�
#define LCD180MQ2_STK_SIZE         256              //����ջ��С    
TaskHandle_t LCD180MQ2Task_Handler;            //������
void LCD180MQ2_task(void *pvParameters);    //������

#define FaceDo_TASK_PRIO        5                    //�������ȼ�
#define FaceDo_STK_SIZE         256              //����ջ��С    
TaskHandle_t FaceDoTask_Handler;            //������
void FaceDo_task(void *pvParameters);    //������

//#define TEST_TASK_PRIO        9                    //�������ȼ�
//#define TEST_STK_SIZE         256              //����ջ��С    
//TaskHandle_t TESTTask_Handler;            //������
//void TEST_task(void *pvParameters);    //������

#define LCD_TASK_PRIO        8                    //�������ȼ�
#define LCD_STK_SIZE         256              //����ջ��С    
TaskHandle_t LCDTask_Handler;            //������
void LCD_task(void *pvParameters);    //������

void Add_FR(void);	//¼ָ��
void Del_FR(void);	//ɾ��ָ��
void press_FR(void);//ˢָ��
void doSG90(void); //SG90�������
void Face(void); //����ʶ��
void Beep_1(void); //����1��
void Beep_2(void); //����2��
void Beep_3(void); //����3��
void ShowErrMessage(u8 ensure);//��ʾȷ���������Ϣ 


//ov7725
void camera_refresh(void);
void camera_new_pathname(u8 *pname);
#define  OV7725_WINDOW_WIDTH		320 	// <=320
#define  OV7725_WINDOW_HEIGHT		240 	// <=240

const u8*LMODE_TBL[6]={"Auto","Sunny","Cloudy","Office","Home","Night"};//6�ֹ���ģʽ	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7����Ч 
extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��	


void BoardInit(void)
{
	
  delay_init();  	    //��ʼ����ʱ����
	Adc_Init();//ADC��ʼ��
	SystemInit();	//System init.
	BEEP_Init();  //��������ʼ��
	uart_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
	uart3_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
	usart2_init(usart2_baund);//��ʼ������2,������ָ��ģ��ͨѶ
	LCD180_Init();
	LCD_Init();
	DHT11_Init();
	PS_StaGPIO_Init();	//��ʼ��FR��״̬����
	KEY_Init();					//������ʼ�� 
	TIM3_PWM_Init(199,7199);	 //(199+1)*(7199+1)/72*10^6
	TIM5_Int_Init(4999,7199);
	mem_init();					//��ʼ���ڴ�� 
	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1);  //����FLASH.
	
	usmart_dev.init(72);		//��ʼ��USMART	
	//LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	TPAD_Init(6);
	W25QXX_Init();				//��ʼ��W25Q128
}

//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
	//LCD_Fill(0,120,lcddev.width,160,WHITE);
	//Show_Str_Mid(0,120,(u8*)EnsureMessage(ensure),16,240);
	printf("%s",EnsureMessage(ensure));
	delay_xms(10);
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
								
		//����FR_Fun����ָ��						
		xReturn =xTaskCreate((TaskFunction_t )FR_Fun_task,         
                (const char*    )"FR_Fun_task",       
                (uint16_t       )FR_Fun_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )FR_Fun_TASK_PRIO,    
                (TaskHandle_t*  )&FR_FunTask_Handler); 
		if(pdPASS == xReturn)
    printf("����FR_Fun����ɹ�!\r\n");						
								
		//����press_FR����ˢָ��						
		xReturn =xTaskCreate((TaskFunction_t )press_FR_task,         
                (const char*    )"press_FR_task",       
                (uint16_t       )press_FR_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )press_FR_TASK_PRIO,    
                (TaskHandle_t*  )&press_FRTask_Handler); 
    if(pdPASS == xReturn)
    printf("����press_FR����ɹ�!\r\n");
		
		//����LCD180DHT11��������ʶ��ͨ��					
		xReturn =xTaskCreate((TaskFunction_t )LCD180DHT11_task,         
                (const char*    )"LCD180DHT11_task",       
                (uint16_t       )LCD180DHT11_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )LCD180DHT11_TASK_PRIO,    
                (TaskHandle_t*  )&LCD180DHT11Task_Handler); 
    if(pdPASS == xReturn)
    printf("����LCD180DHT11����ɹ�!\r\n");
		
		//����LCD180MQ2��������ʶ��ͨ��					
		xReturn =xTaskCreate((TaskFunction_t )LCD180MQ2_task,         
                (const char*    )"LCD180MQ2_task",       
                (uint16_t       )LCD180MQ2_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )LCD180MQ2_TASK_PRIO,    
                (TaskHandle_t*  )&LCD180MQ2Task_Handler); 
    if(pdPASS == xReturn)
    printf("����LCD180MQ2����ɹ�!\r\n");
		
		
				//����LCD����ͨ��					
		xReturn =xTaskCreate((TaskFunction_t )LCD_task,         
                (const char*    )"LCD_task",       
                (uint16_t       )LCD_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )LCD_TASK_PRIO,    
                (TaskHandle_t*  )&LCDTask_Handler); 
    if(pdPASS == xReturn)
    printf("����LCD����ɹ�!\r\n");
		
		//����Facedo��������ʶ��ͨ��					
		xReturn =xTaskCreate((TaskFunction_t )FaceDo_task,         
                (const char*    )"FaceDo_task",       
                (uint16_t       )FaceDo_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )FaceDo_TASK_PRIO,    
                (TaskHandle_t*  )&FaceDoTask_Handler); 
    if(pdPASS == xReturn)
    printf("����FaceDo����ɹ�!\r\n");
		
		//����TEST�����ͨ��					
//		xReturn =xTaskCreate((TaskFunction_t )TEST_task,         
//                (const char*    )"TEST_task",       
//                (uint16_t       )TEST_STK_SIZE, 
//                (void*          )NULL,                
//                (UBaseType_t    )TEST_TASK_PRIO,    
//                (TaskHandle_t*  )&TESTTask_Handler); 
//    if(pdPASS == xReturn)
//    printf("����TEST����ɹ�!\r\n");
		
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//ָ�������������� FRMAIN
void FRMAIN_task(void *pvParameters)
{
	u8 ensure;
	u8 key_num;
	BaseType_t err;

	char *str;
	/*�����û����*/
	printf("AS608 Fingerprint module test\r\n");
	printf("Connect with AS608.....\r\n");
	while(PS_HandShake(&AS608Addr))//��AS608ģ������
	{
		printf("Cannot connect with AS608!\r\n");
		delay_xms(10);
		printf("Try to connect again....\r\n");
		delay_xms(10);	  
	}
	//����ģ��ɹ�����ʼ��
	printf("Connect Success!\r\n");
	str=mymalloc(30);
	sprintf(str,"Baudrate:%d   Addr:%x\n",usart2_baund,AS608Addr);//��ʾ������
	printf("%s\n",str);
	delay_xms(100);
	ensure=PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	if(ensure!=0x00)
		ShowErrMessage(ensure);//��ʾȷ���������Ϣ	
	ensure=PS_ReadSysPara(&AS608Para);  //��AS608ģ����� 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"RemainNum:%d    Level:%d \n",AS608Para.PS_max-ValidN,AS608Para.PS_level);//��ʾʣ��ָ�������Ͱ�ȫ�ȼ�
		printf("%s\n",str);
	}
	else
		ShowErrMessage(ensure);	
	myfree(str);

    while(1)
    {		
			if((press_FRTask_Handler!=NULL)&&(PS_Sta))
			{
				err=xTaskNotify((TaskHandle_t )press_FRTask_Handler, //������ (1)
			 (uint32_t )PS_Sta, //����ֵ֪ͨ
			 (eNotifyAction )eSetValueWithOverwrite); //��д�ķ�ʽ
				if(err==pdFAIL)
				{
				printf("����֪ͨ����ʧ��\r\n");
				}						
			}
			
			key_num=KEY_Scan(0);//ָ��
			if((FR_FunTask_Handler!=NULL)&&(key_num))
			{
				err=xTaskNotify((TaskHandle_t )FR_FunTask_Handler, //������ (1)
			 (uint32_t )key_num, //����ֵ֪ͨ
			 (eNotifyAction )eSetValueWithOverwrite); //��д�ķ�ʽ
				if(err==pdFAIL)
				{
				printf("����֪ͨ����ʧ��\r\n");
				}						
			}
//				key_num=KEY_Scan(0);
//			if((FaceDoTask_Handler!=NULL)&&(key_num))
//			{
//				err=xTaskNotify((TaskHandle_t )FaceDoTask_Handler, //������ (1)
//			 (uint32_t )key_num, //����ֵ֪ͨ
//			 (eNotifyAction )eSetValueWithOverwrite); //��д�ķ�ʽ
//				if(err==pdFAIL)
//				{
//				printf("����֪ͨ����ʧ��\r\n");
//				}						
//			}
			
    }
}

//ָ�ƹ��ܼ���
void FR_Fun_task(void *pvParameters){
	u8 num;
	uint32_t NotifyValue;
  BaseType_t err;
	
	while(1){
		err=xTaskNotifyWait((uint32_t )0x00, //���뺯����ʱ��������� bit (2)
												(uint32_t )ULONG_MAX,//�˳�������ʱ��������е� bit
												(uint32_t* )&NotifyValue, //��������ֵ֪ͨ
 											  (TickType_t )portMAX_DELAY); //����ʱ��
		if(err==pdTRUE) //��ȡ����֪ͨ�ɹ�
		{
			switch((u8)NotifyValue){
				case KEY0_PRES:
					 Add_FR();
				   break;
			  case KEY1_PRES:
					Del_FR();
				  break;
	    }
		}
	}
}

//LCD������
void LCD_task(void *pvParameters){
	u8 res;							 
	u8 *pname;				//��·�����ļ��� 
	u8 key;					//��ֵ		   
	u8 i;						 
	u8 sd_ok=1;				//0,sd��������;1,SD������.
	
	POINT_COLOR=RED;      
 	while(font_init()) 				//����ֿ�
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
	}  	 
	Show_Str(30,50,200,16,"��ӢSTM32F1������",16,0);				    	 
	Show_Str(30,70,200,16,"�����ʵ��",16,0);				    	 
	Show_Str(30,90,200,16,"KEY4:����",16,0);				    	 
	Show_Str(30,110,200,16,"����ԭ��@ALIENTEK",16,0);				    	 
	Show_Str(30,130,200,16,"2019��10��9��",16,0);
	res=f_mkdir("0:/PHOTO");		//����PHOTO�ļ���
	if(res!=FR_EXIST&&res!=FR_OK) 	//�����˴���
	{		    
		Show_Str(30,150,240,16,"SD������!",16,0);
		delay_ms(200);				  
		Show_Str(30,170,240,16,"���չ��ܽ�������!",16,0);
		sd_ok=0;  	
	}else
	{
		Show_Str(30,150,240,16,"SD������!",16,0);
		delay_ms(200);				  
		Show_Str(30,170,240,16,"KEY4:����",16,0);
		sd_ok=1;    	  
	}										   						    
 	pname=mymalloc(30);	//Ϊ��·�����ļ�������30���ֽڵ��ڴ�		    
 	while(pname==NULL)			//�ڴ�������
 	{	    
		Show_Str(30,190,240,16,"�ڴ����ʧ��!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,146,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}   											  
	while(OV7725_Init())//��ʼ��OV7725
	{
		Show_Str(30,190,240,16,"OV7725 ����!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);
	}
		Show_Str(30,190,200,16,"OV7725 ����",16,0);
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY2_PRES)
		{
			OV7725_Window_Set(320,240,0);//QVGAģʽ���
			break;
		}else if(key==KEY3_PRES)
		{
			OV7725_Window_Set(320,240,1);//VGAģʽ���
			break;
		}
		i++;
		if(i==100)LCD_ShowString(30,206,200,16,16,"KEY2:QVGA  KEY3:VGA"); //��˸��ʾ��ʾ��Ϣ
		if(i==200)
		{	
			LCD_Fill(30,206,200,250+16,WHITE);
			i=0; 
		}
		delay_ms(5);
	}				
	OV7725_CS=0;					 
	TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�									  
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	LCD_Clear(BLACK);
 	while(1)
	{	
		key=KEY_Scan(0);//��֧������
		if(key==KEY4_PRES)
		{
			if(sd_ok)
			{
				//LED1=0;	//����DS1,��ʾ��������
				camera_new_pathname(pname);//�õ��ļ���		    
				if(bmp_encode(pname,(lcddev.width-240)/2,(lcddev.height-320)/2,240,320,0))//��������
				{
					Show_Str(40,130,240,12,"д���ļ�����!",12,0);		 
				}else 
				{
					Show_Str(40,130,240,12,"���ճɹ�!",12,0);
					Show_Str(40,150,240,12,"����Ϊ:",12,0);
 					Show_Str(40+42,150,240,12,pname,12,0);		    
 					//BEEP=1;	//�������̽У���ʾ�������
					Beep_1();
					delay_ms(100);
		 		}
			}else //��ʾSD������
			{					    
				Show_Str(40,130,240,12,"SD������!",12,0);
 				Show_Str(40,150,240,12,"���չ��ܲ�����!",12,0);			    
 			}
 		 	//BEEP=0;//�رշ�����
			//LED1=1;//�ر�DS1
			delay_ms(1800);//�ȴ�1.8����
			LCD_Clear(BLACK);
		}else delay_ms(5);
 		camera_refresh();//������ʾ
		i++;
		if(i==40)//DS0��˸.
		{
			i=0;
			//LED0=!LED0;
 		}
	}	   										    
}

//LCD180dht11������
void LCD180DHT11_task(void *pvParameters){
	LCD180_Clear(WHITE);
		while(1){
	u8 t=0;			    
	u8 temperature;  	    
	u8 humidity; 
	Gui_StrCenter(0,23,RED,BLUE,"DHT11TEST",16,1);//������ʾ
	while(DHT11_Init())	//DHT11��ʼ��	
	{
		Gui_StrCenter(0,23,RED,BLUE,"DHT11 Error",16,1);
		delay_xms(200);
	}
	Gui_StrCenter(0,57,GREEN,BLUE,"DHT11 OK",16,1);//������ʾ
	Gui_StrCenter(0,74,GREEN,BLUE,"Temp:    C",16,1);//������ʾ
	Gui_StrCenter(0,91,GREEN,BLUE,"Humi:    %",16,1);//������ʾ

		while(1)
	{	    	    
 		if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ				
			//USART_SendData(USART1,temperature);
			//USART_SendData(USART1,humidity);
			LCD180_ShowNum(70,74,temperature,2,16);	//��ʾ�¶�	  	
			LCD180_ShowNum(70,91,humidity,2,16);		//��ʾʪ��	

		}				   
	 	delay_xms(10);
		t++;
		if(t==20)
		{
			t=0;
		}
	}
	}
}	

//LCD180MQ2������
void LCD180MQ2_task(void *pvParameters){
	
		float vol;//���ԭʼ�ĵ�ѹֵ
    float mq2ppm;//�������Ũ��
		u16 adcx;//adc�ɼ����ݣ�4096��
	  LCD180_Clear(WHITE);
	  Gui_StrCenter(0,108,BLUE,BLUE,"PPM:    ",16,1);//������ʾ
	  Gui_StrCenter(0,125,BLUE,BLUE,"vol:    ",16,1);//������ʾ
	  while(1){
		vol=(adcx*3.3)/4096;//��ѹֵ
		
			mq2ppm = MQ2_GetPPM();
//			printf("MQ2PPM:%0.1f",mq2ppm);
//			printf("VOL:%0.1f",vol);
//			delay_ms(500);
		LCD180_ShowNum(70,108,mq2ppm,2,16);		//��ʾ����Ũ��
		LCD180_ShowNum(70,125,vol,2,16);		//��ʾ����Ũ��
	}
}

   //TEST������
//void TEST_task(void *pvParameters){
//	u8 temperature;  	    
//	u8 humidity; 
//	float mq2ppm;//�������Ũ��
//	u8 t=0;
//	while(1){
//								  
//			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ				
//			mq2ppm = MQ2_GetPPM();
//	if(temperature>=25){
//		GPIO_SetBits(GPIOE,GPIO_Pin_0);
//	}
//	
//	}
//	
//}

//����ʶ��ͨ��do
void FaceDo_task(void *pvParameters){
	u8 num;
	uint32_t NotifyValue;
  BaseType_t err;
	
	while(1){
//		err=xTaskNotifyWait((uint32_t )0x00, //���뺯����ʱ��������� bit (2)
//												(uint32_t )ULONG_MAX,//�˳�������ʱ��������е� bit
//												(uint32_t* )&NotifyValue, //��������ֵ֪ͨ
// 											  (TickType_t )portMAX_DELAY); //����ʱ��
//		if(err==pdTRUE) //��ȡ����֪ͨ�ɹ�
//		{
//			switch((u8)NotifyValue){
//				case Face_PRES:
//					 Face();
//				   break;
//	    }
//		}
		Face();
		vTaskDelay(5);
	}
}


//ˢָ�ƹ���
void press_FR_task(void *pvParameters){
	uint32_t NotifyValue;
	BaseType_t err;
	SearchResult seach;
	u8 ensure;
	char *str;
	
	while(1){
		err=xTaskNotifyWait((uint32_t )0x00, //���뺯����ʱ��������� bit (2)
												(uint32_t )ULONG_MAX,//�˳�������ʱ��������е� bit
												(uint32_t* )&NotifyValue, //��������ֵ֪ͨ
 											  (TickType_t )portMAX_DELAY); //����ʱ��
		if(err==pdTRUE) //��ȡ����֪ͨ�ɹ�
		{
		  if(PS_Sta)	 //���PS_Sta״̬���������ָ����
		  {
			press_FR();//ˢָ��
  	  }
		}
	}
}


//¼ָ��
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
	u8 len1;
	u8 t1;
	u8 a1;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				printf("�밴��ָ��\r\n");
			  
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						//ָ����ȷ
						printf("ָ����ȷ\r\n");
						i=0;
						processnum=1;//�����ڶ���						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				printf("�ٰ�һ����ָ\r\n");
			
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure= PS_GenChar(CharBuffer2);//��������			
					if(ensure==0x00)
					{
						printf("ָ����ȷ\r\n");
						i=0; 
						processnum=2;//����������
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				printf("�Ա�����ָ��\r\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					printf("����ָ�ƶԱ�һ��\r\n");
					processnum=3;//�������Ĳ�
				}
				else 
				{
					printf("�Ա�ʧ�ܣ����ذ���ָ��\r\n");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1000);
				break;

			case 3:
				printf("����һ��ָ��ģ��\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("����ָ��ģ��ɹ�\r\n");
					processnum=4;//�������岽
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1000);
				break;
				
			case 4:	
				printf("�������ָ��ģ���ŵ�λ�ã�����С��300����\r\n");
				while(1)
				{
					if(USART_RX_STA&0X8000)
					{
						len1=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
						printf("\r\n�����͵�λ��Ϊ:\r\n");
						for(a1=0;a1<len1;a1++)
						{
								a1=a1*10+USART_RX_BUF[a1]-'0';
						}		
							a1=a1-1;
					
					for(t1=0;t1<len1;t1++)
					{
						USART1->DR=USART_RX_BUF[t1];
						while((USART1->SR&0X40)==0);//�ȴ����ͽ���
					}
						printf("\r\n\r\n");//���뻻��
						USART_RX_STA=0;
					}
					if(a1>0)
						break;
				}				
				ensure=PS_StoreChar(CharBuffer2,a1);//����ģ��
				if(ensure==0x00) 
				{			

					printf("���ָ�Ƴɹ���\r\n");
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					//��ʾʣ��ָ�Ƹ���
					printf("ʣ��ָ������:%d\r\n",AS608Para.PS_max-ValidN);
					delay_ms(1500);//��ʱ�������ʾ	
					printf("�������Ĳ�����\r\n");
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(500);
		if(i==5)//����5��û�а���ָ���˳�
		{
			printf("�����˳��ˣ�\r\n");
			break;	
		}				
	}
}

//ɾ��ָ�ƹ���
void Del_FR(void)
{
	u8  ensure;
	u8 len;	
	u8 t;
	u8 a;
	printf("ɾ��ָ�ƣ�\r\n");
	printf("������Ҫɾ��ָ�Ƶ�λ��:(��Χ��0=< ID <=299)\r\n");
	
	while(1)
	{
		//���ܴ�����Ϣ�ĺ��Ĳ��ִ��룡
		if(USART_RX_STA&0X8000)
		{
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵�λ��Ϊ:\r\n");
			
			//�������Ĺ����ǽ����ڻ����������飬ת����int��Ĭ��+1.����-1
			for(a=0;a<len;a++)
			{
					a=a*10+USART_RX_BUF[a]-'0';
			}
			a=a-1;
			for(t=0;t<len;t++)
				{
					USART1->DR=USART_RX_BUF[t];
					while((USART1->SR&0X40)==0);//�ȴ����ͽ���
				}
				printf("\r\n\r\n");//���뻻��
				USART_RX_STA=0;
		}
		if(a>0)
			break;
	}
	//������յ�����0 �����ָ�ƿ⣬����λ��ɾ����Ӧλ�õ�ָ�ƣ��ù���Ŀǰ�����ã�ǰ���ж������˳���
	if(a==0)
		ensure=PS_Empty();//���ָ�ƿ�
	else 
		ensure=PS_DeletChar(a,1);//ɾ������ָ��
	if(ensure==0)
	{
		printf("ɾ��ָ�Ƴɹ�\r\n");
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1500);//��ʱ�������ʾ
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	printf("ʣ��ָ��������%d\r\n",AS608Para.PS_max-ValidN);
	delay_ms(50);
	printf("�������Ĳ�����\r\n");
	return;
}

//ˢָ�ƹ���
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//�����ɹ�
			{					
				printf("����ָ�Ƴɹ�\r\n");				
				str=mymalloc(50);
				sprintf(str,"Match ID:%d  Match score:%d\n",seach.pageID,seach.mathscore);//��ʾƥ��ָ�Ƶ�ID�ͷ���
				printf("%s\r\n",str);
				doSG90();//SG90������
				myfree(str);

			}
			else  //ˢָ�ƴ��� ��������һ��
				Beep_1();
				ShowErrMessage(ensure);					
	  }
		else
			ShowErrMessage(ensure);
	 delay_ms(1000);
	}	
}

//SG90�������
void doSG90(void){
	while(1)
   {
		 TIM_SetCompare2(TIM3,175);
		 delay_ms(1000);
		 printf("��������\r\n");
		 delay_ms(1000);
		 TIM_SetCompare2(TIM3,195);//500/20000=0.5/20
		 printf("�������\r\n");
		 break;
	 } 
//t = 0.5ms������������-�����ת�� 0 ��
//t = 1.0ms������������-�����ת�� 45��
//t = 1.5ms������������-�����ת�� 90��
//t = 2.0ms������������-�����ת�� 135��
//t = 2.5ms������������-�����ת��180��
}
//����1��
void Beep_1(void){
				BEEP=1;
				delay_ms(300);
				BEEP=0;
}
//����2��
void Beep_2(void){
	BEEP=1;
				delay_ms(300);
				BEEP=0;
	BEEP=1;
				delay_ms(300);
				BEEP=0;
} 
//����3��
void Beep_3(void){
BEEP=1;
				delay_ms(300);
				BEEP=0;
	BEEP=1;
				delay_ms(300);
				BEEP=0;
	BEEP=1;
				delay_ms(300);
				BEEP=0;
}

 //����ʶ��
void Face(void){
	if(USART3_RX_STA){
		printf("USART3_RX_BUF:%s\r\n",USART3_RX_BUF);
	if(USART3_RX_BUF[14]==0x59) //ascii����ֵ 16���� 0x31==ʮ���Ƶ�1 
		{
			doSG90();//SG90������
			memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
			count=0;
			USART3_RX_STA=0;
		}
		else{
			if(USART3_RX_BUF[14]==0x4E){
				Beep_1();
				printf("face fail\r\n");
			}
			memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
			count=0;
			USART3_RX_STA=0;
		}
	}
}

void camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta)//��֡�жϸ��£�
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//����ʾ�������õ���Ļ����
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7725_RRST=0;				//��ʼ��λ��ָ�� 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//��λ��ָ����� 
		OV7725_RCK_H; 
		for(j=0;j<76800;j++)
		{
			OV7725_RCK_L;
			color=GPIOC->IDR&0XFF;	//������
			OV7725_RCK_H; 
			color<<=8;  
			OV7725_RCK_L;
			color|=GPIOC->IDR&0XFF;	//������
			OV7725_RCK_H; 
			LCD->LCD_RAM=color;    
		}   							  
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}   
 
//�ļ������������⸲�ǣ�
//��ϳ�:����"0:PHOTO/PIC13141.bmp"���ļ���
void camera_new_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:PHOTO/PIC%05d.bmp",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}