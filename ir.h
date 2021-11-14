/*
 * ir.h
 *
 *  Created on: 2020年12月30日
 *      Author: wxy
 */

#ifndef IR_H_
#define IR_H_
#include "queue.h"
volatile uint32_t error = 0;
volatile uint32_t count = 0;
unsigned char IrData[4];
//红外解码输出，数字0-9对应的关系如下，地址吗为0x00
/*address 0x00
 * 0->22
 * 1->12
 * 2->24
 * 3->94
 * 4->8
 * 5->28
 * 6->90
 * 7->66
 * 8->82
 * 9->74
 */


void doOperation() {

	a = key;
	switch (a) {
	case 1:
		if (b == 0)
			redzuo();
		else
			bluezuo();

		showqipan();
		showall();
		break;
	case 2:
		b = !b;
		showqipan();
		showall();
		break;
	case 3:
		if((!restart)&&(length>0)){
			Analyse();//复盘
		}
		break;
	case 4:
		;
		break;
	case 5:
		if (b == 0)
			redxia();
		else
			bluexia();

		showqipan();
		showall();
		break;
	case 6:
		if (b == 0)
			redshang();
		else
			blueshang();

		showqipan();
		showall();
		break;
	case 7:
		bluexia();
		showqipan();
		showall();
		break;
	case 8:
		blueshang();
		showqipan();
		showall();
		break;
	case 9:
		if (b == 0)
			redyou();
		else
			blueyou();

		showqipan();
		showall();
		break;
	case 10:
		requeue();

		break;
	case 11:
		blueyou();
		showqipan();
		showall();
		break;
	case 12:
		;
		break;
	case 13:
		if (b == 0)
			redclick();
		else
			blueclick();

		showqipan();
		showall();
		break;
	case 14:
		if (b == 0)
			redcancel();
		else
			bluecancel();

		showqipan();
		showall();
		break;
	case 15:
		blueclick();
		showqipan();
		showall();
		break;
	case 16:
		bluecancel();
		showqipan();
		showall();
		break;
	default:
		;
	}
	key = 0;
}

void IrOutput() {
	key=0;
	switch (IrData[2]) {

	case 12:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x02); //PN0 PN1 1
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x02); //PF0 PF4
		key = 3;
		break;

	case 24:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x01); //2
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x00);
		break;

	case 94:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x03); //3
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x00);
		key = 14;
		break;

	case 8:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x00); //4
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x10);
		key = 2;
		break;

	case 28:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x02); //5
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x10);
		key = 6;
		break;

	case 90:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x01); //6
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x10);
		key = 13;
		break;

	case 66:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x03); //7
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x10);
		key = 1;
		break;

	case 82:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x00); //8--->TFT
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x01);
		key = 5;
		break;

	case 74:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x02); //9
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x01);
		key = 9;
		break;

	default:
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x03);
		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, 0x11);
	}
	doOperation();
}
/////////////////////////////////////////////////////////////////////
//GPIO口初始化
void GPIOInitial(void) {
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPION);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPION);       //PN 0 PN 1
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOF);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOF);       //PF0 PF1 PF2 PF3 PF4
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
}
//////////////////////////////////////
//中断初始化
void GPIOIntInitial(void) {
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPION);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPION);           //配置PN4中断
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_4);
	GPIOIntTypeSet(GPIO_PORTN_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);     //低电平触发产生中断
	GPIOIntEnable(GPIO_PORTN_BASE, GPIO_INT_PIN_4);
}
/////////////////////////////////////
//红外解码
bool getdata() {
	unsigned int i, j, IrAddressData, IrValueData;
	volatile uint32_t temp = 0x00;
	IrAddressData = 0x00;
	IrValueData = 0x00;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 8; j++) {
			temp = temp >> 1;
			while ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x00) {
			}

			SysCtlDelay(7 * 50000000 / 30000);      //0.7
//				SysCtlDelay(8.4*50000000/30000);
			if ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x10) //判断编码是1/0
					{
				temp = temp | 0x80;      //高位置1
				while ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x10) {
					count++;
					if (count > 0x200000) {
						count = 0;
						return 0;
					}
				}
			} else {
				temp = temp & 0x7f;      //高位置0
			}

		}
		IrData[i] = temp;
	}
	//abc = IrData[2];
	IrAddressData = IrData[0] + IrData[1];
	IrValueData = IrData[2] + IrData[3];
	if ((IrAddressData == 0xff) && (IrValueData == 0xff))
		return 1;
	else
		return 0;
}
/////////////////////////////////////////////
//中断
void GPION(void) {

	volatile uint32_t LowTime, HighTime;
	unsigned long Status;
	LowTime = 0;
	HighTime = 0;
	IntDisable (INT_GPION);      //关闭中断
	IntMasterDisable();

	Status = GPIOIntStatus(GPIO_PORTN_BASE, true);

	if (Status == GPIO_INT_PIN_4) {

//		SysCtlDelay(1 * 50000000 / 3000);

		if ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x00) {
			LowTime = 1;
			while ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x00) {

			}
		} else
			LowTime = 0;

		SysCtlDelay(3 * 50000000 / 3000);
		if ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x10) {
			HighTime = 1;
			while ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x10) {

				if (count > 0x200000) {
					count = 0;
					//return 0;
				}
			}
		} else
			HighTime = 0;

		if ((HighTime == 1) && (LowTime == 1)) {
			if (getdata() == 1)      //解码校验
					{

				IrOutput();      //解码输出
			} else {

				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02); //PF1输出高，点亮LED0
				SysCtlDelay(500 * (50000000 / 3000));   //2
				// for(i=0;i<1000;i++){;}

				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
				SysCtlDelay(500 * (50000000 / 3000));    				     //2

			}
		} else {
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02);   //PF1输出高，点亮LED0
			SysCtlDelay(500 * (50000000 / 3000));
  			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
			SysCtlDelay(500 * (50000000 / 3000));
		}

	}
	SysCtlDelay(50000000 / 3);
	GPIOIntClear(GPIO_PORTN_BASE, Status);

	IntMasterEnable();
}

#endif /* IR_H_ */
