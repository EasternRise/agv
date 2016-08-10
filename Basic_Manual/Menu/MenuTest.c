#include "all.h"

void Sonictest(void)
{
	
	for(;;)
	{
		Sonic_Start=1;
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("55");
		LCD_SetXY(0,1);
		LCD_WriteNum(supersonic_temp1);
		LCD_SetXY(0,2);
		LCD_WriteString("5a");
		LCD_SetXY(0,3);
		LCD_WriteNum(supersonic_temp2);
    	OSTimeDly(4);
		
	}
}

void Shootest1(void)
{
	fp64 dist1,dist2,dist3,zeta,deta,angle;
	//uint8 flag=0;

	Route_Num=8;
	OSTimeDly(100);
	for(;;)
	{
		dist1=supersonic_temp2;
		dist2=supersonic_temp1;		
		zeta=asin((dist1-dist2)/465);
		LCD_SetXY(0,1);
		LCD_WriteNum(supersonic_temp1);
		LCD_SetXY(0,2);
		LCD_WriteNum(supersonic_temp2);
		if((dist1>500)&&(dist1<2000)&&(dist2>500)&&(dist2<2000))
		{
			if(Fp64Abs(zeta)>0.5*PI/180)
			{
				Speed_F=-10*14000*zeta*18/PI;
				Speed_FR=10*14000*zeta*18/PI;
				Speed_BL=-10*14000*zeta*18/PI;
				Speed_BR=10*14000*zeta*18/PI;
			}
			else
			{
				RouteForm=RouteStop;
				OSTimeDly(100);
				break;
			}
		}
		OSTimeDly(1);
	}
	
	for(;;)
	{
		dist1=supersonic_temp2;
		dist2=supersonic_temp1;
		deta=(dist1+dist2)/2;
		if((dist1>500)&&(dist1<2000)&&(dist2>500)&&(dist2<2000))
		{
			dist3=1230-deta;
			angle=Gps_List[0].angle;
			
			break;
		}
		OSTimeDly(1);
	}
	
	//SetKeep(dist3,angle);
	
	for(;;)
	{
		if(Button3_Up)
			ShiftDist=dist3+410;
		if(Button4_Up)
			ShiftDist=dist3+200;
		if(Button5_Up)
			ShiftDist=dist3;
		if(Button3_Down)
			ShiftDist=dist3;
		if(Button4_Down)
			ShiftDist=dist3+200;
		if(Button5_Down)
			ShiftDist=dist3+410;
		
		OSTimeDly(1);
	}
	
	
	}
				
		
		
		
	/*	
		
				
		if(Button10_On)
		{
			RouteForm=RouteFree;
			FBonly_flag=1;
			dist1=supersonic_temp2;
			dist2=supersonic_temp1;
			if(Route_Num==8)
				dist3=1230;
			if(Route_Num==9)
				dist3=1430;
			if(Route_Num==10)
				dist3=1640;
			if((dist1>500)&&(dist1<2000)&&(dist2>500)&&(dist2<2000))
			{	
				zeta=asin((dist1-dist2)/465);
				deta=dist3-(dist1+dist2)/2;
				LCD_Clear();
				LCD_SetXY(0,0);
				LCD_WriteNum(zeta*180/PI);
				LCD_SetXY(0,1);
				LCD_WriteNum(dist1);
				LCD_SetXY(0,2);
				LCD_WriteNum(dist2);
				LCD_SetXY(0,4);
				LCD_WriteNum(Speed_F/14000);
				LCD_SetXY(0,5);
				LCD_WriteNum(Speed_FR/14000);
				LCD_SetXY(0,6);
				LCD_WriteNum(Speed_BL/14000);
				LCD_SetXY(0,7);
				LCD_WriteNum(Speed_BR/14000);
						
				if(Fp64Abs(zeta)>0.5*PI/180)
				{
					
						Speed_F=-10*14000*zeta*18/PI;
						Speed_FR=10*14000*zeta*18/PI;
						Speed_BL=-10*14000*zeta*18/PI;
						Speed_BR=10*14000*zeta*18/PI;
				}
				else
				{
						Speed_F=0;
						Speed_FR=0;
						Speed_BL=0;
						Speed_BR=0;
				}
				if(Fp64Abs(deta)>5)
				{
					if(deta<0)
					{
						Speed_F+=10*14000*Fp64Abs(deta)/50;
						Speed_FR+=-10*14000*Fp64Abs(deta)/50;
						Speed_BL+=-10*14000*Fp64Abs(deta)/50;
						Speed_BR+=10*14000*Fp64Abs(deta)/50;
					}
					else
					{
						Speed_F+=-10*14000*Fp64Abs(deta)/50;
						Speed_FR+=10*14000*Fp64Abs(deta)/50;
						Speed_BL+=10*14000*Fp64Abs(deta)/50;
						Speed_BR+=-10*14000*Fp64Abs(deta)/50;
					}			
				}
			}
		}
		//Data[7]=dist1;
		//Data[8]=dist2;
		//SaveMovingData();
		else
		{
			FBonly_flag=0;
			RouteForm=RouteStop;
		}
		OSTimeDly(1);
	}
}
*/
void Shootest(void)
{
	fp64 dist1,dist2,dist3,zeta,deta,deta1,encoder_initdist,encoder_dist=0;
	uint8 flag=0,filter_flag=0,count_flag=0,sonic_flag=1,encoder_flag=0;
	fp64 count;
	//Ranging_Start(CONTROL_SONIC1);
	//Ranging_Start(CONTROL_SONIC2);
	dist3=DIS_Launch1Board;
	Route_Num=8;
	OSTimeDly(100);
	for(;;)
	{
		if(Button9_On)
		{
			
			RouteForm=RouteFree;
			FBonly_flag=1;
			dist1=supersonic_temp2;
			dist2=supersonic_temp1;
			
			if(Button3_Up)
			{
				Sonic_Start=1;
				encoder_flag=0;
				OSTimeDly(200);
				dist3=DIS_Launch1Board+395;
			}
			if(Button4_Up)
			{
				Sonic_Start=1;
				encoder_flag=0;
				OSTimeDly(200);
				dist3=DIS_Launch1Board+182;
			}
			if(Button5_Up)
			{
				Sonic_Start=1;
				encoder_flag=0;
				OSTimeDly(200);
				dist3=DIS_Launch1Board;
			}
			if(Button3_Down)
			{
				Sonic_Start=1;
				encoder_flag=1;
				OSTimeDly(200);
				dist3=DIS_Launch3Board;
				encoder_dist=encoder_initdist+DIS_Launch2Encoder;
			}
			if(Button4_Down)
			{
				Sonic_Start=1;
				encoder_flag=1;
				OSTimeDly(200);
				dist3=DIS_Launch2Board;
				encoder_dist=encoder_initdist+DIS_Launch1Encoder;
			}
			if(Button5_Down)
			{
				sonic_flag=0;
				encoder_flag=1;
				OSTimeDly(200);
				dist3=DIS_Launch1Board;
				encoder_initdist=Encoders[1].Distance;
				encoder_dist=encoder_initdist;
			}
			
			if((dist1>500)&&(dist1<2000)&&(Fp64Abs(dist1-dist2)<150))
			{	
				zeta=asin((dist1-dist2)/465);
				deta=dist3-(dist1+dist2)/2;
				
					
				if((Fp64Abs(zeta)>0.4*PI/180)&&(flag==0))
				{
						
						Speed_F=-10*14000*zeta*18/PI;
						Speed_FR=10*14000*zeta*18/PI;
						Speed_BL=-10*14000*zeta*18/PI;
						Speed_BR=10*14000*zeta*18/PI;
				}
				else
				{
						flag=1;
						Speed_F=0;
						Speed_FR=0;
						Speed_BL=0;
						Speed_BR=0;
				}
				
				if((Fp64Abs(zeta)>0.8*PI/180)&&(flag==1))
				{
					flag=0;
				}	
				if(Fp64Abs(deta)>5)
				{
					if(deta<0)
					{
						Speed_F+=10*14000*Fp64Abs(deta)/50;
						Speed_FR+=-10*14000*Fp64Abs(deta)/50;
						Speed_BL+=-10*14000*Fp64Abs(deta)/50;
						Speed_BR+=10*14000*Fp64Abs(deta)/50;
					}
					else
					{
						Speed_F+=-10*14000*Fp64Abs(deta)/50;
						Speed_FR+=10*14000*Fp64Abs(deta)/50;
						Speed_BL+=10*14000*Fp64Abs(deta)/50;
						Speed_BR+=-10*14000*Fp64Abs(deta)/50;
					}			
				}
				if((Fp64Abs(zeta)<=0.4*PI/180)&&(Fp64Abs(deta)<=5)&&(sonic_flag==0))
				{
					sonic_flag=1;
					Sonic_End=1;
				}
				//极其简单的滤波
				if((Fp64Abs(deta)>20)||(Fp64Abs(zeta)>0.8*PI/180))	
				{
					filter_flag=1; count_flag=1;
				}else {filter_flag=0;count_flag=0;count=0;}
				
				if(filter_flag==1&&count_flag==1&&count<=200)
				{
					count++;
					Speed_F=0;
					Speed_FR=0;
					Speed_BL=0;
					Speed_BR=0;
				}
				if(count>500)
				{
					count_flag=0;
				}
				
				/////////////////////////////////////////////////
			}
			
			if((encoder_dist!=0)&&(encoder_flag==1))
			{
				deta1=encoder_dist-Encoders[1].Distance;
				if(Fp64Abs(deta1)>5)
				{
					if(Fp64Abs(deta1)>200)
					{
						Speed_F+=10*14000*200/50*(Fp64Abs(deta1)/deta1);
						Speed_FR+=10*14000*200/50*(Fp64Abs(deta1)/deta1);
						Speed_BL+=10*14000*200/50*(Fp64Abs(deta1)/deta1);
						Speed_BR+=10*14000*200/50*(Fp64Abs(deta1)/deta1);
					}
					else
					{
						Speed_F+=10*14000*deta1/50;
						Speed_FR+=10*14000*deta1/50;
						Speed_BL+=10*14000*deta1/50;
						Speed_BR+=10*14000*deta1/50;
					}
				}
			}
		}
		//Data[7]=dist1;
		//Data[8]=dist2;
		//SaveMovingData();
		else
		{
			FBonly_flag=0;
			RouteForm=RouteStop;
		}
				LCD_Clear();
				LCD_SetXY(0,0);
				LCD_WriteNum(zeta*180/PI);
				LCD_SetXY(0,1);
				LCD_WriteNum(dist1);
				LCD_SetXY(0,2);
				LCD_WriteNum(dist2);
				LCD_SetXY(0,3);
				LCD_WriteNum(deta);
				LCD_SetXY(0,4);
				LCD_WriteNum(deta1);
				LCD_SetXY(0,5);
				LCD_WriteString("Encoder:");
				
				LCD_WriteNum(Encoders[1].Distance);
				/*
				LCD_SetXY(0,4);
				LCD_WriteNum(Speed_F/14000);
				LCD_SetXY(0,5);
				LCD_WriteNum(Speed_FR/14000);
				LCD_SetXY(0,6);
				LCD_WriteNum(Speed_BL/14000);
				LCD_SetXY(0,7);
				LCD_WriteNum(Speed_BR/14000);*/
		OSTimeDly(1);
	}
}		


