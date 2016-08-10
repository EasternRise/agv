
/* Includes ------------------------------------------------------------------*/
#include "all.h"

u8 PID_flag=1;

struct PID Pid_List[]=
{
		{1.5,0,0.05},    //大角度PID
		{0.65,0,0.1},    //小角度PID		
		{0.65,0,0.07},    //线位置PID
		

		{1.8,0,0.05},    //大角度PID
		{0.85,0,0.1},    //小角度PID
		{1.10,0,0.7},    //点位置PID
};

/*
 * 函数名: PID_Clear
 * 描  述: 将PID存储的误差值清除
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void PID_Clear(void)
{
	int i;
	
	for(i=0;i<2;i++)                                 //修改
	{
		Pid_List[i].mend=0;
		Pid_List[i].ep1=0;
		Pid_List[i].ep2=0;
	}
}

/*
 * 函数名: PID
 * 描  述: 位置式PID调整，利用了增量式的递推
 * 输  入: -Error 误差量
 *		   -pid pid参数
 * 输  出: 反馈结果
 * 调  用: 外部调用
 */

float Pid_Cal(double Error,int pid_num)
{
    static	double A=0;
    static	double B=0;
    static	double C=0;
    static	double delta=0;
	
	A = (Pid_List[pid_num].p+Pid_List[pid_num].i+Pid_List[pid_num].d);
	B = -(Pid_List[pid_num].p+2*Pid_List[pid_num].d);
	C = Pid_List[pid_num].d;
	
	delta = A*Error + B*Pid_List[pid_num].ep1 + C*Pid_List[pid_num].ep2;
	Pid_List[pid_num].mend += delta;
	
	Pid_List[pid_num].ep2 = Pid_List[pid_num].ep1;
	Pid_List[pid_num].ep1 = Error;
	
	return Pid_List[pid_num].mend;
}



/**********************************END OF FILE*********************************/
