#include "all.h"

fp64 ErrorDistance; 
fp32 speed_rotate=0.0;
uint8 PID_flag=1;
 

 
void SetPointPath(int16 point_startnum, int16 point_endnum,fp32 aimangle, struct Pointfp64 aim_point)
{
	RouteForm=PointLine;
	RouteFinish=1;
	
	Start_angle=Gps_List[0].radian;
	Point_NowNum=point_startnum;
	Point_EndNum=point_endnum;

	Aim_Angle=aimangle;
	AimPoint=aim_point;

	PID_Clear();
	//OSTimeDly(1);
} 
 
void GoPointPath(void)
{	
	uint16	i;

	fp64	error_dis;
	fp64    error_angle;
	uint16	count_delta=1;
	
	fp64	speed;
		

	
	//////////////搜索下一目标点/////////////////
	
	for(i=0;i<Count_Step;i++)
	{
             
		if((Point_NowNum+i)>=(Point_EndNum-1))
		{
			break;
		}
        
		if((Getlength(PointRoute[Point_NowNum+i+1].position,Gps_List[0].position))
		<=(Getlength(PointRoute[Point_NowNum+i].position,Gps_List[0].position)))
        
		{
			count_delta=i+1;
		}
    
	}
	
	if((Point_NowNum+count_delta)<=(Point_EndNum)&&count_delta!=0)
	{
		if(Getlength(PointRoute[Point_NowNum+count_delta+1].position,Gps_List[0].position)
	 	 <=Getlength(PointRoute[Point_NowNum+count_delta-1].position,Gps_List[0].position))
		{
			Point_NowNum=Point_NowNum+count_delta;
		}
		else
			Point_NowNum=Point_NowNum+count_delta-1;
	}

	if (WalkGroundEnable)
		speed=PointRoute[Point_NowNum].speed_point/3;
	else
		speed=PointRoute[Point_NowNum].speed_point;
	

	//error_angle = PI/2- PointRoute[Point_NowNum].lineangle-Gps_List[0].radian;//Start_Angle;
	error_angle = PointRoute[Point_NowNum].lineangle-Gps_List[0].radian-PI/2;//Start_Angle;
	
	if( error_angle > PI )
		error_angle = error_angle - 2*PI;
	if( error_angle < -PI )
		error_angle = error_angle + 2*PI;
			
    error_dis =   ( Gps_List[0].position.x - PointRoute[Point_NowNum].position.x ) * sin( PointRoute[Point_NowNum].lineangle) //位移误差
			    - ( Gps_List[0].position.y - PointRoute[Point_NowNum].position.y ) * cos( PointRoute[Point_NowNum].lineangle);
	
	
	Vout[1]=DistancePID(error_dis,Pid_List[6].PROP,Pid_List[6].Ti,Pid_List[6].Td);
    Vout[0]=1000*AnglePID(error_angle,Pid_List[7].PROP,Pid_List[7].Ti,Pid_List[7].Td);
    
    if(Vout[0]>300)
        Vout[0]=300;
   	if(Vout[0]<-300)
    	Vout[0]=-300;
	
    
    Speed_F=0;
    Speed_FR=0;

    
    Speed_BL=60/PI/210*(speed+Vout[1])-60/PI/210*320*((speed/PointRoute[Point_NowNum].radius)+Vout[0]);
    Speed_BR=60/PI/210*(speed+Vout[1])+60/PI/210*320*((speed/PointRoute[Point_NowNum].radius)+Vout[0]);

    //Speed_BL=60/PI/210*(speed+Vout[1])+60/PI/210*(PointRoute[Point_NowNum].radius-320)*((speed/PointRoute[Point_NowNum].radius)+Vout[0]);
    //Speed_BR=60/PI/210*(speed+Vout[1])+60/PI/210*(PointRoute[Point_NowNum].radius+320)*((speed/PointRoute[Point_NowNum].radius)+Vout[0]);

    //Speed_BL=(PointRoute[Point_NowNum].radius-320)*(speed+Vout[1])/PointRoute[Point_NowNum].radius*60/PI/210;
    //Speed_BR=(PointRoute[Point_NowNum].radius+320)*(speed+Vout[1])/PointRoute[Point_NowNum].radius*60/PI/210;
    
    
    
    
    Data[4]=error_angle;
	Data[6]=error_dis;
	
	SaveMovingData();
	
	
	
	
	LCD_Clear();
	
	LCD_SetXY(0,0);
	LCD_WriteString("NowNum:");
	LCD_WriteNum(Point_NowNum);
	
	LCD_SetXY(0,1);
	LCD_WriteString("error_angle:");
	LCD_WriteNum(error_angle);
	
	LCD_SetXY(0,2);
	LCD_WriteString("Vout[0]:");
	LCD_WriteNum(Vout[0]);
	
	LCD_SetXY(0,3);
	LCD_WriteString("error_dis:");
	LCD_WriteNum(error_dis);
	
	LCD_SetXY(0,4);
	LCD_WriteString("Vout[1]:");
	LCD_WriteNum(Vout[1]);
	
	LCD_SetXY(0,5);
	LCD_WriteString("Angle:");
	LCD_WriteNum(Gps_List[0].angle);
	
	LCD_SetXY(0,6);
	LCD_WriteString("X:");
	LCD_WriteNum(Gps_List[0].position.x);
	
	LCD_SetXY(0,7);
	LCD_WriteString("Y:");
	LCD_WriteNum(Gps_List[0].position.y);
	

	
	
}	

