/*
 * Chess.c
 *
 *  Created on: 2020��12��25��
 *      Author: wxy
 *      TODO ���ְ���
 *      ���Ӹ���
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_epi.h"
#include "inc/hw_ints.h"
#include "driverlib/epi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/ssi.h"
#include "driverlib/fpu.h"
#include "driverlib/uart.h"

#include "utils/uartstdio.h"
#include "TFTinit/TFT_400x240_OTM4001A_16bit.h"
//#include "TFTinit/picture.h"
//#include "TOUCHinit/TOUCH_TSC2046.h"
#include "EPIinit/EPIinit.h"
#include "TFTinit/picture.h"
#include "TFTinit/pic.h"


#include"function.h"
#include "ir.h"
#include "I2Cmizuguang.h"
#include "I2Caccelermeter.h"
#include "music.h"
#include "queue.h"
#include "finger.h"

#include"inc\tm4c1294ncpdt.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"
//
// Select floating-point math.
//
//#define MATH_TYPE FLOAT_MATH

#include "IQmath/IQmathLib.h"


// Provide a definition for M_PI, if it was not provided by math.h.
//
//*****************************************************************************
#ifndef M_PI
#define M_PI                    3.14159265358979323846F
#endif

uint32_t g_ui32SysClock;

//*****************************************************************************
//
// A counter for system clock ticks, used for tracking time.
//
//*****************************************************************************
static volatile unsigned long g_ulTickCount;

#define SERIES_LENGTH 240

//*****************************************************************************
//
// Creates a buffer for holding the values of the data series.  It must be
// large enough to hold the maximum number of data points in the series that
// will be shown on the strip chart.
//
//*****************************************************************************



typedef struct _Series {
	uint32_t xAxis;
	int32_t data;
} tSeries;
static tSeries g_cSeries[240];

volatile uint32_t ui32Loop;

uint32_t TouchXData[6];
uint32_t TouchYData[6];
uint32_t TouchZData[6];   //Z is for pressure measurement

void delay() {
	int ui32Loop0;
	for (ui32Loop0 = 0; ui32Loop0 < 1000; ui32Loop0++)  //delay
			{
		;
	}
}

int identify_key() {
	key = 0;
	GPIO_PORTD_AHB_DATA_R = 0x00;
	GPIO_PORTH_AHB_DATA_R = 0x0c;
	GPIO_PORTM_DATA_R = 0x08;
	for (ui32Loop = 0; ui32Loop < 1000; ui32Loop++)  //delay
			{
		;
	}
	if ((GPIO_PORTP_DATA_R & 0x04) == 0x00)  //&&(GPIO_PORTP_DATA_R==0x04)
			{
		delay();
		if ((GPIO_PORTP_DATA_R & 0x04) == 0x00) {
			key = 1;
			return 0;
		}
	} else if ((GPIO_PORTN_DATA_R & 0x08) == 0x00) {

		delay();
		if ((GPIO_PORTN_DATA_R & 0x08) == 0x00) {
			key = 5;
			return 0;
		}
	} else if ((GPIO_PORTN_DATA_R & 0x04) == 0x00) {
		delay();
		if ((GPIO_PORTN_DATA_R & 0x04) == 0x00) {
			key = 9;
			return 0;
		}
	} else if ((GPIO_PORTD_AHB_DATA_R & 0x01) == 0x00) {
		delay();
		if ((GPIO_PORTD_AHB_DATA_R & 0x01) == 0x00) {
			key = 13;
			return 0;
		}
	}

	GPIO_PORTD_AHB_DATA_R = 0x02;
	GPIO_PORTH_AHB_DATA_R = 0x04;
	GPIO_PORTM_DATA_R = 0x08;
	for (ui32Loop = 0; ui32Loop < 1000; ui32Loop++) {
		;
	}
	if ((GPIO_PORTP_DATA_R & 0x04) == 0x00)  //&&(GPIO_PORTP_DATA_R==0x04)
			{
		delay();
		if ((GPIO_PORTP_DATA_R & 0x04) == 0x00) {
			key = 2;
			return 0;
		}
	} else if ((GPIO_PORTN_DATA_R & 0x08) == 0x00) {

		delay();
		if ((GPIO_PORTN_DATA_R & 0x08) == 0x00) {
			key = 6;
			return 0;
		}
	} else if ((GPIO_PORTN_DATA_R & 0x04) == 0x00) {
		delay();
		if ((GPIO_PORTN_DATA_R & 0x04) == 0x00) {
			key = 10;
			return 0;
		}
	} else if ((GPIO_PORTD_AHB_DATA_R & 0x01) == 0x00) {
		delay();
		if ((GPIO_PORTD_AHB_DATA_R & 0x01) == 0x00) {
			key = 14;
			return 0;
		}
	}

	GPIO_PORTD_AHB_DATA_R = 0x02;
	GPIO_PORTH_AHB_DATA_R = 0x08;
	GPIO_PORTM_DATA_R = 0x08;
	for (ui32Loop = 0; ui32Loop < 1000; ui32Loop++) {
		;
	}
	if ((GPIO_PORTP_DATA_R & 0x04) == 0x00)  //&&(GPIO_PORTP_DATA_R==0x04)
			{
		delay();
		if ((GPIO_PORTP_DATA_R & 0x04) == 0x00) {
			key = 3;
			return 0;
		}
	} else if ((GPIO_PORTN_DATA_R & 0x08) == 0x00) {

		delay();
		if ((GPIO_PORTN_DATA_R & 0x08) == 0x00) {
			key = 7;
			return 0;
		}
	} else if ((GPIO_PORTN_DATA_R & 0x04) == 0x00) {
		delay();
		if ((GPIO_PORTN_DATA_R & 0x04) == 0x00) {
			key = 11;
			return 0;
		}
	} else if ((GPIO_PORTD_AHB_DATA_R & 0x01) == 0x00) {
		delay();
		if ((GPIO_PORTD_AHB_DATA_R & 0x01) == 0x00) {
			key = 15;
			return 0;
		}
	}

	GPIO_PORTD_AHB_DATA_R = 0x02;
	GPIO_PORTH_AHB_DATA_R = 0x0c;
	GPIO_PORTM_DATA_R = 0x00;
	for (ui32Loop = 0; ui32Loop < 1000; ui32Loop++) {
		;
	}
	if ((GPIO_PORTP_DATA_R & 0x04) == 0x00)  //&&(GPIO_PORTP_DATA_R==0x04)
			{
		delay();
		if ((GPIO_PORTP_DATA_R & 0x04) == 0x00) {
			key = 4;
			return 0;
		}
	} else if ((GPIO_PORTN_DATA_R & 0x08) == 0x00) {

		delay();
		if ((GPIO_PORTN_DATA_R & 0x08) == 0x00) {
			key = 8;
			return 0;
		}
	} else if ((GPIO_PORTN_DATA_R & 0x04) == 0x00) {
		delay();
		if ((GPIO_PORTN_DATA_R & 0x04) == 0x00) {
			key = 12;
			return 0;
		}
	} else if ((GPIO_PORTD_AHB_DATA_R & 0x01) == 0x00) {
		delay();
		if ((GPIO_PORTD_AHB_DATA_R & 0x01) == 0x00) {
			key = 16;
			return 0;
		}
	}

	return 1;

}

//*****************************************************************************
//
// The number of SysTick ticks per second.
//
//*****************************************************************************
#define TICKS_PER_SECOND 40
#define FSECONDS_PER_TICK (1.0F / (float)TICKS_PER_SECOND)

//*****************************************************************************
//
// This is the handler for this SysTick interrupt.  It simply increments a
// counter that is used for timing.
//
//*****************************************************************************
void SysTickIntHandler(void) {
	//
	// Update our tick counter.
	//
	g_ulTickCount++;
	if (g_ulTickCount >= 320)
		g_ulTickCount = 0;

}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART(void) {
	//
	// ����UART0ʹ��PA0,PA1�������ţ������Ҫʹ��GPIOAģ��
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//
	// ��Ϊ�����Ÿ��ã�����Ҫ��PA0��PA1�������ŵĹ��ܽ���ѡ��
	// ���ｫ����ѡ��Ϊִ��UART0ģ��Ĺ���
	//
	GPIOPinConfigure(GPIO_PA0_U0RX);	//UARTͨ���첽������/���������൱���첽����ͨ�ţ�ȫ˫�����պ�������
	GPIOPinConfigure(GPIO_PA1_U0TX);		//PA0-RX�����ߣ�PA1-TX�����ߣ���������GPIO���Ź���

	//
	// ����PA0��PA1�������ţ��ڽ�������ΪUART����ʹ��֮ǰ����Ҫ��������һЩ�й�UART������
	//
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);		//����UART��������

	//
	// UART�ı�׼��ʼ��
	//
	UARTStdioConfig(0, 115200, g_ui32SysClock);		//��żУ�飬�����ʣ����ڣ�ʹ��UART
}

//����ָ��ʶ��uart
void UartInit(void) {

	// ����UART3ʹ��PA4,PA5�������ţ������Ҫʹ��GPIOAģ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);

	// ��Ϊ�����Ÿ��ã�����Ҫ��PK0��PK1�������ŵĹ��ܽ���ѡ��
	// ���ｫ����ѡ��Ϊִ��UART4ģ��Ĺ���
	GPIOPinConfigure(GPIO_PA4_U3RX);	//UARTͨ���첽������/���������൱���첽����ͨ�ţ�ȫ˫�����պ�������
	GPIOPinConfigure(GPIO_PA5_U3TX);		//PK0-RX�����ߣ�PK1-TX�����ߣ���������GPIO���Ź���

	// ����PA0��PA1�������ţ��ڽ�������ΪUART����ʹ��֮ǰ����Ҫ��������һЩ�й�UART������
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);		//����UART��������

	// UART�ı�׼��ʼ��
//	UARTStdioConfig(0, 57600, g_ui32SysClock);		//��żУ�飬�����ʣ����ڣ�ʹ��UART
	//���ò�����115200������λ 8 ��У��λ None ��ֹͣλ 1 ��8����N����1ģʽ
	UARTConfigSetExpClk(UART3_BASE, g_ui32SysClock,  38400,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    	UARTEnable(UART3_BASE);
}


void Init_ALL() {
	SYSCTL_RCGCGPIO_R |= (SYSCTL_RCGCGPIO_R13 | SYSCTL_RCGCGPIO_R12
			| SYSCTL_RCGCGPIO_R11 | SYSCTL_RCGCGPIO_R10 | SYSCTL_RCGCGPIO_R7
			| SYSCTL_RCGCGPIO_R3);    //
	/////
	// Enable the GPIO pin for the LED PN0,PN1,PN2,PN3,PP2,PF0,PF4.  Set the direction as output.
	//  Enable the GPIO pin for the LED PM3,PH2,PH3.  Set the direction as input.and
	// enable the GPIO pin for digital function.

	// SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	//	GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_3);
	GPIO_PORTN_DIR_R = 0x03;
	GPIO_PORTM_DIR_R = 0x28;
	GPIO_PORTH_AHB_DIR_R = 0x0c;
	GPIO_PORTP_DIR_R = 0x00;
	GPIO_PORTD_AHB_DIR_R = 0x02;
	//GPIO_PORTF_AHB_DIR_R = 0x11;
	GPIO_PORTL_DIR_R = 0x0f;

	GPIO_PORTN_DEN_R = 0x0f;
	GPIO_PORTM_DEN_R = 0x28;
	GPIO_PORTH_AHB_DEN_R = 0x0c;
	GPIO_PORTP_DEN_R = 0x04;
	GPIO_PORTD_AHB_DEN_R = 0x03;
	GPIO_PORTL_DEN_R = 0x0f;

	uint16_t ui32Loop = 0, ulItemCount = 0, FinishCalculateFlag = 0,
			ulLastTickCount = 0;
	FPUEnable();
	FPULazyStackingEnable();
	g_ui32SysClock = SysCtlClockFreqSet(
			(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL
					| SYSCTL_CFG_VCO_480), 50000000);	//ʱ�ӳ�ʼ�� 50MHz

	SysTickPeriodSet(g_ui32SysClock / TICKS_PER_SECOND);

	GPIOIntInitial();       //�жϳ�ʼ��
	GPIOInitial();          //GPIO�ڳ�ʼ��

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_5);          //��ʼ��������

	PWMIntInitial();

	TimerIntInitial();          //��ʱ�� �жϳ�ʼ��

	IntMasterEnable();
	SysTickIntEnable();
	SysTickEnable();

	ConfigureUART();
	UartInit();

	EPIGPIOinit();

	TFT_400x240_OTM4001Ainit(g_ui32SysClock);			//��ʼ����Ļ
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);

	I2C0GPIOBEnable();          //����I2C0ģ���IO����
	I2C0DeviceInit();          //����PCA9557оƬ���������ֹܵĸ�����Ϊ���

	I2C5GPIOBEnable();			//����I2C1ģ���IO����
	Init_ADXL345();                         //ADXL345��ʼ��

//	Init_queue();		//��ʼ�����̶���

    IntPrioritySet(INT_GPION, 0x20);
    IntPrioritySet(INT_TIMER0A, 0x80);
    IntPrioritySet(INT_TIMER1B, 0x80);

	TOUCH_TSC2046init(g_ui32SysClock);	//��ʼ�����ذ�
	SSIDataPut(SSI0_BASE, 0xd0);
}

void showLight() {
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
	SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
	SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, GPIO_PIN_0);
	SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_PIN_1);
	SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, GPIO_PIN_2);
	SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_PIN_3);
	SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, GPIO_PIN_4);
	SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, 0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, GPIO_PIN_2);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, GPIO_PIN_4);
	SysCtlDelay(2000 * (120000000 / 3000)); //1us*n
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, 0);
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, 0);

}

void Weclome() {
//	showLight();
	Showclrpic(0, 0, 240, 413, gImage_Start);          //show ͼƬ
	int Loop;
	while (1) {
		for (Loop = 0; Loop <= 5; Loop++)      //��β���
				{
			SSIDataPut(SSI0_BASE, 0x90);
			SysCtlDelay(3);
			SSIDataGet(SSI0_BASE, &TouchXData[Loop]);    //��ȡдx����ת������
			SysCtlDelay(3);

			SSIDataPut(SSI0_BASE, 0xd0);
			SysCtlDelay(3);
			SSIDataGet(SSI0_BASE, &TouchYData[Loop]);    //��ȡy����ת������
			SysCtlDelay(3);
		}

		TouchXData[5] = (TouchXData[0] + TouchXData[1] + TouchXData[2]
				+ TouchXData[3] + TouchXData[4]) / 5;
		TouchYData[5] = (TouchYData[0] + TouchYData[1] + TouchYData[2]
				+ TouchYData[3] + TouchYData[4]) / 5;
		TOUCH_PointAdjust(&TouchXData[5], &TouchYData[5]);
//		(sqrt(pow(abs(TouchXData[5]-122),2)+pow(abs(TouchYData[5]-133),2))<=46)
//		if ((TouchXData[5] <= 1000) && (TouchYData[5] <= 2000)) {
////			TFTLCD_DrawPoint(TouchXData[5], TouchYData[5], BLUE);
//			TFTLCD_CLEAR(BLACK);
//			break;
//
//		}

		if (((TouchXData[5]-122)*(TouchXData[5]-122)+(TouchYData[5]-148)*(TouchYData[5]-148))<=2209/*(TouchXData[5] <= 2400) && (TouchYData[5] <= 4000)*/) {
			TFTLCD_DrawPoint(TouchXData[5], TouchYData[5], BLUE);
			TFTLCD_CLEAR(BLACK);
			break;

		}
	}

}
//��ʱ����ʼ��
void TimerIntInitial(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC); //����Timer0��B��ʱ��

	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC); //����Timer0��B��ʱ��

	TimerLoadSet(TIMER1_BASE, TIMER_B, g_ui32SysClock / 300);
	TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock / 1000);

	TimerIntEnable(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	IntEnable(INT_TIMER0A);
	IntEnable(INT_TIMER1B);
}

