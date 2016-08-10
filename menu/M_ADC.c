#include "all.h"
#include "M_ADC.h"

double v_thresh=3.8;

void EleShow(void)
{   
  while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("=====Electricty====");
        
		LCD_SetXY(0,2);
		LCD_WriteString("V: ");
		LCD_WriteDouble(max_vol,4);
		
		LCD_SetXY(0,3);	
		LCD_WriteString("THRESH: ");
		LCD_WriteDouble(v_thresh,4);
		
		refresh();	
        
		if(KeyReady==1)
		  {
			KeyReady=0;
			switch(KeyValue)
			{
				case key3:
					Input_DoubleValue(&v_thresh,"V:");
				case keyback:
					return;
			}
			
		  }
		delay(LCD_DELAY);
	}
}