void LidarUse()
{
	struct Pointfp64 pos={0,0};
	struct Pointfp64 pos_scan,pos_real={250,1405};//{-400,925};
	uint8 flag=0,i;
	fp64 dist;
	GPS_Clear();
	Lidar_Stat=Lidar_Route_3;//第三段的修正方式
	while(Lidar_Stat==Lidar_Route_3)//在雷达传输完成前，液晶屏显示"waiting"
	{
		LCD_SetXY(0,0);
		LCD_WriteString("Waiting");
		OSTimeDly(1);
	}
	LCD_SetXY(0,1);
	LCD_WriteString("scan done");
	LCD_SetXY(0,2);
	LCD_WriteNum(Lidar_Num_Result);
	PressAnyKey();
	//雷达任务会给出 Lidar_Num_Result 个圆环中心
	for (i=0;i<Lidar_Num_Result;i++)//处理雷达数据，将其他环的圆心剔除
	{
		LCD_SetXY(0,2);
		LCD_WriteNum(i);
		pos_scan.x=Lidar_ResultX[i]; 
		pos_scan.y=Lidar_ResultY[i];
		LCD_SetXY(0,3);
		LCD_WriteNum(pos_scan.x);
		LCD_SetXY(0,4);
		LCD_WriteNum(pos_scan.y);
		dist=MATH_Distance(pos_scan,pos_real);
		LCD_SetXY(0,5);
		LCD_WriteNum(dist); 
		PressAnyKey();		        
	    if (dist<500)//若圆心相对车体距离误差小于500，取之
	    {
	    	LCD_SetXY(0,1);
	    	LCD_WriteString("calculating");
	    	flag=i+1; break;
	    }
	}
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("cal done");
	LCD_SetXY(0,1);
	LCD_WriteNum(flag);
	PressAnyKey();
				
	if (flag!=0)//修改坐标
	{
		Gps_List[0].position.x-=(pos_scan.y-pos_real.y)*cos(Gps_List[0].radian)+(pos_scan.x-pos_real.x)*sin(Gps_List[0].radian);
	   	Gps_List[0].position.y-=(pos_scan.y-pos_real.y)*sin(Gps_List[0].radian)-(pos_scan.x-pos_real.x)*cos(Gps_List[0].radian);
	   	LCD_SetXY(0,7);
	   	LCD_WriteString("fix done");
	}else
	{
		LCD_SetXY(0,7);
	   	LCD_WriteString("no fix");
	}
	
	LCD_SetXY(0,0);
	LCD_WriteString("lidar Done");
	LCD_SetXY(0,1);
	LCD_WriteNum(Lidar_ResultY[i]);
	LCD_SetXY(0,2);
	LCD_WriteNum(Lidar_ResultX[i]);
	LCD_SetXY(0,3);
	LCD_WriteNum((pos_scan.y-pos_real.y)*cos(Gps_List[0].radian)+(pos_scan.x-pos_real.x)*sin(Gps_List[0].radian));
	LCD_SetXY(0,4);
	LCD_WriteNum((pos_scan.y-pos_real.y)*sin(Gps_List[0].radian)-(pos_scan.x-pos_real.x)*cos(Gps_List[0].radian));
	
	PressAnyKey();
	LCD_Clear();
	//用走直线的方法教到正确的坐标上//
	SetLine(50,200,40,0.2,0.2,pos,100,0.0);
	LCD_SetXY(0,5);
	LCD_WriteString("Set done");
	while(RouteFinish)
	{
		LCD_SetXY(0,5);
		LCD_WriteString("While");
		if(MATH_Distance(Gps_List[0].position,pos)<10)
	 	{
	 		RouteFinish=0;
	 		RouteForm=RouteStop;
	 		LCD_SetXY(0,2);
			LCD_WriteString("route3 done");	
		}
		OSTimeDly(1);
	}
}
	
