#include "all.h"
#ifdef WheelSelectSetting
////////////////////////三全向轮////////////////////////////////////////////////////////////////////////


 int16  Speed_F;                      //存储前面轮的速度值
 
 int16  Speed_BL;                     //存储后面左轮的速度值
 
 int16  Speed_BR;                     //存储后面右轮的速度值
 
 fp64   MaxAcc=30.0;                  //存储最大加速度值
	

////////////////////////////////轮子前进直线行走校正系数////////////////////////////////////////////////////


fp64  Motor_F_Argument_Ford	 = 1.0;

fp64  Motor_BL_Argument_Ford = 1.0;

fp64  Motor_BR_Argument_Ford = 1.0;


////////////////////////////////轮子后退直线行走校正系数////////////////////////////////////////////////////


fp64  Motor_F_Argument_Back	 = 1.0;

fp64  Motor_BL_Argument_Back = 1.0;

fp64  Motor_BR_Argument_Back = 1.0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Speed_Motor(fp64 speed_BL,fp64 speed_BR,fp64 speed_F)
{


 	fp64  speed_f_delta;                    //存储前轮的速度值，浮点型
 	fp64  speed_bl_delta;                   //存储后左轮的速度值，浮点型
 	fp64  speed_br_delta;                   //存储后右轮的速度值，浮点型
 
 	fp64  speed_delta_max;                  //存储速度变化的最大值
 	fp64  Max_Acc_Three=MaxAcc;             //存储最大加速度值
 
 	fp64  PWM_Max;							//存储三个PWM值中最大的一个
 	
 
 	static fp64     Speed_F_Now =0;      	//存储前轮本次的目标速度
 	static fp64     Speed_BL_Now=0;       	//存储后左轮本次的目标速度
 	static fp64     Speed_BR_Now=0;       	//存储后右轮本次的目标速度
 
 	static fp64     Speed_F_Last =0;    	//存储前轮上次的目标速度
 	static fp64     Speed_BL_Last=0;      	//存储后左轮上次的目标速度
 	static fp64     Speed_BR_Last=0;      	//存储后右轮上次的目标速度
 
 	static fp64 	PWM_F= 0.5;				//存储前轮的占空比
 	static fp64		PWM_BL=0.5;				//存储后左轮的占空比
 	static fp64		PWM_BR=0.5;				//存储后右轮的占空比
 
 
 /***************************************对最大加速度进行限制*****************************************/
 /****************************************************************************************************/
 
 
	Speed_F_Now  = speed_F;
   	Speed_BL_Now = speed_BL;
    Speed_BR_Now = speed_BR;
 
 
 	speed_f_delta  = Speed_F_Now-Speed_F_Last;
 	speed_bl_delta = Speed_BL_Now-Speed_BL_Last;
 	speed_br_delta = Speed_BR_Now-Speed_BR_Last;
 
 
 	speed_delta_max=Max(Fp64Abs(speed_f_delta),Fp64Abs(speed_bl_delta),Fp64Abs(speed_br_delta),0);

    if((speed_delta_max >= Max_Acc_Three) && (Acc_Limit_enable==1))
    {
     	Speed_F_Last  += Max_Acc_Three*(speed_f_delta/speed_delta_max);
        Speed_BL_Last += Max_Acc_Three*(speed_bl_delta/speed_delta_max);
        Speed_BR_Last += Max_Acc_Three*(speed_br_delta/speed_delta_max);
    }
  	else if((speed_delta_max <= -Max_Acc_Three) && (Acc_Limit_enable==1))
   	{
     	Speed_F_Last  -= Max_Acc_Three*(speed_f_delta/speed_delta_max);
        Speed_BL_Last -= Max_Acc_Three*(speed_bl_delta/speed_delta_max);
        Speed_BR_Last -= Max_Acc_Three*(speed_br_delta/speed_delta_max);
 	}
	else
 	{
		Speed_F_Last  = Speed_F_Now;
       	Speed_BL_Last = Speed_BL_Now;
       	Speed_BR_Last = Speed_BR_Now;
    }
                                 

 /*****************************************转速转换为占空比*******************************************/
 /****************************************************************************************************/
 
	speed_F  = Speed_F_Last  * MotorF_Argument;
  	speed_BL = Speed_BL_Last * MotorBL_Argument;
 	speed_BR = Speed_BR_Last * MotorBR_Argument;
 	
  	 	
 	if(speed_F<(-2374.5/28) || speed_F>(2404.5/28))
 		PWM_F = (0.006269*14*speed_F+49.93)/100;
 	else
 	{
 		if(speed_F>0)
 			PWM_F = (0.006186*14*speed_F+50)/100;
 		else
 			PWM_F = (0.006235*14*speed_F+49.76)/100;
 	}
 		//PWM_F = (0.006235*14*speed_F+49.98)/100;
 	if(speed_BL<(-2374.5/28) || speed_BL>(2404.5/28))
 		PWM_BL = (0.006269*14*speed_BL+49.93)/100;
 	else
 	{
 		if(speed_BL>0)
 			PWM_BL = (0.006186*14*speed_BL+50)/100;
 		else
 			PWM_BL = (0.006235*14*speed_BL+49.76)/100;
 	}
 		//PWM_BL = (0.006235*14*speed_BL+49.98)/100;
 	if(speed_BR<(-2374.5/28) || speed_BR>(2404.5/28))
 		PWM_BR = (0.006269*14*speed_BR+49.93)/100;
 	else
 	{
 		if(speed_BR>0)
 			PWM_BR = (0.006186*14*speed_BR+50)/100;
 		else
 			PWM_BR = (0.006235*14*speed_BR+49.76)/100;
 	}
 		//PWM_BR = (0.006235*14*speed_BR+49.98)/100;
 	
 /****************************************对速度进行最终限制******************************************/
 /****************************************************************************************************/

	if(speed_F<=0)
		PWM_F=PWM_F*Motor_F_Argument_Back;
	else
		PWM_F=PWM_F*Motor_F_Argument_Ford;
		
	if(speed_BL<=0)
		PWM_BL=PWM_BL*Motor_BL_Argument_Back;
	else
		PWM_BL=PWM_BL*Motor_BL_Argument_Ford;

	if(speed_BR<=0)
		PWM_BR=PWM_BR*Motor_BR_Argument_Back;
	else
		PWM_BR=PWM_BR*Motor_BR_Argument_Ford;


	PWM_Max=Max(Fp64Abs(PWM_F-0.5),Fp64Abs(PWM_BL-0.5),Fp64Abs(PWM_BR-0.5),0);
	
	if(PWM_Max>=0.5)
	{
		PWM_F  = (PWM_F-0.5)  / PWM_Max + 0.5; 
		PWM_BL = (PWM_BL-0.5) / PWM_Max + 0.5;
		PWM_BR = (PWM_BR-0.5) / PWM_Max + 0.5;		
	}
   
 	SetPWM( Motor_F,  PWM_F);
 	SetPWM( Motor_BL, PWM_BL);
 	SetPWM( Motor_BR, PWM_BR);
	
	/*  LCD_SetXY(0,0);
    	LCD_WriteString("A:");
    	LCD_WriteNum(Gps_List[0].angle);
    	LCD_SetXY(10,0);
	    LCD_WriteString("X:");
	    LCD_WriteNum(Gps_List[0].position.x);
	    LCD_SetXY(0,1);
	    LCD_WriteString("Y:");
	    LCD_WriteNum(Gps_List[0].position.y);
	    LCD_SetXY(10,1);
	    LCD_WriteString("ed");
    	LCD_WriteNum(speed_B_BR);	
	    LCD_SetXY(0,2);
    	LCD_WriteString("F:");
    	LCD_WriteNum(PWM_F);
    	LCD_SetXY(10,2);
    	LCD_WriteString("BL:");
    	LCD_WriteNum(PWM_BL);
	    LCD_SetXY(0,3);
    	LCD_WriteString("BR:");
    	LCD_WriteNum(PWM_BR);
	    LCD_SetXY(10,3);
    	LCD_WriteString("ls");
    	LCD_WriteNum(speed_B_BL);*/
}
#endif