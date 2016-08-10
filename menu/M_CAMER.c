#include "all.h"


void CamerShow(void)
{   
  while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("--------CAMER1--------");
        
		LCD_SetXY(0,1); 
		LCD_WriteString("DIS/ANG_Er:");
		LCD_WriteDouble(guide_dis_error,2);
		LCD_WriteString("/");
	/*LCD_SetXY(0,2);
		LCD_WriteString("G_ANG: ");*/
		LCD_WriteDouble(guide_ang_error,2);
		
		LCD_SetXY(0,2);
		LCD_WriteString("--------CAMER2--------");
		
		LCD_SetXY(0,3);
		LCD_WriteString("Pos_X/Y:");
		LCD_WriteDouble(now_pos.x,2);
		LCD_WriteString(" / ");
	/*LCD_SetXY(0,4);
		LCD_WriteString("L_Y  : ");*/
		LCD_WriteDouble(now_pos.y,2);
		
		LCD_SetXY(0,4);
		LCD_WriteString("ANG    :");
		LCD_WriteDouble(locate_ang,2);
		
		LCD_SetXY(0,5);
		LCD_WriteString("X/Y_ERR:");
		LCD_WriteDouble(locate_x_error,2);
		LCD_WriteString(" / ");
	/*	LCD_SetXY(0,7);
		LCD_WriteString("Y_CEN: ");*/
		LCD_WriteDouble(locate_y_error,2);
		
		LCD_SetXY(0,6);
		LCD_WriteString("!!If Show Wrong,");		
		LCD_SetXY(0,7);
		LCD_WriteString("Contact Mr.Tong!!");
		refresh();	
        
		if(KeyReady==1)
		  {
			KeyReady=0;
			switch(KeyValue)
			{
				case keyback:
					return;
			}
			
		  }
		delay(LCD_DELAY);
	}
}