//��ʱ���ж�
void Timer0BIntHandler(void) { //volatile uint32_t i;
	unsigned long Status;
	TimerDisable(TIMER1_BASE, TIMER_B);
	Status = TimerIntStatus(TIMER1_BASE, true);
	//��ʱ��B --> ����
	if (Status == TIMER_TIMB_TIMEOUT) {
		deal_music();
//		clk0++;
	}

	TimerIntClear(TIMER1_BASE, Status);
	TimerLoadSet(TIMER1_BASE, TIMER_B, g_ui32SysClock / 300);
	TimerEnable(TIMER1_BASE, TIMER_B);
}

void Timer0AIntHandler(void)
{
//    static uint32_t k=0;
	unsigned long Status;
	Status=TimerIntStatus(TIMER0_BASE,true);
	//��ʱ��A ---> ����
	if(Status==TIMER_TIMA_TIMEOUT){
		if(restart){
			Restart();
		}else{
			doOperation();
		}

	}
	TimerIntClear(TIMER0_BASE, Status);
	TimerLoadSet(TIMER0_BASE, TIMER_B, g_ui32SysClock / 1000);
	TimerEnable(TIMER0_BASE, TIMER_B);
}
/**
 * PWM PK5  --> ���ֲ�����
 */
void PWMIntInitial(void){


	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//PWM0ģ��ʹ��

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);//GPIOKģ��ʹ��

	GPIOPinConfigure(GPIO_PK5_M0PWM7);

	GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_5);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN |//����ַ
                    PWM_GEN_MODE_NO_SYNC);//��ͬ������ģʽ

