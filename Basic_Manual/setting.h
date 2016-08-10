#ifndef SETTING_H
#define SETTING_H
//~~~~~~~~~~名称~~~~~~~~当前状态~~~~~~~~~~先决条件~~~~~~~~~~~~~~~~~~~
//CAN总线,1使能，0禁止
#define CANSetting 			0

//初始化检测陀螺仪零漂，1使能，0禁止
#define GyroFloatSetting 	1

//看门狗，1使能，0禁止
#define WatchDogSetting 	1

//雷达， 1使能，0禁止
#define LidarSetting 		1

//數據傳輸,1使能，0禁止
#define TransSetting        0

//无线传输 1使能，0禁止  
#define WirelessSetting     0

//灯板，1使能，0禁止
#define LightBoardSetting 	0

//储存LCD页面显示内容，1使能，0禁止，单独开启可以使用LCD_Save和LCD_Load功能
#define LCDBufferSetting 	0

//shortcut，1使能，0禁止
#define ShortcutSetting 	0		//LCDBufferSetting置1

//暂停键，1使能，0禁止
#define HaltSetting 		0		//LCDBufferSetting置1

//手柄，1使能，0禁止
#define ManualSetting		1		//CANSetting置1

//手动车行走函数，1使能，0禁止
#define RouteGroup1Setting	1		//ManualSetting置1，ManualSetting置1

//死机后自动恢复状态，1使能，0禁止
#define RestorationSetting	0

//databack使能，1使能，0禁止
#define DatabackSetting    1

//走场使能
#define WalkGroundEnable	0

