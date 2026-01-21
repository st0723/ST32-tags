#ifndef __MODBUS_H
#define __MODBUS_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"

typedef struct 
{
	//作为从机时使用
	uint8_t  myadd;        //本设备从机地址
	uint8_t  rcbuf[100];   //modbus接受缓冲区
	uint8_t  timout;       //modbus数据持续时间
	uint8_t  recount;      //modbus端口接收到的数据个数
	uint8_t  timrun;       //modbus定时器是否计时标志
	uint8_t  reflag;       //modbus一帧数据接受完成标志位
	uint8_t  sendbuf[100]; //modbus接发送缓冲区
}MODBUS;
extern MODBUS modbus;

void Modbus_Init(void);
void Modbus_Event(void);
#ifdef __cplusplus
}
#endif
#endif

