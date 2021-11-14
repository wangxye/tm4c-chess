/*
 * I2Caccelermeter.h
 *
 *  Created on: 2020��12��31��
 *      Author: wxy
 */

#ifndef I2CACCELERMETER_H_
#define I2CACCELERMETER_H_

#define SLAVE_ADDRESS_W 0x3A //дADXL345Lʱ�Ĵӻ���ַ
#define SLAVE_ADDRESS_R 0x3B //��ADXL345Lʱ�Ĵӻ���ַ
int mg[3] = { 0x00, 0x00, 0x00 };
char REG_ADDR[6] = { 0x32, 0x33, 0x34, 0x35, 0x36, 0x37 };

bool restart = false;

//**************************************
//��ʱ5��ָ������
//��ͬ�Ĺ�������,��Ҫ�����˺�����ע��ʱ�ӹ���ʱ��Ҫ�޸�
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
 ���� SDA �ź�
 ********************************************/
void I2C_Ca_Set_sda_high(void) {
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4); //����PE4
	_NOP();
	_NOP();
	return;
}

/*******************************************
 ����SDA �ź�
 ********************************************/
void I2C_Ca_Set_sda_low(void) {
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0X00000000); //����PE4
	_NOP();
	_NOP();
	return;
}

/*******************************************
 ����SCL �ź�
 ********************************************/
void I2C_Ca_Set_scl_high(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5); //����PB5
	_NOP();
	_NOP();
	return;
}

/*******************************************
 ����SCL �ź�
 ********************************************/
void I2C_Ca_Set_scl_low(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0X00000000); //����PB5
	_NOP();
	_NOP();
	return;
}

/*******************************************
 IIC �źŽ����źź���
 ********************************************/
void I2C_Ca_STOP(void) {
	int i;
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //����PE4Ϊ�����
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
 IIC �źų�ʼ��
 ********************************************/
void I2C_Ca_Initial(void) {
	I2C_Ca_Set_scl_low();
	I2C_Ca_STOP();
	return;
}

/*******************************************
 IIC �ź���ʼ�źź���
 ********************************************/
void I2C_Ca_START(void) {
	int i;
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //����PE4Ϊ�����
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
 IIC ��ȡӦ����
 ********************************************/
int I2C_Ca_GetACK(void) {
	int j;
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //����PE4Ϊ�����
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
 IIC ����Ӧ����
 ********************************************/
void I2C_Ca_SetNAk(void) {
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //����PE4Ϊ�����
	I2C_Ca_Set_scl_low();
	I2C_Ca_Set_sda_high();
	I2C_Ca_Set_scl_high();
	I2C_Ca_Set_scl_low();
	return;
}

/*******************************************
 IIC �����ֽں���
 ���� 	1��Ҫ�����ֽ�ֵ
 return ���޷���
 ********************************************/
void I2C_Ca_TxByte(unsigned char nValue) {
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //����PE4Ϊ�����
	int i;
	int j;
	for (i = 0; i < 8; i++) {
		if (nValue & 0x80) //��λ�жϣ�1����SDA��0����SDA
			I2C_Ca_Set_sda_high();
		else
			I2C_Ca_Set_sda_low();
		for (j = NUM; j > 0; j--)
			;
		I2C_Ca_Set_scl_high();
		nValue <<= 1;    //����1λ
		for (j = NUM; j > 0; j--)
			;
		I2C_Ca_Set_scl_low();
	}

	return;
}

/*******************************************
 IIC �����ֽں���
 ���� 		��
 return ���޷���
 ********************************************/
unsigned char I2C_Ca_RxByte(void) {
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);    //����PE4Ϊ�����
	char nTemp = 0;
	int i;

	I2C_Ca_Set_sda_high();
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	for (i = 0; i < 8; i++) {
		I2C_Ca_Set_scl_high(); //ģ��SCL�ź�
		Delay5us();
		if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4) == 0x10) //�����ж�PE4��SDA������
				{
			nTemp |= (0x01 << (7 - i));  //8λSDA������һλΪ�߾���1
		}
		I2C_Ca_Set_scl_low();
		Delay5us();
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
		I2C_Ca_START();           //ģ��I2Cд���ݵ�ʱ��
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
 IIC ��ȡ���麯��
 ����  	1 num : �����ֽ���
 2 device_addr : iicĿ���ַ
 3 *data	�����������ַ
 return ���޷���
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
		I2C_Ca_START();        //ģ��I2C������
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

//************����һ���ֽ�����****************

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

void I2C5GPIOBEnable(void) //����I2C5ģ���IO���ţ�
{
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);   //ʹ��  GPIO portB
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);   //ʹ��  GPIO portE

	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_4); //ʹ��PB4��PB5��Ϊ�����
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);   //ʹ��PE5��Ϊ�����
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4); //PB4���øߣ���ADXL345L��CS����
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5); //PE5���øߣ���ADXL345L��ALT ADDRESS����
}
//**********************************************************************
//��ʼ��ADXL345��������Ҫ��ο�pdf�����޸�
//**********************************************************************
/**
 *  DATA_FORMAT���üĴ���������Ӱ�������ݼĴ����е����ݸ�ʽ��
 *       DATA_FORMAT��8λ�Ĵ����ɿ���6�����ã�ͨ������SPIλ���趨SPI�ǲ���3�߻���4�߽ӿ�ģʽ��FULL_RESλ��RANGEλ��
 *      �����趨���ٶ����̺Ͷ�Ӧ�ķֱ���ģʽ��SELF_TESTλ�����Լ졣
 *      INT_INVERTΪ�ж�ģʽ����:0Ϊ��Ըߵ�ƽ�ж�;1Ϊ��Ե͵�ƽ�жϡ���ʵ������λOxOB��
 * BW_RATE�����趨����ģʽ�������ʡ�LOW_POWER Bit : 0������ģʽ;1���͹���ģʽ��
 *      ����λ:ѡ�������Ĵ�����������ʡ���ʵ������Ϊ0x0B����������25Hz��
 * POWER_CTL�����趨����ģʽ����BW_RATE��ϣ����趨�����ʣ�Ĭ��ֵΪ100Hz��
 *      ADXL345������������£��ܸ�����������ʴ�С�Զ������书�ġ����Ҫ��һ�����͹��ģ�
 *      ��BW_RATE�Ĵ����е�LOW_POWERλ��λ������͹���ģʽ��
 *      Link Bit : 1������;0�������ӡ�
 *      AUTO_SLEEP Bit: 0�����Զ�sleepģʽ;1�Զ�sleep ģʽ��
 *      Measure Bit:0����ģʽ;1������ģʽ��
 *      Sleep Bit: 0������ģʽ;1; sleepģʽ����ʵ������Ϊ0x08��
 * OFSX��OFSY��OFSZ������ű궨��X��Y��Z���ƫ��������ʼ��������ʱʹ�á�
 *
 */
