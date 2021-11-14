/*
 * I2Caccelermeter.h
 *
 *  Created on: 2020年12月31日
 *      Author: wxy
 */

#ifndef I2CACCELERMETER_H_
#define I2CACCELERMETER_H_

#define SLAVE_ADDRESS_W 0x3A //写ADXL345L时的从机地址
#define SLAVE_ADDRESS_R 0x3B //读ADXL345L时的从机地址
int mg[3] = { 0x00, 0x00, 0x00 };
char REG_ADDR[6] = { 0x32, 0x33, 0x34, 0x35, 0x36, 0x37 };

bool restart = false;

//**************************************
//延时5个指令周期
//不同的工作环境,需要调整此函数，注意时钟过快时需要修改
//**************************************
void Delay5us() {
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	_NOP();
}

//**************************************
#define NUM 0

/*******************************************
 拉高 SDA 信号
 ********************************************/
void I2C_Ca_Set_sda_high(void) {
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4); //拉高PE4
	_NOP();
	_NOP();
	return;
}

/*******************************************
 拉低SDA 信号
 ********************************************/
void I2C_Ca_Set_sda_low(void) {
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0X00000000); //拉低PE4
	_NOP();
	_NOP();
	return;
}

/*******************************************
 拉高SCL 信号
 ********************************************/
void I2C_Ca_Set_scl_high(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5); //拉高PB5
	_NOP();
	_NOP();
	return;
}

/*******************************************
 拉低SCL 信号
 ********************************************/
void I2C_Ca_Set_scl_low(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0X00000000); //拉低PB5
	_NOP();
	_NOP();
	return;
}

/*******************************************
 IIC 信号结束信号函数
 ********************************************/
void I2C_Ca_STOP(void) {
	int i;
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
	I2C_Ca_Set_sda_low();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_scl_low();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_scl_high();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_sda_high();
	for (i = NUM + 1; i > 0; i--)
		;
	return;
}

/*******************************************
 IIC 信号初始化
 ********************************************/
void I2C_Ca_Initial(void) {
	I2C_Ca_Set_scl_low();
	I2C_Ca_STOP();
	return;
}

/*******************************************
 IIC 信号起始信号函数
 ********************************************/
void I2C_Ca_START(void) {
	int i;
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
	I2C_Ca_Set_sda_high();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_scl_high();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_sda_low();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_scl_low();
	return;
}

/*******************************************
 IIC 获取应答函数
 ********************************************/
int I2C_Ca_GetACK(void) {
	int j;
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
	_NOP();
	_NOP();
	I2C_Ca_Set_scl_low();
	for (j = NUM; j > 0; j--)
		;
	I2C_Ca_Set_scl_high();
	for (j = NUM; j > 0; j--)
		;
	I2C_Ca_Set_sda_low();
	for (j = NUM; j > 0; j--)
		;
	I2C_Ca_Set_scl_low();
	return 1;
}

/*******************************************
 IIC 设置应答函数
 ********************************************/
void I2C_Ca_SetNAk(void) {
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
	I2C_Ca_Set_scl_low();
	I2C_Ca_Set_sda_high();
	I2C_Ca_Set_scl_high();
	I2C_Ca_Set_scl_low();
	return;
}

/*******************************************
 IIC 发送字节函数
 参数 	1：要发送字节值
 return ：无返回
 ********************************************/
void I2C_Ca_TxByte(unsigned char nValue) {
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
	int i;
	int j;
	for (i = 0; i < 8; i++) {
		if (nValue & 0x80) //逐位判断，1拉高SDA，0拉低SDA
			I2C_Ca_Set_sda_high();
		else
			I2C_Ca_Set_sda_low();
		for (j = NUM; j > 0; j--)
			;
		I2C_Ca_Set_scl_high();
		nValue <<= 1;    //左移1位
		for (j = NUM; j > 0; j--)
			;
		I2C_Ca_Set_scl_low();
	}

	return;
}

/*******************************************
 IIC 接收字节函数
 参数 		无
 return ：无返回
 ********************************************/
