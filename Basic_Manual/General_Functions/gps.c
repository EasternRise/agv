#include "all.h"
//Gps_State结构体个数
#define GPS_STATCOUNT	4

struct Gps_State Gps_List[GPS_STATCOUNT];
struct Pointfp64 Gps_Zero={0,0};
fp64 Gps_Speed;
int  PRE_Count1;
int  PRE_Count2;
int  PRE_Count3;

fp64 error_convert[20]={0};


extern fp64 ErrorDistance;

//全场信息刷新函数，即读取全体传感器值，通过定时器定时中断每1ms刷新一次
void Expection_FLS( void )
{
	time_flag++;
	GYRO_Update();
	Encoder_Update();
	GpsUpdate();
}
 
//GPS初始化函数
void GPS_Init(struct Pointfp64 position,fp64 angle)
{
	int i;
	OS_CPU_SR  cpu_sr;
	
	OS_ENTER_CRITICAL();
	ENCODER_Clear(0);
	ENCODER_Clear(1);
	for(i=0;i<GPS_STATCOUNT;i++)
	{
		Gps_List[i].distance1=0.0;
		Gps_List[i].distance2=0.0;
		Gps_List[i].distance=0.0;
		Gps_List[i].distancestep1=0.0;
		Gps_List[i].distancestep2=0.0;
		Gps_List[i].position=position;
		Gps_List[i].angle=angle;
		Gps_List[i].anglestep=0.0;
		Gps_List[i].radian=angle/180.0*PI;
		Gps_List[i].speed=0.0;
		Gps_List[i].acceleration=0.0;
		Gyro_Total=angle/0.0000001/Gyro_Convert1;
	}
	OS_EXIT_CRITICAL();
}

//GPS清零函数
void GPS_Clear(void)
{
	Gyro_Total=0;
	Gyro_Last=Gyro_Now;
	GPS_Init(Gps_Zero,0);
	
	ENCODER_Clear(0);
	ENCODER_Clear(1);
	
}

//显示当前的GPS
void GPS_Display(void)
{
	LCD_SetXY(0,0);
	LCD_WriteString("A:");
	LCD_WriteDouble(Gps_List[0].angle,4);
	LCD_SetXY(0,1);
	LCD_WriteString("S:");
	LCD_WriteNum(Gps_List[0].distance);
	LCD_SetXY(0,2);  
	LCD_WriteString("X:");
	LCD_WriteDouble(Gps_List[0].position.x,3);
	LCD_SetXY(0,3);
	LCD_WriteString("Y:");
	LCD_WriteDouble(Gps_List[0].position.y,3);	
}

//GPS更新
void GpsUpdate(void)
{
	fp64 radian;
	fp64 radianstep = 0.0;						//弧度步进值
	int64 digital_angle;						//角度的数字量
	//fp64 xx;
	//fp64 yy;

	//测速变量
	static int count=0;
	static fp64 gps_length;
	static struct Pointfp64 gps_last;
	static struct Pointfp64 gps_now;
    
	if(Gyro_Roll==0)
		Gyro_Roll=1;
	digital_angle=Gyro_Total%Gyro_Roll;//Gyro_Roll是转一圈时对应的陀螺仪的原始值
	if(digital_angle<0)
		digital_angle+=Gyro_Roll;
		
	Gps_List[1]=Gps_List[0];
	

	Gps_List[0].angle=digital_angle*0.0000001*Gyro_Convert1;
	
	//把角度约束到-180~180
	while(Gps_List[0].angle>=180)
		Gps_List[0].angle-=360;
	
	while(Gps_List[0].angle<-180)
		Gps_List[0].angle+=360;
	
	//转换为弧度值并限制在-PI~PI
	Gps_List[0].radian=Gps_List[0].angle*PI/180.0;
	if( Gps_List[0].radian >= PI )
		Gps_List[0].radian = Gps_List[0].radian - 2*PI;
	if( Gps_List[0].radian < -PI )
		Gps_List[0].radian = Gps_List[0].radian + 2*PI;

    //取上一时刻与现在时刻的角平分线做为该时间内车身的角度，前两个判断是为了解决在PI分界线处的突变问题
	if(Gps_List[1].radian<-1&&Gps_List[0].radian>1)
	{
		radian=PI-(Gps_List[0].radian-Gps_List[1].radian)/2;
		radian=Gps_List[0].radian+radian;
	}
	else if(Gps_List[1].radian>1&&Gps_List[0].radian<-1)
	{
		radian=PI+(Gps_List[0].radian-Gps_List[1].radian)/2;
		radian=Gps_List[1].radian+radian;
	}
	else
	{
		radian=((Gps_List[1].radian+Gps_List[0].radian)/2.0);
	}
	
	//直接取和，对2*PI取余，再除以2
	//radian=(((Gps_List[1].radian+Gps_List[0].radian)%(2*PI))/2.0);
	
	
    //获得弧度步进值
	if(Gps_List[1].radian<-1&&Gps_List[0].radian>1)
	{
		radianstep =-2*PI+Gps_List[0].radian-Gps_List[1].radian;
	}
	else if(Gps_List[1].radian>1&&Gps_List[0].radian<-1)
	{
		radianstep =2*PI+Gps_List[0].radian-Gps_List[1].radian;
	}
	else
	{
		radianstep = Gps_List[0].radian-Gps_List[1].radian;
	}
	
	
	
		
	Gps_List[0].distance1 = Encoders[0].Distance;
	//Gps_List[0].distance2 = Encoders[1].Distance;
	
	Gps_List[0].distancestep1 = Gps_List[0].distance1 - Gps_List[1].distance1;
	//Gps_List[0].distancestep2 = Gps_List[0].distance2 - Gps_List[1].distance2; 
	
	//xx=Gps_List[0].distancestep1/radianstep*(1-cos(radianstep));
	//yy=Gps_List[0].distancestep1/radianstep*sin(radianstep);
	
	//Gps_List[0].position.x+=xx*cos(radian)-yy*sin(radian);
	//Gps_List[0].position.y+=xx*sin(radian)+yy*cos(radian);
	
	Gps_List[0].distancestep1-=Ac1*radian;
	
	Gps_List[0].position.x-= Gps_List[0].distancestep1*sin(radian);
	Gps_List[0].position.y+= Gps_List[0].distancestep1*cos(radian);
	
	
	if(count==0)
	{
		gps_last=Gps_List[0].position;
	}
	if(count==9)
	{
		gps_now=Gps_List[0].position;
		gps_length=MATH_Distance(gps_last,gps_now);
		Gps_Speed=gps_length*100;
		count=-1;
	}
	count++;
	
	PRE_Count1=(int)(PointRoute[Point_NowNum].speed_point*94.875/2000-15);
	PRE_Count2=(int)(PointRoute[Point_NowNum].speed_point*95/2000+25);
	PRE_Count3=(int)(PointRoute[Point_NowNum].speed_point*90.8889/2000);
}

void GPSModulation(fp64 d)
{
   fp64 r=1500;
   fp64 xx,yy;
   fp64 theta=-PI/4-Gps_List[0].radian;
   xx=(r+d)*cos(theta);
   yy=(r+d)*sin(theta)+12000/1.414;
   Gps_List[0].position.x=xx/1.414-yy/1.414 ;
   Gps_List[0].position.y=xx/1.414+yy/1.414 ;
 }