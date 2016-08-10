#include "All.h"
#if ManualSetting

uint8 Route_Num=0;
uint8 ManualData[11];//前九字节传数据，第十字节判断出错或断线
uint8 FBonly_flag=0;
uint8 SA2_en=1;
//手柄赋值速度，与Speed_F复合得到总速度，完成后要替代原FaulHaber_speed_FL
fp32 Manual_speed_FL=0;			//手动左前轮速度
fp32 Manual_speed_FR=0;			//手动右前轮速度
fp32 Manual_speed_BL=0;			//手动左后轮速度
fp32 Manual_speed_BR=0;			//手动右后轮速度
 
void TaskManual(void *pdata)
{
	//uint8 auto_flag=0;
	int16 Speed_UP_DOWN;					//存储手柄返回速度值
	int16 Speed_LEFT_RIGHT;
	int16 Speed_TURN;
	
	fp64  Speed_UD;							//存储转换后速度值
	fp64  Speed_LR;
	fp64  Speed_T;
	
	uint8 temp;								//配合判断ManualData[10]
	fp32 mutiple=1.0;
	//static uint8 flag=0;
	
	fp32 Rotation[6]={0};//储存6个电机的位置信息
	
	OSTimeDly(100);
	MaxonDisable();
	OSTimeDly(10);
	MaxonEnable();
	OSTimeDly(10);
	MaxonSpeedMode();
	
	for(;;)
	{
		temp=ManualData[10];
		FPGA_ReadManual(ManualData);
		if(ManualData[10]!=temp)			//手柄工作正常
		{
			time_flag=0;
			//摇杆动作赋速度
			Speed_UP_DOWN=((uint16)ManualData[2]<<8)+(uint16)ManualData[1];
			
			if(Speed_UP_DOWN>=0x3000)
    		 	Speed_UD = (fp64)(Speed_UP_DOWN-0x3000)/1.5;
			else
				Speed_UD = (fp64)(Speed_UP_DOWN-0x3000)/1.5;
						
			Speed_LEFT_RIGHT=((uint16)ManualData[4]<<8)+(uint16)ManualData[3];
		
			if(Speed_LEFT_RIGHT>=0x3000)
				Speed_LR = -(fp64)(Speed_LEFT_RIGHT-0x3000)/1.5;
			else
				Speed_LR = -(fp64)(Speed_LEFT_RIGHT-0x3000)/1.5;
						
			Speed_TURN=((uint16)ManualData[6]<<8)+(uint16)ManualData[5];
		
		 	if(Speed_TURN>=0x3000)
    		   	Speed_T = (fp64)(Speed_TURN-0x3000)/8.0;
    		else
    		   	Speed_T = (fp64)(Speed_TURN-0x3000)/8.0;
    		
    		if(FBonly_flag==1)
    		{
    			Speed_LR=0;
    			Speed_T =0;
    		}
    		//四轮速度分解
			Manual_speed_FL=(5.53859*Speed_LR)*mutiple;
			Manual_speed_FR=(5.53859*Speed_LR)*mutiple;
			Manual_speed_BL=(0.09095*Speed_UD-0.28648*Speed_LR)*mutiple;
			Manual_speed_BR=(0.09095*Speed_UD+0.28648*Speed_LR)*mutiple;
			
			if(Button8_On)
			{
				Manual_speed_FL=Manual_speed_FL/10;
				Manual_speed_FR=Manual_speed_FR/10;
				Manual_speed_BL=Manual_speed_BL/10;
				Manual_speed_BR=Manual_speed_BR/10;
			}
			
    		Speed_Motor((Speed_F+Manual_speed_FL), (Speed_FR+Manual_speed_FR), (Speed_BL+Manual_speed_BL),(Speed_BR+Manual_speed_BR));	
		  	
		  	error_convert[0] = Manual_speed_FL/14000;
		  	error_convert[1] = Speed_F/14000;
		  	error_convert[2] = Manual_speed_FR/14000;
		  	error_convert[3] = Speed_FR/14000;
		  	error_convert[4] = Manual_speed_BL/14000;
		  	error_convert[5] = Speed_BL/14000;
		  	error_convert[6] = Manual_speed_BR/14000;
		  	error_convert[7] = Speed_BR/14000;
		  	
		  	
		  	//上层动作

		  	
		  	//除了1之外，单键用来控制电磁阀

		  	if(Button2_Up && Button7_Off)
			{
				Relay_Clear(24);
			}
			
			if(Button2_Down && Button7_Off)
			{
				Relay_Set(24);
			}

			if(Button3_Up && Button7_Off)
			{
				Relay_Set(23);
			}
			
			if(Button3_Down && Button7_Off)
			{
				Relay_Clear(23);
			}

			if(Button4_Up && Button7_Off)
			{
				Relay_Clear(22);
				Relay_Set(21);
			}
			
			if(Button4_Down && Button7_Off)
			{
				Relay_Clear(21);
				Relay_Set(22);
			}

			if(Button5_Up && Button7_Off)
			{
				Relay_Clear(19);
				Relay_Set(20);				
			}
			
			if(Button5_Down && Button7_Off)
			{
			    Relay_Clear(20);
				Relay_Set(19);
;
			}
			
			

			
			
			//7键为第二功能键，单键1和组合键控制上层电机
			
			if(Button1_Up && Button7_On)
			{
			    Rotation[0]+=0.05;
				arm_motor(ID_MOTOR1 , Rotation[0]);
			}
			
			if(Button1_Down && Button7_On)
			{
			    Rotation[0]-=0.05;
				arm_motor(ID_MOTOR1 , Rotation[0]);
			}
			
			if(Button2_Up && Button7_On)
			{
			    Rotation[1]+=0.05;
				arm_motor(ID_MOTOR2 , Rotation[1]);
			}
			
			if(Button2_Down && Button7_On)
			{
			    Rotation[1]-=0.05;
				arm_motor(ID_MOTOR2 , Rotation[1]);
			}		
			
			if(Button3_Up && Button7_On)
			{
			    Rotation[2]=1;
				arm_motor(ID_MOTOR3 , Rotation[2]);
			}
			
			if(Button3_Down && Button7_On)
			{
			    Rotation[2]=-1;
				arm_motor(ID_MOTOR3 , Rotation[2]);
			}
	        
	       /* if(Button3_Off)
			{
			    Rotation[2]=0;
				arm_motor(ID_MOTOR3 , Rotation[2]);
			}*/


			if(Button6_Left && Button7_On)
			{
			    Rotation[5]-=0.05;
				arm_motor(ID_MOTOR6 , Rotation[5]);
			}
			
			if(Button6_Right && Button7_On)
			{
			    Rotation[5]+=0.05;
				arm_motor(ID_MOTOR6 , Rotation[5]);
			}	
				
			/*
			//自动程序启动
			if(Button6_Off)
				auto_flag=0;
			if((auto_flag==0) && Button8_On && Button6_Right)
			{
				auto_flag=1;
				if (Route_Num<=7)
				{
					Route_Num++;
					OSMboxPost(AutoMbox,&Route_Num);
					OSMboxPost(ArmMbox,&Route_Num);
				}
			
			}*/
			
		}
		else			//手柄断线或传输数据异常
		{
		    if(time_flag>200)//time_flag对时间进行计数，如果当时间大于一定值，电机抱死。
						 	 //主要用来防止手柄线与arm分开时车疯跑。当线分开时，1s后自动停车
		    {
                  Speed_F=0;
                  Speed_FR=0;
                  Speed_BL=0;
                  Speed_BR=0;
                  
			      Acc_Limit_enable=0;
			      Speed_Motor(0,0,0,0);
			      Acc_Limit_enable=1;
			      
			      OSTaskDel(RoutePrio);	
			      OSTaskCreate (TaskRoute ,  (void *)0,  &TaskRouteStk[TaskStkLengh -1],RoutePrio);
		    }       
		 }
		 
		 OSTimeDly(1);
	}
}			

#endif			
		
		
		
		
		
		
		