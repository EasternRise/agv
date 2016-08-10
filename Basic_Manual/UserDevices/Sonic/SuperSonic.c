#include "all.h"

uint16 supersonic_temp1,supersonic_temp2;
uint8 Sonic_Start=0,Sonic_End=0;

void TaskSuperSonic(void *pdata)
{
	pdata = pdata;
	
	for(;;)
	{
		FPGA_UartSendByte(9,0x55);
		OSTimeDly(2);
		supersonic_temp1=FPGA_ReadUltrasonic(9);
		FPGA_UartSendByte(9,0x5a);
		OSTimeDly(2);
		supersonic_temp2=FPGA_ReadUltrasonic(9);
		if(Sonic_Start==1){
		
			FPGA_UartSendByte(9,0x11);			
			Sonic_Start = 0 ;
			OSTimeDly(1);
		}
		if(Sonic_End==1){
			
			FPGA_UartSendByte(9,0x0);
			Sonic_End = 0 ;
			OSTimeDly(1);
		}
	}

}