#ifndef _GPS_H
#define _GPS_H
  #include "stm32f10x.h"
  #include "MyTypeDef.h"

#define GPS_STATCOUNT	2


// #define CARWIDTH 246.5 
// #define CARLENGTH 301

#define CARWIDTH 202 
#define CARLENGTH 300

typedef struct 
{   
	Pointfp64  	                position;	  	    //当前坐标	
    double  	        		angle;        	    //当前角度 		
	double				        radian;			    //当前弧度

	double			            distance1;	  	    //码盘1前进距离
	double			        	distance2;		    //码盘2前进距离
    double                      GyroAngle;          //从陀螺仪得到的角度
    
    double				        distancestep1; 	    //码盘1本次前进距离
	double				        distancestep2;      //码盘2本次前进距离
	
	float				        speed;			    //当前速度
	float				        acceleration;	    //当前加速度
}Gps_State;

extern	Pointfp64  	position_forward;//前轮坐标
extern	Pointfp64  	position_center;//前轮坐标

extern Gps_State Gps_List[GPS_STATCOUNT];
extern Pointfp64 Gps_assume;//利用摄像头或者激光等校正时试验用的变量

void GPS_Update(void);
void GPS_Clear(void);
void GPS_Init(Pointfp64 position,double radian);



 
#endif
