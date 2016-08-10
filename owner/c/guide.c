#include "all.h"
#include "guide.h"

float guideDisError=0;
float guideAngError=0;

float Vout_0=0;
float Vout_1=0;

float Vout_X=0;
float Vout_Y=0;
float Vout_W=0;

//keep line
//参数为像素点单位，弧度，mm/s
void keep_line(float dis_err, float ang_err, float forward_speed)
{
	//camer1传回位置误差和角度误差

	  
		Vout_X=0;
		Vout_Y=0;
		Vout_W=0;
			
		if(fabs(ang_err)<1.5/180.0*pi)
		{
			Vout_0 = Pid_Cal(ang_err,0);
		}
		else
		{
			Vout_0 = Pid_Cal(ang_err,1);
		}
		
		Vout_1 = Pid_Cal(dis_err,2);
	
	//沿着直线方向前进的速度
	//分解到局部坐标系下的速度 
	Vout_X+=forward_speed*sin(ang_err);
	Vout_Y+=forward_speed*cos(ang_err);
	
	//校正角度误差的速度 
	Vout_X+=Vout_1;
	
	Vout_W=Vout_0;
		
	//赋速度
	SetSpeed(Vout_X,Vout_Y,Vout_W);
}

//keep point

void keep_point(float x_err, float y_err,float ang_err)
{
	//camer1传回位置误差和角度误差
	
		Vout_X=0;
		Vout_Y=0;
		Vout_W=0;
			
	
		float dis=sqrt(x_err*x_err + y_err*y_err);
	
		if(fabs(ang_err)<3.5/180.0*pi)
		{
			Vout_0 = Pid_Cal(ang_err,3);
		}
		else
		{
			Vout_0 = Pid_Cal(ang_err,4);
		}
		if(Vout_0>1.4)
			Vout_0=1.4;
		if(Vout_0<-1.4)
			Vout_0=-1.4;
		
		
		Vout_1 = Pid_Cal(dis,5);
		Vout_W=Vout_0;
		
	Vout_X=Vout_1*x_err/dis;
	Vout_Y=Vout_1*y_err/dis;
		
	//赋速度
	SetSpeed(Vout_X,Vout_Y,Vout_W);
	
}

void change_pos(void)
{	
		static float tmp_ang=0;
		
		tmp_ang=move_angle-locate_ang;
	
		if(tmp_ang>pi || tmp_ang <-pi)
				SetSpeed(0,0,30);
		
		if(tmp_ang<pi || tmp_ang>-pi)
				SetSpeed(0,0,-30);
}

//点编号
point_pos start_pos={0};
point_pos target_pos={0};
point_pos now_pos={0};

//初始化为在线上
//用来解决可能处于定位点处，但是由于漏判而导致的看不到二维码
enum robot_status my_robot=on_road;

//状态根摄像头传回来的数据来改变
//运动根据机器人的状态也选择不同的运动模式

//避障模式在另外的进程中处理

//巡线为摄像头1
//二维码为摄像头2

//摄像头1刚会直接传回来位置和角度误差
//摄像头2传回来的是当前在世界坐标系下的角度和位置

//角度和位置分别用什么样的单位需要我们去提前定义好




void move_car(float speed)
{
	float tmp_ang_err=0;
	
	switch(my_robot)
	{
			case on_road:
				//误差为摄像头传回来的数据
				if(slow_down == 1)
					keep_line(guide_dis_error,guide_ang_error/180*pi,150);
				else
					keep_line(guide_dis_error,guide_ang_error/180*pi,speed);
				break;
			case on_road_slow:
			case unloacted:
					keep_line(guide_dis_error,guide_ang_error/180*pi,150);
				break;
			case back_slow:
				keep_line(guide_dis_error,guide_ang_error/180*pi,-150);
				break;
			case see_point:
				keep_point(locate_x_error,locate_y_error,0);
				break;
			
			//进入这个状态时一次性确定机器人的目标姿态
			//根据当前姿态，用反馈调节来调整机器人的姿态
			
			case adjust_pos:
				tmp_ang_err=(locate_ang-move_angle)/180*pi;
				if(tmp_ang_err>pi)
					tmp_ang_err-=2*pi;
				else if(tmp_ang_err<-pi)
					tmp_ang_err+=2*pi;
				keep_point(locate_x_error,locate_y_error,tmp_ang_err);
// 				change_pos();
				break;
				
				//离开所当前点
				case leave_point:
					SetSpeed(0,500,0);
					break;
				
				case arrive_target:
				
				SetSpeed(0,0,0);
				break;
			
			
		}
}

void stop_car(void)
{
	my_robot=arrive_target;
	
	SetSpeed(0,0,0);
	
	set_mecf(0);
	set_mecbr(0);
	set_mecbl(0);
	
}

