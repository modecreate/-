#ifndef _ESP32_H_
#define _ESP32_H_





#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־


void ESP32_Init(void);

void ESP32_Clear(void);

void ESP32_SendData(unsigned char *data, unsigned short len);

unsigned char *ESP32_GetIPD(unsigned short timeOut);


#endif
