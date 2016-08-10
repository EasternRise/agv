/****************************************Copyright (c)**************************************************
**                                     XJTU Robot Team
**-----------------------------------------File Info----------------------------------------------------
** File name:			iic.c
** Last modified Date:  20010-1-2
** Descriptions:		
** Created by:			赵瓅晔	
********************************************************************************************************/
#include "basic.h"
/***************************************************************
*函数名称：I2CInit(void)
*描述：	初始化I2c接口，适用于7th\8th\9th键盘
		改初始化函数同时开启了IIC中断服务
*****************************************************************/
void I2CInit( void )
{
    rGPECON 	&= ~0xf0000000;
    rGPECON 	|=  0xa0000000;              //GPE15:IICSDA , GPE14:IICSCL 设置IIC的端口
    rGPEUP  	|=  0xc000;                  //Pull-up disable    不允许上拉
    
    rIICADD		= 0x10;                    	//2440 slave address = [7:1] 从机地址
	rIICCON		= 0xe0 | IICDEV;
	rIICSTAT	= 0x10;    
	                                     	//IIC bus data output enable(Rx/Tx)   接受和发送的使能
	UNMASKIrq( BIT_IIC );					//开I2C中断
}

/***************************************************************
*函数名称：I2CInit(void)
*描述：	初始化I2c接口，适用于11th键盘
		改初始化函数同时开启了IIC中断服务
*****************************************************************/
void Expection_I2C(void)
{
	static uint8 KEYBOARD_Value[1];
	static int IIC_Stat=0;
	switch(IIC_Stat)
	{
		case 0:
			rIICCON       = 0xe0 | IICDEV; 		//resume IIC
			IIC_Stat=1;
		break;
		case 1:
			KEYBOARD_Value[0]   = rIICDS;		//读键盘值
			rIICCON       = 0xe0 | IICDEV; 		//resume IIC 
			IIC_Stat=2;
		break;
		case 2:
			rIICSTAT	  = 0x50;
			rIICDS		  = KEYBOARD_Value[0];	//返回键盘值
			rIICCON       = 0xe0 | IICDEV; 		//resume IIC 
			IIC_Stat=3;
		break;
		case 3:
			rIICSTAT	  = 0x10;
			rIICCON       = 0xe0 | IICDEV; 		//resume IIC 
			IIC_Stat=0;
			OSMboxPost(I2cMbox,KEYBOARD_Value);	//启动TaskI2cKeys()
		break;
	}
	ClearPending( BIT_IIC ); 
}
/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/