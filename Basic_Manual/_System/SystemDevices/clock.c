#include "basic.h"

void CLOCK_Define(void);
void ChangeMPllValue(int m,int p,int s);
void ChangeClockDivider(int hdivn_val,int pdivn_val);
void ChangeUPllValue(int m,int p,int s);

int Sys_Fclk=400000000;
int Sys_Hclk=100000000;
int Sys_Pclk=50000000;
int Sys_Fin=12000000;		//晶振频率
int Sys_Freq=100000000;

/******************************************************
时钟初始化函数
参数:
0:200MHz
1:300Mhz
2:400Mhz(默认)
3:440Mhz(超频)
********************************************************/
void CLOCK_Init(int mode)
{
	uint8 key;
	uint32 mpll_val;
	switch ( mode ) {
	case 0:								//200MHz
		key = 12;
		mpll_val = (92<<12)|(4<<4)|(1);
		break;
	case 1:								//300MHz
		key = 13;
		mpll_val = (67<<12)|(1<<4)|(1);
		break;
	case 2:								//400MHz
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	case 3:								//440MHz!!!
		key = 14;
		mpll_val = (102<<12)|(1<<4)|(1);
		break;
	default:							//400MHz
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	}
	ChangeMPllValue((mpll_val>>12)&0xff, (mpll_val>>4)&0x3f, mpll_val&3);
	ChangeClockDivider(key, 12);		//设置FCLK,HCLK,PCLK,使FCLK变化时(现设为400M),HCLK保持100M,PCLK保持50M
	CLOCK_Define();						//设置表示FCLK,HCLK,PCLK值的全局变量,这些值会在后面配置寄存器时用到
}


//设置表示FCLK,HCLK,PCLK值的全局变量,这些值会在后面配置寄存器时用到
void CLOCK_Define(void)
{
	uint32 val;
	uint8 m, p, s;
	
	val = rMPLLCON;
	m = (val>>12)&0xff;
	p = (val>>4)&0x3f;
	s = val&3;

	//(m+8)*FIN*2 不要超出32位数!
	Sys_Fclk = ((m+8)*(SYS_FIN/100)*2)/((p+2)*(1<<s))*100;
	
	val = rCLKDIVN;
	m = (val>>1)&3;
	p = val&1;	
	val = rCAMDIVN;
	s = val>>8;
	
	switch (m) {
	case 0:
		Sys_Hclk = Sys_Fclk;
		break;
	case 1:
		Sys_Hclk = Sys_Fclk>>1;
		break;
	case 2:
		if(s&2)
			Sys_Hclk = Sys_Fclk>>3;
		else
			Sys_Hclk = Sys_Fclk>>2;
		break;
	case 3:
		if(s&1)
			Sys_Hclk = Sys_Fclk/6;
		else
			Sys_Hclk = Sys_Fclk/3;
		break;
	}
	
	if(p)
		Sys_Pclk = Sys_Hclk>>1;
	else
		Sys_Pclk = Sys_Hclk;
	
	if(s&0x10)
		Sys_Freq = Sys_Hclk;
	else
		Sys_Freq = Sys_Fclk;
		
	val = rUPLLCON;
	m = (val>>12)&0xff;
	p = (val>>4)&0x3f;
	s = val&3;

}

//*************************[ MPLL ]*******************************
void ChangeMPllValue(int mdiv,int pdiv,int sdiv)
{
    rMPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
}

void ChangeClockDivider(int hdivn_val,int pdivn_val)
{
	int hdivn=2, pdivn=0;
	
     // hdivn_val (FCLK:HCLK)ratio hdivn
     // 11           1:1       (0)
     // 12           1:2       (1)
     // 13           1:3       (3) 
     // 14           1:4       (2)
     // pdivn_val (HCLK:PCLK)ratio pdivn
     // 11           1:1       (0)
     // 12           1:2       (1)
	switch(hdivn_val) {
		case 11: hdivn=0; break;
		case 12: hdivn=1; break;
		case 13:
		case 16: hdivn=3; break;
		case 14: 
		case 18: hdivn=2; break;
	}
	
	switch(pdivn_val) {
		case 11: pdivn=0; break;
		case 12: pdivn=1; break;
	}
	
	//Uart_Printf("Clock division change [hdiv:%x, pdiv:%x]\n", hdivn, pdivn);
	rCLKDIVN = (hdivn<<1) | pdivn;

	switch(hdivn_val) {
		case 16:		// when 1, HCLK=FCLK/8.
			rCAMDIVN = (rCAMDIVN & ~(3<<8)) | (1<<8); 
		break; 
		case 18: 	// when 1, HCLK=FCLK/6.
			rCAMDIVN = (rCAMDIVN & ~(3<<8)) | (1<<9); 
		break;
	}
	
    if(hdivn!=0)
        MMU_SetAsyncBusMode();
    else 
        MMU_SetFastBusMode();
}

//**************************[ UPLL ]*******************************
void ChangeUPllValue(int mdiv,int pdiv,int sdiv)
{
    rUPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
}

/*********************************************************************************************************
** 函数名称	:	Delay10us函数
** 输　入 	:	time=0: adjust the Delay function by WatchDog timer.
**				time>0: the number of loop time
*********************************************************************************************************/
void Delay10us(int time)
{
	static uint32 delay10us = 400;
	
	int i;
	
    for(;time>0;time--)
        for(i=0;i<delay10us;i++);
}

/*********************************************************************************************************
** 函数名称	:	Delay100us函数
** 输　入 	:	time=0: adjust the Delay function by WatchDog timer.
**				time>0: the number of loop time
*********************************************************************************************************/
void Delay100us(int time)
{
	static uint32 delay100us = 4000;
	
	int i;
	
    for(;time>0;time--)
        for(i=0;i<delay100us;i++);
}

/*********************************************************************************************************
** 函数名称	:	Delay1ms函数
** 输　入 	:	time=0: adjust the Delay function by WatchDog timer.
**				time>0: the number of loop time
********************************************************************************************************/
void Delay1ms(int time)
{
	static uint32 delay1ms = 40000;
	
    int i;
    for(;time>0;time--)
        for(i=0;i<delay1ms;i++);
}

/*********************************************************************************************************
** 函数名称	:	Delay10ms函数
** 输　入 	:	time=0: adjust the Delay function by WatchDog timer.
**				time>0: the number of loop time
********************************************************************************************************/
void Delay10ms(int time)
{
	OSTimeDly(2*time);
	/*static uint32 delay10ms = 400000;
	
    int i;
    for(;time>0;time--)
        for(i=0;i<delay10ms;i++);*/
}

/*********************************************************************************************************
** 函数名称	:	Delay100ms函数
** 输　入 	:	time=0: adjust the Delay function by WatchDog timer.
**				time>0: the number of loop time
********************************************************************************************************/
void Delay100ms(int time)
{
	static uint32 delay10ms = 4000000;
	
    int i;
    for(;time>0;time--)
        for(i=0;i<delay10ms;i++);
}

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