void KeepTest()
{
	struct Value Values[]=
     {   
        {(void*)&Pid_List[2].PROP,	TYPE_FP64,	"DP"}, 
        {(void*)&Pid_List[2].Ti,	TYPE_FP64,	"DI"}, 
        {(void*)&Pid_List[2].Td,	TYPE_FP64,	"DD"},
        {(void*)&Pid_List[3].PROP,	TYPE_FP64,	"AP"}, 
        {(void*)&Pid_List[3].Ti,	TYPE_FP64,	"AI"}, 
        {(void*)&Pid_List[3].Td,	TYPE_FP64,	"AD"},
        {(void*)&TopSpeed,			TYPE_INT32, "TS"},
        {(void*)&AimPoint.x,		TYPE_FP64,	"Ax"},
		{(void*)&AimPoint.y,		TYPE_FP64,	"Ay"},
		{(void*)&Aim_Angle,			TYPE_FP64,	"AA"}
     };
	DISPLAY_ValuesManager(Values,10,PID_NANDPAGE);
	
	GPS_Clear();
	PID_Clear();
	LCD_Clear();
	
	RouteForm=RouteStop;
	
	RouteFinish=1;
	RouteForm=RouteKeep;
	
	while(RouteFinish)
	{
		if((Fp64Abs(Gps_List[0].angle-Aim_Angle)<0.1)&&Getlength(AimPoint,Gps_List[0].position)<5)
		{
			RouteForm=RouteStop;
		}else
		{
			RouteForm=RouteKeep;
		}
		OSTimeDly(1);
	}
			
}		
	