/*
 * finger.h
 *
 *  Created on: 2021年1月5日
 *      Author: wxy
 */
#include "driverlib/timer.h"

#ifndef FINGER_H_
#define FINGER_H_

#define Max_User 10
#define error 2

int SaveNumber = 0, searchnum = 0;
int SearchNumber = 0;
int clk0 = 0;

char str[6] = { 0, 0, 0, 0, 0, '\0' };

char Identify[16] = "Mode:   Identify";
char Input[16] = "Mode:   Input   ";

int modeflag = 0, clearallflag = 0, changeflag = 0;
int identify = 0;
//modeflag=0 识别模式 modeflag=1 为录入指纹模式
//每次成功录入 SaveNumber++

char OpenLED[12]={12,0X01,0Xff, 0xff, 0xff, 0xff,0x01,0,3,0x50,0x0054
};

//Verify Password :验证设备握手指令
char VPWD[16] = { 16, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 7, 0x13,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x1b };

//设置设备握手口令
char STWD[16] = { 16, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 7, 0x12,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x1a };	 //回送12个

//GetImage        探测手指并从传感器上读入图像
char GIMG[14] = { 12, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 3, 1,
		0x00, 0x05 };	//回送12个

//Gen Templet1    根据原始图像生成指纹特征1
char GENT1[14] = { 13, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 4, 2, 1,
		0x00, 0x08 };	//回送12个

//Gen Templet2     根据原始图像生成指纹特征2
char GENT2[14] = { 13, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 4, 2, 2,
		0x00, 0x09 }; //回送12个

//Search Finger	以CharBufferA或CharBufferB中的特征文本检索整个或部分指纹库
char SEAT[18] = { 17, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 8, 4, 1,
		0, 0, 0, 0xf0, 0x00, 0xfe };	//»ØËÍ16¸ö

//Merge Templet      以CharBufferA或CharBufferB中的特征文本整合并生成模板，结果存在ModelBuffer
char MERG[14] = { 12, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 3, 5,
		0x00, 0x09 };	//回送12个

//Store Templet      将ModelBuffer中的文件储存到f1ash指纹库中
char STOR[16] = { 15, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 6, 6, 2,
		0x00, 0x00, 0x00, 0x0f }; //回送12个

//Read Note
char RDNT[14] = { 13, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 4, 0x19,
		0, 0x00, 0x1e };

//Clear Note
char DENT[46] = { 45, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 36, 0x18,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x3d };

//DEL one templet
char DELE_one[16] = { 16, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 7,
		0x0c, 0x00, 0x00, 0, 1, 0x00, 0x15 };

//DEL templet      ;清空指纹库
char DELE_all[12] = { 12, 0X01, 0Xff, 0xff, 0xff, 0xff, 0x01, 0, 3,
		0x0d, 0x00, 0x11 };

//////////////////////////////////////常用指令定义------结束//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////宏定义////////////////////////////////////////////////////////////////////////
#define FALSE  0
#define TURE   1
#define ERROR  255

//状态定义表
#define on    	1
#define off   	0

#define MAX_NUMBER    63
#define _Nop()  _nop_()

char FifoNumber = 0;
char FIFO[MAX_NUMBER + 1] = { 0 };

//1毫秒延时程序
void delay1ms(int t) {
	int i, j;
	for (i = 0; i < t; i++)
		for (j = 0; j < 120; j++)
			;
}
//串口发送信息,通过查询方式发送一个字符
void TxdByte(char dat) {
//	UARTprintf("%c", dat);
	 while(UARTBusy(UART3_BASE));
	 UARTCharPutNonBlocking(UART3_BASE, dat);
//	UART_OutChar();
}

