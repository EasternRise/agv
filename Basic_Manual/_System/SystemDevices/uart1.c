#include "all.h"
/*********************************************
**功能：UART 
**用途：
**用法：该UART有两种模式，FIFO和非FIFO
**
**********************************************/

#define  UART1_FIFO_MOD  1

//框里面的是选择模式的时候用的，这样写是为了在修改或变换模式的时候方便一些而已，
//不想用也可以直接给寄存器赋值
//====================================================================================================
#define  UART1_FIFO_ENABLE  1																		 //
#define  UART1_FIFO_DISABLE  0																		 //

																									 //																									 //
#define  UART1_FIFO(data)     rUFCON1 &= ~(0xff);  rUFCON1 |= data									 //
																									 //
#define	 UART1_CONTROL(data)  rUCON1  &= ~(0xf)	;  rUCON1  |= data									 //
//=====================================================================================================																									 //
uint8 UART1_RxBuffer[256]={0};
uint8 UART1_RxBufferTop=0;
uint8 UART1_RxBufferEnd=0;

uint8 UART1_TxBuffer[256]={0};
uint8 UART1_TxBufferTop=0;
uint8 UART1_TxBufferEnd=0;

void UART1_Init(int baud)
{
	int i;    
			
	rGPHCON &= (~(0xf00));
	rGPHCON |= (0xa00);
    rGPHUP  &= (~(0x30));    						// The pull up function is enable
    
    rULCON1 = 0x3;									//设置数据传输模式：无奇偶校验，1位停止位，8bit数据
    rUBRDIV1=( (int)(Sys_Pclk/16./baud+0.5) -1 );   //Baud rate divisior register
	rUMCON1 = 0x00;   								//AFC disable 
	rUCON1  |= 0x000<<4  | 1<<7  |  0<<8  |  1<<9  ; //设置基本的模式：RX错误状态使能，超时使能，RX为脉冲触发，TX电平触发，PCLK

	/*
	**以上几行不需要改动，可根据需要改下面两行
	*/
	
    UART1_FIFO	 ( UART1_FIFO_ENABLE | TX_FIFO_TR_LEVEL_16  | RX_FIFO_TR_LEVEL_32 );//FIFO使能，并设置接收和发送触发深度(字节)   								
    UART1_CONTROL( TX_INT_AND_POOL_MOD  |  RX_INT_AND_POOL_MOD);//发送和接收都为中断或查询模式
       

    UART1_RxBufferTop=0;
	UART1_RxBufferEnd=0;

    UNMASKIrq(BIT_UART1);
    for(i=0;i<100;i++);					
    UNMASKSubIrq(BIT_SUB_RXD1);			//注意：这里开的只是接收中断的标志位
    ClearSubPending( BIT_SUB_RXD1 );	//      发送中断的标志位在下面才开
}


/*******************************************************
**	根据FIFO模式或非FIFO模式来选择对应的发送程序，
**	因为两者判断的条件不同
********************************************************/

#if  UART1_FIFO_MOD   
	void UART1_SendByte (uint8 data)	//发送一个字符				  /*   ------ UART传输方法  1    ---------*/
	{	
		do
		{
			if(((rUFSTAT1 & 0x3F<<8)>>8)< 16)//FIFO的发送触发深度是16字节
			{
				WrUTXH1(data);
				break;
			}
		}while(!(rUFSTAT1 & (uint16)1<<14));//判断FIFO是否满了
	}

#else
	//非FIFO 模式													  /*   ------ UART传输方法  2    ---------*/
	void UART1_SendByte (uint8 data)	//发送一个字符
	{
		while(!(rUTRSTAT1 & 0x2));
		WrUTXH1(data);
	}
#endif	


void UART1_SendString(char *data)	//发送一个字符串
{
	while(*data)
	{
		UART1_SendByte(*data++);
	}
}


void Expection_UART1(void)			//UART0 FIFO中断
{	
	if((rSUBSRCPND&8)!=0)
	{
		if(WIRELESS_buffer_End!=WIRELESS_buffer_Top)
		{
			WrUTXH1(WIRELESS_buffer[(WIRELESS_buffer_End++)%256]);
		}
		
		else
		{
			MASKSubIrq(BIT_SUB_TXD1);
		}
   		 	
   		ClearSubPending( BIT_SUB_TXD1 );
		ClearPending( BIT_UART1 );
	}

	if((rSUBSRCPND&4)!=0)			//接收中断
	{
		
		int length1=rUFSTAT1&0x3f;	//判断接收到的数据长度
		int i;
		for(i=0;i<length1;i++)
		{
			UART1_RxBuffer[(UART1_RxBufferTop++)%256]=RdURXH1();//存入数组，数据从这里读取
		}
		ClearSubPending( BIT_SUB_RXD1 );
		ClearPending   ( BIT_UART1 );     
	}
}

