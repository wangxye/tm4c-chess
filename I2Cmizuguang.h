/*
 * I2Cmizuguan.h
 *
 *  Created on: 2020年12月30日
 *      Author: wxy
 */

#ifndef I2CMIZUGUAN_H_
#define I2CMIZUGUAN_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define _NOP() _nop()

//*********************************************************************
//*********************************************************************
#define I2C0_MASTER_BASE 0x40020000
#define I2C0_SLAVE_BASE 0x40020000

//*********************************************************************
// 地址、寄存器等定义部分
//*********************************************************************
//*********************************************************************
//
// 设定slave（从）模块的地址。这是一个7-bit的地址加上RS位，具体形式如下:
//                      [A6:A5:A4:A3:A2:A1:A0:RS]
// RS位是一个指示位，如果RS=0，则说明是主发送数据，从接收数据；RS=1说明是主接收数据，从发送数据
//
//*********************************************************************
//U21控制4个米字管和特殊管脚的亮灭
#define I2C0_ADDR_TUBE_SEL	      0x30  //00110000//U22控制米字管7~14管脚对应的码段
#define I2C0_ADDR_TUBE_SEG_LOW    0x32  //00110010//U23控制米字管15~18管脚对应的码段
#define I2C0_ADDR_TUBE_SEG_HIGH  0x34   //00110100//U24控制LED光柱//PCA9557内部寄存器，也称子地址
#define PCA9557_REG_INPUT	 0x00
#define PCA9557_REG_OUTPUT	 0x01
#define PCA9557_REG_PolInver 0x02
#define PCA9557_REG_CONFIG	 0x03

//*************************************************************************************
#define NUM 0
//IIC 接受数据临时缓冲区
unsigned char I2C_RECV_DATA[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/*******************************************
 拉高 SDA 信号
 ********************************************/
void I2C_Set_sda_high(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);  //拉高PB3
	_NOP();
	_NOP();
	return;
}

/*******************************************
 拉低SDA 信号
 ********************************************/
void I2C_Set_sda_low(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0X00000000);  //拉低PB3
	_NOP();
	_NOP();
	return;
}

/*******************************************
 拉高SCL 信号
 ********************************************/
void I2C_Set_scl_high(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);  //拉高PB2
	_NOP();
	_NOP();
	return;
}

/*******************************************
 拉低SCL 信号
 ********************************************/
void I2C_Set_scl_low(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0X00000000);  //拉低PB2
	_NOP();
	_NOP();
	return;
}

/*******************************************
 IIC 信号结束信号函数
 ********************************************/
void I2C_STOP(void) {
	int i;
	I2C_Set_sda_low();
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_scl_low();
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_scl_high();
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_sda_high();
	for (i = NUM + 1; i > 0; i--)
		;
	return;
}

/*******************************************
 IIC 信号初始化
 ********************************************/
void I2C_Initial(void) {
	I2C_Set_scl_low();
	I2C_STOP();
	return;
}

/*******************************************
 IIC 信号起始信号函数
 ********************************************/
void I2C_START(void) {
	int i;

	I2C_Set_sda_high();  //拉高SDA
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_scl_high();  //拉高SCL
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_sda_low();  //拉低SDA
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_scl_low();  //拉低SCL
	return;
}

/*******************************************
 IIC 获取应答函数
 ********************************************/
int I2C_GetACK(void) {
	int j;
	_NOP();
	_NOP();
	I2C_Set_scl_low();  //拉低SCL
	for (j = NUM; j > 0; j--)
		;
	I2C_Set_scl_high();  //拉高SCL
	for (j = NUM; j > 0; j--)
		;
	I2C_Set_sda_low();  //拉低SDA
	for (j = NUM; j > 0; j--)
		;
	I2C_Set_scl_low();  //拉低SCL
	return 1;
}

/*******************************************
 IIC 设置应答函数
 ********************************************/
void I2C_SetNAk(void) {
	I2C_Set_scl_low();
	I2C_Set_sda_high();
	I2C_Set_scl_high();
	I2C_Set_scl_low();
	return;
}

/*******************************************
 IIC 发送字节函数
 参数 	1：要发送字节值
 return ：无返回
 ********************************************/
void I2C_TxByte(unsigned char nValue) {
	int i;
	int j;
	for (i = 0; i < 8; i++) {
		if (nValue & 0x80)
			I2C_Set_sda_high();
		else
			I2C_Set_sda_low();
		for (j = NUM; j > 0; j--)
			;
		I2C_Set_scl_high();
		nValue <<= 1;
		for (j = NUM; j > 0; j--)
			;
		I2C_Set_scl_low();
	}

	return;
}

