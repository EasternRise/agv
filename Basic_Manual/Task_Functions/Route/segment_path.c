#include "all.h"
#define LidarBoardFlag 1


void GoRoute1(void)//从发车位到取树叶位置
{
	//uint16 sonic_dist;
	//uint8 flag=0;
	//static uint8 count=0;

	//int flaglidar=1;
	//fp64 robot_radian,y_record,y_scan;	
	//extern fp64 Lidar_Dist1;



	Route_Num=1;	
//	SetPointPath(0,Route[1],PointRoute[Route[1]].selfangle,PointRoute[Route[1]].position);
	
	while(RouteFinish) 
	 {
		if(Point_NowNum>=Point_EndNum-1)
	 	{	 		
	 		RouteFinish=0;
	 		
	 		Route_Num++;		
	 		OSMboxPost(ArmMbox,&Route_Num);

			LCD_SetXY(0,0);
			LCD_WriteString("route1 done");			
		}
				
		OSTimeDly(1);
	 }
}

void GoRoute2(void)//向前走抓树叶
{
	//struct Pointfp64 position;
	//fp64 aimangle;
	LCD_Clear();
	Route_Num=2;
//	SetPointPath(Route[1]+1,Route[2],PointRoute[Route[2]].selfangle,PointRoute[Route[2]].position);
	
	while(RouteFinish) 
	 {
	 
		if((SA_Read(1)==0)||(Point_NowNum>=Point_EndNum-1))
	 	{	
	 		rGPJDAT&=~0x1000;    //关蜂鸣器 				
	 		RouteFinish=0;
	 		RouteForm=RouteStop;
	 		
			LCD_SetXY(0,0);
			LCD_WriteString("route2 done");
		}
		
		OSTimeDly(1);
	 }
	 
	
} 


