#include "compass.h"
#include "M_COMPASS.h"
#include "all.h"

void ComShow(void)
{   
  while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("--------COMPASS--------");
        
		LCD_SetXY(0,1);
		LCD_WriteString("ANGLE: ");
		/*LCD_SetXY(0,2);*/
    LCD_WriteDouble(angle_average,2);
		LCD_SetXY(0,2);
		LCD_WriteString("2.CORRECT");
		LCD_SetXY(0,3);
		LCD_WriteString(" ACU(9 is the best):");
//		LCD_WriteInt(USART1_RxBuffer[6]-0x30);
		

		refresh();	
        
		if(KeyReady==1)
		  {
			KeyReady=0;
				
			switch(KeyValue)
			{
				/**********************************************/
 				case 2:
 				  comadjust();	
 //         LCD_WriteInt(USART1_RxBuffer[6]-0x30);				
 					break;

 				/*	float v_w=3.1415926*0.201;
 					float nspeed_w=0;*/
 				//	int i=0;					
				/*	while(nspeed_w<v_w)
 					{
 							accurate(0,0,v_w);
 							setspeed (0 ,0,nspeed_w);
 							delay(50);
 					};*/
 				  
// 				/**********************************************/
				case keyback:
					return;
			}
			LCD_WriteInt(USART1_RxBuffer[6]-0x30);
		  }
		delay(LCD_DELAY);
		/***********************************/
		//refresh();
		/************************************/
	}
}

void comadjust (void)
{
<<<<<<< HEAD
	while (1)
	{
=======
>>>>>>> 2e8c624a0a3a1d3839c54e5692f6a1a626c4e09e
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("------NOTICE------");
	LCD_SetXY(0,1);
	LCD_WriteString("When correcting,make sure no iron products around.Press OK to continue,or back to cancel");
	
	refresh();
<<<<<<< HEAD
	delay(LCD_DELAY);
		
		if(KeyReady==1)
		{
			KeyReady=0;
			int i=0;
			switch(KeyValue)
			{
				case 18://ok
					while (i<550)
					{
						LCD_Clear();
						LCD_SetXY(0,0);
						LCD_WriteString("------NOTICE------");
						LCD_SetXY(0,1);
						LCD_WriteString("ROTATION FOR 10S AROUND,BE CAREFUL!");
						UART1_SendByte(0xc0);
						refresh(); 
						SetSpeed(0,0,0.205*pi);
						delay(20);
						i+=1;
					}
=======
	
	while (1)
	{
		if(KeyReady==1)
		{
			KeyReady=0;
			switch(KeyValue)
			{
				case 18://ok
					LCD_Clear();
					LCD_SetXY(0,0);
					LCD_WriteString("------NOTICE------");
				  LCD_SetXY(0,1);
	        LCD_WriteString("ROTATION FOR 10S AROUND,BE CAREFUL!");
					UART1_SendByte(0xc0);
					for(int i=0;i<500;i+=1)
					{	   
						SetSpeed(0,0,0.205*pi);
						delay(20);
					}

>>>>>>> 2e8c624a0a3a1d3839c54e5692f6a1a626c4e09e
					for(int i=0;i<50;i+=1)
					{		
					SetSpeed(0,0,0);
					delay(10);
<<<<<<< HEAD
				  }
=======
					}
>>>>>>> 2e8c624a0a3a1d3839c54e5692f6a1a626c4e09e
					UART1_SendByte(0xc1);
					return;
				case keyback:
					return;
			}
		}
<<<<<<< HEAD
  
 }
=======
  }
>>>>>>> 2e8c624a0a3a1d3839c54e5692f6a1a626c4e09e
}
