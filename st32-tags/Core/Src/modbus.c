#include "modbus.h"
#include "modbus_crc.h"
#include "usart.h"
//常用的功能码

/*
0x01 读取输出继电器的状态
0x02 读取输入继电器的状态
0x03 读多个寄存器
0x05 写单个输出继电器的状态
0x0F 写多个输出继电器的状态
0x10 写多个寄存器
*/

MODBUS modbus;//结构体变量

//reg是提前定义好的寄存器和寄存器数据，要读取和改写的部分内容
uint16_t Reg[] ={
			0x0001,
			0x0002,
			0x0003,
			0x0004,
			0x0005,
			0x0006,
			0x0007,
			0X0008,
           };

void Modbus_Send_Byte(uint8_t ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xff);	
}

		   
void Modbus_Init(void)
{
	modbus.myadd = 0x01; 		//从机设备地址为1
	modbus.timrun = 0;    		//modbus定时器停止计算
}

// Modbus 3号功能码函数
// Modbus 主机读取寄存器值
/*
 */
void Modbus_Func3(void)
{
	uint16_t Regadd,Reglen,crc;
	uint8_t i,j;	
	//得到要读取寄存器的首地址
	Regadd = modbus.rcbuf[2]*256+modbus.rcbuf[3];//读取的首地址
	//得到要读取寄存器的数据长度
	Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//读取的寄存器个数
	//回应数据包
	i = 0;
	modbus.sendbuf[i++] = modbus.myadd;      //ID号：发送本机设备地址
	modbus.sendbuf[i++] = 0x03;              //发送功能码
	modbus.sendbuf[i++] = ((Reglen*2)%256);   //返回字节个数
	for(uint8_t k = 0; k < Reglen; k++)                    //返回数据
	{
		//reg是提前定义好的16位数组（模仿寄存器）
		modbus.sendbuf[i++] = Reg[Regadd + k]/256;//高位数据
		modbus.sendbuf[i++] = Reg[Regadd + k]%256;//低位数据
	}
	//校验
	crc = Modbus_CRC16(modbus.sendbuf,i);
	modbus.sendbuf[i++] = crc/256;			
	modbus.sendbuf[i++] = crc%256;   		 
    //数据包打包完成
	for(j = 0; j < i; j++)
	{
		Modbus_Send_Byte(modbus.sendbuf[j]);	
	}
}


// Modbus 6号功能码函数
// Modbus 主机写入寄存器值
void Modbus_Func6(void)  
{
	uint16_t Regadd;
	uint16_t val;
	uint16_t i,crc;
	i=0;
	Regadd = modbus.rcbuf[2]*256 + modbus.rcbuf[3];  //得到要修改的地址 
	val = modbus.rcbuf[4]*256 + modbus.rcbuf[5];     //修改后的值（要写入的数据）
	Reg[Regadd] = val;

	//以下为回应主机
	modbus.sendbuf[i++] = modbus.myadd;
	modbus.sendbuf[i++] = 0x06;
	modbus.sendbuf[i++] = Regadd/256;
	modbus.sendbuf[i++] = Regadd%256;
	modbus.sendbuf[i++] = val/256;
	modbus.sendbuf[i++] = val%256;
	crc = Modbus_CRC16(modbus.sendbuf,i);
	modbus.sendbuf[i++] = crc/256;
	modbus.sendbuf[i++] = crc%256;
	//数据发送包打包完毕
	for(uint8_t k = 0; k < i; k++)
	{
		Modbus_Send_Byte(modbus.sendbuf[k]);
	}
}

// Modbus事件处理函数
void Modbus_Event(void)
{
	uint16_t crc,rccrc;		//crc和接收到的crc
	//参数1是数组首地址，参数2是要计算的长度（除了CRC校验位其余全算）
	crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount - 2);
	// 读取数据帧的CRC
	rccrc = (modbus.rcbuf[modbus.recount - 2] << 8) | modbus.rcbuf[modbus.recount - 1];//计算读取的CRC校验位
	if(crc == rccrc)
	{	
		if(modbus.rcbuf[0] == modbus.myadd)  // 检查地址是否时自己的地址
		{
			switch(modbus.rcbuf[1])   		 //分析modbus功能码
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					Modbus_Func3();
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					Modbus_Func6();
					break;
				case 7:
					break;
				case 8:
					break;
				case 9:
					break;
				case 16:
					break;					
				//case 16:     Modbus_Func16(); 	  break;//写入多个寄存器数据
			}
		} 
	}	
	modbus.recount = 0;//接收计数清零
	modbus.reflag = 0; //接收标志清零
}

