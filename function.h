/*
 * function.h
 *
 *  Created on: 2020年12月25日
 *      Author: wxy
 */

#include "TFTinit/picture.h"
#include "TFTinit/pic.h"
#include "I2Cmizuguang.h"
#include "I2Caccelermeter.h"
typedef struct qizi
{
	uint32_t color;
	int x;//棋子坐标
	int y;
	int dead;
	int rule;//车=0，马=1，炮=2，兵卒4，象5，士6，帅7，将8
}qizi;
int xor(int a,int b)
{
return (a||b)&&!(a&&b);
}


int offsetx[9]={17,	43,	69,	95,	121,	147,	173,	199,	225,
};

int offsety[17]={
		15,
		41,
		67,
		93,
		119,
		145,
		171,
		197,
		223,
		249,
		275,
		301,
		327,
		353,
		379,
		405,
		431

};
/////////////////////////////////////////////////////////////////////////////////////////////////
struct qizi shiziblue={BLUE,0,0,0};
struct qizi shizired={RED,0,9,0};

struct qizi heiche1={BLUE,0,0,0,0};
struct qizi heiche2={BLUE,8,0,0,0};
struct qizi hongche1={RED,0,9,0,0};
struct qizi hongche2={RED,8,9,0,0};

struct qizi heima1={BLUE,1,0,0,1 };
struct qizi heima2={BLUE,7,0,0,1 };
struct qizi hongma1={RED,1,9,0,1 };
struct qizi hongma2={RED,7,9,0,1 };

struct qizi heipao1={BLUE,1,2,0,2 };
struct qizi heipao2={BLUE,7,2,0,2 };
struct qizi hongpao1={RED,1,7,0,2 };
struct qizi hongpao2={RED,7,7,0,2 };


struct qizi zu1={BLUE,0,3,0,4};
struct qizi zu2={BLUE,2,3,0,4};
struct qizi zu3={BLUE,4,3,0,4};
struct qizi zu4={BLUE,6,3,0,4};
struct qizi zu5={BLUE,8,3,0,4};

struct qizi bing1={RED,0,6,0,4};
struct qizi bing2={RED,2,6,0,4};
struct qizi bing3={RED,4,6,0,4};
struct qizi bing4={RED,6,6,0,4};
struct qizi bing5={RED,8,6,0,4};

struct qizi heixiang1={BLUE,2,0,0,5};
struct qizi heixiang2={BLUE,6,0,0,5};
struct qizi hongxiang1={RED,2,9,0,5};
struct qizi hongxiang2={RED,6,9,0,5};


struct qizi heishi2={BLUE,5,0,0,6};
struct qizi heishi1={BLUE,3,0,0,6};
struct qizi hongshi1={RED,3,9,0,6};
struct qizi hongshi2={RED,5,9,0,6};


struct qizi shuai={RED,4,9,0,7};
struct qizi jiang={BLUE,4,0,0,8};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct qizi *qz[32]={&heiche1,&heiche2,&hongche1,&hongche2,&heima1,&heima2,&hongma1,&hongma2,&heipao1,&heipao2,&hongpao1,&hongpao2,&heixiang1,&heixiang2,&hongxiang1,&hongxiang2,&heishi1,&heishi2,&hongshi1,&hongshi2,
		&zu1,&zu2,&zu3,&zu4,&zu5,&bing1,&bing2,&bing3,&bing4,&bing5,&shuai,&jiang
};
char select=0;uint32_t lastcolor;char deadnumx=0;char deadnumy=0; int ii;

char redselect=0;uint32_t redlastcolor; int jj;

volatile uint32_t key;
int a;
int b = 0;

