#ifndef __MALLOC_H
#define __MALLOC_H
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


 
#ifndef NULL
#define NULL 0
#endif

//�ڴ�����趨.
#define MEM_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM_MAX_SIZE			35*1024  						//�������ڴ� 35K
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	//�ڴ����С
 
		 
//�ڴ����������
struct _m_mallco_dev
{
	void (*init)(void);				//��ʼ��
	u8 (*perused)(void);		  	//�ڴ�ʹ����
	u8 	*membase;					//�ڴ�� 
	u16 *memmap; 					//�ڴ����״̬��
	u8  memrdy; 					//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	//��mallco.c���涨��

void mymemset(void *s,u8 c,u32 count);	//�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void mem_init(void);					 //�ڴ������ʼ������(��/�ڲ�����)
u32 mem_malloc(u32 size);		 		//�ڴ����(�ڲ�����)
u8 mem_free(u32 offset);		 		//�ڴ��ͷ�(�ڲ�����)
u8 mem_perused(void);					//���ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(void *ptr);  				//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u32 size);				//�ڴ����(�ⲿ����)
void *myrealloc(void *ptr,u32 size);	//���·����ڴ�(�ⲿ����)
#endif
//#ifndef __MALLOC_H
//#define __MALLOC_H
//#include "stm32f10x.h"
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
////ALIENTEK��ӢSTM32F103������V1
////�ڴ���� ��������	   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2015/1/20
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
////All rights reserved									  
////////////////////////////////////////////////////////////////////////////////////
// 
// 
//#ifndef NULL
//#define NULL 0
//#endif

////���������ڴ��
//#define SRAMIN	 0		//�ڲ��ڴ��
//#define SRAMEX   1		//�ⲿ�ڴ��(��ӢSTM32�����岻֧���ⲿ�ڴ�) 

//#define SRAMBANK 	2	//����֧�ֵ�SRAM����. ��Ӣ��ʵ����ֻ֧��1���ڴ�����,���ڲ��ڴ�.	


////mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
//#define MEM1_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
//#define MEM1_MAX_SIZE			40*1024  						//�������ڴ� 40K
//#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ����С

////mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSRAM����
//#define MEM2_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
//#define MEM2_MAX_SIZE			1 *32  							//��Ϊ��Ӣ��û�������ڴ�,����������һ����Сֵ
//#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ����С 
//		 
// 
////�ڴ����������
//struct _m_mallco_dev
//{
//	void (*init)(u8);					//��ʼ��
//	u8 (*perused)(u8);		  	    	//�ڴ�ʹ����
//	u8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
//	u16 *memmap[SRAMBANK]; 				//�ڴ����״̬��
//	u8  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
//};
//extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

//void mymemset(void *s,u8 c,u32 count);	//�����ڴ�
//void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
//void my_mem_init(u8 memx);				//�ڴ������ʼ������(��/�ڲ�����)
//u32 my_mem_malloc(u8 memx,u32 size);	//�ڴ����(�ڲ�����)
//u8 my_mem_free(u8 memx,u32 offset);		//�ڴ��ͷ�(�ڲ�����)
//u8 my_mem_perused(u8 memx);				//����ڴ�ʹ����(��/�ڲ�����) 
//////////////////////////////////////////////////////////////////////////////////
////�û����ú���
//void myfree(u8 memx,void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
//void *mymalloc(u8 memx,u32 size);			//�ڴ����(�ⲿ����)
//void *myrealloc(u8 memx,void *ptr,u32 size);//���·����ڴ�(�ⲿ����)
//#endif












