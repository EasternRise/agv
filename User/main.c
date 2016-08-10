#include "all.h"
#include "obstacle.h"

//程序整体逻辑
//通过优感器改变机器人的状态
//通过move_car控制底盘电机运动
//通过菜单来进行目标点的设定
//可以有多个点

u8 col_error=0;

int main(void)
{ 
    InitAll();
	
	
	col_error=0;
	//电量
	low_ele=0;
	
	my_robot=unloacted;
	
	{
			LCD_Clear();
		  
	//		ShowMenuList();
			LCD_Welcom();		
	/*		while(1)
			{
					if (KeyReady==1)
					{ 
							KeyReady=0;
							KeyNumOperate(KeyValue);	
							
					}
					refresh();
					
					delay(10);
			}*/
	}
}
