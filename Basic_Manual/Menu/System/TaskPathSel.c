#include "all.h"

void TaskPathSel (void *pdata)
{
	uint8  err;
	uint8  *msg;	
	pdata = pdata;
		
	for(;;)
	{
		msg = OSMboxPend(AppStartMbox,0,&err);//指向Menu_Selectindex
#if HaltSetting
		if(*msg==0xff)//先由邮箱挂起程序，再使用挂起函数挂起，再向邮箱发数据，再用停止函数停止挂起
			continue;//此时函数会自动运行，对应于这个情景，就是得到了key[1]的当前值，也即退出键键值
#endif
		LCD_Clear();
		LCD_SetXY(0,0);
		switch(Display_Page)
		{   
			case MENU:
				Display_Page=APPLICATION;
				(*Menu[*msg-1].entry)();//进入Menu中相应的函数
			break;
			case SUBMENU1:
				Display_Page=APPLICATION;
				(*SubMenu1[*msg-1].entry)();//进入SubMenu1中相应的函数
			break;
			case SUBMENU2:
				Display_Page=APPLICATION;
				(*SubMenu2[*msg-1].entry)();//进入SubMenu2中相应的函数
			break;
			case SUBMENU3:
				Display_Page=APPLICATION;
				(*SubMenu3[*msg-1].entry)();//进入SubMenu3中相应的函数
			break;
			case SUBMENU4:
				Display_Page=APPLICATION;
				(*SubMenu4[*msg-1].entry)();//进入SubMenu4中相应的函数
			break;
			case SUBMENU5:
				Display_Page=APPLICATION;
				(*SubMenu5[*msg-1].entry)();//进入SubMenu5中相应的函数
			break;
			case SUBMENU6:
				Display_Page=APPLICATION;
				(*SubMenu6[*msg-1].entry)();//进入SubMenu6中相应的函数
			break;
			case SUBMENU7:
				Display_Page=APPLICATION;
				(*SubMenu7[*msg-1].entry)();//进入SubMenu7中相应的函数
			break;
			case SUBMENU8:
				Display_Page=APPLICATION;
				(*SubMenu8[*msg-1].entry)();//进入SubMenu8中相应的函数
			break;
		}
	}
}