//三轮/四轮底盘选择，def三轮，ndef四轮
//#define WheelSelectSetting	1

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~各种功能和用法说明~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//--------------------------------------------setting.h----------------------------------------------------------------------
//按照相应注释设置标志位，可以只编译你需要的部分
//这样做主要出于两个原因：1.模板加入太多东西之后会有冗余，不编译不需要的东西会使你的程序小很多，烧写也会更快
//						  2.尽量将所有大家做出来的东西都放入模板，这样的机制可以保证即便下一届没有人用上你做的东西，你所做
//							也不会被遗忘，我们已经丢了太多前人的成果了，要吸取教训~送大家一句话：我走得很慢，但我从不后退
//
//----------------------------------------------LED-------------------------------------------------------------------------
//LED1：在nandflash开始向内存装载程序前亮起，此时已经完成了系统时钟配置，清空内存，设置堆栈等工作
//LED2：在nandflash向内存装载程序结束后亮起，前两个灯要是不亮。。买彩票去吧
//LED3：系统心跳，如果看门狗使能，系统心跳不跳就会重启
//LED4：如果cpu占用率100%，则等亮起，如果你的程序这个灯经常亮，最好考虑优化一下程序了，否则实时性会有问题
//
//------------------------------------------增加用户函数--------------------------------------------------------------------
//现在的菜单为两级菜单，你可以在一级或者二级菜单下添加用户函数，每个menu下最多添加8个函数，最后不要忘了相应menu的count值哦亲
//如果在一级菜单下添加函数，一级菜单相应位的属性不能是FOLDER
//如果在二级菜单下添加函数，一级菜单相应位的属性必须是FOLDER
//至于二级菜单相应位的属性，随便你填什么好了
//添加函数时不必再写Display_Page=APPILICATION;LCD_Clear();LCD_Set(0,0);这些已经在系统里默认了
//
//----------------------------------------菜单里的system函数----------------------------------------------------------------
//模板将原来的system和values下的函数整合进了这一个文件夹
//相应改动：Gyro：加入3float选项，即开机时的陀螺仪零漂检测程序，如果你选择禁止初始化检测零漂（可以大大加快开机速度哦亲），
//				  你可以在这里找到相应的函数
//			encoder："Con:"不为0时，码盘才会有读数
//			lightboard：加入了4.Yu Zhi可以测试阈值，汉语拼音是不是弱爆了？
//			Mecanum：将更多信息放入菜单页显示
//				步骤：先进入3.SetPort设置端口，然后进入2.ChangeArgu设置参数，再进入4.SaveChange存好，以后1.SpeedMode就能正常用了
//			SA_ST：ST是接触开关，SA是接近开关，进入函数后函数一直在刷新，开关状态的变化时实时显示的
//			Relay：这是一个在从前程序里很憋屈的函数，由于它在从前system函数的第二页，很多人表示没有见过它（摊手耸肩），
//					但是现在它出头了！重写过的Relay函数地理位置有所改善。。进入函数后，首先可以看到前8个继电器的状态，
//					按1-8键可改变继电器的状态，按翻页键可查看另外8个继电器
//			gps：改为进入页面直接显示动态gps，并且可以进行设置和清零操作
//			pid：改为进入页面直接显示pid
//			此外，你曾经为save，load等等选项按下去没有任何反应迷茫吗？现在所有选项都有所表示了
//
//--------------------------------------------shortcut----------------------------------------------------------------------
//shortcut：快捷方式的意思（我才不会告诉你我是翻词典找出来的），旨在在用户任务运行同时查看和更改各种全局变量与寄存器，
//			你还为在用户程序中显示gps，码盘读数，接触开关状态而烦恼吗？你还为上层电机少走了一厘米，gps少清一次0，所以重烧程序
//			而懊悔吗？这些统统交给shortcut吧
//shortcut的返回：按shortcut键退出shortcut时（注意不是停止键，停止键会将shortcut和用户程序一并退出），可以返回Shortcut使用前的
//				  LCD页面，并继续进行用户函数没有执行完的操作
//SaveScreen() and ShowScreen()：聪明的你可能发现，想返回用户程序的LCD界面不是一件简单的事（好吧反正我是想破头了），
//								 而随着这个问题的结局，shortcut里就出现了这个小彩蛋：你可以在任意页面下进入shortcut程序，
//								 然后截屏~问我有什么用？孩子，没用的，别问了。。
//和暂停键配合：不退出用户函数而进入shortcut，聪明的你可能又想到了，分为两种情况：
//						1.用户程序和shortcut同时运行，这样可以不打断用户函数，适合有实时性要求的场合，但如果用户函数有
//						  LCD循环显示程序，两个任务会抢显示屏
//						2.用户程序挂起，在退出shortcut时继续：这样可以保证shortcut程序即便在用户程序不间断显示其他东西时
//						  也能正常插入显示，但不能用在用户程序有实时性要求的场合
//自定义SC_SelfDef：如果你想在你的车运行中修改或者查看一些正在被用户任务使用的变量，可将它设为全局变量，加入次函数中
//					你也可以自己编写自定义的函数，但要注意shortcut函数和用户函数是在两个任务中运行，两个任务访问同时访问静态变量
//					等资源时会出现难以察觉的错误
//
//---------------------------------------------暂停键-----------------------------------------------------------------------
//按一下会暂停用户程序，暂停时会在LCD右下角出现一个halt标志，再按一下会继续用户程序
//暂停键的作用：1.和shortcut配合
//				2.使显示屏hold住，便于记录数据
//				3.便于分段调试程序，遛狗线通过停止电机工作停住程序，而暂停键可以直接暂停程序
//注意：暂停键不能停止除用户任务之外的所有东西，也就是说，如果一个设备可以独立在用户任务之外工作，暂停键是停止不了的
//
//--------------------------------------------传递键值----------------------------------------------------------------------
//key_flag已经取消掉了，全局变量传递键值的方法不适合多任务共用键盘
//键值应从邮箱中取得，在用户函数中为OSMboxPend(AppKey1Mbox,0,&err)和OSMboxAccept(AppKey2Mbox,0,&err)
//                    在shortcut函数中为OSMboxPend(SCKey1Mbox,0,&err)和OSMboxAccept(SCKey2Mbox,0,&err)
//1和2的区别在于，1适用于等待用户按键，2适用于未知按键何时到来，也即打断，具体用法可以参见MenuSystem.c和MenuShortcut.c
//通常来说，用1就行
//
//----------------------------------------------uart------------------------------------------------------------------------
//如果没有特殊需求，就配置为输入fifo中断，输出直接发出，fifo中断可以减少数据规模较大时中断的次数，同时也不会降低太多实时性
//函数的格式统一写为Uart0_和Uart2_，建议大家以后都统一这种写法，方便移植
//
//----------------------------------------------GPIO------------------------------------------------------------------------
//整合为Relay_Set，Relay_Clear，Relay_Read，SA_Read，ST_Read五个函数，Relay:继电器，SA：接近开关，ST：接触开关
//
//--------------------------------------------WatchDog----------------------------------------------------------------------
//判断时间100ms，判断死机时会重启程序，保证车不疯转，主要是为看总线和激光雷达两个死机大户设计的
//如果你开启看门狗，运行用户任务时看见函数无缘无故返回主菜单了，不要怀疑，一定是你死机之后重启了
//
//-----------------------------------nandflash and ValuesManager------------------------------------------------------------
//新的特点：1.现在可以在每一个地址上赋数据了，依然建议在40000以后赋值
//			2.改为进入函数直接显示数值，而非首先进入菜单页面，显示改为动态显示，如果数据在变化，现在是可以看到的
//			3.当地址赋值小于30000时，将不出现nandflash存取选项
//建议和注意事项：为了地址不写重复，建议在def.h底部统一定义名称
//				  一个地址至多写入2k数据，需要写入更多数据时，需要空出足够的地址不赋给其他数据
//				  nandflash有坏点的问题，而且相应的ECC校验也不完整，虽然你的数据因为坏点出错的几率基本为0，但正是比赛时还是
//				  不要用nandflash里的数据为好，要直接把数据写在变量的初始化中
//
//--------------------------------------------2440lib.c---------------------------------------------------------------------
//什么？你没找到？其实。。它已经消失了，消失的函数很多，这里给最脸熟的以外再露个脸


