/****************************************Copyright (c)**************************************************
**                                     XJTU Robot Team
**-----------------------------------------File Info----------------------------------------------------
** File name:			spi0.c
** Last modified Date:  2013-3-17
** Descriptions:		
** Created by:			hanlibin
** Last modified By :  	dong	
********************************************************************************************************/

#include "basic.h"

void  SPI1_Init(void)
{
    //STM32片选线(GPF6)的初始化
	rGPFCON |= 0x1<<12;	 				//STM32的片选线,普通IO口做片选线
	rGPFDAT |= 1<<6;
	
	
	//FPGA片选线(GPG3)和SPI的初始化
	rGPGCON |= 0xfc40;					//使能SPI1,GP7为时钟,GP6为MOSI,GP5为MISO,GP3为输出
	rGPGCON &=~(0x80);		   			//设置成普通的IO口模式，在当从机的时候才需要片选线
	rGPGDAT |= 0x8;
	rGPGUP  |= 0xE8;
	
	rSPPRE1  =   1;											    //PCLK = 50MHz,Baud rate = PCLK/2/(Prescaler value + 1)
	rSPCON1  =   (0<<5)|(1<<4)|(1<<3)|(0<<2)|(1<<1)|(0<<0);		//polling, En-SCK, master, CPOL=0, CPHA=1, normal
	rSPPIN1  =   (0<<2)|(0<<0);							        //dis-ENMUL,release;
}

/*****************************************************************************************
** 函数名称： SPI1_SendData（）
** 功能描述： 向FPGA发送一个数据，同时会把FPGA上次放在FPGA自己发送寄存器的值读回来
** 输	  入： 向FPGA发送的数据
** 输     出： 读回来的数据
** 注意事项： 这里没有写成原子操作是因为在fpga.c文件里面会有原子操作
*******************************************************************************************/
uint8  SPI1_SendData(uint8 data)
{
	uint8 i;
    rGPGDAT &= ~(0x8);	      		//GPG3置0->选中FPGA
    
    while( (rSPSTA1 & 0x1) == 0 );	//判断断据寄存器是否就绪
    rSPTDAT1 = data;
    while( (rSPSTA1 & 0x1) == 0 );	//写数据到SPTDATn自动清零REDY标志,此处为判断是否传完
   
    rGPGDAT |= 0x8;				    //GPG3置1->释放FPGA
    
    i = rSPRDAT1;		//读FPGA发送的值
    
    return i;
}

/**********************************************************************
** 函数名称： SPI1_SendData_stm32（）
** 功能描述： 
** 输	  入： 
** 输     出： 
** 注意事项： 
***********************************************************************/  

uint8  SPI1_SendData_stm32(uint8 data)
{
	uint8 i;
	OS_CPU_SR  cpu_sr;
	
	OS_ENTER_CRITICAL();		  //Disable interrupts
    rGPFDAT &= ~(0x1<<6);		  //GPF6置0->片选stm32
    
    while( (rSPSTA1 & 0x1) == 0 );//判断断据寄存器是否就绪
    rSPTDAT1 = data;
    while( (rSPSTA1 & 0x1) == 0 );//写数据到SPTDATn自动清零REDY标志,此处为判断是否传完
   
    rGPFDAT |= 0x1<<6;			  //GPF6置1->释放stm32
    
    i = rSPRDAT1;				  //读stm32发送的值
    
    OS_EXIT_CRITICAL();			//Restore  interrupts
    return i;
}










