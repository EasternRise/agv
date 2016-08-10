#include "stm32f10x.h"
#include "all.h"
#include "M_move.h"

void all_test(void);
void my_test(int t_x,int t_y);

float AGV_speed=500;
float speed_point=200;

int Stop_time=1000;

void Keep_Test(void)
{
	//extern float nspeed_x,nspeed_y,nspeed_w;
	
	while(1)
	{
		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("!!! BE CAREFUL !!!");
		
		LCD_SetXY(0,1);
		LCD_WriteString("1.camer1:");
		
		LCD_SetXY(0,2);
		LCD_WriteString("2.camer2:");
		
		LCD_SetXY(0,3);
		LCD_WriteString("3.compass");
		
	  LCD_SetXY (0,4);
		LCD_WriteString ("4.STOP");
		
		LCD_SetXY (0,5);
		LCD_WriteString ("5.All");
		
		refresh();	
		
		if(KeyReady==1)		
		 {
				KeyReady=0;

				switch(KeyValue)
				{
					case 1:
						camer1_Test();
						break;
					case 2:
						camer2_Test();
						break;
					case 3:
						compass_test();
						break;
					case 4:
						break;
					case key5:
						all_test();
						break;
					case keyback:
							return;
				}
		}
		delay(LCD_DELAY);
		}
}


extern float Vout_X;
extern float Vout_Y;
extern float Vout_W;

extern u8 pattern_find;

void camer1_Test(void)
{
	double speed=0;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("!!! CAMER1 !!!");
		
		LCD_SetXY(0,1);
		LCD_WriteString("ang_error");
		LCD_WriteDouble(guide_ang_error,3);
		
		LCD_SetXY(0,2);
		LCD_WriteString("dis_error:");
		LCD_WriteDouble(guide_dis_error,3);
		
		LCD_SetXY(0,3);
		LCD_WriteString("forward_speed:");
		LCD_WriteDouble(speed,1);
		
	  LCD_SetXY (0,4);
		LCD_WriteString ("4.STOP");

		LCD_SetXY(0,5);
		LCD_WriteString("V_X    :");
		LCD_WriteDouble(Vout_X,1);
		LCD_SetXY(0,6);
		LCD_WriteString("V_Y :");
		LCD_WriteDouble(Vout_Y,1);
		LCD_SetXY(0,7);
		LCD_WriteString("V_W:");
		LCD_WriteDouble(Vout_W,3);
		
		
 		keep_line(guide_dis_error,guide_ang_error/180*pi,speed);
// 		keep_line(guide_dis_error,0,f_speed);
//		keep_line(0,guide_ang_error/180*pi,f_speed);

// 		if(pattern_find == 1)
// 		{
// 			speed=500;
// 			pattern_find=0;
// 		}
		refresh();	
		
		if(KeyReady==1)		
		 {
				KeyReady=0;

				switch(KeyValue)
				{
					case 1:
						break;
					case 2:
						break;
					case 3:
						SetSpeed(0,0,0);
						Input_DoubleValue(&speed,"SPEED");
						break;
					case 4:
						f_speed=0;
						break;
					case keyback:
							return;
				}
		}
		delay(LCD_DELAY);
		}
}


void camer2_Test(void)
{
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("!!! CAMER2 !!!");
		
		LCD_SetXY(0,1);
		LCD_WriteString("ang");
		LCD_WriteDouble(locate_ang,3);
		
		LCD_SetXY(0,2);
		LCD_WriteString("x_error:");
		LCD_WriteDouble(locate_x_error,3);
		
		LCD_SetXY(0,3);
		LCD_WriteString("y_error:");
		LCD_WriteDouble(locate_y_error,1);
		
	  LCD_SetXY (0,4);
		LCD_WriteString ("4.STOP");

		LCD_SetXY(0,5);
		LCD_WriteString("V_X    :");
		LCD_WriteDouble(Vout_X,1);
		LCD_SetXY(0,6);
		LCD_WriteString("V_Y :");
		LCD_WriteDouble(Vout_Y,1);
		LCD_SetXY(0,7);
		LCD_WriteString("V_W:");
		LCD_WriteDouble(Vout_W,3);
		
		
// 		keep_line(guide_dis_error,guide_ang_error/180*pi,f_speed);
keep_point(locate_x_error,locate_y_error,locate_ang/180*pi);
// 		keep_line(guide_dis_error,0,f_speed);
//		keep_line(0,guide_ang_error/180*pi,f_speed);

		refresh();	
		
		if(KeyReady==1)		
		 {
				KeyReady=0;

				switch(KeyValue)
				{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						f_speed=0;
					break;
					case keyback:
							return;
				}
		}
		delay(LCD_DELAY);
		}
}


