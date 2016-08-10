#include "all.h"

//申请事件控制块
OS_EVENT *I2cMbox;
OS_EVENT *MenuMbox;
OS_EVENT *AppStartMbox;
OS_EVENT *AppKey1Mbox;
OS_EVENT *AppKey2Mbox;
OS_EVENT *ArmMbox;
OS_EVENT *AutoMbox;

#if ShortcutSetting
OS_EVENT *SCKey1Mbox;
OS_EVENT *SCKey2Mbox;
#endif

#if LidarSetting
OS_EVENT *LidarMbox;
#endif

OS_EVENT *McMbox;
OS_EVENT *RouteMbox;

#if WirelessSetting
OS_EVENT *WirelessMbox;
#endif

//为各任务申请堆栈
OS_STK	TaskI2cKeysStk		[TaskStkLengh];
OS_STK  TaskLcdDisplayStk  	[TaskStkLengh];
OS_STK  TaskLcdRefreshStk  	[TaskStkLengh];
OS_STK	TaskPathSelStk		[TaskStkLengh];
OS_STK  TaskRouteStk        [TaskStkLengh];
OS_STK  TaskArmStk          [TaskStkLengh];
OS_STK  TaskAutoStk			[TaskStkLengh];
OS_STK  TaskSonicStk		[TaskStkLengh];
OS_STK  TaskAutoSwitchStk	[TaskStkLengh];
OS_STK  TaskRetryStk		[TaskStkLengh];




#if WirelessSetting
OS_STK  TaskWirelessStk     [TaskStkLengh];
#endif


#if ManualSetting
OS_STK	TaskManualStk		[TaskStkLengh];
#endif

#if LidarSetting
OS_STK  TaskLidarStk        [TaskStkLengh];
#endif


int Main(void)
{
	CLOCK_Init(CLKMODE_400M);	//系统时钟初始化
	MMU_Init();					//内存管理单元初始化
	ISR_Init();					//中断服务函数初始化
	 
	UART0_Init(9600);			//UART0初始化
	UART1_Init(115200);			//UART1初始化
	UART2_Init(115200);			//UART2初始化
		
	IO_Init();					//IO初始化
	SPI1_Init();				//SPI初始化
	I2CInit();					//I2C初始化
	//FPGA_PWM_INIT();			//FPGA上的PWM波初始化
#if LightBoardSetting
	LightBoardInit();			//灯板初始化
#endif
	STM32_Reset();
	InitNandCfg();				//NandFlash初始化
	
	
	
	
	#if WatchDogSetting
	WatchDog_Init();			//看门狗初始化
	#endif
	
    OSInit ();	 				//操作系统初始化
    
///////////////////////////以上交给底层/////////////////////////////

	GYRO_NandInit();			//读取陀螺仪参数
    ENCODER_NandInit();			//读取码盘参数
	Mecanum_NandInit();			//读取全向轮参数
	PID_NandInit();				//读取PID参数
	
	
	
	BackDataInit();
		#if WalkGroundEnable
	Info_NandInit();
		#else
	Fix_Init();
		#endif
	
	OSTimeSet(0);				//系统时间设置
	
/////////////////////////////注册邮箱/////////////////////////////////

	I2cMbox		= 	OSMboxCreate((void *)0);	//发送iic得到的键盘信息给处理键值的函数TaskI2cKeys.c
	MenuMbox	= 	OSMboxCreate((void *)0);	//用于显示菜单路径
	AppStartMbox=	OSMboxCreate((void *)0);	//用于开启用户程序
	AppKey1Mbox	= 	OSMboxCreate((void *)0);	//向用户程序传递键值
	AppKey2Mbox	= 	OSMboxCreate((void *)0);	//用于按键打断
	
	RouteMbox	=	OSMboxCreate((void *)0);	//控制行走路径
	ArmMbox     = 	OSMboxCreate((void *)0);
	AutoMbox    =   OSMboxCreate((void *)0);

	
	#if LidarSetting
	LidarMbox   =   OSMboxCreate((void *)0);	
	#endif

////////////////////////////////注册任务//////////////////////////////

	OSTaskCreate (TaskI2cKeys,	(void *)0, &TaskI2cKeysStk[TaskStkLengh - 1], 	I2cKeysPrio);			//键值传递任务
	OSTaskCreate (TaskPathSel,	(void *)0, &TaskPathSelStk[TaskStkLengh  - 1], 	PathSelPrio );			//用户程序运行任务
	
	#if LidarSetting
    OSTaskCreate (TaskLidar,(void *)0, &TaskLidarStk[TaskStkLengh - 1], TaskLidarPrio);
	#endif
	
	#if ManualSetting
    OSTaskCreate (TaskManual,	(void *)0, &TaskManualStk[TaskStkLengh  - 1], ManualPrio );
	#endif
	 
	OSTaskCreate (TaskLcdDisplay,(void *)0, &TaskLcdDisplayStk[TaskStkLengh - 1],	TaskLcdDisplayPrio);	//菜单显示任务
   	OSTaskCreate (TaskLcdRefresh,(void *)0, &TaskLcdRefreshStk[TaskStkLengh - 1],	TaskLcdRefreshPrio);	//液晶刷新任务
	
	OSTaskCreate (TaskRoute,	 (void *)0, &TaskRouteStk[TaskStkLengh  - 1], 		RoutePrio );			//行走任务
	OSTaskCreate (TaskArm,       (void *)0, &TaskArmStk[TaskStkLengh -1],			ArmPrio);
    OSTaskCreate (TaskAuto,      (void *)0, &TaskAutoStk[TaskStkLengh-1],			AutoPrio);
	OSTaskCreate (TaskSuperSonic,(void *)0, &TaskSonicStk[TaskStkLengh - 1],		SonicPrio);
	//OSTaskCreate (TaskAutoSwitch,(void *)0, &TaskAutoSwitchStk[TaskStkLengh-1],		AutoSwitchPrio);
	//OSTaskCreate (TaskRetry,     (void *)0, &TaskRetryStk[TaskStkLengh-1],			RetryPrio);

///////////////////////////////////////////////////////////////////	


    TimerPWMInit();				//定时器和PWM波初始化， 必须放这儿！！！
    OSStart (); 
	
	return 0;
}