void GoRoute3(void)//放最远的树叶
{
	struct Pointfp64 pos_scan1,pos_scan2,pos_tmp;
	struct Pointfp64 pos_real1,pos_real2;
	struct Pointfp64 pos_ring1= C_Ring1,pos_ring2=C_Ring2;
	struct Pointfp64 pos_robot;
	uint8 flag1=0,flag2=0,i;
	fp64 dist1,dist2,radian1,radian2,robot_radian;
	struct Pointfp64 error1,error2;
	int flaglidar;
	struct Pointfp64 temp;
	
	
	Route_Num=3;
//	SetPointPath(Route[2]+1,Route[3],PointRoute[Route[3]].selfangle,PointRoute[Route[3]].position);
	flaglidar=1;
	
	while(RouteFinish) 
	{

		if((Point_NowNum>=Point_EndNum-250)&&flaglidar==1)
		{
			pos_robot=Gps_List[0].position;
			dist1=MATH_Distance(pos_robot,pos_ring1);
			dist2=MATH_Distance(pos_robot,pos_ring2);
			radian1=atan((pos_robot.y-pos_ring1.y)/(pos_robot.x-pos_ring1.x))-Gps_List[0].radian;
			radian2=atan((pos_robot.y-pos_ring2.y)/(pos_robot.x-pos_ring2.x))-Gps_List[0].radian;
			pos_real1.x=dist1*sin(-radian1);
			pos_real1.y=dist1*cos(-radian1)-335;
			pos_real2.x=dist2*sin(-radian2);
			pos_real2.y=dist2*cos(-radian2)-335;
			Lidar_Stat=Lidar_Route_3;//第三段的修正方式
			flaglidar=0;
			robot_radian=Gps_List[0].radian;
			
	 			#if WalkGroundEnable
	 		RouteForm=RouteStop;
				#endif

			
		}
		if(Lidar_Stat==Lidar_Route_3)//在雷达传输完成前，液晶屏显示"waiting"
		{
			LCD_SetXY(0,0);
			LCD_WriteString("Waiting1");
		}
		else if (flaglidar==0)
		{
			//雷达任务会给出 Lidar_Num_Result 个圆环中心
			for (i=0;i<Lidar_Num_Result;i++)//处理雷达数据，将其他环的圆心剔除
			{
				pos_tmp.x=Lidar_ResultX[i]-Ring1_Shift.x; 
				pos_tmp.y=Lidar_ResultY[i]-Ring1_Shift.y;
				dist1=MATH_Distance(pos_tmp,pos_real1);
				dist2=MATH_Distance(pos_tmp,pos_real2);
			    if (dist1<500)//若圆心相对车体距离误差小于500，取之
			    {
			    	flag1=i+1; pos_scan1=pos_tmp;
			    	error1.x=pos_tmp.x-pos_real1.x;
			    	error1.y=pos_tmp.y-pos_real1.y;
			    }
			    if (dist2<200)
			    {
			    	flag2=i+1; pos_scan2=pos_tmp;
			    	error2.x=pos_tmp.x-pos_real2.x;
			    	error2.y=pos_tmp.y-pos_real2.y;
			    }
			}

			 error2=error1;
						
			if (flag1!=0)//修改坐标
			{
				temp.y=(error1.y+error2.y)/2;
				temp.x=(error1.x+error2.x)/2;
				Gps_List[0].position.x-=temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian);
			   	Gps_List[0].position.y+=temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian);
				fix_2.x=temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian);
				fix_2.y=-(temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian));
					#if WalkGroundEnable
				rGPJDAT|=0x1000;     //开蜂鸣器
					#endif
				flaglidar=-1;
			}
			else
			{
				flaglidar=-2;
		 		rGPJDAT&=~0x1000;    //关蜂鸣器 				
			}
			LCD_Clear();
			LCD_SetXY(10,0);
			LCD_WriteInt(flaglidar);  //-3
			LCD_SetXY(0,3);
			LCD_WriteInt(pos_real1.x); //495
			LCD_SetXY(10,3); 
			LCD_WriteInt(pos_real1.y); //1448
			LCD_SetXY(0,4);
			LCD_WriteInt(pos_scan1.x); //
			LCD_SetXY(10,4);
			LCD_WriteInt(pos_scan1.y); //

			LCD_SetXY(0,5);
			LCD_WriteInt(temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian)); //
			LCD_SetXY(10,5);
			LCD_WriteInt(-(temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian))); //

			LCD_SetXY(0,6);
			LCD_WriteNum(radian1/PI*180); //-15
			LCD_SetXY(10,6);
			LCD_WriteInt(radian2/PI*180); //-46
			OSTimeDly(1);

				#if WalkGroundEnable
			for(;;)
			{	
				if(Button7_On)
				{
					RouteForm=PointLine;
					LCD_SetXY(0,7);
					LCD_WriteString("out break!");
					break;
				}
				OSTimeDly(1);
			}
				#endif 

		}
 

		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;	
	 			
	 		Route_Num++;		
	 		OSMboxPost(ArmMbox,&Route_Num); 		

			LCD_SetXY(0,0);
			LCD_WriteString("route3 done");	


		}
		OSTimeDly(1);
	}
	
	
} 

void GoRoute4(void)//放第二片树叶
{
	struct Pointfp64 pos_scan1,pos_tmp;
	struct Pointfp64 pos_real1;
	struct Pointfp64 pos_ring1=C_Ring2;
	struct Pointfp64 pos_robot;
	uint8 flag1=0,i;
	fp64 dist1,radian1,robot_radian;
	struct Pointfp64 error1,error2;
	int flaglidar;
	struct Pointfp64 temp;

	Route_Num=4;
//	SetPointPath(Route[3]+1,Route[4],PointRoute[Route[4]].selfangle,PointRoute[Route[4]].position);
	
	flaglidar=2;
	while(RouteFinish) 
	 {
		if((Point_NowNum>=Point_EndNum-250)&&flaglidar==1)
		{
				#if WalkGroundEnable
			RouteForm=RouteStop;
				#endif
			pos_robot=Gps_List[0].position;
			dist1=MATH_Distance(pos_robot,pos_ring1);
			radian1=atan((pos_robot.y-pos_ring1.y)/(pos_robot.x-pos_ring1.x))-Gps_List[0].radian;
			pos_real1.x=dist1*sin(-radian1);
			pos_real1.y=dist1*cos(-radian1)-325;
			robot_radian=Gps_List[0].radian;
			Lidar_Stat=Lidar_Route_3;//第三段的修正方式
			flaglidar=0;
		}
		if(Lidar_Stat==Lidar_Route_3)//在雷达传输完成前，液晶屏显示"waiting"
		{
			LCD_SetXY(0,0);
			LCD_WriteString("Waiting1");
		}
		else if (flaglidar==0)
		{
			//雷达任务会给出 Lidar_Num_Result 个圆环中心
			for (i=0;i<Lidar_Num_Result;i++)//处理雷达数据，将其他环的圆心剔除
			{
				pos_tmp.x=Lidar_ResultX[i]-Ring2_Shift.x; 
				pos_tmp.y=Lidar_ResultY[i]-Ring2_Shift.y;
				dist1=MATH_Distance(pos_tmp,pos_real1);
			    if (dist1<300)//若圆心相对车体距离误差小于500，取之
			    {
			    	flag1=i+1; pos_scan1=pos_tmp;
			    	error1.x=pos_tmp.x-pos_real1.x;
			    	error1.y=pos_tmp.y-pos_real1.y;
			    }
			}

			error2=error1;
						
			if (flag1!=0)//修改坐标
			{
				temp.y=(error1.y+error2.y)/2;
				temp.x=(error1.x+error2.x)/2;
				Gps_List[0].position.x-=temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian);
			   	Gps_List[0].position.y+=temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian);
				fix_3.x=temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian);
			   	fix_3.y=-(temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian));
