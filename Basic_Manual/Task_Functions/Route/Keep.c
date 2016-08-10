#include "all.h"

int32 TopSpeed=1500;
fp64 ShiftDist;
fp64 Last_Point;

fp64 Route_PID(void)
{
    
    fp64 delta_length; 
    
    fp64  R_a0 = 0.0;
	fp64  R_a1 = 0.0;
	fp64  R_a2 = 0.0;
	fp64  R_Vd;
	
	uint8 flag;
	
	fp64 speed;
	
	/* 用于调试时显示使用  
       fp64 show1;
       fp64 show2;
       fp64 show3; 
       fp64 show4;
	*/
    delta_length=Getlength(AimPoint,Gps_List[0].position);
	//delta_length=ShiftDist+(Encoders[0].Distance- Ac1*(Gps_List[0].radian-Aim_Angle/180*PI) -Last_Point);
	LCD_SetXY(0,0);
	LCD_WriteNum(delta_length);


    //对误差较大时的积分进行分离
    if(Fp64Abs(delta_length)<15)
    {
       flag=1;
    }
    else
    {
       flag=0;
    }

    R_a0 = ( Pid_List[2].PROP * ( 1 + 1 / (2*Pid_List[2].Ti)*flag + Pid_List[2].Td));     
	R_a1 = ( -Pid_List[2].PROP * ( 1 - 1 / (2*Pid_List[2].Ti)*flag + 2 * Pid_List[2].Td));         
	R_a2 = ( Pid_List[2].PROP * Pid_List[2].Td );    	
    R_Vd =  R_a0 * delta_length + R_a1 *D_EP1 + R_a2 *D_EP2; 
    
    AmendV = AmendV + R_Vd;
    D_EP2 = D_EP1;
	D_EP1 = delta_length;
	
	speed=AmendV;
	
	if(speed>TopSpeed)
	{
	   speed=TopSpeed;
	}
	
	if(speed<-TopSpeed)
	{
	   speed=-TopSpeed;
	}
	
	if(speed>-200&&speed<0)	speed=-200;
	if(speed<200&&speed>0)	speed=200;
	
	       /*
	       show1=delta_length;
           show2=speed;
           show3=delta_angle;
           show4=;
           
           LCD_SetXY(0,0);
	       LCD_WriteString("rl:");
	       LCD_WriteNum(show1);
	       LCD_WriteString("        ");
	       LCD_SetXY(0,1);
	       LCD_WriteString("sp:");
	       LCD_WriteNum(show2);
	       LCD_WriteString("        ");
	       LCD_SetXY(0,2);
	       LCD_WriteString("3:");
	       LCD_WriteNum(show3);
	       LCD_WriteString("        ");
	       LCD_SetXY(0,3);
	       LCD_WriteString("speed:");
	       LCD_WriteNum(show4);
	       LCD_WriteString("        ");
	       */
	
	
	return speed;
	
}

void SetKeep(struct Pointfp64 aimpos,fp64 aimangle)
{
	RouteFinish=1;
	RouteForm=RouteKeep;
	Aim_Angle=aimangle/PI*180;
	AimPoint=aimpos;
}

void GoKeep()
{
	fp64 			error_angle = 0.0;	// 角度误差
	fp64           	v_err_x;
	fp64            v_err_y;	
	fp64            phi;
	
	Line_Angle = MATH_GetLineAngle(Gps_List[0].position,AimPoint);
	error_angle = Gps_List[0].angle-Aim_Angle;
	if(error_angle>=180)
	{
		error_angle-=360;
	}
	if(error_angle<=-180)
	{
		error_angle+=360;
	}	
	
	Vout[1] = Route_PID();
	Vout[0] = AnglePID(error_angle,Pid_List[3].PROP,Pid_List[3].Ti,Pid_List[3].Td);
	
	//速度限幅
	if(Vout[0]>300)	Vout[0]=300;
	if(Vout[0]<-300) Vout[0]=-300;
	
	v_err_x=Vout[1]*cos(Line_Angle);
    v_err_y=Vout[1]*sin(Line_Angle);
    
    phi=Gps_List[0].radian;
	
    Speed_F =1812.747826*v_err_x*(cos(phi)-sin(phi)) + 7478.744943*(Vout[0]) + 1812.747826*v_err_y*(cos(phi)+sin(phi));

    Speed_FR=-7478.744943*(Vout[0]) + 1812.747826*v_err_y*(cos(phi)-sin(phi)) -1812.747826*v_err_x*(cos(phi)+sin(phi));

    Speed_BL=1812.747826*v_err_y*(cos(phi)-sin(phi)) + 7478.744943*(Vout[0]) -1812.747826*v_err_x*(cos(phi)+sin(phi));

    Speed_BR=-7478.744943*(Vout[0])+ 1812.747826*v_err_x*(cos(phi)-sin(phi)) +1812.747826*v_err_y*(cos(phi)+sin(phi));
    
    LCD_SetXY(0,1);
    LCD_WriteNum(error_angle);
    LCD_SetXY(0,2);
    LCD_WriteNum(Line_Angle);
    LCD_SetXY(0,3);
    LCD_WriteNum(v_err_x);
    LCD_SetXY(0,4);
    LCD_WriteNum(v_err_y);
    
}

