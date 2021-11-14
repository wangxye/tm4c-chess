/*
 * I2Cmizuguan.h
 *
 *  Created on: 2020��12��30��
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
// ��ַ���Ĵ����ȶ��岿��
//*********************************************************************
//*********************************************************************
//
// �趨slave���ӣ�ģ��ĵ�ַ������һ��7-bit�ĵ�ַ����RSλ��������ʽ����:
//                      [A6:A5:A4:A3:A2:A1:A0:RS]
// RSλ��һ��ָʾλ�����RS=0����˵�������������ݣ��ӽ������ݣ�RS=1˵�������������ݣ��ӷ�������
//
//*********************************************************************
//U21����4�����ֹܺ�����ܽŵ�����
#define I2C0_ADDR_TUBE_SEL	      0x30  //00110000//U22�������ֹ�7~14�ܽŶ�Ӧ�����
#define I2C0_ADDR_TUBE_SEG_LOW    0x32  //00110010//U23�������ֹ�15~18�ܽŶ�Ӧ�����
#define I2C0_ADDR_TUBE_SEG_HIGH  0x34   //00110100//U24����LED����//PCA9557�ڲ��Ĵ�����Ҳ���ӵ�ַ
#define PCA9557_REG_INPUT	 0x00
#define PCA9557_REG_OUTPUT	 0x01
#define PCA9557_REG_PolInver 0x02
#define PCA9557_REG_CONFIG	 0x03

//*************************************************************************************
#define NUM 0
//IIC ����������ʱ������
unsigned char I2C_RECV_DATA[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/*******************************************
 ���� SDA �ź�
 ********************************************/
void I2C_Set_sda_high(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);  //����PB3
	_NOP();
	_NOP();
	return;
}

/*******************************************
 ����SDA �ź�
 ********************************************/
void I2C_Set_sda_low(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0X00000000);  //����PB3
	_NOP();
	_NOP();
	return;
}

/*******************************************
 ����SCL �ź�
 ********************************************/
void I2C_Set_scl_high(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);  //����PB2
	_NOP();
	_NOP();
	return;
}

/*******************************************
 ����SCL �ź�
 ********************************************/
void I2C_Set_scl_low(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0X00000000);  //����PB2
	_NOP();
	_NOP();
	return;
}

/*******************************************
 IIC �źŽ����źź���
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
 IIC �źų�ʼ��
 ********************************************/
void I2C_Initial(void) {
	I2C_Set_scl_low();
	I2C_STOP();
	return;
}

/*******************************************
 IIC �ź���ʼ�źź���
 ********************************************/
void I2C_START(void) {
	int i;

	I2C_Set_sda_high();  //����SDA
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_scl_high();  //����SCL
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_sda_low();  //����SDA
	for (i = NUM; i > 0; i--)
		;
	I2C_Set_scl_low();  //����SCL
	return;
}

/*******************************************
 IIC ��ȡӦ����
 ********************************************/
int I2C_GetACK(void) {
	int j;
	_NOP();
	_NOP();
	I2C_Set_scl_low();  //����SCL
	for (j = NUM; j > 0; j--)
		;
	I2C_Set_scl_high();  //����SCL
	for (j = NUM; j > 0; j--)
		;
	I2C_Set_sda_low();  //����SDA
	for (j = NUM; j > 0; j--)
		;
	I2C_Set_scl_low();  //����SCL
	return 1;
}

/*******************************************
 IIC ����Ӧ����
 ********************************************/
void I2C_SetNAk(void) {
	I2C_Set_scl_low();
	I2C_Set_sda_high();
	I2C_Set_scl_high();
	I2C_Set_scl_low();
	return;
}

/*******************************************
 IIC �����ֽں���
 ���� 	1��Ҫ�����ֽ�ֵ
 return ���޷���
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
 IIC �����ֽں���
 ���� 		��
 return ���޷���
 ********************************************/
unsigned char I2C_RxByte(void) {
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);  //����PB3Ϊ�����
	unsigned char nTemp = 0;
	int i;

	I2C_Set_sda_high();

	_NOP();
	_NOP();
	_NOP();
	_NOP();
	for (i = 0; i < 8; i++) {
		I2C_Set_scl_high(); //ģ��SCL�ź�
		if (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3) == 0x18) //�����ж�PB3��SDA������
				{
			nTemp |= (0x01 << (7 - i));  //8λSDA������һλΪ�߾���1
		}
		I2C_Set_scl_low();
		SysCtlDelay(200);
	}

	return nTemp;

}

