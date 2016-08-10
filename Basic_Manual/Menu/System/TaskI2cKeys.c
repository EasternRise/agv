#include "all.h"

void PressAnyKey(void)
{
	uint8*msg;
	/*LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Waiting for any key");*/
	for (;;)			
	{
    	OSMboxAccept(AppKey2Mbox);
    	OSTimeDly(20);
		msg = OSMboxAccept(AppKey2Mbox);							
		if(*msg!=*(uint8*)0)			
		{
			break;
		}
	}
}

void TaskI2cKeys(void *pdata)
{
	uint8  *KEYBOARD_Value;
	uint8  err, key[1];
#if ShortcutSetting
	uint8  Shortcut_Page;
#endif
#if HaltSetting
	uint8  LCDIndexTmp;
#endif
	OS_CPU_SR cpu_sr;
	pdata=pdata;
	
	for(;;)
	{
		KEYBOARD_Value=OSMboxPend(I2cMbox,0,&err);//从I2c获得键值
		
		if(KEYBOARD_Value[0]>0&&KEYBOARD_Value[0]<5)//key[0]为数字1-8，用于菜单选项
			key[0]=KEYBOARD_Value[0];
		else if(KEYBOARD_Value[0]>6&&KEYBOARD_Value[0]<11)
			key[0]=KEYBOARD_Value[0]-2;
		else
			key[0]=0xff;
				
////////////////////////////创建任务或设备时要在这里进行紧急停止设置
		if(KEYBOARD_Value[0]==6)//stop
		{
		 	//uint8  err;
  			//uint8  *msg;
			
			MASKIrq( BIT_IIC );	
			OS_ENTER_CRITICAL();//关中断
			rGPJDAT&=~0x1000;
            PID_Clear();
			OSTaskDel(PathSelPrio);//停止必要的任务，然后重新创建
			OSTaskCreate (TaskPathSel,  (void *)0,&TaskPathSelStk  [TaskStkLengh-1], PathSelPrio);
			OSTaskDel(RoutePrio);
			RouteForm=RouteStop;
			OSTaskCreate (TaskRoute, 	(void *)0,&TaskRouteStk	   [TaskStkLengh-1], RoutePrio);
			
			
#if LidarSetting
			OSTaskDel(TaskLidarPrio);
			Lidar_Stat=Lidar_Waiting;
			OSTaskCreate (TaskLidar,(void *)0,&TaskLidarStk[TaskStkLengh -1],TaskLidarPrio);
#endif
			
#if HaltSetting
			if(HaltFlag==1)//stop时如果有halt的标志，则清除
			{
				LCDIndexTmp=LCDIndex;
				HaltFlag=1-HaltFlag;
				LCD_SetXY(16,3);
				LCD_WriteChar(HaltBuffer[0]);//恢复右下角本应显示的字符
				LCD_WriteChar(HaltBuffer[1]);
				LCD_WriteChar(HaltBuffer[2]);
				LCD_WriteChar(HaltBuffer[3]);
				LCDIndex=LCDIndexTmp;//改变右下角字符后，要恢复原来的LCD引索位置
				LCD_SetXY(LCDIndex%20,LCDIndex/20);
			}
#endif

			LCD_Clear();

#if ShortcutSetting
			if(Display_Page==SHORTCUTAPP || Display_Page==SHORTCUTMENU)//如果shortcut任务在运行，则也删除和重新创建shortcut
			{
				OSTaskDel(TaskLcdDisplayPrio);
				OSTaskCreate (TaskLcdDisplay,(void *)0,&TaskLcdDisplayStk[TaskStkLengh - 1],TaskLcdDisplayPrio);
			}
#endif
			Display_Page=MENU;
		
	        Acc_Limit_enable=0;
	        
#ifdef WheelSelectSetting
    		Speed_Motor(0,0,0);
#else
    		Speed_Motor(0,0,0,0);
    		Speed_F=0;
            Speed_FR=0;
            Speed_BL=0;
            Speed_BR=0;
            
	#if		ManualSetting
            Manual_speed_FL=0;
            Manual_speed_FR=0;
            Manual_speed_BL=0;
            Manual_speed_BR=0;
	#endif          
            
#endif

	        Acc_Limit_enable=1;
	        
			OSMboxPost(MenuMbox, key);				
			OS_EXIT_CRITICAL();//开中断
			UNMASKIrq( BIT_IIC );
			continue;
		}
		
		
#if ShortcutSetting
		if(KEYBOARD_Value[0]==12)//shortcut
		{
			if(Display_Page!=SHORTCUTAPP && Display_Page!=SHORTCUTMENU)//进入shortcut
			{
				LCD_Save();//存储进入shortcut前的LCD页面
				Shortcut_Page=Display_Page;
				Display_Page=SHORTCUTMENU;
				OSMboxPost(MenuMbox, key);
			}
			else if(Display_Page==SHORTCUTAPP || Display_Page==SHORTCUTMENU)//退出shortcut
			{
				LCD_Load();//显示进入shortcut前的LCD页面
				Display_Page=Shortcut_Page;
				OSTaskDel(TaskLcdDisplayPrio);//退出时可能还有程序在运行，所以要删除任务
				OSTaskCreate (TaskLcdDisplay,(void *)0,&TaskLcdDisplayStk[TaskStkLengh - 1],TaskLcdDisplayPrio);
				OSMboxPost(MenuMbox, key);
			}
			continue;
		}
#endif

#if HaltSetting
		if(KEYBOARD_Value[0]==11)//halt
		{
			LCDIndexTmp=LCDIndex;
			if(HaltFlag==0)//halt
			{
				OSTaskSuspend(PathSelPrio);//挂起任务
				HaltBuffer[0]=LCDTable[16][3];//将右下角本应显示的字符存入buffer
				HaltBuffer[1]=LCDTable[17][3];
				HaltBuffer[2]=LCDTable[18][3];
				HaltBuffer[3]=LCDTable[19][3];
				LCD_SetXY(16,3);
				LCD_WriteString("halt");
				HaltFlag=1-HaltFlag;
			}
			else//resume
			{
				OSTaskResume(PathSelPrio);//继续任务
				HaltFlag=1-HaltFlag;
				LCD_SetXY(16,3);
				LCD_WriteChar(HaltBuffer[0]);//恢复右下角本应显示的字符
				LCD_WriteChar(HaltBuffer[1]);
				LCD_WriteChar(HaltBuffer[2]);
				LCD_WriteChar(HaltBuffer[3]);
			}
			LCDIndex=LCDIndexTmp;//改变右下角字符后，要恢复原来的LCD引索位置
			LCD_SetXY(LCDIndex%20,LCDIndex/20);
			continue;
		}
#endif
//******************************************************************
		switch(Display_Page)
		{
			case APPLICATION://向用户程序传递键值
#if HaltSetting
				if(HaltFlag==1)//在halt时不能传递键值
					continue;
#endif
				OSMboxPost1(AppKey1Mbox, KEYBOARD_Value);//适用于程序等待按键
				OSMboxPost2(AppKey2Mbox, KEYBOARD_Value);//适用于按键打断程序
			break;
			case MENU:
				if(key[0]>Menu_Count)
				{
					break;
				}
				if(Menu[key[0]-1].type!=FOLDER)//如果一级菜单不是文件夹属性，则进入相应用户任务
				{
  					//Display_Page=APPLICATION;
  					OSMboxPost(AppStartMbox, key);
  				}
  				else//如果一级菜单是文件夹属性，则进入相应二级菜单
  				{
  					switch(key[0])
  					{
  						case 1:
  							Display_Page=SUBMENU1;
  						break;
  						case 2:
  							Display_Page=SUBMENU2;
						break;
						case 3:
							Display_Page=SUBMENU3;
						break;
						case 4:
							Display_Page=SUBMENU4;
						break;
						case 5:
							Display_Page=SUBMENU5;
						break;
						case 6:
							Display_Page=SUBMENU6;
						break;
						case 7:
							Display_Page=SUBMENU7;
						break;
						case 8:
							Display_Page=SUBMENU8;
						break;
  					}
  					OSMboxPost(MenuMbox, key);
  				}
			break;
			case SUBMENU1://如果已经进入了二级菜单，再有相应键按下时，则启动用户任务
				if(key[0]<=SubMenu1_Count)
				{
					OSMboxPost(AppStartMbox, key);
				}
			break;
			case SUBMENU2:
				if(key[0]<=SubMenu2_Count)
				{
					OSMboxPost(AppStartMbox, key);
				}
			break;
			case SUBMENU3:
				if(key[0]<=SubMenu3_Count)
				{
					OSMboxPost(AppStartMbox, key);
				}
			break;
			case SUBMENU4:
				if(key[0]<=SubMenu4_Count)
				{
					OSMboxPost(AppStartMbox, key);
				}
			break;
			case SUBMENU5:
				if(key[0]<=SubMenu5_Count)
				{
					OSMboxPost(AppStartMbox, key);
				}
			break;
			case SUBMENU6:
				if(key[0]<=SubMenu6_Count)
				{
					OSMboxPost(AppStartMbox, key);
				}
			break;
			case SUBMENU7:
				if(key[0]<=SubMenu7_Count)
				{
					OSMboxPost(AppStartMbox, key);
				}
			break;
			case SUBMENU8:
				if(key[0]<=SubMenu8_Count)
				{
					OSMboxPost(AppStartMbox, key);
				}
			break;
			
#if ShortcutSetting
			case SHORTCUTMENU://进入shortcut任务
				if(key[0]<=ShortcutMenu_Count)
				{
					LCD_Clear();
					Display_Page=SHORTCUTAPP;
					OSMboxPost(MenuMbox, key);
				}
			break;
			case SHORTCUTAPP:
				OSMboxPost1(SCKey1Mbox, KEYBOARD_Value);//适用于程序等待按键
				OSMboxPost2(SCKey2Mbox, KEYBOARD_Value);//适用于按键打断程序
			break;
#endif
		}
	}
}
