#include "esp32.h"
#include "onenet.h"
#include "usart.h"
#include "delay.h"
#include "mqttKit.h"
#include "string.h"
#include "led.h"
#include "dht11.h"
//#include "mq9.h"

//��ʱAPI
#define ONENET_DELAYMS   delay_ms
#define ONENET_DELAYUS   delay_us

//�������--Mqtt ��Ʒ
#define ONENET_MQTT_PRO  "460402"  //��ƷID
#define ONENET_MQTT_DEV  "793987873"  //�豸ID
#define ONENET_MQTT_AUT  "321"  //��Ȩ��Ϣ
#define ONENET_MQTT_API  "PvSnB6DeZFIItN10Bbetqv9XfJQ=="  //API
//Mqtt�ϴ�DS18B20����¶�
#define ONENET_MQTT_UP_DSTEMP  0x1820
//Mqtt�ϴ�DHT11�����ʪ��
#define ONENET_MQTT_UP_DHT11   0xDD11
//ʹ��Mqtt�ϴ���ʪ���ഫ��������Ĭ�Ϲ���DS��DHT��ѡһ
#define ONENET_MQTT_TSEN ONENET_MQTT_UP_DHT11


/*********************************************************************
 * FunctionName : MqttLinkOneNet()
 * Description  : ��OneNet����MQTT����
 * Parameters   : void
 * Returns      : uint8_t 1-�ɹ� 0-ʧ��
*********************************************************************/
uint8_t MqttLinkOneNet(void) {
  MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};//Э���
  uint8_t *dataPtr;
  uint8_t status = 1;
	
  printf("OneNet_DevLink\r\n"
    "PROID: %s,	AUIF: %s,	DEVID:%s\r\n", 
    ONENET_MQTT_PRO, ONENET_MQTT_AUT, ONENET_MQTT_DEV);
	
  if(MQTT_PacketConnect(ONENET_MQTT_PRO, 
        ONENET_MQTT_AUT, ONENET_MQTT_DEV, 
        256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0) {
    ESP32_SendData(mqttPacket._data, mqttPacket._len);//�ϴ�ƽ̨
    dataPtr = ESP32_GetIPD(250);//�ȴ�ƽ̨��Ӧ
    if(dataPtr != NULL) {
      if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK) {  
        switch(MQTT_UnPacketConnectAck(dataPtr)) {
          case 0:printf("Tips:	���ӳɹ�\r\n");status = 0;break;
          case 1:printf("WARN:	����ʧ�ܣ�Э�����\r\n");break;
          case 2:printf("WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
          case 3:printf("WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
          case 4:printf("WARN:	����ʧ�ܣ��û������������\r\n");break;
          case 5:printf("WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;					
          default:printf("ERR:	����ʧ�ܣ�δ֪����\r\n");break;
        }
      }
    }
    MQTT_DeleteBuffer(&mqttPacket);//ɾ��
  } else {
    printf("WARN:	MQTT_PacketConnect Failed\r\n");
  }	
	return status;
}
/*********************************************************************
 * FunctionName : PackMsgInType3OneNet()
 * Description  : ��OneNet��ʽ3�������ݷ�װ
 * Parameters   : char *buf  ����> ���ڷ�װ�Ļ����ַ
 * Returns      : uint8_t ��װ�󳤶�
*********************************************************************/

 
uint8_t PackMsgInType3OneNet(char *buf) {
  char text[16];
	
  	uint8_t temp=0;//�¶�ֵ
		uint8_t humi=0;//ʪ��ֵ
	u8 LED1_Status;
	u8 LED0_Status;
	u8 FAN0_Status;
	u8 FAN1_Status;

	 DHT11_Read_Data(&temp, &humi);
		printf("temp:%d\r\n",temp);
		printf("humi:%d\r\n",humi);	
	LED1_Status=get_LED1_status();
	LED0_Status=get_LED0_status();
	FAN0_Status=get_FAN0_status();
	FAN1_Status=get_FAN1_status();
	printf("LED1_Status:%d\r\n",LED1_Status);
	printf("LED0_Status:%d\r\n",LED0_Status);	
	printf("FAN0_Status:%d\r\n",FAN0_Status);	
	printf("FAN1_Status:%d\r\n",FAN1_Status);	
	
	memset ( text, 0, sizeof ( text ) );
	
	strcat ( buf, "{" );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"LED1\":%d,", LED1_Status );
	strcat ( buf, text );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"LED0\":%d,", LED0_Status );
	strcat ( buf, text );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"FAN0\":%d,", FAN0_Status );
	strcat ( buf, text );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"FAN1\":%d,", FAN1_Status );
	strcat ( buf, text );
	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"temp\":%d,", temp );
	strcat ( buf, text );

	
	memset ( text, 0, sizeof ( text ) );
	sprintf ( text, "\"humi\":%d", humi );
	strcat ( buf, text );


	strcat ( buf, "}" );

	return strlen ( buf );
}
/*********************************************************************
 * FunctionName : MqttSendDataOneNet()
 * Description  : ��װMQTT���ݰ����ϴ�OneNet
 * Parameters   : void
 * Returns      : void
*********************************************************************/
void MqttSendDataOneNet(void) {	
  MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};//Э���
  char buf[128];
  short body_len = 0, i = 0;
  printf("Tips:	OneNet_SendData-MQTT\r\n");
  memset(buf, 0, sizeof(buf));
  body_len = PackMsgInType3OneNet(buf);//��ȡ�����������ܳ���
	
  if(body_len) {
    if(MQTT_PacketSaveData(ONENET_MQTT_DEV, 
          body_len, NULL, 3, &mqttPacket) == 0) {
      for(; i < body_len; i++)
        mqttPacket._data[mqttPacket._len++] = buf[i];
			
      ONENET_MQTT_UP(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
      printf("Send %d Bytes\r\n", mqttPacket._len);
			
      MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
    } else {
      printf("WARN:	MQTT_NewBuffer Failed\r\n");
    }
	}	
}
/*********************************************************************
 * FunctionName : MqttRecvProOneNet()
 * Description  : �յ�OneNet��MQTT���ݰ�������
 * Parameters   : uint8_t *cmd  ����> �յ����ݻ�ַ
 * Returns      : void
*********************************************************************/
void MqttRecvProOneNet(uint8_t *cmd) {
  MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};//Э���	
  char *req_payload = NULL;
  char *cmdid_topic = NULL;	
  unsigned short req_len = 0;	
  unsigned char type = 0;	
  short result = 0;
  char *dataPtr = NULL;
  char numBuf[10];
  int num = 0;
	
  type = MQTT_UnPacketRecv(cmd);
	switch(type) {
    case MQTT_PKT_CMD:   //�����·�		
      result = MQTT_UnPacketCmd(cmd, &cmdid_topic, 
                 &req_payload, &req_len);	//���topic����Ϣ��
      if(result == 0) {
        printf("cmdid: %s, req: %s, req_len: %d\r\n", 
                 cmdid_topic, req_payload, req_len);
        if(MQTT_PacketCmdResp(cmdid_topic, 
               req_payload, &mqttPacket) == 0) {//����ظ����
          printf("Tips:	Send CmdResp\r\n");
          ONENET_MQTT_UP(mqttPacket._data, mqttPacket._len);
          MQTT_DeleteBuffer(&mqttPacket);
        }
      }		
    break;
			
    case MQTT_PKT_PUBACK://����Publish��Ϣ��ƽ̨�ظ���Ack		
      if(MQTT_UnPacketPublishAck(cmd) == 0)
        printf("Tips:	MQTT Publish Send OK\r\n");			
    break;
		
    default:
      result = -1;
    break;
  }
	
  ESP32_Clear(); //��ջ���	
  if(result == -1) return;
	
  dataPtr = strchr(req_payload, ':');//����':'

  if(dataPtr != NULL && result != -1) {
    dataPtr++;
  //�ж��Ƿ����·��������������		
    while(*dataPtr >= '0' && *dataPtr <= '9')
      numBuf[num++] = *dataPtr++;
    num = atoi((const char *)numBuf);//תΪ��ֵ��ʽ
  //����"led1"		
		 if(strstr((char *)req_payload, "led1")) {
      if(num == 0)     //�����������Ϊ1������			
        LED1=1;
      else if(num == 1)//�����������Ϊ0�������
        LED1=0;
    } else if(strstr((char *)req_payload, "led0")) {
      if(num == 0)
        LED0=1;
      else if(num == 1)
        LED0=0;
    } else if(strstr((char *)req_payload, "fan0")) {
      if(num == 0)
        FAN0=0;
      else if(num == 1)
        FAN0=1;
    } else if(strstr((char *)req_payload, "fan1")) {
      if(num == 0)
        FAN1=0;
      else if(num == 1)
        FAN1=1;
    }
    
  }

  if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH) {
    MQTT_FreeBuffer(cmdid_topic);
    MQTT_FreeBuffer(req_payload);
  }

}

