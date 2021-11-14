/*
 * music.h
 *
 *  Created on: 2020年12月31日
 *      Author: wxy
 */
#include "driverlib/pwm.h"
#include "TFTinit/picture.h"
#include "TFTinit/pic.h"

#ifndef MUSIC_H_
#define MUSIC_H_
///////////////////////////////////////////////////////////////

double twotigers[32][2]={13,1.5,15,2,17,2,13,1.5,13,1.5,15,2,17,2,13,1.5,17,2,18,2,20,4,17,2,18,2,
		20,4,20,1,22,1,20,1,18,1,17,2,13,1.5,20,1,22,1,20,1,18,1,17,2,13,1.5,15,2,8,1.25,13,3,15,2,8,1.25,13,3};//两只老虎音乐

double happybirthday[25][2]={8,0.5,8,0.5,10,1,8,1,13,1.5,12,2.5,
							8,0.5,8,0.5,10,1,8,1,15,1.5,13,2.75,
							8,0.5,8,0.5,20,2,17,2,13,1.5,12,1.25,10,2,18,1,18,1,17,2,13,1.5,15,2,13,3};//生日快乐歌

double Londonbridge[24][2]={20,1.5,22,0.5,20,1,18,1,17,1,18,1,20,2,15,1,17,1,18,2,17,1,18,1,20,2,
		20,1.5,22,0.5,20,1,18,1,17,1,18,1,20,2,15,2,20,2,17,1,13,1};

double fpy[16][2]={5,10,10,5,5,9,9,16,8,8,8,9,10,5,5,16,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
double tone[] = {247,262,294,330,349,392,440,294,523,587,659,698,784,1000};//音频数据表

int SUM = 4;				//歌曲总数
int display_onlyonce = 1;
int playmode = 1;				//播放模式
int pageNum;
int music_playflag = 0;			//播放音乐序号
int process_clearflag = 0;		//播放进度条
//曲调
double tunearray[4][60]={
		{ 658, 658, 697, 783, 783, 697, 658, 586, 523,
		523, 586, 658, 658, 586, 586, 658, 658, 697, 783, 783, 697, 658, 586,
		523, 523, 586, 658, 586, 523, 523, 586, 586, 658, 523, 586, 658, 697,
		658, 523, 586, 658, 697, 658, 586, 523, 586, 392, -1, 658, 658, 697,
		783, 783, 697, 658, 697, 586, 523, 523, 586, 658, 586, 523, 523 }, //欢乐颂
		{ 880, 988, 1046, 988, 1046, 1318, 988, 988, 988, 659, 659, 880, 784, 880,
		1046, 784, 784, 784, 659, 698, 659, 698, 1046, 659, 659, 3000, 1046,
		1046, 1046, 988, 740, 698, 988, 988, 988, 3000, 880, 988, 1046, 988,
		1046, 1318, 988, 988, 988, 659, 659, 880, 784, 880, 1046, 784, 784, 784,
		587, 659, 698, 1046, 988, 988, 1046, 1046, 1175, 1175, 1318, 1046, 1046,
		1046, 1046, 988, 880, 880, 988, 831, 880, 880, 880, 1046, 1175, 1318,
		1175, 1318, 1568, 1175, 1175, 1175, 784, 784, 1046, 988, 1046, 1318,
		1318, 1318, 1318, 1318, 880, 988, 1046, 988, 1175, 1175, 1046, 784, 784,
		784, 1397, 1318, 1175, 1046, 1318, 1318, 1318, 1318, 1760, 1760, 1568,
		1568, 1318, 1175, 1046, 1046, 3000, 1046, 1175, 1046, 1175, 1175, 1568,
		1318, 1318, 1318, 1318, 1760, 1760, 1568, 1568, 1318, 1175, 1046, 1046,
		3000, 1046, 1175, 1046, 1175, 1175, 988, 880, 880, 880, 880, 988 },//天空之城
		{ 587, 523, 440, 523, 523, 440, 523, 523, 440, 523, 440, 392, 587, 523,
				440, 523, 523, 440, 523, 523, 659, 587, 523, 523, 392, 440, 659,
				659, 659, 587, 659, 659, 587, 659, 784, 659, 659, 659, 659, 659,
				659, 587, 587, 587, 587, 523, 587, 659, 587, 587, 523, 440, 523,
				523, 440, 523, 523, 440, 523, 440, 392, 392, 440, 659, 784, 784,
				659, 784, 784, 659, 587, 523, 523 }, //青花瓷
		{ 392, 523, 523, 586, 658, 523, 658, 586, 494, 523, 523, 586, 658, 523,
				494, 392, 523, 523, 586, 658, 697, 658, 586, 523, 494, 392, 440,
				494, 523, 523, 440, 494, 440, 392, 440, 494, 523, 440, 392, 440,
				392, 349, 329, 349, 392, 440, 494, 440, 392, 440, 494, 523, 440,
				392, 523, 494, 586, 523, 523 } //yankee doodle


};
int music_num = 0;
int music_time = 0;		//音乐播放计数器
int turn_on_flag = 0;
double play_speed = 1.0;
double Tarray[4]={
		0.5,1,2,0.5
};
//节拍
double durationarray[4][280]={
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		+ 0.5, 0.5, 1 + 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 + 0.5, 0.5, 1
		+ 1, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 1, 1 + 0.5, 0.5, 1 + 1, 100},
		{ 0.5, 0.5, 1.5, 0.5, 1, 1, 1, 1, 1,
		0.5, 0.5, 1.5, 0.5, 1, 1, 1, 1, 1, 1, 1.5, 0.5, 1, 1, 1, 1, 0.5, 0.5,
		0.5, 0.5, 1 + 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 1 + 0.5, 0.5, 1, 1, 1,
		1, 1, 0.5, 0.5, 1 + 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 0.5, 0.25,
		0.25, 0.25, 0.5, 0.5, 0.5, 0.5, 0.25, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 1, 1,
		1, 1, 1, 0.5, 0.5, 1 + 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 1.5, 0.5, 1,
		1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 0.5, 0.5, 1.5, 0.25, 0.5, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 0.5, 0.5, 1, 0.5, 0.5, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 0.5, 0.5, 1, 0.5, 0.25, 0.5, 1, 1,
		1, 1, 0.5, 0.5 ,100},
		{ 2, 2, 2, 4, 2, 2,
		4, 2, 2, 2, 2, 4, 2, 2, 2, 4, 2, 2, 4, 2, 2, 2, 2, 4, 2, 2, 2, 4, 2, 2,
		4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 4, 4, 2, 2, 2, 4, 2,
		2, 4, 2, 2, 2, 2, 4, 2, 2, 2, 4, 2, 2, 4, 2, 2, 2, 2, 4,100},
		{ 1, 1, 1.5, 1, 1, 1, 1, 1, 1, 1, 1, 1.5, 1, 1, 1, 1, 1, 1, 1, 1.5,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1 + 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1 + 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 + 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 + 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1.5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.5,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 100 }
};


