#include "all.h"

extern float nspeed_x,nspeed_y,nspeed_w;
extern float wheel_i_R[3][3];
	
void accurate(float speed_x,float speed_y,float speed_w)
{
	
	//float l=355.97;   //指轮子绕车中心的转弯半径
	
	
	float delt_speed=1000/100;
	float delt_speed_x=speed_x-nspeed_x;	float delt_speed_y=speed_y-nspeed_y; float delt_speed_wl=(speed_w-nspeed_w)*wheel_i_R[0][0];
  float sdelt_speed=pow(pow(delt_speed_x,2)+pow(delt_speed_y,2)+pow(delt_speed_wl,2),0.5);
	
	if (sdelt_speed>delt_speed)
	{
		nspeed_x+=delt_speed_x/sdelt_speed*delt_speed;
		nspeed_y+=delt_speed_y/sdelt_speed*delt_speed;
    nspeed_w+=delt_speed_wl/sdelt_speed*delt_speed/wheel_i_R[0][0];
	}
  else
  {
		nspeed_w =speed_w ;
		nspeed_x =speed_x ;
		nspeed_y =speed_y ;
	}
};
