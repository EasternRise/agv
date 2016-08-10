#ifndef _COMMAND_H
#define _COMMAND_H
	#include "stm32f10x.h"
	
	
extern float  guide_dis_error;
extern float  guide_ang_error;

extern float locate_ang;
extern float locate_x_error;
extern float locate_y_error;


extern float max_vol;
extern float min_vol;
extern u8 low_ele;

extern int move_angle;

extern u8 slow_down;


void CAMERA_G_FUNC(void);
void CAMERA_L_FUNC(void);
void CAMERA_P_FUNC(void);
void ADC_ELE_FUNC(void);

extern float delta_x,delta_y,delta_slow,move_distance;

#endif