void compass_test(void)
{
	double speed=0;
	double target_angle=96.5;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("!!! COMPASS !!!");
		
		LCD_SetXY(0,1);
		LCD_WriteString("ang");
		LCD_WriteDouble(angle_average,3);
		
		LCD_SetXY(0,2);
		LCD_WriteString("target_:");
		LCD_WriteDouble(target_angle,3);
		
		LCD_SetXY(0,3);
		LCD_WriteString("forward_speed:");
		LCD_WriteDouble(speed,1);
		
	  LCD_SetXY (0,4);
		LCD_WriteString ("4.STOP");


		LCD_SetXY(0,6);
		LCD_WriteString("V_X: ");
		LCD_WriteDouble(Vout_X,1);
		
		LCD_WriteString(" V_Y: ");
		LCD_WriteDouble(Vout_Y,1);

		LCD_WriteString(" V_W: ");
		LCD_WriteDouble(Vout_W,3);
		
		
			keep_line(0,(target_angle-angle_average)/180*pi,speed);


		refresh();	
		
		if(KeyReady==1)		
		 {
				KeyReady=0;

				switch(KeyValue)
				{
					case 1:
						break;
					case 2:
						SetSpeed(0,0,0);
						Input_DoubleValue(&target_angle,"ANGLE");
						break;
					case 3:
						SetSpeed(0,0,0);
						Input_DoubleValue(&speed,"SPEED");
						break;
					case 4:
						f_speed=0;
						break;
					
					case keyback:
							return;
					
				}
		}
		delay(LCD_DELAY);
		}
}
//int mytime=0;//计时
//**************************+

extern float speed_propor;
//*************************

void all_test(void)
{
	u8 test_status=0;
	double speed=0;
	
	int target_x=0;
	int target_y=0;
	
	obstacle_init();
	//*****************
	
	start_pos.x=0xff;
	start_pos.y=0xff;

	//***************
	if(my_robot == arrive_target)
		my_robot=adjust_pos;
	slow_down=0;
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		
		if(test_status)
		{
				if(low_ele == 1)
				{
						LCD_Clear();
						LCD_SetXY(0,5);
						LCD_WriteString("LOW ELECTRICITY");
				}
				else
				{
					switch(my_robot)
					{
							case on_road:
								//误差为摄像头传回来的数据
								LCD_WriteString("ON ROAD ");
								if(slow_down == 1)
									LCD_WriteString(" SLOWing");
								break;
							case on_road_slow:
									LCD_WriteString("ON ROAD ");
									LCD_WriteString(" SLOWing");
							case see_point:
								LCD_WriteString("SEE POINT");
								break;
							
							//进入这个状态时一次性确定机器人的目标姿态
							//根据当前姿态，用反馈调节来调整机器人的姿态
							
							case adjust_pos:
								LCD_WriteString("ADJUST ing :");
								LCD_WriteDouble(move_angle,2);
								break;
							case leave_point:
								LCD_WriteString("LEAVing :");
								LCD_WriteDouble(move_angle,2);
								break;
							case arrive_target:
								LCD_WriteString("ARRIVE");
								break;
						}	
					}
			}
		else
		LCD_WriteString("!!! STOP!!!");	
		
		LCD_SetXY(0,1);
		LCD_WriteString("1.START");
		LCD_SetXY(0,2);
		LCD_WriteString("2.STOP:");
				
		LCD_SetXY(0,3);
		LCD_WriteString("3:F_SPEED:");
		LCD_WriteDouble(speed,1);
		
	  LCD_SetXY (0,4);
		
		LCD_WriteString ("4:NOW:");
		LCD_WriteString (" x:");
		LCD_WriteInt(start_pos.x);
		LCD_WriteString (" y:");
		LCD_WriteInt(start_pos.y);
		
		LCD_SetXY(0,5);
		LCD_WriteString ("5:TARGET:");
		LCD_WriteString (" x:");
		LCD_WriteInt(target_pos.x);
		LCD_WriteString (" y:");
		LCD_WriteInt(target_pos.y);

		LCD_SetXY (0,6);
		LCD_WriteString ("6.GO");
		
		LCD_SetXY(0,7);
		//LCD_WriteString("V_X :");
		LCD_WriteDouble(Vout_X,1);
		//LCD_WriteString(" V_Y :");
		LCD_WriteDouble(Vout_Y,1);
		//LCD_WriteString("V_W:");
		LCD_WriteDouble(Vout_W,3);
		
		
		refresh();	
		
		if(KeyReady==1)		
		 {
				KeyReady=0;

				switch(KeyValue)
				{
					case 1:
						test_status=1;
						break;
					case 2:
						test_status=0;
						break;
					case 3:
						SetSpeed(0,0,0);
						Input_DoubleValue(&speed,"SPEED");
						break;
					case 4:
						break;
					case key5:
						Input_IntValue(&target_x,"X: ");
						target_pos.x=target_x;
						Input_IntValue(&target_y,"y: ");
						target_pos.y=target_y;
						obstacle_init();
						break;

						case key6:
// 						my_robot=on_road;
// 						start_pos.x=0xff;
// 						start_pos.y=0xff;
// 						speed=100;
						break;

					case keyback:
							return;
				}
		}
		if(test_status)
		//move_car(speed);
		{
			move_distance+=speed*speed_propor*LCD_DELAY/1000;
			
			if(move_distance>4500)
				move_car(speed_point);
			else
				move_car(speed);
		}	
		
		delay(LCD_DELAY);
	}
}