/*******************************************
 IIC �������麯��
 ����  	1 num : �����ֽ���
 2 device_addr : iicĿ���ַ
 3 *data	�����������ַ
 return ���޷���
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
		I2C_START();           //ģ��I2Cд���ݵ�ʱ��
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
		IIC ��ȡ���麯��
	����  	1 num : �����ֽ���
	    2 device_addr : iicĿ���ַ
	    3 *data	�����������ַ
	return ���޷���
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
    I2C_START();               //ģ��I2C������
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
//******����I2C0ģ���IO���ţ�**********************************************
void I2C0GPIOBEnable(void) { // Enable GPIO portB containing the I2C pins (PB2&PB3)
	//���ֹ�
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);

}

//******����PCA9557оƬ���������ֹܵĸ�����Ϊ���***********************************
void I2C0DeviceInit(void) {
	unsigned char dataBuf[2] = { PCA9557_REG_CONFIG, 0x00 };
	i2c_write(2, I2C0_ADDR_TUBE_SEL, dataBuf);
	i2c_write(2, I2C0_ADDR_TUBE_SEG_LOW, dataBuf);
	i2c_write(2, I2C0_ADDR_TUBE_SEG_HIGH, dataBuf);

}

//*******�������ֹܵĹ�ѡ�ź�**************************************************
void I2C0TubeSelSet(char data) {   //ѡ��1��2��3��4��5�ĸ����ֹ���
	unsigned char dataBuf[2] = { PCA9557_REG_OUTPUT, data };
	i2c_write(2, I2C0_ADDR_TUBE_SEL, dataBuf);
}
//*******�������ֹܵ���Ӧ���**************************************************
void I2C0TubeLowSet(char data) {  //����7-14�ܽŶ�Ӧ�����
	unsigned char dataBuf[2] = { PCA9557_REG_OUTPUT, data };
	i2c_write(2, I2C0_ADDR_TUBE_SEG_LOW, dataBuf);
}
void I2C0TubeHighSet(char data) {  //����15-18�ܽŶ�Ӧ�����
	unsigned char dataBuf[2] = { PCA9557_REG_OUTPUT, data };
	i2c_write(2, I2C0_ADDR_TUBE_SEG_HIGH, dataBuf);
}
/**   L       H
 *  E 0x70    0x1C
 *  F 0x60    0x1C
 *  G 0x70    0x1e 1110
 *  H 0x4a    0x22
 */

//********Ԥ�����ֵ���������************************************************
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

	I2C0TubeSelSet(~0x10);      //ѡ�е�������ֹ�
	I2C0TubeLowSet(0x82); // ����7-14�ܽŶ�Ӧ�����
	I2C0TubeHighSet(0x00); //����15-18�ܽŶ�Ӧ����� ---> 0000,0100 ----> �������ӹ�K1
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	I2C0TubeSelSet(~0x10);  //���״̬
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	I2C0TubeSelSet(0x1e);  //ѡ��DIG1
	showvalue((char) redscore / 10);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	I2C0TubeSelSet(0x1e);  //���״̬
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	//��ĸB����Ҫ����DIG2��A��B��C��D��G2��J��M
	I2C0TubeSelSet(~0x02);  //ѡ��DIG2
	showvalue((char) redscore % 10);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	I2C0TubeSelSet(~0x02);  //���״̬
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	//��ĸC,�����DIG3��A��F��E��D
	I2C0TubeSelSet(~0x04);  //ѡ��DIG3
	showvalue((char) score / 10);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	I2C0TubeSelSet(~0x04);  //���״̬
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	I2C0TubeSelSet(~0x08);  //ѡ��DIG4
	showvalue((char) score % 10);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��

	I2C0TubeSelSet(~0x08);  //���״̬
	I2C0TubeLowSet(0x00);
	I2C0TubeHighSet(0x00);
	SysCtlDelay(1 * (1000000 / 3000));  //��ʱn*3��ָ��ʱ��
}



#endif /* I2CMIZUGUAN_H_ */
