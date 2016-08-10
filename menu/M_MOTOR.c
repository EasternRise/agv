#include "M_motor.h"
#include "all.h"
/*
 * 函数名: sys_Mecanum
 * 描  述: 全向轮相关功能
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void sys_Mecanum()
{
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Mecanum=======");
		LCD_SetXY(0,1);
		LCD_WriteString("1.F port:");
		LCD_WriteInt(Mec_F.port);
		LCD_SetXY(0,2);
		LCD_WriteString("2.BL port:");
		LCD_WriteInt(Mec_BL.port);
		LCD_SetXY(0,3);
		LCD_WriteString("3.BR port:");
		LCD_WriteInt(Mec_BR.port);
		
		refresh();	
        
		if(KeyReady==1)
		  {
				KeyReady=0;
				switch(KeyValue)
				{
					case key1:
						Mecanum_test(&Mec_F);
						break;
					case key2:
						Mecanum_test(&Mec_BL);
						break;
					case key3:
						Mecanum_test(&Mec_BR);
						break;
					case keyback:
						return;
				}
		  }
		delay(LCD_DELAY);
			
	}
}


extern float f_speed;
extern float bl_speed;
extern float br_speed;
extern float nspeed_x,nspeed_y,nspeed_w;

void move_test(void)
{
	double v_x=0;
	double v_y=0;
	double v_w=0;
	
	//extern float nspeed_x,nspeed_y,nspeed_w;
	
	while(1)
	{
		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("!!! BE CAREFUL !!!");
		
		LCD_SetXY(0,1);
		LCD_WriteString("1.VX:");
		LCD_WriteDouble(v_x,1);
		LCD_SetXY(0,2);
		LCD_WriteString("2.VY:");
		LCD_WriteDouble(v_y,1);
		LCD_SetXY(0,3);
		LCD_WriteString("3.VW");
		LCD_WriteDouble(v_w,1);
	  LCD_SetXY (0,4);
		LCD_WriteString ("4.STOP");
		
		
/*		
		LCD_SetXY(0,5);
		LCD_WriteString("FOWARD    :");
		LCD_WriteDouble(f_speed,1);
		LCD_SetXY(0,6);
		LCD_WriteString("BACK_LEFT :");
		LCD_WriteDouble(bl_speed,1);
		LCD_SetXY(0,7);
		LCD_WriteString("BACK_RIGHT:");
		LCD_WriteDouble(br_speed,1);
		*/
/*********************************************************/
    LCD_SetXY(0,5);
		LCD_WriteString("5.TEST_X");
		LCD_WriteDouble(v_x ,1);
		LCD_SetXY(0,6);
		LCD_WriteString("6.TEST_Y");
		LCD_WriteDouble(v_y,1);
		LCD_SetXY(0,7);
		LCD_WriteString("7.TEST_W");
		LCD_WriteDouble(v_w,1);
/**********************************************************/

		refresh();	
		
		if(KeyReady==1)		
		 {
				KeyReady=0;

				switch(KeyValue)
				{
					case 1:
						Input_DoubleValue(&v_x,"V_X");
						break;
					case 2:
						Input_DoubleValue(&v_y,"V_Y");
						break;
					case 3:
						Input_DoubleValue(&v_w,"V_W");
						break;
					case 4:
						v_x=0;
					  v_y=0;
					  v_w=0;
					  stop_car ();
					  break ;
					case 7://5
						v_x=500;
					/*  nspeed_x=0;
					  while(nspeed_x<v_x)
						{
					  accurate(v_x,0,0);
					  SetSpeed (nspeed_x ,0,0);
						delay(50);
						};*/
						for (int i=0;i<500;i+=1)
							{
								SetSpeed(v_x,0,0);
								delay(20);
							};
					    v_x=0;v_y=0;v_w=0;
						for (int i=0;i<100;i+=1)
							{
					      SetSpeed(0,0,0);
								delay(2);
							};
					  break;
					case 8://6
						v_y=500;
				/*	  nspeed_y=0;
					  while(nspeed_y<v_y)
						{
					  accurate(0,v_y,0);
					  SetSpeed (0 ,nspeed_y,0);
						delay(50);
						}; */
 					  for (int i=0;i<500;i+=1)
							{
									SetSpeed (0 ,v_y,0);
									delay(20);
							};
					  v_x=0;v_y=0;v_w=0;
					  for (int i=0;i<100;i+=1)
							{
					      SetSpeed(0,0,0);
								delay(2);
							};
					  break;
					case 9://7
						v_w=3.1415926*1.0;
					/*  nspeed_w=0;
					  while(nspeed_w<v_w)
						{
					  accurate(0,0,v_w);
					  SetSpeed (0 ,0,nspeed_w);
						delay(50);
						};*/
					  for (int i=0;i<500;i+=1)
							{
								SetSpeed (0 ,0,v_w);
								delay(20);
							};
					  v_x=0;v_y=0;v_w=0;
					  for (int i=0;i<100;i+=1)
							{
					      SetSpeed(0,0,0);
								delay(2);
							};
					  break;
					case keyback:
							return;
				}
		}
		SetSpeed(v_x,v_y,v_w);
		
		delay(LCD_DELAY);
		}
}