//				rGPJDAT|=0x1000;     //开蜂鸣器
				flaglidar=-1;
			}
			else
			{
				flaglidar=-2;
		 		rGPJDAT&=~0x1000;    //关蜂鸣器 				
			}
			LCD_Clear();
			LCD_SetXY(10,0);
			LCD_WriteInt(flaglidar);  //-3
			LCD_SetXY(0,3);
			LCD_WriteInt(pos_real1.x); //495
			LCD_SetXY(10,3);
			LCD_WriteInt(pos_real1.y); //1448
			LCD_SetXY(0,4);
			LCD_WriteInt(pos_scan1.x); //
			LCD_SetXY(10,4);
			LCD_WriteInt(pos_scan1.y); //

			LCD_SetXY(0,5);
			LCD_WriteInt(temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian)); //
			LCD_SetXY(10,5);
			LCD_WriteInt(-(temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian))); //

			LCD_SetXY(0,6);
			LCD_WriteNum(radian1/PI*180); //-15
				#if WalkGroundEnable
			for(;;)
			{	
				if(Button7_On)
				{
					RouteForm=PointLine;
					LCD_SetXY(0,7);
					LCD_WriteString("out break!");
					break;
				}
				OSTimeDly(1);
			}
				#endif 
		}
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;
	 			#if WalkGroundEnable	
	 		RouteForm=RouteStop;
	 			#else
	 		Route_Num++;		
	 		OSMboxPost(ArmMbox,&Route_Num);
	 			#endif 		
	 		LCD_SetXY(0,0);
			LCD_WriteString("route4 done");
			
		}
		
		OSTimeDly(1);
	 }
}

