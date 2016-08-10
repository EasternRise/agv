#include "All.h"

void UART2_Init(int baud)
{
	int i;    

	
	rGPHCON &= (~(0xf000));
	rGPHCON |= (0xa000);
    rGPHUP  &= (~(0xc0));    						// The pull up function is enable

    rULCON2 = 0x3;									//无奇偶校验，1位停止位，8bit数据
    rUCON2  = 0x385;   								//收发皆为中断或查询模式，非回写模式，不生成出错中断，接受超时中断，发送接受中断为电平，选择PCLK
    rUFCON2 = 0x11;   								//FIFO使能，自动清零，接收8Byte触发
    rUMCON2 = 0x00;   								//AFC disable 
    rUBRDIV2=( (int)(Sys_Pclk/16./baud+0.5) -1 );   //Baud rate divisior register
  
    UNMASKIrq(BIT_UART2);   
    for(i=0;i<100;i++);
    UNMASKSubIrq(BIT_SUB_RXD2);
    ClearSubPending(BIT_SUB_RXD2);
}

void UART2_SendByte (uint8 data)	//发送一个字符
{
	while(!(rUTRSTAT2 & 0x2));
	WrUTXH2(data);
}

void UART2_SendString(char *data)	//发送一个字符串
{
	while(*data)
	{
		UART2_SendByte(*data++);
	}
}

void Expection_UART2(void)
{
	//int length,i=0,j;
	if((rSUBSRCPND&BIT_SUB_TXD2)!=0)
	{
		MASKSubIrq(BIT_SUB_TXD2);
	}

	if((rSUBSRCPND&BIT_SUB_RXD2)!=0)			//接收中断
	{

		LidarBuffer[LidarBufferTop]=RdURXH2();			
		LidarBufferTop++;
		
		ClearSubPending( BIT_SUB_RXD2 );
		ClearPending( BIT_UART2 );     
	}
}

uint8 UART2_PopByte()
{
	return 0;
}

int UART2_RxReady()
{
	return 0;
}