void my_test(int t_x,int t_y)
{

	target_pos.x=t_x;
	target_pos.y=t_y;
	
	if(my_robot != unloacted)
		obstacle_init();
	
	u8 test_status=1;

	slow_down=1;
	u8 ok_status=1;
	
	if(my_robot == arrive_target)
		my_robot=see_point;
	
	while (1)
	{		
	LCD_Clear();
	LCD_SetXY(0,0);
	if(test_status)
		{
				if(low_ele == 1)
				{
						LCD_Clear();
						LCD_SetXY(0,5);
						LCD_WriteString("LOW ELECTRICITY");
				}
				else
				{
					switch(my_robot)
					{
							case unloacted:
								LCD_WriteString("UN LOCATED!!!");
							case back_slow:
								LCD_WriteString("BACKING!!!");
							case on_road:
								//误差为摄像头传回来的数据
								LCD_WriteString("ON ROAD ");
								if(slow_down == 1)
									LCD_WriteString(" SLOWing");
								break;
							case see_point:
								LCD_WriteString("SEE POINT");
								break;
							
							//进入这个状态时一次性确定机器人的目标姿态
							//根据当前姿态，用反馈调节来调整机器人的姿态
							
							case adjust_pos:
								LCD_WriteString("ADJUST ing :");
								LCD_WriteDouble(move_angle,2);
							//  move_distance=0;
								break;
							
							case leave_point:
								LCD_WriteString("LEAVE POINT");
						 // move_distance=0;
							break;
							
							case on_road_slow:
								LCD_WriteString("ON ROAD SLOW");
							break;
							
							case arrive_target:
								LCD_WriteString("ARRIVE");
					  //  move_distance=0;
								if(f_speed==0 && br_speed==0 && bl_speed == 0)
									return;
								break;
							
						}	
					}
			}
			else
			LCD_WriteString("!!! STOP!!!");	
			
			refresh();
				
      if(KeyReady)
        {
			     KeyReady=0;	//清按键标志位
            switch(KeyValue)
            {
							case 5:							
							  return;
							case 18: //ok
								if (ok_status)
								{
								  test_status=0;
								  ok_status=0;
								}
								else 
								{
								  test_status=1;
									ok_status=1;
								}
								break;
						}
				}
			
			if(test_status)
		  move_car(AGV_speed);
			else
				SetSpeed(0,0,0);
/*			{
				if (my_robot==on_road)
				move_distance+=AGV_speed*speed_propor*LCD_DELAY/1000;
				
				if(move_distance>600)
					move_car(speed_point);
				else
					move_car(AGV_speed);
			 }	
				 */
		//test_status=0;
		delay(LCD_DELAY);
		}
}
