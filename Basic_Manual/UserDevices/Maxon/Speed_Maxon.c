#include "all.h"
#ifndef WheelSelectSetting
////////////////////////四全向轮////////////////////////////////////////////////////////////////////////


 fp32  Speed_F =0;                      //存储前面左轮的速度值r/min
 
 fp32  Speed_FR=0;                      //存储前右轮的速度值
 
 fp32  Speed_BL=0;                     //存储后面左轮的速度值
 
 fp32  Speed_BR=0;                     //存储后面右轮的速度值
 
 fp64   MaxAcc=50.0;                  //存储最大加速度值
	

////////////////////////////////轮子前进直线行走校正系数////////////////////////////////////////////////////


fp64  Motor_FL_Argument_Ford = 1.0;

fp64  Motor_FR_Argument_Ford = 1.0;

fp64  Motor_BL_Argument_Ford = 1.0;

fp64  Motor_BR_Argument_Ford = 1.0;


////////////////////////////////轮子后退直线行走校正系数////////////////////////////////////////////////////


fp64  Motor_FL_Argument_Back = 1.0;

fp64  Motor_FR_Argument_Back = 1.0;

fp64  Motor_BL_Argument_Back = 1.0;

fp64  Motor_BR_Argument_Back = 1.0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////


//输入参数为实际想要的转速r/min，不考虑减速比，减速比的问题在本函数里解决
void Speed_Motor(fp64 speed_FL,fp64 speed_FR,fp64 speed_BL,fp64 speed_BR)
{


    //fp64  speed_max;                        //电机的最大转速r/min

 	fp64  speed_fl_delta;                   //存储前左轮的速度值，浮点型
 	fp64  speed_fr_delta;                   //存储前右轮的速度值，浮点型
 	fp64  speed_bl_delta;                   //存储后左轮的速度值，浮点型
 	fp64  speed_br_delta;                   //存储后右轮的速度值，浮点型
 
 	fp64  speed_delta_max;                  //存储速度变化的最大值
 	fp64  Max_Acc=MaxAcc;             		//存储最大加速度值
 
 	fp64  PWM_Max;							//存储四个PWM值中最大的一个
 
 	static fp64     Speed_FL_Now = 0;      	//存储前左轮本次的目标速度
 	static fp64     Speed_FR_Now = 0;      	//存储前右轮本次的目标速度
 	static fp64     Speed_BL_Now = 0;       //存储后左轮本次的目标速度
 	static fp64     Speed_BR_Now = 0;       //存储后右轮本次的目标速度
 
 	static fp64     Speed_FL_Last = 0;    	//存储前左轮上次的目标速度
 	static fp64     Speed_FR_Last = 0;    	//存储前右轮上次的目标速度
 	static fp64     Speed_BL_Last = 0;      //存储后左轮上次的目标速度
 	static fp64     Speed_BR_Last = 0;      //存储后右轮上次的目标速度
 
 	static fp64 	PWM_FL = 0.5;			//存储前左轮的占空比
 	static fp64 	PWM_FR = 0.5;			//存储前右轮的占空比
 	//static fp64		MAXON_BL = 0;			//存储后左轮的转速
 	//static fp64		MAXON_BR = 0;			//存储后右轮的转速
 
 
 /***************************************对最大加速度进行限制*****************************************/
 /****************************************************************************************************/
 
 
	Speed_FL_Now = speed_FL;
	Speed_FR_Now = speed_FR;
   	Speed_BL_Now = speed_BL;
    Speed_BR_Now = speed_BR;
 
    //速度增量
 	speed_fl_delta = Speed_FL_Now-Speed_FL_Last;
 	speed_fr_delta = Speed_FR_Now-Speed_FR_Last;
 	speed_bl_delta = Speed_BL_Now-Speed_BL_Last;
 	speed_br_delta = Speed_BR_Now-Speed_BR_Last;
 
    //速度增量最大值
 	speed_delta_max=Max(Fp64Abs(speed_fl_delta),Fp64Abs(speed_fr_delta),Fp64Abs(speed_bl_delta),Fp64Abs(speed_br_delta));

    //限制最大加速度，*_Last为将要赋给电机的值
    if((speed_delta_max >= Max_Acc) && (Acc_Limit_enable==1))
    {
     	Speed_FL_Last += Max_Acc*(speed_fl_delta/speed_delta_max);
     	Speed_FR_Last += Max_Acc*(speed_fr_delta/speed_delta_max);
        Speed_BL_Last += Max_Acc*(speed_bl_delta/speed_delta_max);
        Speed_BR_Last += Max_Acc*(speed_br_delta/speed_delta_max);
    }
  	else if((speed_delta_max <= -Max_Acc) && (Acc_Limit_enable==1))
   	{
     	Speed_FL_Last -= Max_Acc*(speed_fl_delta/speed_delta_max);
     	Speed_FR_Last -= Max_Acc*(speed_fr_delta/speed_delta_max);
        Speed_BL_Last -= Max_Acc*(speed_bl_delta/speed_delta_max);
        Speed_BR_Last -= Max_Acc*(speed_br_delta/speed_delta_max);
 	}
	else
 	{
		Speed_FL_Last = Speed_FL_Now;
		Speed_FR_Last = Speed_FR_Now;
       	Speed_BL_Last = Speed_BL_Now;
       	Speed_BR_Last = Speed_BR_Now;
    }

 /*****************************************电机转向矫正*******************************************/
 /****************************************************************************************************/

	speed_FL = Speed_FL_Last * MotorF_Argument;
	speed_FR = Speed_FR_Last * MotorFR_Argument;
  	speed_BL = Speed_BL_Last * MotorBL_Argument;
 	speed_BR = Speed_BR_Last * MotorBR_Argument;


 /*****************************************转速转换为占空比*******************************************/
 
 //PWM=((50/speed_max)*speed*转速比+50)/100
 
 /****************************************************************************************************/
 
 

 	if(speed_FL<(-2374.5/28) || speed_FL>(2404.5/28))
 		PWM_FL = (0.006269*14*speed_FL+49.93)/100;
 	else
 	{
 		if(speed_FL>0)
 			PWM_FL = (0.006186*14*speed_FL+50)/100;
 		else
 			PWM_FL = (0.006235*14*speed_FL+49.76)/100;
 	}
 		//PWM_FL = (0.006235*14*speed_FL+49.98)/100;
 	if(speed_FR<(-2374.5/28) || speed_FR>(2404.5/28))
 		PWM_FR = (0.006269*14*speed_FR+49.93)/100;
 	else
 	{
 		if(speed_FR>0)
 			PWM_FR = (0.006186*14*speed_FR+50)/100;
 		else
 			PWM_FR = (0.006235*14*speed_FR+49.76)/100;
 	}
 		//PWM_FR = (0.006235*14*speed_FR+49.98)/100;
  	
 /****************************************对速度进行最终限制******************************************/
 /****************************************************************************************************/

	if(speed_FL<=0)
		PWM_FL=PWM_FL*Motor_FL_Argument_Back;
	else
		PWM_FL=PWM_FL*Motor_FL_Argument_Ford;

	if(speed_FR<=0)
		PWM_FR=PWM_FR*Motor_FR_Argument_Back;
	else
		PWM_FR=PWM_FR*Motor_FR_Argument_Ford;
		
	if(speed_BL<=0)
		speed_BL=speed_BL*Motor_BL_Argument_Back;
	else
		speed_BL=speed_BL*Motor_BL_Argument_Ford;

	if(speed_BR<=0)
		speed_BR=speed_BR*Motor_BR_Argument_Back;
	else
		speed_BR=speed_BR*Motor_BR_Argument_Ford;


	PWM_Max=max(Fp64Abs(PWM_FL-0.5),Fp64Abs(PWM_FR-0.5));
	
	if(PWM_Max>=0.48)
	{
		PWM_FL = 0.48*(PWM_FL-0.5) / PWM_Max + 0.5; 
		PWM_FR = 0.48*(PWM_FR-0.5) / PWM_Max + 0.5; 
	}
        
 	SetPWM( Motor_F, PWM_FL);
 	SetPWM( Motor_FR, PWM_FR);
 	
 	Maxon1SetSpeed(speed_BL*26);
 	Maxon2SetSpeed(speed_BR*26);
	
}   
#endif