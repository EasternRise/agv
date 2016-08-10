#include "all.h"

#define Acc_Limit_Enable 1

//下面的坐标系是机器人的局部坐标系
float Speed_X;				// mm/s
float Speed_Y;				// mm/s
float Speed_Rotation;		// (rad/s)/100

float coeff_x,coeff_y,coeff_r; //雅克比矩阵化简后的系数

float Max_Acc = 2
;

// struct Mecanum_State Mec_FL={2, -1};		//四轮的状态参数
// struct Mecanum_State Mec_FR={3, -1};
// struct Mecanum_State Mec_BL={4, -1};
// struct Mecanum_State Mec_BR={1, -1};

struct Mecanum_State Mec_F ={1, 1};		//三轮的状态参数
struct Mecanum_State Mec_BR={3, 1};
struct Mecanum_State Mec_BL={2, 1};

static float robo_L = 636.6;//左右轮距
static float robo_W = 636.2;//前后轮距

// static float robot_R = 360;
float wheel_r = 63.5;      

float nspeed_x,nspeed_y,nspeed_w;
static float wheel_rr = 9.56;

//*******************************************************************陈实增加部分
float wheel_i_R[3][3]=          //各个轮子在小车坐标系下的相对位置
{
	{355.97,  PI/2 ,   PI/2+1.0/180*PI   },      //{355.97+del_l,       PI/2+del_a1,   PI/2+del_a2},下同极坐标系下表示，第一个角度代表轮子位置角度
  {355.97,  11.0/6*PI, 11.0/6*PI-0.0/180*PI  },      //第二个角度代表轮子朝向角度
  {355.97,  7.0/6*PI,  7.0/6*PI-1.0/180*PI},
};

float a_OR=0/180*PI;

extern float mycoeff_arg[3][3];
/*float mycoeff_arg[3][3]=
{  2016/8/1储存今天感觉不错的值 2016/8/1储存昨天感觉不错的值
	{sin (a_OR+PI/2+1.0/180*PI  ) , cos (-0.45/180*PI+PI/2),                -1*wheel_i_R[1][1]*cos(wheel_i_R[1][3]-wheel_i_R[1][2])},
	{sin (a_OR+11.0/6*PI-0.0/180*PI) , cos (-0.45/180*PI+11.0/6*PI),           -1*wheel_i_R[2][1]*cos(wheel_i_R[2][3]-wheel_i_R[2][2])},
	{sin (a_OR+7.0/6*PI-1.0/180*PI) , cos (-0.45/180*PI+7.0/6*PI+1.1/180*PI), -1*wheel_i_R[3][1]*cos(wheel_i_R[3][3]-wheel_i_R[3][2])},
};*/

float motor_speed=0;

float Speed_Limit = 1000;

//除单独测试每个轮子外
//其余时刻控制底盘都用这个setSpeed函数来控制
//每个轮子的当前速度可能通过下面的变量得到
float speed_propor=0;//当前速度与目标速度比
//为了保持一致性，在测试轮子的时候也要求更新这三个变量的值

	float f_speed=0;
	float bl_speed=0;
	float br_speed=0;
	
