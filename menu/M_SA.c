#include "sa.h"

void SaShow(void)
{
    int i=0;
    char *PSwitch_str[8]={"None Return","Back_Left","Back_Right","Back_L&Right","Front","F&Back_Left","F&Back_Right","All"};
  while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("-Proximity Switch-");
        
		LCD_SetXY(0,1);
		LCD_WriteString("P_Switch STATUS:");
		LCD_SetXY(0,2);
  //  LCD_WriteInt(sa_status);
		LCD_WriteString(*(PSwitch_str+sa_status));
<<<<<<< HEAD
		LCD_WriteString(" is Blocked.");
=======
		LCD_WriteString(" is OK.");
>>>>>>> 2e8c624a0a3a1d3839c54e5692f6a1a626c4e09e
    LCD_SetXY(0,3);
        
        for(i=0; i<5; i++)
        {
            LCD_WriteInt( (sa_status>>(i)  & 0x01 ));
        }
        
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
