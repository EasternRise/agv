#ifndef _ALL_H
#define _ALL_H

	#include "stm32f10x.h"
	#include "math.h"
	
	#define fp64 double
	#define fp32  float
	#define PI	3.14159265359	
	
	#include "compass.h"
	#include "guide.h"
	#include "mecanum.h"
	#include "Command.h"
		
	
	#include "MyTypeDef.h"
	
	#include "init.h"
	#include "can.h"
  #include "can_database.h"
  
  #include "canljx.h"
	#include "usart.h"
	
	#include "SysTick.h"
	
	
	#include "measure.h"
	
  #include "on_off_status.h"
	
	#include "mymath.h"
	
	#include "flash.h"
	#include "pwm.h"
  
	#include "sa.h"
			
	#include "lcd.h"
	#include "key.h"
	#include "pid.h"
	
	#include "TypeDef.h"
	#include "func.h"
	#include "list.h"
		
	#include "M_Colour.h"
  		
	#include "sys.h"
	
	#include "tim.h"
	#include "obstacle.h"
	
	void accurate(float speed_x,float speed_y,float speed_w);
	void my_test(int t_x,int t_y);
	void comadjust (void);
<<<<<<< HEAD
	void init_parameter(void);
	void parameter_setting (void);
	void load_defaults (void);
=======
	void parameter_setting (void);
>>>>>>> 2e8c624a0a3a1d3839c54e5692f6a1a626c4e09e
	
#endif
