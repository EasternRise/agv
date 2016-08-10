#include "all.h"
uint8 PC_Buffer[256];
uint8 PC_Top=0;
uint8 PC_End=0;


uint8 UART0_RxBuffer[256];
uint8 UART0_RxBufferTop=0;
uint8 UART0_RxBufferEnd=0;

uint8 UART0_TxBuffer[256];
uint8 UART0_TxBufferTop=0;
uint8 UART0_TxBufferEnd=0;

uint8 WIRELESS_buffer[256];
uint8 WIRELESS_buffer_Top=0;
uint8 WIRELESS_buffer_End=0;

void UART0_Init(int baud)
{
	int i;    

	 
	rGPHCON &= (~(0xf0));
	rGPHCON |= (0xa0);								//初始化GPH2、GPH3为发送和接收数据功能
    rGPHUP  &= (~(0xc));    						// The pull up function is enable

     
    rULCON0 = 0x3;									//无奇偶校验，1位停止位，8bit数据
    rUCON0  = 0x385;   								//选择PCLK，发送中断电平类型，接受中断电平类型，接受超时中断，不生成出错中断，非回写模式，收发皆为中断或查询模式，
    rUFCON0 = 0x31;   								//
    rUMCON0 = 0x00;   								//AFC disable 
    rUBRDIV0=( (int)(Sys_Pclk/16./baud+0.5) -1 );   //Baud rate divisior register
    
    UART0_RxBufferTop=0;
	UART0_RxBufferEnd=0;

    UNMASKIrq(BIT_UART0);                //关中断屏蔽(UART0)
    for(i=0;i<100;i++);
    UNMASKSubIrq(BIT_SUB_RXD0);          //关中断次级屏蔽（RXD0）
    ClearSubPending( BIT_SUB_RXD0 );     //次级源挂起寄存器（RXD0）

}


void UART0_SendByte (uint8 data)	//发送一个字符
{
	while(!(rUTRSTAT0 & 0x2));		//等待发送缓冲器为空
	WrUTXH0(data);                  //UTXH0为UART通道0发送缓冲寄存器
}

void UART0_SendString(char *data)	//发送一个字符串
{
	while(*data)
	{
		UART0_SendByte(*data++);
	}
}

void PC_PushByte(uint8 data)
{
	PC_Buffer[PC_Top++]=data;		//PC_Top++，先调用PC_Top赋值再加1
	UNMASKSubIrq(BIT_SUB_TXD0);		//关中断次级屏蔽（TXD0）
	
}

void PC_PushString(char *data)
{
	while(*data)
	{
		PC_PushByte(*data++);
	}
	
}

void Expection_UART0(void)			//UART0 FIFO中断
{	
	if((rSUBSRCPND&2)!=0)           //如果TXD0有中断请求(发送中断)
	{
		/*if(PC_End!=PC_Top)
		{
			WrUTXH0(PC_Buffer[PC_End++]);
		}
		else
		{
			MASKSubIrq(BIT_SUB_TXD0);
		}*/
		if(WIRELESS_buffer_End!=WIRELESS_buffer_Top)
		{
			UART0_SendByte(WIRELESS_buffer[WIRELESS_buffer_End++]);
		}
		
		else
		{
			UART0_TxBufferTop=0;
			UART0_TxBufferEnd=0;
			MASKSubIrq(BIT_SUB_TXD0);
		}
   		ClearPending( BIT_UART0 ); 
   		ClearSubPending( BIT_SUB_TXD0 );	
		
	}

	if((rSUBSRCPND&1)!=0)			//接收中断
	{
		int length1=rUFSTAT0&0x3f;	//取 接收到的数据长度
		int i;
		for(i=0;i<length1;i++)
		{
			UART0_RxBuffer[UART0_RxBufferTop]=RdURXH0();//URXH为UART通道0接收缓冲寄存器
			if(UART0_RxBufferTop==255)
			{
				UART0_RxBufferTop=0;
			}
			else
			{
				UART0_RxBufferTop++;
			}       
		}
		ClearSubPending( BIT_SUB_RXD0 );
		ClearPending( BIT_UART0 );     
	}
}


void PC_WriteChar(char data)//输出字符
{
	UART0_SendByte(data);
}

void PC_WriteInt(int data)//输出整型(逐步取余)
{
	char answer[100];
	int index=98;
	int pose=0;
	if(data<0)
	{
		pose=1;
		data=-data;
	}
	answer[99]='\0';
	while(data>9)
	{
		answer[index--]=data%10+'0';
		data=data/10;
	}
	answer[index]=data+'0';
	if(pose)
	{
		answer[--index]='-';
	}
	UART0_SendString(answer+index);
}
void PC_WriteNum(double data)//输出double型
{
	char str[5];
	int flag=0,i;
	if(data<0)
	{
		PC_WriteChar('-');
		data=-data;
	}
	PC_WriteInt((int)data);
	data=data-(int)data;
	for(i=0;i<5;i++)
	{
		data=data*10;
		str[i]=(int)data+'0';
		if(str[i]!='0')
		{
			flag=i+1;
		}
		data=data-(int)data;
		
	}
	if(flag!=0)
		PC_WriteChar('.');
	for(i=0;i<flag;i++)
	{
		PC_WriteChar(str[i]);
	}	
	for(i=flag;i<6;i++)
	{
		PC_WriteChar(' ');
	}
	PC_WriteChar(' ');

}
void WIRELESS_PushByte(uint8 data)
{
	WIRELESS_buffer[WIRELESS_buffer_Top++]=data;
	UNMASKSubIrq(BIT_SUB_TXD0);
}

void WIRELESS_PushString(char *data)
{
	while(*data)
	{
		WIRELESS_PushByte(*data++);
	}
}



