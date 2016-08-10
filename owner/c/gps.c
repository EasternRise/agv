#include "all.h"
//Gps_State结构体个数

Gps_State Gps_List[GPS_STATCOUNT];
Pointfp64 Gps_Zero={0,0};
Pointfp64 Gps_assume;//利用摄像头或者激光等校正时试验用的变量

Pointfp64  	position_forward={0};//前轮坐标

Pointfp64  	position_center={0};//前中心坐标

int16_t     relative_start=0;
Pointfp64   position_relative={0};
fp64        angle_relative=0;

	fp64 x_deta;
	fp64 y_deta;
	fp64 theta_deta;
	fp64 R;
	fp64 L;
float disSpeed=0;
int   disCount=0;

void GPS_Update(void)
{
 	fp64 radian=0;


	Gps_List[1]=Gps_List[0];
	
	Gps_List[0].distance1 = Encoders[0].Distance;
	Gps_List[0].distance2 = Encoders[1].Distance;
    Gps_List[0].GyroAngle = Gyro_Angle_Total;
	
 	Gps_List[0].distancestep1 = Gps_List[0].distance1 - Gps_List[1].distance1;
 	Gps_List[0].distancestep2 = Gps_List[0].distance2 - Gps_List[1].distance2; 
	
    //speed
    disSpeed+=(Gps_List[0].distancestep2+Gps_List[0].distancestep1)/2;
    disCount++;
    if(disCount>=10)
    {
        disCount=0;
        
        Gps_List[0].speed=disSpeed/10.0;	//	m/s
        Gps_List[0].acceleration=(Gps_List[0].speed-Gps_List[1].speed)/1*1000;	        // m/s^2
        disSpeed=0;
    }
    
    
	/**********************************
	*	* 2015/12/21了
	*	* TongMin
	*	* GPS算法，参考电子科技大学算法
	*	* GPS约定，车体正方向为Y轴正方向，Y轴正方向角度为0，顺时针为正
	*	* 注意：角度换算为笛卡尔坐标系时应为 PI/2-radian
	*	* d1为左轮，d2为右轮
	
	************************************/

	
//		陀螺仪加码盘
	{
        theta_deta=Gps_List[0].GyroAngle-Gps_List[1].GyroAngle;
        
		Gps_List[0].angle += theta_deta;//角度范围为-180~180        
        ////角度范围限制为-180~180        
        Gps_List[0].angle =Gps_List[0].angle -(int)(Gps_List[0].angle /360)*360;
        if(Gps_List[0].angle >180.0)
            Gps_List[0].angle -=360.0;
        if(Gps_List[0].angle <-180.0)
            Gps_List[0].angle +=360.0;
        
        Gps_List[0].radian=Gps_List[0].angle*PI/180;
        
		//取上一时刻与现在时刻的角平分线做为该时间内车身的角度，前两个判断是为了解决在PI分界线处的突变问题
		radian=(Gps_List[0].radian+Gps_List[1].radian)/2;
		if(radian>PI)
			radian-=2*PI;
		if(radian<-PI)
			radian+=2*PI;
        
		L=(Gps_List[0].distancestep1+Gps_List[0].distancestep2)/2;
        
		x_deta=L*cos(PI/2-radian);
		y_deta=L*sin(PI/2-radian);
		
		Gps_List[0].position.y+=y_deta;
		Gps_List[0].position.x+=x_deta;
        
        if(relative_start)
        {
            position_relative.x += x_deta;
            position_relative.y += y_deta;
            angle_relative += theta_deta;
        }
	}
	

	position_forward.x=Gps_List[0].position.x+CARLENGTH*cos(PI/2-Gps_List[0].radian);
	position_forward.y=Gps_List[0].position.y+CARLENGTH*sin(PI/2-Gps_List[0].radian);
 	position_center.x=Gps_List[0].position.x+CARLENGTH/2*cos(PI/2-Gps_List[0].radian);
    position_center.y=Gps_List[0].position.y+CARLENGTH/2*sin(PI/2-Gps_List[0].radian);
}

void GPS_Clear(void)
{
	GPS_Init(Gps_Zero,0);
}
void GPS_Init(Pointfp64 position,fp64 radian)
{
	int i;
    //GPS使用码盘与陀螺仪的增量，它们的绝对量不影响GPS数据 所以将数据清零
	Encoder_Clear(0);
	Encoder_Clear(1);
    Gyro_Clear();
    
	for(i=0;i<GPS_STATCOUNT;i++)
	{
        Gps_List[i].position=position;
		Gps_List[i].angle=radian/PI*180.0;
        
        Gps_List[i].radian=radian;
        Gps_List[i].speed=0.0;
		Gps_List[i].acceleration=0.0;
        
		Gps_List[i].distance1=0.0;
		Gps_List[i].distance2=0.0;
        Gps_List[i].GyroAngle=0.0;
        
		Gps_List[i].distancestep1=0.0;
		Gps_List[i].distancestep2=0.0;	
	}
}

//		双码盘
//	{	
	/**********************************
	*	* 2015/12/21了
	*	* TongMin
	*	* GPS算法，参考电子科技大学算法
	*	* GPS约定，车体正方向为Y轴正方向，Y轴正方向角度为0，顺时针为正
	*	* 注意：角度换算为笛卡尔坐标系时应为 PI/2-radian
	*	* d1为左轮，d2为右轮
	
	************************************/
// 		if(Gps_List[0].distancestep1 == Gps_List[0].distancestep2 )
// 		{
// 			theta_deta=0;
// 			x_deta=(Gps_List[0].distancestep1+Gps_List[0].distancestep2)/2*cos(PI/2-Gps_List[0].radian);			
// 			y_deta=(Gps_List[0].distancestep1+Gps_List[0].distancestep2)/2*sin(PI/2-Gps_List[0].radian);
// 		}
// 		else
// 		{
// 			theta_deta=(Gps_List[0].distancestep1-Gps_List[0].distancestep2)/carwidth;
// 			R= (Gps_List[0].distancestep1+Gps_List[0].distancestep2)/(2*theta_deta) ; 
// 			L=sin(theta_deta/2)*R*2;
// 			
// // 			x_deta=cos(PI/2-Gps_List[0].radian-theta_deta/2)*L;
// 			x_deta=R*cos(-Gps_List[0].radian)-R*cos(- (Gps_List[0].radian+theta_deta) );
// 			y_deta=sin(PI/2-Gps_List[0].radian-theta_deta/2)*L;
// 		}
// 	
//  		Gps_List[0].radian+=theta_deta;
// 		if(Gps_List[0].radian>2*PI)
// 			Gps_List[0].radian-=2*PI;
// 		if(Gps_List[0].radian<-2*PI)
// 			Gps_List[0].radian+=2*PI;
// 		Gps_List[0].angle=180*Gps_List[0].radian/PI;
// 		
// 		Gps_List[0].position.y+=y_deta;
// 		Gps_List[0].position.x+=x_deta;
// 	}