unsigned char I2C_Ca_RxByte(void) {
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);    //配置PE4为输入口
	char nTemp = 0;
	int i;

	I2C_Ca_Set_sda_high();
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	for (i = 0; i < 8; i++) {
		I2C_Ca_Set_scl_high(); //模拟SCL信号
		Delay5us();
		if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) == 0x10) //依次判断PE4（SDA）输入
				{
			nTemp |= (0x01 << (7 - i));  //8位SDA数据哪一位为高就置1
		}
		I2C_Ca_Set_scl_low();
		Delay5us();
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
void I2C_Ca_write(int num, unsigned char device_addr, char *data) {
	int i = 0;
	int count = num;
	char *send_data = data;
	unsigned char write_addr = device_addr;

	I2C_Ca_Set_scl_high();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_sda_high();
	for (i = NUM; i > 0; i--)
		;

	for (i = 0; i < count; i++) {
		I2C_Ca_START();           //模拟I2C写数据的时序
		I2C_Ca_TxByte(write_addr);
		I2C_Ca_GetACK();
		I2C_Ca_TxByte(send_data[i]);
		I2C_Ca_GetACK();
		i++;
		I2C_Ca_TxByte(send_data[i]);
		I2C_Ca_GetACK();
		I2C_Ca_STOP();
		Delay5us();
	}

}

/*******************************************
 IIC 读取数组函数
 参数  	1 num : 发送字节数
 2 device_addr : iic目标地址
 3 *data	：接收数组地址
 return ：无返回
 ********************************************/
void I2C_Ca_read(int num, unsigned char device_addr, char *data) {
	int i = 0;
	int count = num;
	char *send_data = data;
	unsigned char read_addr = device_addr;

	I2C_Ca_Set_scl_high();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_sda_high();
	for (i = NUM; i > 0; i--)
		;

	for (i = 0; i < count; i++) {
		I2C_Ca_START();        //模拟I2C读数据
		I2C_Ca_TxByte((read_addr - 1));
		I2C_Ca_GetACK();

		I2C_Ca_TxByte(send_data[i]);
		I2C_Ca_GetACK();

		I2C_Ca_START();
		I2C_Ca_TxByte(read_addr);
		I2C_Ca_GetACK();

		I2C_RECV_DATA[i] = I2C_Ca_RxByte();
		I2C_Ca_SetNAk();
		I2C_Ca_STOP();
		Delay5us();
	}
}

//************单读一个字节数据****************

unsigned char SINGLE_read(unsigned char device_addr, char data) {
	int i = 0;
	char value;
	char send_data = data;
	unsigned char read_addr = device_addr;

	I2C_Ca_Set_scl_high();
	for (i = NUM; i > 0; i--)
		;
	I2C_Ca_Set_sda_high();
	for (i = NUM; i > 0; i--)
		;

	I2C_Ca_START();
	I2C_Ca_TxByte((read_addr - 1));
	I2C_Ca_GetACK();

	I2C_Ca_TxByte(send_data);
	I2C_Ca_GetACK();

	I2C_Ca_START();
	I2C_Ca_TxByte(read_addr);
	I2C_Ca_GetACK();

	value = I2C_Ca_RxByte();
	I2C_Ca_SetNAk();
	I2C_Ca_STOP();
	Delay5us();
	return value;
}