//队列--> 复盘
void move(qizi next,int num){
	int i;

	for (i = 0; i < 32; i++)
		if ((*qz[i]).x == (next).x && (*qz[i]).y == (next).y)
			break;
	if (i != 32 && i != num) {
		(*qz[i]).dead = 1;
		if (jiang.dead || shuai.dead) {
			TFTLCD_ShowString(80, 180, "GAMEOVER", CYAN, LIGHTBLUE);
//			redscore++;
			restart = true;
		}
		(*qz[i]).x = deadnumx;
		(*qz[i]).y = deadnumy + 12;
		deadnumx++;
		if (deadnumx == 9) {
			deadnumy++;
			deadnumx = 0;
		}
	}
	clearshow(offsetx[(*qz[num]).x]-10,offsety[(*qz[num]).y]-10,20,20,gImage_jiang,jiang.color);
	(*qz[num]).x=(next).x;
	(*qz[num]).y=(next).y;
//	TFTLCD_CLEAR(BLACK);
	showqipan();
	showall();
	SysCtlDelay(40*(50000000/3000));

}
/*
void initqi(){

	struct qizi shiziblue0={BLUE,0,0,0};
	struct qizi shizired0={RED,0,9,0};

	struct qizi heiche10={BLUE,0,0,0,0};
	struct qizi heiche20={BLUE,8,0,0,0};
	struct qizi hongche10={RED,0,9,0,0};
	struct qizi hongche20={RED,8,9,0,0};

	struct qizi heima10={BLUE,1,0,0,1 };
	struct qizi heima20={BLUE,7,0,0,1 };
	struct qizi hongma10={RED,1,9,0,1 };
	struct qizi hongma20={RED,7,9,0,1 };

	struct qizi heipao10={BLUE,1,2,0,2 };
	struct qizi heipao20={BLUE,7,2,0,2 };
	struct qizi hongpao10={RED,1,7,0,2 };
	struct qizi hongpao20={RED,7,7,0,2 };


	struct qizi zu10={BLUE,0,3,0,4};
	struct qizi zu20={BLUE,2,3,0,4};
	struct qizi zu30={BLUE,4,3,0,4};
	struct qizi zu40={BLUE,6,3,0,4};
	struct qizi zu50={BLUE,8,3,0,4};

	struct qizi bing10={RED,0,6,0,4};
	struct qizi bing20={RED,2,6,0,4};
	struct qizi bing30={RED,4,6,0,4};
	struct qizi bing40={RED,6,6,0,4};
	struct qizi bing50={RED,8,6,0,4};

	struct qizi heixiang10={BLUE,2,0,0,5};
	struct qizi heixiang20={BLUE,6,0,0,5};
	struct qizi hongxiang10={RED,2,9,0,5};
	struct qizi hongxiang20={RED,6,9,0,5};


	struct qizi heishi20={BLUE,5,0,0,6};
	struct qizi heishi10={BLUE,3,0,0,6};
	struct qizi hongshi10={RED,3,9,0,6};
	struct qizi hongshi20={RED,5,9,0,6};


	struct qizi shuai0={RED,4,9,0,7};
	struct qizi jiang0={BLUE,4,0,0,8};


	////////////////////

	shiziblue=shiziblue0;
	shizired=shizired0;

	heiche1=heiche10;
	heiche2=heiche20;
	hongche1=hongche10;
	hongche2=hongche20;

	heima1=heima10;
	heima2=heima20;
	hongma1=hongma10;
	hongma2=hongma20;

	heipao1=heipao10;
	heipao2=heipao20;
	hongpao1=hongpao10;
	hongpao2=hongpao20;


	zu1=zu10;
	zu2=zu20;
	zu3=zu30;
	zu4=zu40;
	zu5=zu50;

	bing1=bing10;
	bing2=bing20;
	bing3=bing30;
	bing4=bing40;
	bing5=bing50;

	heixiang1=heixiang10;
	heixiang2=heixiang20;
	hongxiang1=hongxiang10;
	hongxiang2=hongxiang20;


	heishi2=heishi20;
	heishi1=heishi10;
	hongshi1=hongshi10;
	hongshi2=hongshi20;


	shuai=shuai0;
	jiang=jiang0;


}*/
void initqizipos(){
	//重新初始化棋盘
	rche();
	rshizi();
	rshi();
	rma();
	rpao();
	rzu();
	rbing();
	rxiang();
	rjiangshuai();
	int i;for(i=0;i<32;i++){
		(*qz[i]).dead=0;
	}
	//重新初始化标志位
	deadnumx=0;
	deadnumy=0;
	select=0;
	ii=0;
	jj=0;
	redselect=0;
}

void rche(void){
	hongche1.x=0;
	hongche1.y=9;
	hongche2.x=8;
	hongche2.y=9;

	heiche1.x=0;
	heiche1.y=0;
	heiche2.x=8;
	heiche2.y=0;
}

void rshizi(void){
	shiziblue.x=0;
	shiziblue.y=0;
	shizired.x=0;
	shizired.y=9;

}

void rma(void){
	heima1.x=1;
	heima1.y=0;
	heima2.x=7;
	heima2.y=0;

	hongma1.x=1;
	hongma1.y=9;
	hongma2.x=7;
	hongma2.y=9;

}

void rpao(void){
	heipao1.x=1;
	heipao1.y=2;
	heipao2.x=7;
	heipao2.y=2;

	hongpao1.x=1;
	hongpao1.y=7;
	hongpao2.x=7;
	hongpao2.y=7;

}