//    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
//
//    PWMGenEnable(PWM0_BASE, PWM_GEN_3);//������ʹ��
}

/**
 * ��������PWM����Ƶ�������Ƶģ���������PWM��ռ�ձȾ���
 * ���ֲ���������ʵ����ͣ���ţ�����̶�ʱ��͵��˹̶�ʱ��Ĺ��ܡ�
 * ���ֲ�������������ѭ������/����ѭ��ģʽ��
 * ���ֲ����ڶ�ʱ���ж�ʵ�����Կ�������������Ϸ�в��š�
 * ���ֲ�����������ʾ��ǰ���������Ľ�������
 *
 */
void deal_music() //���ֲ��Ŵ����ڶ�ʱ���ж���
{
//	IntMasterDisable();
	static int delay = 0;
	if (music_playflag) //���ݲ��ŵ�����������ӦPWM���
	{
		PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3,
				g_ui32SysClock / 5 * 3
						/ (tunearray[music_playflag - 1])[music_num]);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7,
				g_ui32SysClock / 5 / (tunearray[music_playflag - 1])[music_num]
						/ 2 );

		if (turn_on_flag == 1) //����
	   {
			PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
			PWMGenEnable(PWM0_BASE, PWM_GEN_3);
			turn_on_flag = 0;
		}
		music_time++;
		if (music_time >= /*600 * Tarray[music_playflag - 1]*/ (int) ((play_speed * Tarray[music_playflag - 1] * (durationarray[music_playflag - 1])[music_num] * 600))) {
			music_num++;
			music_time = 0;
		}
		if ((durationarray[music_playflag - 1])[music_num] == 100) { //һ�׸貥������
			delay++;
			if (delay == 10000) {
				display_onlyonce = 0;
				if (playmode == 1) //Ϊ����ѭ��ģʽ
						{
					music_num = 0;
					music_time = 0;
					turn_on_flag = 1;
					if (pageNum == 3) //�����ֲ���������Ļ���ս�����
						process_clearflag = 1;
				} else //Ϊ�б�ѭ��ģʽ
				{
					music_num = 0;
					music_time = 0;
					if (music_playflag == SUM)
						music_playflag = 1;
					else
						music_playflag++;
					PWMGenDisable(PWM0_BASE, PWM_GEN_3);
					PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);
					turn_on_flag = 1;
					if (pageNum == 3) //�����ֲ���������Ļ���ս�����
						process_clearflag = 1;
				}
				delay = 0;
			}
		}
	} else {
		PWMGenDisable(PWM0_BASE, PWM_GEN_3);
		PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);
