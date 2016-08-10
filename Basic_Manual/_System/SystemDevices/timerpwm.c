/****************************************Copyright (c)**************************************************
**                                     XJTU Robot Team
**-----------------------------------------File Info----------------------------------------------------
** File name:			pwm.c
** Last modified Date:  2009-11-27
** Descriptions:		
** Created by:			xingxing		
********************************************************************************************************/
#include "basic.h"

int32 time_flag=0;
/********************************************************************************************************
** 函数名称：PWMInit
** 函数功能：PWM初始化 包括0、1、2、3四路PWM波的初始化
** 入口参数：
** 出口参数：
*********************************************************************************************************/
void  TimerPWMInit(void)
{
	//分频设置
	rTCFG0	=  (DEADZONE<<16)|(PRESCALER1<<8)|(PRESCALER0<<0);
	rTCFG1	=  (DMAMODE<<20)|(MUX4<<16)|(MUX3<<12)|(MUX2<<8)|(MUX1<<4)|(MUX0<<0);
	
	//端口设置为TOUT口
	rGPBCON &= (~(0xff));			
	rGPBCON |=  0xaa ;
    rGPBUP  &= (~(0xff));
    
    //0-3为pwm波输出口，4为定时器
    //pwm波的频率是2000Hz
	rTCNTB0	=  CNTB0;
	rTCMPB0	=  CMPB0;
	
	rTCNTB1	=  CNTB1;
	rTCMPB1	=  CMPB1;
	
	rTCNTB2	=  CNTB2;
	rTCMPB2	=  CMPB2;
	
	rTCNTB3	=  CNTB3;
	rTCMPB3	=  CMPB3;
	
	rTCNTB4	=  CNTB4;

	rTCON	&= (~(0x7fff0f));//刷新rTCON
	rTCON	|=    0x6aaa0a  ;//关反向器，设置自动重装入，更新相关寄存器值
	rTCON	&= (~(0x222202));//清空手动更新标志位
	rTCON	|=    0x111101  ;//定时器开始工作
	
	UNMASKIrq( BIT_TIMER4 );	
}
/********************************************************************************************************
** 函数名称：SetPWM()
** 函数功能：共四路PWM波0，1，2，3  
** 入口参数：
** 出口参数：
*********************************************************************************************************/
uint8 SetPWM(uint8 Num, fp64 Ratio)
{
	if( Num<=3 )
	{
		switch(Num)
		{
			case 0:
			    rTCMPB0 =(uint16) (CNTB0*Ratio);
			    break;
			case 1:
				rTCMPB1 =(uint16)(CNTB1*Ratio);
				break;
			case 2:
				rTCMPB2 =(uint16)(CNTB2*Ratio);
				break;
			case 3:
				rTCMPB3 =(uint16)(CNTB3*Ratio);
				break;
		}
		return 1;
	}
	else
    {return 0;}
}