int Command(char *p, int MaxTime) //命令解析，给模块发送指令
{
	char tmpdat = 0, temp = 0, i = 0, package = 0, flag = 0,
			checksum = 0;
	char rs = 0;
	int result = 0, start = 0, stop = 0, count = 0 ;

	TxdByte(0xef); //数据包包头识别码
	TxdByte(0x01); //数据包包头识别码
	i = *p;         //数组的第"0"个元素、里面存放了本数组的长度，把这个长度给变量i，方便进行操作
	p++;
	p++;
	for (count = i - 1; count != 1; count--)  //Sent command String
	{
		temp = *p++;  //取第个“1"个元素的内容，然后发送
		TxdByte(temp);  //将数据发送
	}
	result = TURE;  //发送完成,结果为真(真为1)
	FifoNumber = 0;
	for (count = MAX_NUMBER + 1; count != 0; count--) //清空所有FIFO[]数组里面的内容，写入0x00
		FIFO[count - 1] = 0x00;

	if (result) {
		result = FALSE;
		start = FALSE;
		stop = FALSE;
		count = 0;
		clk0 = 0;	//清零CL0T

		//接受响应
		TimerEnable(TIMER1_BASE, TIMER_B);
		do /////////////////////////////do开始////////////////////////////////
		{
			restart0:
			rs = UARTCharGetNonBlocking(UART3_BASE);
			if (sizeof(rs) > 0) {
				tmpdat = rs; //
				if ((tmpdat == 0xef) && (start == FALSE)) //这个数据为第一个传回来的数据，也就是"指令应答”的第一个字节
						{
					count = 0;
					FIFO[0] = tmpdat; //读入第一个应答字节(OXEF)，存在第”o“个元素中
					flag = 1;
					goto
					restart0;
					//可以用中断方式进行

				}
				if (flag == 1) //第一个字节已经回来,所以flag==1成立

						{
					if (tmpdat != 0x01)  ///接收数据错误，将重新从缓冲区接收数据
							{
						flag = 0;  //接收应答失败
						result = FALSE;
						start = FALSE;
						stop = FALSE;
						count = 0;
						goto
						restart0;

					}
					///如果成功接收到0xef01，可以开始接收数据
					flag = 2;  //flag=2表示应答成功,可以开始接受数据
					count++;  //现在count=1;
					FIFO[count] = tmpdat;  //读入第二个应答字节(oxo1)，存在第”l”个元素中
					start = TURE;	//应答成功 可以开始接收数据
					goto
					restart0;
				}
				if ((flag == 2) && (start == TURE))	//flag=2;表示应答成功，可以开始接收数据了
						{
					count++; //数据元素下标++
					FIFO[count] = tmpdat; //存入数据
					if (count >= 6) {
						checksum = FIFO[count] + checksum; //计算校验和
					}
					if (count == 8) {
						package = FIFO[7] * 0X100 + FIFO[8];	//计算包长度
						stop = TURE;
					}
					if (stop) {
						if (count == package + 8) {
							checksum = checksum - FIFO[count - 1] - FIFO[count];
							if (checksum != FIFO[count] & 0xff)
								result = FALSE; //校验失败，置结果标注为0
							else
								result = TURE;
							flag = 0;
							break;
						}
					}
				}
			}
		} /////////////////////////////do内容----------------结束////////////////////////////////
		while ((clk0 <= MaxTime) && (count <= MAX_NUMBER) && (changeflag == 0)); //由定时器以及最大接收数据来控制,，保证不会在此一直循环
		TimerDisable(TIMER1_BASE, TIMER_B);
		clk0 = 0;
		FifoNumber = count;	//保存接收到的数据个数
	}
	return (result);
}
int VefPSW(void) //验证设备握手口令，成功返回1
{
	char count = 0;
	while (1) {
		if (Command(VPWD, 20) && (FifoNumber == 11) && (FIFO[9] == 0x00)) {
			return (1);
		}
		count++;
		if (count >= 2) //如果不成功，再验证一次，两次不成功，返回失败
		{
			return (0);
		}
	}
}

int Openled(void) //
{
	char count = 0;
	while (1) {
		if (Command(OpenLED, 20) && (FifoNumber == 11) && (FIFO[9] == 0x00)) {
			return (1);
		}
		count++;
		if (count >= 2) //如果不成功，再验证一次，两次不成功，返回失败
		{
			return (0);
		}
	}
}

void Clear_All(void) //清空所有指纹信息，清空前首先验证和指纹模块通讯是否正常
{

	delay1ms(200);
	Command(DELE_all, 50); //清空指纹库
	delay1ms(200);
}

char ImgProcess(char BUFID) //发获取图像并生成特征文件，存入BUFID中//输入参数为缓冲区号
{
	if (Command(GIMG, 89) && (FifoNumber == 11) && (FIFO[9] == 0x00)) {
		if (BUFID == 1) {
			if (Command(GENT1, 60) && (FifoNumber == 11) && (FIFO[9] == 0x00)) {
				return 1;
			}
		} else if (BUFID == 2) {
			if (Command(GENT2, 60) && (FifoNumber == 11) && (FIFO[9] == 0x00)) {
				return 1;
			}
		} else {
			;
		}
	}

	return 0;
}

int Searchfinger(void)  //搜索指纹(发送搜索命令、以及根据返回值确定是否存在)
		{
	if (Command(SEAT, 60) && (FifoNumber == 15) && (FIFO[9] == 0x00)) {
		SearchNumber = FIFO[10] * 0x100 + FIFO[11];  //搜索到的页码
		//MatchScore=FIFO[12]*0x100+FIFO[13]   可以在此计算得分，从而进行安全级别设定，本程序忽略
		return 1;
	} else {
		return 0;
	}
}

char search(void)		//检索用户
		{
	char SearchBuf = 0, i = 0;
	while (i < 2) {
		if (ImgProcess(1) == 1) //首先读入一次指纹这里表示有指纹按在上面了
				{
			SearchBuf = Searchfinger(); ///进行指纹比对，如果搜索到,返回搜索到的指纹序号
			if (SearchBuf == 1) {
				return SearchNumber;
			} else {
				return 255; //由于只有162个指纹，如果返回255，表示这个指纹识别失败

			}
		}
		i++;
	}
	return 0;
}

