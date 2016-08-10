//4行20列共80单元，每10个单元为一组在一个系统周期内显示，目前80ms刷新一遍

#include "all.h"

void TaskLcdRefresh(void *pdata)
{
	uint8 i=0,j=0;
	//char testarray[80];
	pdata = pdata;
	//for(i=0;i<=79;i++)
	//	testarray[i]=i+33; 
	while(1)
	{
	
		/*
		UART1_SendByte (0xff);
		
		if(j>=0&&j<=4)
			UART1_SendByte (0x0+j*4);
		if(j>=5&&j<=9)
			UART1_SendByte ((j-5)*4+0x1e);
		if(j>=10&&j<=14)
			UART1_SendByte (0x3c+(j-10)*4);
		if(j>=15&&j<=19)
			UART1_SendByte (0x5a+(j-15)*4);
			
	
		for(i=0;i<=3;i++)
		{
			 UART1_SendByte (LCD_Table[i+j*4]);
			
		}
		if(j==19)
			j=0;
		else
			j++;
		OSTimeDly(1);
		*/ 
		UART1_SendByte (0xff);
		//OSTimeDly(1);
		UART1_SendByte (30*(i/2)+(i%2)*10);
		OSTimeDly(4);
		for(j=0;j<=9;j++)
		{
			 UART1_SendByte (LCD_Table[10*i+j]);
			 //OSTimeDly(1);
			
		}
		if(i==15)
			i=0;
		else
			i++;
		OSTimeDly(4);
	
		
	}
}