void PointPathtest(void)
{ 
	//int err;
	
	struct Value Values[]=
     {   
        {(void*)&Pid_List[6].PROP,	TYPE_FP64,	"DP"}, 
        {(void*)&Pid_List[6].Ti,	TYPE_FP64,	"DI"}, 
        {(void*)&Pid_List[6].Td,	TYPE_FP64,	"DD"},
        {(void*)&Pid_List[7].PROP,	TYPE_FP64,	"AP"}, 
        {(void*)&Pid_List[7].Ti,	TYPE_FP64,	"AI"}, 
        {(void*)&Pid_List[7].Td,	TYPE_FP64,	"AD"},

      };
	DISPLAY_ValuesManager(Values,6,PID_NANDPAGE);
	Route_Num=0;
	LCD_Clear();
	GPS_Clear();
	PID_Clear();
	
	RouteForm=RouteStop;
	
	/******************************Route[1]******************************************************************/
	Route_Num++;
	OSMboxPost(AutoMbox,&Route_Num);
	OSMboxPost(ArmMbox,&Route_Num);
	LCD_SetXY(0,6);
	LCD_WriteNum(Route_Num);
	while(RouteFinish) OSTimeDly(1);
	
	/*LCD_SetXY(0,7);
	LCD_WriteString("Press Any Key.");
	PressAnyKey();*/
	
	/******************************Route[2]******************************************************************/
	Route_Num++;
	OSMboxPost(AutoMbox,&Route_Num);
	OSMboxPost(ArmMbox,&Route_Num);
	
	LCD_WriteString("Press Any Key.");
	PressAnyKey();
	 /******************************Route[3]******************************************************************/
	 
	//SetPointPath(Route[2]+1,Route[3],PointRoute[Route[3]].selfangle,PointRoute[Route[3]].position);
	
	while(RouteFinish) 
	 {
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;
	 		RouteForm=RouteStop;
	 		LCD_SetXY(0,2);
			LCD_WriteString("route3 done");
			
		}
		
		OSTimeDly(1);
	 }
	 PressAnyKey();
	 /******************************Route[4]******************************************************************/
	 
//	SetPointPath(Route[3]+1,Route[4],PointRoute[Route[4]].selfangle,PointRoute[Route[4]].position);
	
	while(RouteFinish) 
	 {
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;
	 		RouteForm=RouteStop;
	 		LCD_SetXY(0,2);
			LCD_WriteString("route4 done");
			
		}
		
		OSTimeDly(1);
	 }
	 PressAnyKey();
	/******************************Route[5]******************************************************************/
	 
//	SetPointPath(Route[4]+1,Route[5],PointRoute[Route[5]].selfangle,PointRoute[Route[5]].position);
	
	while(RouteFinish) 
	 {
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;
	 		RouteForm=RouteStop;
	 		LCD_SetXY(0,2);
			LCD_WriteString("route5 done");
			
		}
		
		OSTimeDly(1);
	 }
	 PressAnyKey();	 
	 
	 /******************************Route[6]******************************************************************/
//	SetPointPath(Route[5]+1,Route[6],PointRoute[Route[6]].selfangle,PointRoute[Route[6]].position);
	
	
	while(RouteFinish) 
	 {
		if(Point_NowNum>=Point_EndNum-1)
	 	{
	 		RouteFinish=0;
	 		RouteForm=RouteStop;
	 		LCD_SetXY(0,2);
			LCD_WriteString("route6 done");
			
		}
		
		OSTimeDly(1);
	 }
}