int savefingure(char ID) //保存指纹
		{
	char i = 0;
	//现在开始进行存储指纹模块的操作
	for (i = 0; i < 16; i++)	//保存指纹信息
			{
		FIFO[i] = STOR[i];
	}
	FIFO[12] = ID;           //把指纹模板存放的PAGE_ID也就是FLASH的位置
	FIFO[14] = FIFO[14] + ID;	//校验和
	if (Command(FIFO, 70) == 1)	//不成功返回0	//´此处进行存放指纹模板的命
			{
		return (1);
	}

	return (0); //不成功返回0
}

char enroll(void) //²É¼¯Á½´ÎÖ¸ÎÆ£¬Éú³É1¸ö Ö¸ÎÆÄ£°å
		{
	char temp = 0, count = 0;
	delay1ms(30);
	while (1) {
		temp = ImgProcess(1); //生成特征1
		if (temp == 1) //生成特征文件成功
				{
			break;
		} else {
			if (temp == 0) //采集指纹失败
					{
				count++;
				if (count >= 40) //如果采集了40次，还不成功，直接采集失败，直接退出enroll函数——―—返回o
					return (0);
			}
		}
	}
	//采集第一个特征成功
	GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
	SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
	GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
	SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
	//延时开始采集下一个特征

	//开始采集第二个特征
	while (1) {
		temp = ImgProcess(2); //生成特征2
		if (temp == 1) //生成特征文件2成功
				{
			if ((Command(MERG, 40) && (FifoNumber == 11) && (FIFO[9] == 0x00))
					== 0) //合并不成功返回0，成功返回1
					{
				return 0;
			} else ///特征文件合并生成模板，结果存于ModelBuffer
			{
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
				SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
				SysCtlDelay(478 * (120000000 / 3000000));   //1us*n

				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
				SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
				SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
				//响两声，表示生成一个模板成功
				return 1;
			}
		} else {
			count++;

			if (count >= 25)

				return (0);

		}
	}
}

void identity_finger(void) {

	TFTLCD_ShowString(75, 105, "Validation...", GREEN,
							WHITE);
	Openled();
	while(!VefPSW()){
		TFTLCD_ShowString(75, 105, "connecting...", GREEN,
						BLACK);
	}

	while (!identify) {
		TFTLCD_ShowString(75, 105, "Please verify your identity....", GREEN,
				BLACK);
		identify_key();
		a = key;
		switch (a) {
		case 1:
			entryfinger();
			break;
		case 2:
			 recognitionMode();
			 break;
		case 3:
			 deletMode();
			 break;
		}

	}
}
void entryfinger() {
	if (SaveNumber < 240)				//与模块握手通过
			{
		if (enroll() == 1)				//采集两次，生成指纹成功
				{
			if (savefingure(SaveNumber + 1) == 1)				//保存也成功
					{
				SaveNumber++;				//加一次
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
				SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
				SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
			}

		}

	} else {
//		TFTLCD_ShowString(75, 120, "The number of fingerprints reached a threshold", RED,
//						BLACK);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //PF1输出高，点亮LED0
		SysCtlDelay(100 * (120000000 / 3000));         //延时n*1ms
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //PF1输出低，关闭LED0
	}
}

void recognitionMode(){   //为识别模式
		searchnum = search();
		if (searchnum >= 1 && searchnum <= 240)   //最多只能存储1000个指纹
		{
			//显示检索到的指纹号
//			shownum(searchnum);

			//蜂鸣器响一声
			GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
			SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
			GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
			SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
			identify=1;
		}
		if (searchnum == ERROR)   //表示放上去了没有登记的指纹或者识别错误
		{
//			shownum(searchnum);
			//蜂鸣器响三声
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(478 * (120000000 / 3000000));   //1us*n

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //PF1输出高，点亮LED0
		SysCtlDelay(100 * (120000000 / 3000));         //延时n*1ms
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //PF1输出低，关闭LED0

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //PF2输出高，点亮LED1
		SysCtlDelay(100 * (120000000 / 3000));         //延时n*1ms
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //PF2输出低，点亮LED1

		}
}

void deletMode(){
	Clear_All();
	//指示灯亮
	GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
	SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
	GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
	SysCtlDelay(478 * (120000000 / 3000000));   //1us*n

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //PF1输出高，点亮LED0
	SysCtlDelay(100 * (120000000 / 3000));         //延时n*1ms
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //PF1输出低，关闭LED0
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //PF2输出高，点亮LED1
	SysCtlDelay(100 * (120000000 / 3000));         //延时n*1ms
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //PF2输出低，点亮LED1

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	SysCtlDelay(100 * (120000000 / 3000));         //延时n*1ms
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

	//蜂鸣器长响一声，表示清除所有指纹结束
	modeflag = 1;	     //进入录入模式
	SaveNumber = 0;
	//		shownum(0);

	GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
	SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
	GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
	SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
}

#endif /* FINGER_H_ */