//		turn_on_flag = 0;
	}
//	IntMasterEnable();

}
int flag=0;
int start = 0;
void get_musicinformation() {

//	identity_finger();
//	Showclrpic(0, 0, 240, 412, gImage_music);
	int Loop;
	Showclrpic(0, 0, 240, 413, gImage_musicList);
	TimerEnable(TIMER1_BASE, TIMER_B);
	while(1){
		for (Loop = 0; Loop <= 5; Loop++)      //��β���
		{
			SSIDataPut(SSI0_BASE, 0x90);
			SysCtlDelay(3);
			SSIDataGet(SSI0_BASE, &TouchXData[Loop]);    //��ȡдx����ת������
			SysCtlDelay(3);

			SSIDataPut(SSI0_BASE, 0xd0);
			SysCtlDelay(3);
			SSIDataGet(SSI0_BASE, &TouchYData[Loop]);    //��ȡy����ת������
			SysCtlDelay(3);
		}
		TouchXData[5] = (TouchXData[0] + TouchXData[1] + TouchXData[2]
				+ TouchXData[3] + TouchXData[4]) / 5;
		TouchYData[5] = (TouchYData[0] + TouchYData[1] + TouchYData[2]
				+ TouchYData[3] + TouchYData[4]) / 5;
		TOUCH_PointAdjust(&TouchXData[5], &TouchYData[5]);
//		TFTLCD_DrawPoint(TouchXData[5], TouchYData[5],BLUE);
		identify_key();
		a = key;
		if (a == 1) {
			TFTLCD_DrawPoint(TouchXData[5], TouchYData[5], BLUE);
			TFTLCD_CLEAR(BLACK);
			break;
		}

		//�˳�����ģʽ
		if (((TouchXData[5] - 120) * (TouchXData[5] - 120) + (TouchYData[5] - 50) * (TouchYData[5] - 50)) <= 500/*(TouchXData[5] <= 2400) && (TouchYData[5] <= 4000)*/) {
			flag=1;
		}
		if(flag&&!(((TouchXData[5] - 120) * (TouchXData[5] - 120) + (TouchYData[5] - 50) * (TouchYData[5] - 50)) <= 500)){
			if(start){
				PWMGenDisable(PWM0_BASE, PWM_GEN_3);
				PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);
				TimerDisable(TIMER1_BASE, TIMER_B);
				start=0;
			}else{
				if (turn_on_flag == 0) {
					turn_on_flag = 1;
					start = 1;
					TimerEnable(TIMER1_BASE, TIMER_B);
					if (music_playflag == 0) {
						music_playflag = 1;
					}
				}
			}
			showLight();
			flag=0;
		}

		//��ʼ/ֹͣ
//		if (((TouchXData[5] - 120) * (TouchXData[5] - 120) + (TouchYData[5] - 166) * (TouchYData[5] - 166)) <= 120/*(TouchXData[5] <= 2400) && (TouchYData[5] <= 4000)*/)
//			flag=1;
//			if(flag&&!(((TouchXData[5] - 120) * (TouchXData[5] - 120) + (TouchYData[5] - 166) * (TouchYData[5] - 166)) <= 120)) {
//			flag=0;
//			}
		//���
		if(((TouchXData[5]>60)&&(TouchXData[5]<91)&&(TouchYData[5]>193)&&(TouchYData[5]<203))|(a==2)){
			play_speed *= 1.25;
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
			SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

		}
		//����
		if (((TouchXData[5] > 147) && (TouchXData[5] < 174) && (TouchYData[5] > 193) && (TouchYData[5] < 203))|(a==3)) {
			play_speed *= 0.75;
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
			SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
		}
		//������
		if ((TouchXData[5] > 12) && (TouchXData[5] < 83)&& (TouchYData[5] > 237) && (TouchYData[5] < 267)){
//		if(flag&&!((TouchXData[5] > 12) && (TouchXData[5] < 83)
//				&& (TouchYData[5] > 237) && (TouchYData[5] < 267))){
			music_playflag = 1;
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
			SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
//			flag=0;
		}
		//���֮��
		if ((TouchXData[5] > 155) && (TouchXData[5] < 226)
				&& (TouchYData[5] > 237) && (TouchYData[5] < 267)) {
			music_playflag = 2;
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, GPIO_PIN_0);
			SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, 0);
		}
		//�໨��
		if ((TouchXData[5] > 12) && (TouchXData[5] < 83)
				&& (TouchYData[5] > 290) && (TouchYData[5] < 320)) {
			music_playflag = 3;
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_PIN_1);
			SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, 0);
		}
		//Yankee Doodle
		if ((TouchXData[5] > 156) && (TouchXData[5] < 225)
				&& (TouchYData[5] > 290) && (TouchYData[5] < 320)) {
			music_playflag = 4;
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, GPIO_PIN_2);
			SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, 0);
		}

		//����ѭ��
		if ((TouchXData[5] > 30) && (TouchXData[5] < 100)
				&& (TouchYData[5] > 349) && (TouchYData[5] < 379)) {
			playmode = 1;
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_PIN_3);
			SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, 0);
		}

		//�б�ѭ��
		if (((TouchXData[5] > 127) && (TouchXData[5] < 198) && (TouchYData[5] > 349) && (TouchYData[5] < 379))|(a==5)) {
			playmode = 0;
			music_playflag = 1;
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, GPIO_PIN_4);
			SysCtlDelay(100 * (120000000 / 3000)); //��ʱn*1ms
			GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, 0);
		}
		a=0;
	}
//	showmizuguang();
	TimerEnable(TIMER0_BASE, TIMER_A);
	showqipan();
	showall();
}

 void main() {
	Init_ALL();
	Weclome();
	get_musicinformation();
	while (1) {
		showmizuguang();
		identify_key();
//		doOperation();
		for (ui32Loop = 0; ui32Loop < 5000; ui32Loop++) {
			;
		}
		if ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x00) //�ж�PN4�Ƿ�Ϊ�͵�ƽ---->ң�����Ƿ���ͬ����
				{
			SysCtlDelay(2 * 50000000 / 3000);
			if ((GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_4) & 0x10) == 0x00)
				IntEnable(INT_GPION);
		}

	}

}