//任务堆栈长度
#define TaskStkLengh    2048

extern OS_EVENT *I2cMbox;
extern OS_EVENT *MenuMbox;
extern OS_EVENT *AppStartMbox;
extern OS_EVENT *AppKey1Mbox;
extern OS_EVENT *AppKey2Mbox;
	#if ShortcutSetting
extern OS_EVENT *SCKey1Mbox;
extern OS_EVENT *SCKey2Mbox;
	#endif
extern OS_EVENT *RouteMbox;
extern OS_EVENT *ArmMbox;
extern OS_EVENT *AutoMbox;               

extern OS_STK	TaskI2cKeysStk		[TaskStkLengh];//键盘服务任务







extern OS_STK   TaskLcdDisplayStk  	[TaskStkLengh];//液晶显示服务
extern OS_STK  TaskLcdRefreshStk  	[TaskStkLengh];
extern OS_STK	TaskPathSelStk		[TaskStkLengh];//路径服务任务
extern OS_STK  TaskUpdateStk       [TaskStkLengh];
	#if ManualSetting
extern OS_STK	TaskManualStk		[TaskStkLengh];
	#endif
extern OS_STK	TaskArmStk			[TaskStkLengh];
extern OS_STK  TaskRouteStk        [TaskStkLengh];
extern OS_STK   TaskAutoStk         [TaskStkLengh];
	#if WirelessSetting
extern OS_STK	TaskWirelessStk		[TaskStkLengh];
	#endif
	#if LidarSetting
extern OS_STK	TaskLidarStk		[TaskStkLengh];
	#endif
extern OS_STK	TaskSonicStk		[TaskStkLengh];
extern OS_STK  TaskAutoSwitchStk	[TaskStkLengh];
extern OS_STK  TaskRetryStk			[TaskStkLengh];

//任务优先级
//优先级在4-59之间

#define UpdatePrio				4
#define I2cKeysPrio				5
#define PathSelPrio				6



 	#if LidarSetting
#define  TaskLidarPrio          16
 	#endif
 	
#define SonicPrio				13 	
#define AutoSwitchPrio			8
#define RetryPrio				9
#define AutoPrio				11
#define RoutePrio               12
#define ArmPrio                 10

	#if ManualSetting
#define ManualPrio				7
	#endif

#define TaskLcdDisplayPrio		14
#define TaskLcdRefreshPrio		15



#endif