void musicPlayer(void){
	int i,j,n;
	for (i = 0; i < 24; i++) {
		n = 400 * Londonbridge[i][1];
		for (j = 0; j < n; j++) {
			voice(Londonbridge[i][0]); //音乐播放，逐个播放音，根据拍数拉长
		}
	}
}

void Sound(int frq)
{
	int time;
	if(frq != 1000)
	{
		time = 500000/((int)frq);
		GPIOPinWrite(GPIO_PORTM_BASE,GPIO_PIN_5,GPIO_PIN_5);
		SysCtlDelay(time*(120000000/3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE,GPIO_PIN_5,0);
		SysCtlDelay(time*(120000000/3000000));   //1us*n
	}
}

void voice(uint32_t key)    //每一个音调的延时函数
{
	//钢琴中音区
	if (key == 13)    //1
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(478 * (120000000 / 3000000));   //1us*n
	}

	if (key == 14)   //higher1
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(451 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(451 * (120000000 / 3000000));   //1us*n
	}

	if (key == 15)   //2
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(426 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(426 * (120000000 / 3000000));   //1us*n
	}

	if (key == 16)   //higher2
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(402 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(402 * (120000000 / 3000000));   //1us*n
	}

	if (key == 17)   //3
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(379 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(379 * (120000000 / 3000000));   //1us*n
	}

	if (key == 18)   //4
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(358 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(358 * (120000000 / 3000000));   //1us*n
	}

	if (key == 19)   //higher4
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(338 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(338 * (120000000 / 3000000));   //1us*n
	}

	if (key == 20)   //5
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(319 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(319 * (120000000 / 3000000));   //1us*n
	}

	if (key == 21)   //higher5
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(301 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(301 * (120000000 / 3000000));   //1us*n
	}

	if (key == 22)   //6
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(284 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(284 * (120000000 / 3000000));   //1us*n
	}

	if (key == 23)   //higher6
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(268 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(268 * (120000000 / 3000000));   //1us*n
	}

	if (key == 24)   //7   506
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(254 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(254 * (120000000 / 3000000));   //1us*n
	}

	//低音区
	if (key == 1)   //1
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(956 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(956 * (120000000 / 3000000));   //1us*n
	}

	if (key == 2)   //1#
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(903 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(903 * (120000000 / 3000000));   //1us*n
	}

	if (key == 3)   //2
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(852 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(852 * (120000000 / 3000000));   //1us*n
	}

	if (key == 4)   //2#
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(804 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(804 * (120000000 / 3000000));   //1us*n
	}

	if (key == 5)   //3
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(759 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(759 * (120000000 / 3000000));   //1us*n
	}

	if (key == 6)   //4
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(716 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(716 * (120000000 / 3000000));   //1us*n
	}

	if (key == 7)   //4#
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(676 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(676 * (120000000 / 3000000));   //1us*n
	}

	if (key == 8)   //5
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(638 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(638 * (120000000 / 3000000));   //1us*n
	}

	if (key == 9)   //5#
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(602 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(602 * (120000000 / 3000000));   //1us*n
	}

	if (key == 10)   //6
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(568 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(568 * (120000000 / 3000000));   //1us*n
	}

	if (key == 11)   //6#
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(536 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(536 * (120000000 / 3000000));   //1us*n
	}

	if (key == 12)   //7
			{
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(506 * (120000000 / 3000000));   //1us*n
		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(506 * (120000000 / 3000000));   //1us*n
	}
}

//void showmusiclsit(){
//	int i;
//	for(i = 0 ;i < 8 ;i++){
//		TFTLCD_DrawHorizontalLine(60,170,225+20*i,BLACK);
//	}
//
//	for(i = 0 ;i < 4 ;i++){
//		TFTLCD_DrawVerticalLine(225+20*i/2,245+20*i/2,60,BLACK);
//		TFTLCD_DrawVerticalLine(225+20*i/2,245+20*i/2,170,BLACK);
//	}
//
//
////	TFTLCD_DrawHorizontalLine(60,170,245,BLACK);
//}

#endif /* MUSIC_H_ */
