/* Includes ------------------------------------------------------------------*/
#include "all.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/*
 * 函数名: show_Colour
 * 描  述: 频率计显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void show_Colour()
{
	while(1)
	{
		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========COLOUR========");
		LCD_SetXY(0,1);
		LCD_WriteString("COLOUR1:");
		LCD_WriteDouble(temp1,1);
        LCD_SetXY(0,2);
        LCD_WriteString("COLOUR2:");
		LCD_WriteDouble(temp2,1);
		LCD_SetXY(0,3);
		LCD_WriteString("DIFF:");
		LCD_WriteDouble(temp1-temp2,1);
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
/********************************    END OF FILE    ***************************/
