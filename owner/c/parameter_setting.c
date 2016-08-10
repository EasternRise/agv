#include "all.h"
#include "can.h"

extern float AGV_speed,delta_x,delta_y;
extern int Stop_time;

void parameter_setting (void)
{
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("PARAMETER SETTING");	
		
		LCD_SetXY(0,1);
		LCD_WriteString("1.SPEED");
		LCD_WriteDouble(AGV_speed,1);
		
		LCD_SetXY(0,2);
		LCD_WriteString("2.DELTA");
		LCD_WriteDouble(delta_x,0);
		LCD_WriteString("/");
		LCD_WriteDouble(delta_y,0);
				
		LCD_SetXY(0,3);
		LCD_WriteString("3.STOPTIME");
		LCD_WriteInt(Stop_time);
		
		refresh();
		
		if (KeyReady==1)
		{
			KeyReady=0;
			
			switch (KeyValue)
			{
				case 1:
					Input_FloatValue(&AGV_speed,"SPEED");
				
				case 2:
					Input_FloatValue(&delta_x,"DELTA_X");
				  Input_FloatValue(&delta_y,"DELTA_Y");
				
				case 3:
					Input_IntValue(&Stop_time,"STOPTIME");
				
				case 5:
					return;
				
			}
		}
		
		delay(LCD_DELAY);
		
	}
}