void Init_ADXL345(void) {
	char dataBuf1[2] = { 0x31, 0x0B };       //DATA_FORMAT
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf1);   //������Χ,����16g��13λģʽ
	char dataBuf2[2] = { 0x2C, 0x18 };       //BW_RATE
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf2);   //�����趨Ϊ12.5 �ο�pdf13ҳ
	char dataBuf3[2] = { 0x2D, 0x08 };        //POWER_CTL
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf3);   //ѡ���Դģʽ   �ο�pdf24ҳ
	char dataBuf4[2] = { 0x2E, 0x00 };        //INT_ENAVLE
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf4);   //��ʹ�� DATA_READY �ж�
	char dataBuf5[2] = { 0x1E, 0x02 };          //OFSX
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf5);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
	char dataBuf6[2] = { 0x1F, 0x02 };       //OFSY
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf6);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
	char dataBuf7[2] = { 0x20, 0x06 };       //OFSZ
	I2C_Ca_write(2, SLAVE_ADDRESS_W, dataBuf7);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
}

void detectAcceleration() {
	int i;
	I2C_Ca_read(6, SLAVE_ADDRESS_R, REG_ADDR); //����6�ηֱ��ȡ0x32-0x37�е����ݡ�����I2C_Ca_RECV_DATA[]
	if(I2C_RECV_DATA[3]>16)
	{
	   I2C_RECV_DATA[3]=0xFF-I2C_RECV_DATA[3];
	   I2C_RECV_DATA[2]=0xFF-I2C_RECV_DATA[2];
	   mg[1]=I2C_RECV_DATA[2]*4+I2C_RECV_DATA[3]*1024;//����16g��13λģʽ 4mg
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

//	SysCtlDelay(40 * (50000000 / 3000));         //��ʱn*1ms
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