/*******************************************
 IIC 接收字节函数
 参数 		无
 return ：无返回
 ********************************************/
unsigned char I2C_RxByte(void) {
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);  //配置PB3为输入口
	unsigned char nTemp = 0;
	int i;

	I2C_Set_sda_high();

	_NOP();
	_NOP();
	_NOP();
	_NOP();
	for (i = 0; i < 8; i++) {
		I2C_Set_scl_high(); //模拟SCL信号
		if (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3) == 0x18) //依次判断PB3（SDA）输入
				{
			nTemp |= (0x01 << (7 - i));  //8位SDA数据哪一位为高就置1
		}
		I2C_Set_scl_low();
		SysCtlDelay(200);
	}

	return nTemp;

}

/*******************************************
 IIC 发送数组函数
 参数  	1 num : 发送字节数
 2 device_addr : iic目标地址
 3 *data	：发送数组地址
 return ：无返回
 ********************************************/
void i2c_write(int num, unsigned char device_addr, unsigned char *data) {
	int i = 0;
	int count = num;
	unsigned char *send_data = data;
	unsigned char write_addr = device_addr;

	I2C_Set_scl_high();
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_sda_high();
	for (i = NUM; i > 0; i--)
		;

	for (i = 0; i < count; i++) {
		I2C_START();           //模拟I2C写数据的时序
		I2C_TxByte(write_addr);
		I2C_GetACK();
		I2C_TxByte(send_data[i]);
		I2C_GetACK();
		i++;
		I2C_TxByte(send_data[i]);
		I2C_GetACK();
		I2C_STOP();
	}

}

/*******************************************
		IIC 读取数组函数
	参数  	1 num : 发送字节数
	    2 device_addr : iic目标地址
	    3 *data	：接收数组地址
	return ：无返回
********************************************/
void i2c_read(int num, unsigned char device_addr,unsigned char *data)
{
  int i = 0;
  int count = num;
  unsigned char *send_data = data;
  unsigned char read_addr = device_addr;

  I2C_Set_scl_high();
  for(i = NUM;i > 0;i--);
  I2C_Set_sda_high();
  for(i = NUM;i > 0;i--);

  for(i = 0; i < count;i++)
  {
    I2C_START();               //模拟I2C读数据
    I2C_TxByte((read_addr - 1));
    I2C_GetACK();
    I2C_TxByte(send_data[2*i]);
    I2C_GetACK();

    I2C_START();
    I2C_TxByte(read_addr);
    I2C_GetACK();

    I2C_RECV_DATA[i] = I2C_RxByte();
    data[2*i+1]=I2C_RECV_DATA[i];
    I2C_SetNAk();
    I2C_STOP();
  }
}

//*********************************************************************
//******配置I2C0模块的IO引脚，**********************************************
void I2C0GPIOBEnable(void) { // Enable GPIO portB containing the I2C pins (PB2&PB3)
	//米字管
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);

}

//******配置PCA9557芯片中连接米字管的各引脚为输出***********************************
void I2C0DeviceInit(void) {
	unsigned char dataBuf[2] = { PCA9557_REG_CONFIG, 0x00 };
	i2c_write(2, I2C0_ADDR_TUBE_SEL, dataBuf);
	i2c_write(2, I2C0_ADDR_TUBE_SEG_LOW, dataBuf);
	i2c_write(2, I2C0_ADDR_TUBE_SEG_HIGH, dataBuf);

}

//*******设置米字管的管选信号**************************************************
void I2C0TubeSelSet(char data) {   //选择1、2、3、4、5哪个米字管亮
	unsigned char dataBuf[2] = { PCA9557_REG_OUTPUT, data };
	i2c_write(2, I2C0_ADDR_TUBE_SEL, dataBuf);
}
//*******点亮米字管的相应码段**************************************************
void I2C0TubeLowSet(char data) {  //点亮7-14管脚对应的码段
	unsigned char dataBuf[2] = { PCA9557_REG_OUTPUT, data };
	i2c_write(2, I2C0_ADDR_TUBE_SEG_LOW, dataBuf);
}
void I2C0TubeHighSet(char data) {  //点亮15-18管脚对应的码段
	unsigned char dataBuf[2] = { PCA9557_REG_OUTPUT, data };
	i2c_write(2, I2C0_ADDR_TUBE_SEG_HIGH, dataBuf);
}
/**   L       H
 *  E 0x70    0x1C
 *  F 0x60    0x1C
 *  G 0x70    0x1e 1110
 *  H 0x4a    0x22
 */