void rzu(void){
	zu1.x=0;
	zu1.y=3;
	zu2.x=2;
	zu2.y=3;

	zu3.x=4;
	zu3.y=3;
	zu4.x=6;
	zu4.y=3;

	zu5.x=8;
	zu5.y=3;

}

void rbing(void){
	bing1.x=0;
	bing1.y=6;
	bing2.x=2;
	bing2.y=6;

	bing3.x=4;
	bing3.y=6;
	bing4.x=6;
	bing4.y=6;

	bing5.x=8;
	bing5.y=6;

}

void rxiang(void){
	heixiang1.x=2;
	heixiang1.y=0;
	heixiang2.x=6;
	heixiang2.y=0;

	hongxiang1.x=2;
	hongxiang1.y=9;
	hongxiang2.x=6;
	hongxiang2.y=9;

}

void rshi(void){
	heishi1.x=3;
	heishi1.y=0;
	heishi2.x=5;
	heishi2.y=0;

	hongshi1.x=3;
	hongshi1.y=9;
	hongshi2.x=5;
	hongshi2.y=9;

}

void rjiangshuai(void){

	shuai.x=4;
	shuai.y=9;
//	shuai.dead=0;
	jiang.x=4;
	jiang.y=0;
//	jiang.dead=0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawline(uint32_t x0,uint32_t y0,uint32_t x1,uint32_t y1,uint32_t color)
{
	uint32_t i=0,j=0,PointNum = 0;
	PointNum = x1-x0;
	for(i=x0;i<=x1;i++)
		for(j=y0;j<=y1;j++)
	{
		TFTLCD_DrawPoint(i,j,color);
	}

}
void ShowPic(uint32_t offsetx,uint32_t offsety,uint32_t x,uint32_t y,uint8_t gImage[],uint16_t color)
{
	uint32_t temp,i,j,m;m=0;
	for(i=offsety;i<(y+offsety);i++)
	{
		for(j=offsetx;j<(x+offsetx);j++)
		{
			temp=gImage[m];
			if(temp!=0xFF)
			TFTLCD_DrawPoint(j, i,color);
			else TFTLCD_DrawPoint(j, i,BLACK);
			m++;
		}
	}
}
void ShowShizi(uint32_t offsetx,uint32_t offsety,uint32_t x,uint32_t y,uint8_t gImage[],uint16_t color)
{
	uint32_t temp,i,j,m;m=0;
	for(i=offsety;i<(y+offsety);i++)
	{
		for(j=offsetx;j<(x+offsetx);j++)
		{
			temp=gImage[m];
			if(temp!=0xFF)
			TFTLCD_DrawPoint(j, i,color);
			m++;
		}
	}
}
void clearshow(uint32_t offsetx,uint32_t offsety,uint32_t x,uint32_t y,uint8_t gImage[],uint16_t color)
{
	uint32_t temp,i,j,m;m=0;
	for(i=offsety;i<(y+offsety);i++)
	{
		for(j=offsetx;j<(x+offsetx);j++)
		{
			temp=gImage[m];
			if(temp!=0xFF)
			TFTLCD_DrawPoint(j, i,BLACK);
			else TFTLCD_DrawPoint(j, i,BLACK);
			m++;
		}
	}
}

void ShowQizi(uint32_t offsetx,uint32_t offsety,uint32_t x,uint32_t y,uint8_t gImage[],uint16_t color)
{
	uint32_t temp,i,j,m;m=0;
	for(i=offsety;i<(y+offsety);i++)
	{
		for(j=offsetx;j<(x+offsetx);j++)
		{
			temp=gImage[m];
			if(temp!=0xFF)
			TFTLCD_DrawPoint(j, i,color);
			else TFTLCD_DrawPoint(j, i,BLACK);
			m++;
		}
	}
}
void showqipan(){
	int i;
	for(i=0;i<10;i++)
	{TFTLCD_DrawHorizontalLine(17,225,offsety[i],WHITE);
	}
	for(i=0;i<9;i++){
	TFTLCD_DrawVerticalLine(17,119,offsetx[i],WHITE);
	TFTLCD_DrawVerticalLine(145,249,offsetx[i],WHITE);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void showall(){

//	ShowPic(0,0,240,267,gImage_qipan,WHITE);

	ShowQizi(offsetx[heiche1.x]-10,offsety[heiche1.y]-10,20,20,gImage_che,heiche1.color);
	ShowQizi(offsetx[heiche2.x]-10,offsety[heiche2.y]-10,20,20,gImage_che,heiche2.color);
	ShowQizi(offsetx[hongche1.x]-10,offsety[hongche1.y]-10,20,20,gImage_che,hongche1.color);
	ShowQizi(offsetx[hongche2.x]-10,offsety[hongche2.y]-10,20,20,gImage_che,hongche2.color);

	ShowQizi(offsetx[heima1.x]-10,offsety[heima1.y]-10,20,20,gImage_ma,heima1.color);
	ShowQizi(offsetx[heima2.x]-10,offsety[heima2.y]-10,20,20,gImage_ma,heima2.color);
	ShowQizi(offsetx[hongma1.x]-10,offsety[hongma1.y]-10,20,20,gImage_ma,hongma1.color);
	ShowQizi(offsetx[hongma2.x]-10,offsety[hongma2.y]-10,20,20,gImage_ma,hongma2.color);

	ShowQizi(offsetx[heipao1.x]-10,offsety[heipao1.y]-10,20,20,gImage_pao,heipao1.color);
	ShowQizi(offsetx[heipao2.x]-10,offsety[heipao2.y]-10,20,20,gImage_pao,heipao2.color);
	ShowQizi(offsetx[hongpao1.x]-10,offsety[hongpao1.y]-10,20,20,gImage_pao,hongpao1.color);
	ShowQizi(offsetx[hongpao2.x]-10,offsety[hongpao2.y]-10,20,20,gImage_pao,hongpao2.color);

	ShowQizi(offsetx[heixiang1.x]-10,offsety[heixiang1.y]-10,20,20,gImage_heixiang,heixiang1.color);
	ShowQizi(offsetx[heixiang2.x]-10,offsety[heixiang2.y]-10,20,20,gImage_heixiang,heixiang2.color);
	ShowQizi(offsetx[hongxiang1.x]-10,offsety[hongxiang1.y]-10,20,20,gImage_hongxiang,hongxiang1.color);
	ShowQizi(offsetx[hongxiang2.x]-10,offsety[hongxiang2.y]-10,20,20,gImage_hongxiang,hongxiang2.color);


	ShowQizi(offsetx[heishi1.x]-10,offsety[heishi1.y]-10,20,20,gImage_heishi,heishi1.color);
	ShowQizi(offsetx[heishi2.x]-10,offsety[heishi2.y]-10,20,20,gImage_heishi,heishi2.color);

	ShowQizi(offsetx[hongshi1.x]-10,offsety[hongshi1.y]-10,20,20,gImage_hongshi,hongshi1.color);
	ShowQizi(offsetx[hongshi2.x]-10,offsety[hongshi2.y]-10,20,20,gImage_hongshi,hongshi2.color);


	ShowQizi(offsetx[zu1.x]-10,offsety[zu1.y]-10,20,20,gImage_zu,zu1.color);
	ShowQizi(offsetx[zu2.x]-10,offsety[zu2.y]-10,20,20,gImage_zu,zu2.color);
	ShowQizi(offsetx[zu3.x]-10,offsety[zu3.y]-10,20,20,gImage_zu,zu3.color);
	ShowQizi(offsetx[zu4.x]-10,offsety[zu4.y]-10,20,20,gImage_zu,zu4.color);
	ShowQizi(offsetx[zu5.x]-10,offsety[zu5.y]-10,20,20,gImage_zu,zu5.color);


	ShowQizi(offsetx[bing1.x]-10,offsety[bing1.y]-10,20,20,gImage_bing,bing1.color);
	ShowQizi(offsetx[bing2.x]-10,offsety[bing2.y]-10,20,20,gImage_bing,bing2.color);
	ShowQizi(offsetx[bing3.x]-10,offsety[bing3.y]-10,20,20,gImage_bing,bing3.color);
	ShowQizi(offsetx[bing4.x]-10,offsety[bing4.y]-10,20,20,gImage_bing,bing4.color);
	ShowQizi(offsetx[bing5.x]-10,offsety[bing5.y]-10,20,20,gImage_bing,bing5.color);



	ShowQizi(offsetx[jiang.x]-10,offsety[jiang.y]-10,20,20,gImage_jiang,jiang.color);
	ShowQizi(offsetx[shuai.x]-10,offsety[shuai.y]-10,20,20,gImage_shuai,shuai.color);



	ShowShizi(offsetx[shiziblue.x]-15,offsety[shiziblue.y]-15,30,30,gImage_shizi,GREEN);
	ShowShizi(offsetx[shizired.x]-15,offsety[shizired.y]-15,30,30,gImage_shizi,YELLOW);


}


////////////////////////////////////////////////////////////////////////////////////
int rule(struct qizi judged){
	if(judged.rule==0)			return ruleche(judged);;
	if(judged.rule==1)			return rulema(judged);
	if(judged.rule==2)			return rulepao(judged);
	if(judged.rule==4)			return rulezu(judged);
	if(judged.rule==5)			return ruleheixiang(judged);
	if(judged.rule==6)			return ruleheishi(judged);
	if(judged.rule==8)			return rulejiang(judged);
}
//将 车
int rulered(struct qizi judged){
	if(judged.rule==0)			return ruleredche(judged);
	if(judged.rule==1)			return ruleredma(judged);
	if(judged.rule==2)			return ruleredpao(judged);
	if(judged.rule==4)			return rulebing(judged);
	if(judged.rule==5)			return rulehongxiang(judged);
	if(judged.rule==6)			return rulehongshi(judged);
	if(judged.rule==7)			return ruleshuai(judged);
}
/////////////////////////////////////////////////////////////////////////////////////////
int ruleche(struct qizi judged){
	int dx,dy;//坐标变化量
	int i,j;
	dx=shiziblue.x-judged.x;
	dy=shiziblue.y-judged.y;
	if (xor(dx,dy)){
		if(dx>0)
			for(j=judged.x+1;j<shiziblue.x;j++){
			for(i=0;i<32;i++)
				if( (*(qz[i])).x==j&&(*(qz[i])).y==judged.y )
					return 0;

		}

		if(dx<0)
			for(j=judged.x-1;j>shiziblue.x;j--){
			for(i=0;i<32;i++)
				if( (*(qz[i])).x==j&&(*(qz[i])).y==judged.y )
					return 0;


		}

		if(dy>0)
			for(j=judged.y+1;j<shiziblue.y;j++){
					for(i=0;i<32;i++)
						if((*(qz[i])).y==j&&(*(qz[i])).x==judged.x)
							return 0;


				}
		if(dy<0)
			for(j=judged.y-1;j>shiziblue.y;j--){
			for(i=0;i<32;i++)
				if((*(qz[i])).y==j&&(*(qz[i])).x==judged.x)
					return 0;



		}
	}

	else return 0;
	return 1;

}


int ruleredche(struct qizi judged){
	int dx,dy;//坐标变化量
	int i,j;
	dx=shizired.x-judged.x;
	dy=shizired.y-judged.y;
	if (xor(dx,dy)){
		if(dx>0)
			for(j=judged.x+1;j<shizired.x;j++){
			for(i=0;i<32;i++)
				if( (*(qz[i])).x==j&&(*(qz[i])).y==judged.y )
					return 0;

		}

		if(dx<0)
			for(j=judged.x-1;j>shizired.x;j--){
			for(i=0;i<32;i++)
				if( (*(qz[i])).x==j&&(*(qz[i])).y==judged.y )
					return 0;


		}

		if(dy>0)
			for(j=judged.y+1;j<shizired.y;j++){
					for(i=0;i<32;i++)
						if((*(qz[i])).y==j&&(*(qz[i])).x==judged.x)
							return 0;


				}
		if(dy<0)
			for(j=judged.y-1;j>shizired.y;j--){
			for(i=0;i<32;i++)
				if((*(qz[i])).y==j&&(*(qz[i])).x==judged.x)
					return 0;



		}
	}

	else return 0;
	return 1;

}


int rulema(struct qizi judged){
	int x,y;//马脚坐标
	if (abs(judged.x-shiziblue.x)+abs(judged.y-shiziblue.y)==3&&
	(abs(judged.x-shiziblue.x)==1||abs(judged.y-shiziblue.y)==1))//椹蛋鏃�
	{
	if(abs(judged.x-shiziblue.x)==2){
		x=(judged.x+shiziblue.x)/2;
	y=judged.y;}
	else if(abs(judged.y-shiziblue.y)==2){
		y=(judged.y+shiziblue.y)/2;
					x=judged.x;
	}
	int i=0;
	for(i=0;i<32;i++)
		if((*(qz[i])).x==x&&(*(qz[i])).y==y)
			return 0;
	if(i==32)
	return 1;


}

		return 0;
}

int ruleredma(struct qizi judged){


	int x,y;//马脚坐标
	if (abs(judged.x-shizired.x)+abs(judged.y-shizired.y)==3&&
	(abs(judged.x-shizired.x)==1||abs(judged.y-shizired.y)==1))//椹蛋鏃�
	{


	if(abs(judged.x-shizired.x)==2){
		x=(judged.x+shizired.x)/2;
	y=judged.y;}

	else if(abs(judged.y-shizired.y)==2){
		y=(judged.y+shizired.y)/2;
					x=judged.x;
	}

	int i=32;
	for(i=0;i<32;i++)
		if((*(qz[i])).x==x&&(*(qz[i])).y==y)
			return 0;
	if(i==32)
	return 1;


}
	return 0;
}

int rulepao(struct qizi judged){
	int dx,dy;//坐标变化量
	int i,j;
	int count=0;
	dx=shiziblue.x-judged.x;
	dy=shiziblue.y-judged.y;
	if (xor(dx,dy)){
		if(dx>0)
			for(j=judged.x+1;j<shiziblue.x;j++){
			for(i=0;i<32;i++)
				if( (*(qz[i])).x==j&&(*(qz[i])).y==judged.y )
					count++;

		}

		if(dx<0)
			for(j=judged.x-1;j>shiziblue.x;j--){
			for(i=0;i<32;i++)
				if( (*(qz[i])).x==j&&(*(qz[i])).y==judged.y )
					count++;


		}

		if(dy>0)
			for(j=judged.y+1;j<shiziblue.y;j++){
					for(i=0;i<32;i++)
						if((*(qz[i])).y==j&&(*(qz[i])).x==judged.x)
							count++;


				}
		if(dy<0)
			for(j=judged.y-1;j>shiziblue.y;j--){
			for(i=0;i<32;i++)
				if((*(qz[i])).y==j&&(*(qz[i])).x==judged.x)
					count++;




		}
	}

	else return 0;
	for(i=0;i<32;i++)
					if((*qz[i]).x==shiziblue.x&&(*qz[i]).y==shiziblue.y)break;
	if((count==1&&i!=32)||count==0&&i==32)
		return 1;
	else return 0;

}





int ruleredpao(struct qizi judged){
	int dx,dy;//坐标变化量
	int i,j;
	int count=0;
	dx=shizired.x-judged.x;
	dy=shizired.y-judged.y;
	if (xor(dx,dy)){
		if(dx>0)
			for(j=judged.x+1;j<shizired.x;j++){
			for(i=0;i<32;i++)
				if( (*(qz[i])).x==j&&(*(qz[i])).y==judged.y )
					count++;

		}

		if(dx<0)
			for(j=judged.x-1;j>shizired.x;j--){
			for(i=0;i<32;i++)
				if( (*(qz[i])).x==j&&(*(qz[i])).y==judged.y )
					count++;


		}

		if(dy>0)
			for(j=judged.y+1;j<shizired.y;j++){
					for(i=0;i<32;i++)
						if((*(qz[i])).y==j&&(*(qz[i])).x==judged.x)
							count++;


				}
		if(dy<0)
			for(j=judged.y-1;j>shizired.y;j--){
			for(i=0;i<32;i++)
				if((*(qz[i])).y==j&&(*(qz[i])).x==judged.x)
					count++;




		}
	}

	else return 0;
	for(i=0;i<32;i++)
					if((*qz[i]).x==shizired.x&&(*qz[i]).y==shizired.y)break;
	if((count==1&&i!=32)||count==0&&i==32)
		return 1;
	else return 0;

}

int rulezu(struct qizi judged){
	int dx,dy;//坐标变化量
	int i,j;
	int count=0;
	dx=shiziblue.x-judged.x;
	dy=shiziblue.y-judged.y;
	if(dx*dy!=0)
		return 0;
		if(dx!=0&&judged.y>=5||dy==1)
		return 1;

	else return 0;
}

int rulebing(struct qizi judged){
	int dx,dy;
	int i,j;
	int count=0;
	dx=shizired.x-judged.x;
	dy=shizired.y-judged.y;
	if(dx*dy!=0)return 0;
		if(dx!=0&&judged.y<=4||dy==-1)
		return 1;

	else return 0;
}

int ruleshuai(struct qizi judged){
	int dx,dy;
	int i,j;
	int count=0;
	dx=shizired.x-judged.x;
	dy=shizired.y-judged.y;
	if(dx*dy!=0)return 0;
	if(!(abs(dx)==1||abs(dy)==1))return 0;
		if(shizired.x<=5&&shizired.x>=3&&shizired.y<=9&&shizired.y>=8)
		return 1;

	else return 0;
}

int rulejiang(struct qizi judged){
	int dx,dy;
	int i,j;
	int count=0;
	dx=shiziblue.x-judged.x;
	dy=shiziblue.y-judged.y;
	if(dx*dy!=0)return 0;
	if(!(abs(dx)==1||abs(dy)==1))return 0;
	if(shiziblue.x<=5&&shiziblue.x>=3&&shiziblue.y<=1&&shiziblue.y>=0)
		return 1;

	else return 0;
}

int rulehongshi(struct qizi judged){
	int dx,dy;
	int i,j;
	int count=0;
	dx=shizired.x-judged.x;
	dy=shizired.y-judged.y;
	if(abs(dx)+abs(dy)!=2&&abs(dx*dy)!=1)return 0;
		if(shizired.x<=5&&shizired.x>=3)
		return 1;

	else return 0;
}

int ruleheishi(struct qizi judged){
	int dx,dy;
	int i,j;
	int count=0;
	dx=shiziblue.x-judged.x;
	dy=shiziblue.y-judged.y;
	if(abs(dx)+abs(dy)!=2&&abs(dx*dy)!=1)return 0;
		if(shiziblue.x<=5&&shiziblue.x>=3)
		return 1;

	else return 0;
}

int rulehongxiang(struct qizi judged){
	int dx,dy;//坐标变化量
	int x,y;//象脚坐标
	int i,j;
	int count=0;
	dx=shizired.x-judged.x;
	dy=shizired.y-judged.y;
	x=(shizired.x+judged.x)/2;
	y=(shizired.y+judged.y)/2;

	if(abs(dx)+abs(dy)!=4&&abs(dx*dy)!=4)return 0;
	for(i=0;i<32;i++)
		if((*(qz[i])).x==x&&(*(qz[i])).y==y)
			return 0;
		if(shizired.y==9||shizired.y==7||shizired.y==5)
		return 1;

	else return 0;
}

int ruleheixiang(struct qizi judged){
	int dx,dy;//坐标变化量
	int x,y;//象脚坐标
	int i,j;
	int count=0;
	x=(shiziblue.x+judged.x)/2;
	y=(shiziblue.y+judged.y)/2;
	dx=shiziblue.x-judged.x;
	dy=shiziblue.y-judged.y;
	if(abs(dx)+abs(dy)!=4&&abs(dx*dy)!=4)return 0;
	for(i=0;i<32;i++)
		if((*(qz[i])).x==x&&(*(qz[i])).y==y)
			return 0;
		if(shizired.y==9||shizired.y==7||shizired.y==5)
		return 1;

	else return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void bluezuo(){
	if(shiziblue.x>0){
		ShowShizi(offsetx[shiziblue.x]-15,offsety[shiziblue.y]-15,30,30,gImage_shizi,BLACK);
	shiziblue.x-=1;}
}
void blueyou(){
	if(shiziblue.x<8){
		ShowShizi(offsetx[shiziblue.x]-15,offsety[shiziblue.y]-15,30,30,gImage_shizi,BLACK);
	shiziblue.x+=1;}
}
void blueshang(){
	if(shiziblue.y>0)	{
		ShowShizi(offsetx[shiziblue.x]-15,offsety[shiziblue.y]-15,30,30,gImage_shizi,BLACK);
	shiziblue.y-=1;}
}
void bluexia(){
	if(shiziblue.y<9)	{
		ShowShizi(offsetx[shiziblue.x]-15,offsety[shiziblue.y]-15,30,30,gImage_shizi,BLACK);
	shiziblue.y+=1;}
}
void bluecancel(){
	select=0;
	(*qz[ii]).color=lastcolor;
}

void redzuo(){
	if(shizired.x>0){
		ShowShizi(offsetx[shizired.x]-15,offsety[shizired.y]-15,30,30,gImage_shizi,BLACK);
	shizired.x-=1;}
}
void redyou(){
	if(shizired.x<8){
		ShowShizi(offsetx[shizired.x]-15,offsety[shizired.y]-15,30,30,gImage_shizi,BLACK);
	shizired.x+=1;}
}
void redshang(){
	if(shizired.y>0)	{
		ShowShizi(offsetx[shizired.x]-15,offsety[shizired.y]-15,30,30,gImage_shizi,BLACK);
	shizired.y-=1;}
}
void redxia(){
	if(shizired.y<9)	{
		ShowShizi(offsetx[shizired.x]-15,offsety[shizired.y]-15,30,30,gImage_shizi,BLACK);
	shizired.y+=1;}
}

void redcancel(){
	redselect=0;
	(*qz[jj]).color=redlastcolor;
}

//////////////////////////////////////////////////////////////////////////////////////////

void blueclick(){
	int i;int x,y;
	if(select){//选择移动位置
//////////////////////////////////////////////////////////////////
		if(checkteammate()){
			if(rule(*qz[ii])) {//棋子
			clearshow(offsetx[(*qz[ii]).x]-10,offsety[(*qz[ii]).y]-10,20,20,gImage_jiang,jiang.color);
			for(i=0;i<32;i++)
				if((*qz[i]).x==shiziblue.x&&(*qz[i]).y==shiziblue.y)break;
			if(i!=32&&i!=ii){	//已经吃掉棋子
				Undoqueue(qz[i],i);
				(*qz[i]).dead=1;
				if(jiang.dead||shuai.dead){
					TFTLCD_ShowString(80,180,"GAMEOVER",CYAN,LIGHTBLUE);
					score++;
					restart=true;
				}
				(*qz[i]).x=deadnumx;
				(*qz[i]).y=deadnumy+13;
				deadnumx++;
				if(deadnumx==9){deadnumy++;deadnumx=0;}
			}
			Undoqueue(qz[ii],ii);
			select=0;
			(*qz[ii]).color=lastcolor;
			(*qz[ii]).x=shiziblue.x;
			(*qz[ii]).y=shiziblue.y;
			Enqueue(qz[ii],ii);//记录落子位置
			b = !b;
			}
		}
////////////////////////////////////////////////////////////////////////////////

	}
	else{	//选中移动棋子
	for(i=0;i<32;i++){
		if((*qz[i]).x==shiziblue.x&&(*qz[i]).y==shiziblue.y&&(*qz[i]).color==BLUE)
			break;
	}
	if(i!=32){
		ii=i;
		select=1;
		lastcolor=(*qz[ii]).color;
		(*qz[ii]).color=WHITE;
	}
	}
}



void redclick(){
	int i;
	if(redselect){
//////////////////////////////////////////////////////////////////////////////
		if(redcheckteammate()){
			if(rulered(*qz[jj])){
			clearshow(offsetx[(*qz[jj]).x]-10,offsety[(*qz[jj]).y]-10,20,20,gImage_jiang,jiang.color);
			for(i=0;i<32;i++)
				if((*qz[i]).x==shizired.x&&(*qz[i]).y==shizired.y)break;
			if(i!=32&&i!=jj){
				Undoqueue(qz[i],i);
				(*qz[i]).dead=1;
				if(jiang.dead||shuai.dead){
					TFTLCD_ShowString(80,180,"GAMEOVER",CYAN,LIGHTBLUE);
					redscore++;
					restart=true;
				}
				(*qz[i]).x=deadnumx;
				(*qz[i]).y=deadnumy+12;
				deadnumx++;
				if(deadnumx==9){deadnumy++;deadnumx=0;}
			}
			redselect=0;
			Undoqueue(qz[jj],jj);
			(*qz[jj]).color=redlastcolor;
			(*qz[jj]).x=shizired.x;
			(*qz[jj]).y=shizired.y;
			Enqueue(qz[jj],jj);//记录落子位置
			b = !b;
		}
		}
////////////////////////////////////////////////////////////////////
	}
	else{
	for(i=0;i<32;i++){
		if((*qz[i]).x==shizired.x&&(*qz[i]).y==shizired.y&&(*qz[i]).color==RED)
			break;
	}
	if(i!=32){
		jj=i;
		redselect=1;
		redlastcolor=(*qz[jj]).color;
		(*qz[jj]).color=PURPLE;
	}
	}
}

void Showclrpic(uint32_t offsetx, uint32_t offsety, uint32_t x, uint32_t y,
		uint8_t gImage[]) {
	uint32_t temp, i, j, m;
	m = 0;
	for (i = offsety; i < (y + offsety); i++) {
		for (j = offsetx; j < (x + offsetx); j++) {
			temp = gImage[m];
			temp |= gImage[m+1]<<8;
				TFTLCD_DrawPoint(j, i, temp);
			m++;
			m++;
		}
	}
}

int checkteammate(){
	int i;
	for(i=0;i<32;i++)
	if((*qz[i]).x==shiziblue.x&&(*qz[i]).y==shiziblue.y)break;
	if(i!=32&&(*qz[i]).color==BLUE)return 0;
	return 1;
}
int redcheckteammate(){
	int i;
	for(i=0;i<32;i++)
	if((*qz[i]).x==shizired.x&&(*qz[i]).y==shizired.y)break;
	if(i!=32&&(*qz[i]).color==RED)return 0;
	return 1;
}




////////////////////////////////