void GoRoute5(void)//放第三片树叶
{
	struct Pointfp64 pos_scan1,pos_tmp;
	struct Pointfp64 pos_real1;
	struct Pointfp64 pos_ring1=C_Ring3;
	struct Pointfp64 pos_robot;
	uint8 flag1=0,i;
	fp64 dist1,radian1,robot_radian;
	struct Pointfp64 error1,error2;
	int flaglidar;
	struct Pointfp64 temp;

	Route_Num=5;
//	SetPointPath(Route[4]+1,Route[5],PointRoute[Route[5]].selfangle,PointRoute[Route[5]].position);
	
	flaglidar=1;
	while(RouteFinish) 
	{

		if((Point_NowNum>=Point_EndNum-250)&&flaglidar==1)
		{
			pos_robot=Gps_List[0].position;
			dist1=MATH_Distance(pos_robot,pos_ring1);
			radian1=atan((pos_robot.y-pos_ring1.y)/(pos_robot.x-pos_ring1.x))-Gps_List[0].radian;
			pos_real1.x=dist1*sin(-radian1);
			pos_real1.y=dist1*cos(-radian1)-370;
			robot_radian=Gps_List[0].radian;
			Lidar_Stat=Lidar_Route_3;//第三段的修正方式
			flaglidar=0;
			
	 			#if WalkGroundEnable
	 		RouteForm=RouteStop;
				#endif

		}
		if(Lidar_Stat==Lidar_Route_3)//在雷达传输完成前，液晶屏显示"waiting"
		{
			LCD_SetXY(0,0);
			LCD_WriteString("Waiting1");
		}
		else if (flaglidar==0)
		{
			//雷达任务会给出 Lidar_Num_Result 个圆环中心
			for (i=0;i<Lidar_Num_Result;i++)//处理雷达数据，将其他环的圆心剔除
			{
				pos_tmp.x=Lidar_ResultX[i]-Ring3_Shift.x; 
				pos_tmp.y=Lidar_ResultY[i]-Ring3_Shift.y;
				dist1=MATH_Distance(pos_tmp,pos_real1);
			    if (dist1<300)//若圆心相对车体距离误差小于500，取之
			    {
			    	flag1=i+1; pos_scan1=pos_tmp;
			    	error1.x=pos_tmp.x-pos_real1.x;
			    	error1.y=pos_tmp.y-pos_real1.y;
			    }
			}

			error2=error1;
						
			if (flag1!=0)//修改坐标
			{
				temp.y=(error1.y+error2.y)/2;
				temp.x=(error1.x+error2.x)/2;
				Gps_List[0].position.x-=temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian);
			   	Gps_List[0].position.y+=temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian);
				fix_4.x=temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian);
			   	fix_4.y=-(temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian));
					#if WalkGroundEnable
				rGPJDAT|=0x1000;     //开蜂鸣器
					#endif
				flaglidar=-1;
			}
			else
			{
				flaglidar=-2;
		 		rGPJDAT&=~0x1000;    //关蜂鸣器 				
			}
			LCD_Clear();
			LCD_SetXY(10,0);
			LCD_WriteInt(flaglidar);  //-3
			LCD_SetXY(0,3);
			LCD_WriteInt(pos_real1.x); //495
			LCD_SetXY(10,3);
			LCD_WriteInt(pos_real1.y); //1448
			LCD_SetXY(0,4);
			LCD_WriteInt(pos_scan1.x); //
			LCD_SetXY(10,4);
			LCD_WriteInt(pos_scan1.y); //

			LCD_SetXY(0,5);
			LCD_WriteInt(temp.y*cos(-robot_radian)-temp.x*sin(-robot_radian)); //
			LCD_SetXY(10,5);
			LCD_WriteInt(-(temp.y*sin(-robot_radian)+temp.x*cos(-robot_radian))); //

			LCD_SetXY(0,6);
			LCD_WriteNum(radian1/PI*180); //-15

	 			#if WalkGroundEnable
			for(;;)
			{	
				if(Button7_On)
				{
					RouteForm=PointLine;
					break;
				}
				OSTimeDly(1);
			}
				#endif
		}
 
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;
	 			#if WalkGroundEnable
			RouteForm=RouteStop; 
				#else		
	 		Route_Num++;
	 		OSMboxPost(ArmMbox,&Route_Num);
	 			#endif
	 		LCD_SetXY(0,0);
			LCD_WriteString("route5 done");
			
		}
		
		OSTimeDly(1);
	 }
}

void GoRoute6(void)//给自动车树叶
{
	//PID_flag=0;
	Route_Num=6;
	RouteFinish=1;
	OSTimeDly(400);
//	SetPointPath(Route[5]+1,Route[6],PointRoute[Route[6]].selfangle,PointRoute[Route[6]].position);
	
	while(RouteFinish) 
	 {
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		rGPJDAT&=~0x1000;    //关蜂鸣器
	 		RouteFinish=0;
	 		//PID_flag=1;
	 		RouteForm=RouteStop;
	 		LCD_SetXY(0,0);
			LCD_WriteString("route6 done");
			
		}
		
		OSTimeDly(1);
	 }
}

void GoRoute7(void)
{
	Route_Num=7;
//	SetPointPath(Route[6]+1,Route[7],PointRoute[Route[7]].selfangle,PointRoute[Route[7]].position);
	
	while(RouteFinish)
	 {
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;	
	 		RouteForm=RouteStop;	 		
	 		LCD_SetXY(0,0);
			LCD_WriteString("route7 done");
			
		}
		
		OSTimeDly(1);
	 }
}

void GoRoute8(void)
{
	Route_Num=8;
//	SetPointPath(Route[7]+1,Route[8],PointRoute[Route[8]].selfangle,PointRoute[Route[8]].position);
	
	while(RouteFinish) 
	 {
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;	
	 		RouteForm=RouteStop; 		
	 
	 		LCD_SetXY(0,0);
			LCD_WriteString("route8 done");
			
		}
		
		OSTimeDly(1);
	 }
}