void SetSpeed(float speed_x,float speed_y,float speed_rotation)
{
	/*角速度*/
	
	
	float f_delta;
	float bl_delta;
	float br_delta;
	
	static float f_last=0;	//储存四轮的速度
	static float bl_last=0;
	static float br_last=0;
	


 	float delta_max;			//速度变化量最大值
	float cov = 60.0/(2*pi);//角速度和转速的转化系数

//	这两个参数不能随意设置 雅克比矩阵的各个正负号只是能确保正常运作，但是定量的分析上就会出问题
	coeff_x = coeff_y = 1 / (wheel_r + wheel_rr);
	coeff_r = (robo_L + robo_W) / 2 / (wheel_r + wheel_rr);

	// 	float coeff_arg[3][3] = 
// 	{	
// 		
// 		{ -0.866,	 -0.5	, -360},
// 		{ -0.866, -0.5	, 360 },
// 		{ 0			, 	-1	, -360},
// 	};
	
	
	//转速
//****************************陈实注释部分
	
/****************************通过雅克比矩阵把速度分解到各个轮子******************************/
/*	bl_speed = (coeff_arg[0][0] * speed_x + coeff_arg[0][1] * speed_y + coeff_arg[0][2] * speed_rotation)/-wheel_r * cov;
	br_speed = (coeff_arg[1][0] * speed_x + coeff_arg[1][1] * speed_y + coeff_arg[1][2] * speed_rotation)/-wheel_r * cov;
	f_speed =  (coeff_arg[2][0] * speed_x + coeff_arg[2][1] * speed_y + coeff_arg[2][2] * speed_rotation)/-wheel_r * cov;
*/	
//*****************************陈实注释部分

f_speed  = (mycoeff_arg[0][0] * speed_x + mycoeff_arg[0][1] * speed_y + mycoeff_arg[0][2] * speed_rotation)* cov;
bl_speed = (mycoeff_arg[1][0] * speed_x + mycoeff_arg[1][1] * speed_y + mycoeff_arg[1][2] * speed_rotation)* cov;
br_speed = (mycoeff_arg[2][0] * speed_x + mycoeff_arg[2][1] * speed_y + mycoeff_arg[2][2] * speed_rotation)* cov;

/**************************对加速度进行限制(以理论速度限制赋速度)****************************/

	f_delta = f_speed - f_last;
	bl_delta = bl_speed - bl_last;
	br_delta = br_speed - br_last;

  speed_propor =MaxFour(f_last/(f_speed+0.0000001),0,bl_last/(bl_speed+0.0000001),br_last/(br_speed+0.00000001));
	
	delta_max=MaxFour(Abs(f_delta),0,Abs(bl_delta),Abs(br_delta));
	
	if((delta_max >= Max_Acc) && (Acc_Limit_Enable==1))
	{
		f_last += Max_Acc*(f_delta/delta_max);		
		bl_last += Max_Acc*(bl_delta/delta_max);
		br_last += Max_Acc*(br_delta/delta_max);
	} 
	else if((delta_max <= -Max_Acc) && (Acc_Limit_Enable==1))
	{
		f_last -= Max_Acc*(f_delta/delta_max);
		bl_last -= Max_Acc*(bl_delta/delta_max);
		br_last -= Max_Acc*(br_delta/delta_max);
	}
	else
	{
		f_last = f_speed;
		bl_last = bl_speed;
		br_last = br_speed;
	}
	
	
	f_speed=f_last;
	bl_speed=bl_last;
	br_speed=br_last;
	
	set_mecf(f_speed);
	set_mecbl(bl_speed);
	set_mecbr(br_speed);
	
}



void Mecanum_test(struct Mecanum_State *mec)
{
	double speed=0;
	
/*->*/mec_test:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Mecanum=======");
		LCD_SetXY(0,1);
		LCD_WriteString("1.Speed");
		LCD_SetXY(0,2);
		LCD_WriteString("2.ChangeArg");
		LCD_SetXY(0,3);
		LCD_WriteString("3.ChangePort now:");
		LCD_WriteInt((*mec).port);
		
				refresh();	
        
		if(KeyReady==1)
		  {
			KeyReady=0;
			switch(KeyValue)
			{
				case key1:
					goto mec_speed;
				case key2:
					Input_IntValue(&(*mec).arg,"Argument");
					break;
				case key3:
 					Input_IntValue(&(*mec).port,"Can");
					break;
				case keyback:
					return;
			}
			
		  }
		delay(LCD_DELAY);
			
	}
	
/*->*/mec_speed:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Mecanum=======");
		LCD_SetXY(0,1);
		LCD_WriteString("1.SpeedUp");
		LCD_SetXY(0,2);
		LCD_WriteString("2.SpeedDown");
		LCD_SetXY(0,3);
		LCD_WriteString("3.Stop");
		LCD_SetXY(0,4);
		LCD_WriteString("4.Set Speed");
		
				refresh();	
        
		if(KeyReady==1)
		  {
			KeyReady=0;
			switch(KeyValue)
			{
				case key1:
					speed+=10;
					break;
				case key2:
					speed-=10;
					break;
				case key3:
					speed=0;
					break;
				case key4:
					Input_DoubleValue(&speed,"Goal Speed");
					break;
				case keyback:	
// 					motor_speed=0;
					goto mec_test;
			}
			switch( (*mec).port)
			{
				case 1:
					set_mecf(speed);
					break;
				
				case 2:
					set_mecbl(speed);
					break;
				
				case 3:
					set_mecbr(speed);
					break;
			}
		}
		delay(LCD_DELAY);
	}
}



