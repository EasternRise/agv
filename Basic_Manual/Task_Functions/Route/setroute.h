#ifndef SETROUTE_H
#define SETROUTE_H

//TaskAuto.c
extern void TaskAuto( void *pdata );

//mecanum_motor.c
extern uint8 Acc_Limit_enable;

extern  fp64  MotorF_Argument;
extern  fp64  MotorFL_Argument;
extern  fp64  MotorFR_Argument;
extern  fp64  MotorBL_Argument;
extern  fp64  MotorBR_Argument;

extern  fp64  MotorUP_Argument;

extern int Motor_F;
extern int Motor_FR;
extern int Motor_BL;
extern int Motor_BR;
extern int Motor_UP;

extern fp32 Speed_F;
extern fp32 Speed_FR;
extern fp32 Speed_BL;
extern fp32 Speed_BR;


extern void Mecanum_NandInit(void);
extern void Mecanum_NandSave(void);

extern void Mecanum_SpeedMode(int port,fp64 con);
extern void Steer_SpeedMode(int port);

extern fp64 MaxAcc;

//SetRoute.c
//路径种类选择
extern uint8 RouteForm;
#define RouteStop		1
#define RouteFree       2
#define Lineopen        3   
#define RouteStay       4
#define PointLine       5
#define RouteLine       6
#define RouteKeep		7

#define StopSpeed	0
//路径任务完成的标志,1表示未完成，0表示已完成
extern uint8 RouteFinish;

//TaskRoute.c
extern void TaskRoute(void *pdata);
extern void GoLine(void);
extern void SetLine( int16 speed_now, int16 speed_top, int16 speed_end, fp32 AP, fp32 DP, struct Pointfp64 aim_pos, fp64 Macc, fp64 radian);


//segment_path.c
extern void GoRoute1(void);
extern void GoRoute2(void);
extern void GoRoute3(void);
extern void GoRoute4(void);
extern void GoRoute5(void);
extern void GoRoute6(void);
extern void GoRoute7(void);
extern void GoRoute8(void);

//Point_line.c
extern uint16 Route[];

extern struct  Path_Point PointRoute[];

extern uint16 Point_Num;       //当前所用点阵中点的总数
extern uint16 Point_NowNum;    //当前对应点在所用点阵中的下标
extern uint16 Point_EndNum;    //结束时对应点的下标
extern uint16 Count_Step;      //每次搜寻参考点的范围
extern uint8  SetPointList;    //选择当前所用的点阵
extern struct Pointfp64 StartPoint;
extern struct Pointfp64 AimPoint;
extern fp64 Start_angle;
extern fp64 Aim_Angle;

extern void PointPathtest(void);
extern void SetPointPath(int16 point_startnum, int16 point_endnum,fp32 aimangle, struct Pointfp64 aim_point);
extern void GoPointPath(void);

extern uint8 PID_flag;
extern void GoLineTest(void);

extern fp64 s_now;
extern struct Pointfp64 now_pos;
extern fp64 Line_Angle;
extern fp64 s_NowAim;

extern fp64 Start_Speed;
extern fp64 Top_Speed;
extern fp64 End_Speed;
extern fp64   	Up_Ratio;
extern fp64   	Down_Ratio;
extern  fp64  	Amend_a1 ;					// 计算加速度
extern  fp64 	Amend_a2 ;					// 计算减速度

//Keep.c
extern int32 TopSpeed;
extern fp64 ShiftDist;
extern fp64 Last_Point;
extern void GoKeep(void);
extern void SetKeep(struct Pointfp64 aimpos,fp64 aimangle);
extern void KeepTest(void);

//TaskAutoSwitch.c
extern void TaskAutoSwitch(void *pdata);

#endif