void I2C5GPIOBEnable(void) //配置I2C5模块的IO引脚，
{
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);   //使能  GPIO portB
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);   //使能  GPIO portE

	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_4); //使能PB4、PB5作为输出口
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);   //使能PE5作为输出口
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4); //PB4口置高，即ADXL345L的CS引脚
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5); //PE5口置高，即ADXL345L的ALT ADDRESS引脚
}
//**********************************************************************
//初始化ADXL345，根据需要请参考pdf进行修改
//**********************************************************************
/**
 *  DATA_FORMAT：该寄存器的设置影响着数据寄存器中的数据格式。
 *       DATA_FORMAT该8位寄存器可控制6项设置，通过设置SPI位可设定SPI是采用3线还是4线接口模式，FULL_RES位与RANGE位，
 *      用于设定加速度量程和对应的分辨率模式，SELF_TEST位用于自检。
 *      INT_INVERT为中断模式设置:0为相对高电平中断;1为相对低电平中断。本实验设置位OxOB。
 * BW_RATE用来设定功耗模式和数据率。LOW_POWER Bit : 0，正常模式;1，低功率模式。
 *      速率位:选择器件的带宽和数据速率。本实验设置为0x0B，数据速率25Hz。
 * POWER_CTL用来设定供电模式，与BW_RATE配合，可设定数据率，默认值为100Hz。
 *      ADXL345正常供电情况下，能根据输出数据率大小自动调节其功耗。如果要进一步降低功耗，
 *      将BW_RATE寄存器中的LOW_POWER位置位，进入低功耗模式。
 *      Link Bit : 1，连接;0，非连接。
 *      AUTO_SLEEP Bit: 0，非自动sleep模式;1自动sleep 模式。
 *      Measure Bit:0独立模式;1，测量模式。
 *      Sleep Bit: 0，正常模式;1; sleep模式。本实验设置为0x08。
 * OFSX、OFSY、OFSZ用来存放标定的X、Y、Z轴的偏移量，初始化传感器时使用。
 *
 */
void Init_ADXL345(void) {
	char dataBuf1[2] = { 0x31, 0x0B };       //DATA_FORMAT
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf1);   //测量范围,正负16g，13位模式
	char dataBuf2[2] = { 0x2C, 0x18 };       //BW_RATE
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf2);   //速率设定为12.5 参考pdf13页
	char dataBuf3[2] = { 0x2D, 0x08 };        //POWER_CTL
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf3);   //选择电源模式   参考pdf24页
	char dataBuf4[2] = { 0x2E, 0x00 };        //INT_ENAVLE
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf4);   //不使用 DATA_READY 中断
	char dataBuf5[2] = { 0x1E, 0x02 };          //OFSX
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf5);   //X 偏移量 根据测试传感器的状态写入pdf29页
	char dataBuf6[2] = { 0x1F, 0x02 };       //OFSY
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf6);   //Y 偏移量 根据测试传感器的状态写入pdf29页
	char dataBuf7[2] = { 0x20, 0x06 };       //OFSZ
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf7);   //Z 偏移量 根据测试传感器的状态写入pdf29页
}

void detectAcceleration() {
	int i;
	I2C_Ca_read(6, SLAVE_ADDRESS_R, REG_ADDR); //连续6次分别读取0x32-0x37中的数据。存于I2C_Ca_RECV_DATA[]
	if(I2C_RECV_DATA[3]>16)
	{
	   I2C_RECV_DATA[3]=0xFF-I2C_RECV_DATA[3];
	   I2C_RECV_DATA[2]=0xFF-I2C_RECV_DATA[2];
	   mg[1]=I2C_RECV_DATA[2]*4+I2C_RECV_DATA[3]*1024;//正负16g，13位模式 4mg
	   UARTprintf("-%d mg     ",mg[1]);
	 }
	 else
	 {
	   	mg[1]=I2C_RECV_DATA[2]*4+I2C_RECV_DATA[3]*1024;
	    UARTprintf("%d mg       ",mg[1]);
	 }
//	for (i = 0; i < 6; i++) {
////		UARTprintf("%x   ", I2C_RECV_DATA[i]);
//		if (i == 5)
//			UARTprintf("\n");
//	}

//	SysCtlDelay(40 * (50000000 / 3000));         //延时n*1ms
}

void Restart() {
	detectAcceleration();static int count=0;
	if(mg[1]>500)count++;if(mg[1]<500)count=0;
	if(count>=5){
		count=0;
		TFTLCD_CLEAR(BLACK);
		initqizipos();
		showqipan();
		showall();
		restart=false;
	}
}

#endif /* I2CACCELERMETER_H_ */
