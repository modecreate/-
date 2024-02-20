//#include "adc.h"
//#include "delay.h"
////////////////////////////////////////////////////////////////////////////////////	 
//#include "math.h"
//#include "timer.h"
//static float R0=6;
//u16 adcx;
//// ��ӢSTM32������
////ADC ����	   
//								  
//////////////////////////////////////////////////////////////////////////////////// 
//	   
//		   
////��ʼ��ADC
////�������ǽ��Թ���ͨ��Ϊ��
////����Ĭ�Ͻ�����ͨ��0~3																	   
//void  Adc_Init(void)
//{ 	
//	ADC_InitTypeDef ADC_InitStructure; 
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
// 

//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

//	//PA1 ��Ϊģ��ͨ����������                         
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	

//	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
//	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

//  
//	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
//	
//	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
//	 
//	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
//	
//	ADC_StartCalibration(ADC1);	 //����ADУ׼
// 
//	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
// 
////	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

//}				  
////���ADCֵ
////ch:ͨ��ֵ 0~3
//u16 Get_Adc(u8 ch)   
//{
//  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
//  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

//	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
//}

//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 	 

#include "adc.h"
#include "delay.h"
#include "math.h"
#include "timer.h"
static float R0=6;
u16 adcx;
void Adc_Init()//��ʼ������
{
		GPIO_InitTypeDef GPIO_Initstructre;
		ADC_InitTypeDef ADC_InitStruct;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC2,ENABLE);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//
		GPIO_Initstructre.GPIO_Mode=GPIO_Mode_AIN; 
		GPIO_Initstructre.GPIO_Pin=GPIO_Pin_7;
		GPIO_Initstructre.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_Initstructre);
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);//��֤������14M
	
		ADC_DeInit(ADC2);//��λADC1
		ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//��ʹ������ɨ��
		ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
		ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�����������
		ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//����ģʽ
		ADC_InitStruct.ADC_NbrOfChannel = 1;
		ADC_InitStruct.ADC_ScanConvMode = DISABLE;//��ʹ��ɨ��ģʽ
	  
		ADC_Init(ADC2,&ADC_InitStruct);
	
		ADC_Cmd(ADC2,ENABLE);//ʹ��ָ����ADC1
	
		ADC_ResetCalibration(ADC2);	//ʹ�ܸ�λУ׼  
	 
		while(ADC_GetResetCalibrationStatus(ADC2));	//�ȴ���λУ׼����
	
		ADC_StartCalibration(ADC2);	 //����ADУ׼
 
		while(ADC_GetCalibrationStatus(ADC2));	 //�ȴ�У׼����
}
	
u16 Get_Adc(u8 ch)
{
		ADC_RegularChannelConfig(ADC2,ch,1,ADC_SampleTime_239Cycles5);//ADC1,ͨ��1��239.5
 
		ADC_SoftwareStartConvCmd(ADC2,ENABLE);//�����λʹ��
	
		while(!ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC));
	
		return ADC_GetConversionValue(ADC2);	

}

u16 Get_Adc_Average(u8 ch,u8 times)
{
		u32 temp_val=0;
		u8 t;
		for(t=0;t<times;t++)
		{
			temp_val+=Get_Adc(ch);
			delay_ms(5);
		}
		return temp_val/times;

}

void MQ2_cumlate(float RS)
{
		R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
}


float MQ2_GetPPM(void)
{   
	  float Vrl;
	float RS;
	float ppm;
	  adcx=Get_Adc_Average(ADC_Channel_9,30);//ADC1,ȡ30�ε�ƽ��ֵ
     Vrl = 3.3f * adcx / 4096.f;
	  Vrl = ( (float)( (int)( (Vrl+0.005)*100 ) ) )/100;
     RS = (3.3f - Vrl) / Vrl * RL;
	  
    if(times<6) // ��ȡϵͳִ��ʱ�䣬3sǰ����У׼
    {
		  R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
    } 
		
	   ppm = 613.9f * pow(RS/R0, -2.074f);

    return  ppm;
}


