//********预设码段值，方便查找************************************************
static const char tubeCodeTable[20][2] = { //  SegmLow, SegHigh
		{ 0x10, 0x3E },		// 	    	0
				{ 0x00, 0x18 },		// 	        1
				{ 0x70, 0x2C },		//      	2
				{ 0x70, 0x26 },		//       	3
				{ 0x60, 0x32 },		//      	4
				{ 0x70, 0x16 },		//     		5
				{ 0x70, 0x1E },		//     		6
				{ 0x00, 0x26 },		//     		7
				{ 0x70, 0x3E },		//     		8
				{ 0x70, 0x36 },	    //     		9
//            {	0x60,	0x3E	},	    //     		a
				{ 0x70, 0x1C },      //          e
				{ 0x60, 0x1C },      //          f
				{ 0x70, 0x1e },      //          g
				{ 0x4a, 0x22 },      //          h
		};

/**************************************************************************/
unsigned char num[2];
void setnumber(char value) {
	char b;
	b = value;
	switch (b) {
	case 0: {
		num[0] = tubeCodeTable[0][0];
		num[1] = tubeCodeTable[0][1];
		break;
	}
	case 1: {
		num[0] = tubeCodeTable[1][0];
		num[1] = tubeCodeTable[1][1];
		break;
	}
	case 2: {
		num[0] = tubeCodeTable[2][0];
		num[1] = tubeCodeTable[2][1];
		break;
	}
	case 3: {
		num[0] = tubeCodeTable[3][0];
		num[1] = tubeCodeTable[3][1];
		break;
	}
	case 4: {
		num[0] = tubeCodeTable[4][0];
		num[1] = tubeCodeTable[4][1];
		break;
	}
	case 5: {
		num[0] = tubeCodeTable[5][0];
		num[1] = tubeCodeTable[5][1];
		break;
	}
	case 6: {
		num[0] = tubeCodeTable[6][0];
		num[1] = tubeCodeTable[6][1];
		break;
	}
	case 7: {
		num[0] = tubeCodeTable[7][0];
		num[1] = tubeCodeTable[7][1];
		break;
	}
	case 8: {
		num[0] = tubeCodeTable[8][0];
		num[1] = tubeCodeTable[8][1];
		break;
	}
	case 'e': {
		num[0] = tubeCodeTable[10][0];
		num[1] = tubeCodeTable[10][1];
		break;
	}
	case 'f': {
		num[0] = tubeCodeTable[11][0];
		num[1] = tubeCodeTable[11][1];
		break;
	}
	case 'g': {
		num[0] = tubeCodeTable[12][0];
		num[1] = tubeCodeTable[12][1];
		break;
	}
	case 'h': {
		num[0] = tubeCodeTable[13][0];
		num[1] = tubeCodeTable[13][1];
		break;
	}
	}
}
void I2C0Tubeset(void) {
	I2C0TubeLowSet(num[0]);
	I2C0TubeHighSet(num[1]);

}
void showvalue(char value) {
	setnumber(value);
	I2C0Tubeset();
}

int score = 1;
int redscore = 0;

void showmizuguang() {

	I2C0TubeSelSet(~0x10);      //选中第五个米字管
	I2C0TubeLowSet(0x82); // 点亮7-14管脚对应的码段
	I2C0TubeHighSet(0x00); //点亮15-18管脚对应的码段 ---> 0000,0100 ----> 点亮米子管K1
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	I2C0TubeSelSet(~0x10);  //清除状态
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	I2C0TubeSelSet(0x1e);  //选中DIG1
	showvalue((char) redscore / 10);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	I2C0TubeSelSet(0x1e);  //清除状态
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	//字母B，需要点亮DIG2的A、B、C、D、G2、J、M
	I2C0TubeSelSet(~0x02);  //选中DIG2
	showvalue((char) redscore % 10);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	I2C0TubeSelSet(~0x02);  //清除状态
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	//字母C,需点亮DIG3的A，F，E，D
	I2C0TubeSelSet(~0x04);  //选中DIG3
	showvalue((char) score / 10);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	I2C0TubeSelSet(~0x04);  //清除状态
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	I2C0TubeSelSet(~0x08);  //选中DIG4
	showvalue((char) score % 10);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间

	I2C0TubeSelSet(~0x08);  //清除状态
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //延时n*3个指令时间
}



#endif /* I2CMIZUGUAN_H_ */
