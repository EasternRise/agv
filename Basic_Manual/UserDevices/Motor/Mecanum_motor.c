#include "all.h"

//加速度限制使能位
uint8 Acc_Limit_enable=0;

//warning:以下变量的初始值从nandflash中取得///////////////////////////
//四轮的开环系数

 fp64  MotorUP_Argument=-1;
 fp64  MotorF_Argument =-1; 
 fp64  MotorFR_Argument=1;
 fp64  MotorBL_Argument=1;
 fp64  MotorBR_Argument= 1; 

//四轮对应的PWM口
int Motor_F=0 ;      //同时是四轮车的FL
int Motor_FR=1 ;
int Motor_BL=2 ;
int Motor_BR=3 ;

int Motor_UP=1;
//////////////////////////////////////////////////////////////////////


void Mecanum_NandInit()
{
	uint8 data[2048];
	
	ReadPage(MECANUM_MOTOR_NANDPAGE,data);
	
	
	MotorF_Argument=*(fp64*)(data);

    MotorBL_Argument=*(fp64*)(data+8);

    MotorBR_Argument=*(fp64*)(data+16);
	
    MotorFR_Argument=*(fp64*)(data+24);
	
	Motor_F=*(fp64*)(data+48);
	
	Motor_FR=*(fp64*)(data+56);
	
    Motor_BL=*(fp64*)(data+64);
    
    Motor_BR=*(fp64*)(data+80);
    
}


void Mecanum_NandSave()
{
	uint8 data[2048];
	
	*(fp64*)(data)=MotorF_Argument;

    *(fp64*)(data+8)=MotorBL_Argument;

    *(fp64*)(data+16)=MotorBR_Argument;
    
    *(fp64*)(data+24)=MotorFR_Argument;
   
	
	*(fp64*)(data+48)=Motor_F;
	
	*(fp64*)(data+56)=Motor_FR;
    
    *(fp64*)(data+64)=Motor_BL;
    
    *(fp64*)(data+80)=Motor_BR;
  
	
	WritePartBlock(MECANUM_MOTOR_NANDPAGE,data);
}


//四轮调试函数
void Mecanum_SpeedMode(int port,fp64 con)
{
   	uint8 err;
	uint8 *msg;
	fp64 speed;
	fp64 delta=0;
    
	speed=0.5;
	SetPWM(port,speed);
	LCD_Clear();
	
	for(;;)
	{
		OSTimeDly(5);
		LCD_SetXY(0,0);
		LCD_Printf("Speed:");
		LCD_WriteNum(speed);
		LCD_SetXY(0,1);
		LCD_Printf("1.SpeedUp           ");
		LCD_SetXY(0,2);
		LCD_Printf("2.SpeedDown         ");
		LCD_SetXY(0,3);
		LCD_Printf("3.Stop              ");
		
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				if(speed<1)
					delta+=0.0001;
					speed=0.5+delta;
				SetPWM(port,0.5+delta*con);
			break;
			case 2:
				if(speed>0)
					delta-=0.0001;
					speed=0.5+delta;
				SetPWM(port,0.5+delta*con);
			break;
			case 3:
				SetPWM(port,0.5);
			break;
			case 5:
				SetPWM(port,0.5);
				return;
			break;
			case 15:
				SetPWM(port,0.5);
				return;
			break;
			case 16:
				SetPWM(port,0.5);
				return;
			break;
			case 24:
				SetPWM(port,0.5);
				return;
			break;	
		}
	}
}

void Steer_SpeedMode(int port)
{
	
	uint8 *msg;
	uint8 err;
	uint32 speed=0;
	LCD_Clear();
	
	for(;;)
	{
		OSTimeDly(5);
		LCD_SetXY(0,0);
		LCD_Printf("Speed:%d   ",speed);
		LCD_SetXY(0,1);
		LCD_Printf("1.SpeedUp           ");
		LCD_SetXY(0,2);
		LCD_Printf("2.SpeedDown         ");
		
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				if(speed<1000)
					speed+=1;
				FPGA_SetPWM(port,1,50,speed);
			break;
			case 2:
				if(speed>0)
					speed-=1;
				FPGA_SetPWM(port,1,50,speed);
			break;
		}
	}
}