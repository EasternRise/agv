#include "all.h"

int Menu_Count=8;//这个数字要和下面函数的数量相等
int SubMenu1_Count=8;	
int SubMenu2_Count=8;
int SubMenu3_Count=1;
int SubMenu4_Count=1;
int SubMenu5_Count=1;
int SubMenu6_Count=1;
int SubMenu7_Count=1;
int SubMenu8_Count=3;
int ShortcutMenu_Count=8;

struct Path Menu[]=//下面函数最好写成死循环
{
	{PATH_System,	   "System",		FOLDER},
	{test,			   "Test",			FOLDER},
	{Error_Show,	   "Debug",			CONFIG},
	{Retry,	 		   "Restart",		CONFIG},
	{Ringtest,	 	   "Ringtest",		CONFIG},
	{RouteInfo,		   "FixInfo",		CONFIG},
	{Fix,		   	   "Fix",			CONFIG},
	{Owen,			   "owen",          FOLDER},
};

struct Path SubMenu1[]=//(参考：System)当主菜单下的第1个函数属性为FOLDER时，此子菜单启用
{	
	{sys_Gyro,      	"Gyro",    		 	CONFIG},
	{sys_encoder,     	"Encoder",			CONFIG},
	{sys_gps,   		"GPS",		    	CONFIG}, 
	{sys_mecanum,       "Mecanum",       	CONFIG},
	{sys_SA_ST,			"SA ST",  	    	CONFIG},
	{sys_can,			"CAN_BUS",  	    CONFIG},
	{steertest,         "steertest",		CONFIG},
	{manualdata,	    "man_data",	    	CONFIG},
};

struct Path SubMenu2[]=//(参考: test)当主菜单下的第2个函数属性为FOLDER时，此子菜单启用
{
	{DataBackTest, "databack",		CONFIG},
	{PointPathtest,"Ppathtest",		CONFIG},
	{Shootest,	   "Shootest",		CONFIG},
	{Sonictest,	   "Sonictest",		CONFIG},
	{LidarInit,    "lidarInit",		CONFIG},
	{LidarTest,	   "Lidartest",		CONFIG},
	{LidarUse,	   "LidarUse",		CONFIG},
	{KeepTest,	   "KeepTest",		CONFIG}
};

struct Path SubMenu3[]=//(参考: red)当主菜单下的第3个函数属性为FOLDER时，此子菜单启用
{
	{PATH_System,	   "System",		FOLDER},
};

struct Path SubMenu4[]=//(参考: blue)当主菜单下的第4个函数属性为FOLDER时，此子菜单启用
{
	{PATH_System,	   "System",		FOLDER},

};

struct Path SubMenu5[]=//当主菜单下的第5个函数属性为FOLDER时，此子菜单启用
{
	{PATH_System,	   "System",		FOLDER},
};

struct Path SubMenu6[]=//当主菜单下的第6个函数属性为FOLDER时，此子菜单启用
{
	{PATH_System,	   "System",		FOLDER},
};

struct Path SubMenu7[]=//当主菜单下的第7个函数属性为FOLDER时，此子菜单启用
{
	{PATH_System,	   "System",		FOLDER},
};

struct Path SubMenu8[]=//(参考：owen)当主菜单下的第8个函数属性为FOLDER时，此子菜单启用
{
	{owen_mecanum,	   "Mecanum",		CONFIG},
	{owen_maxon,	   "Maxon",		    CONFIG},
	{owen_route,	   "Route",		    CONFIG},
};

//warning:由于shortcut和其他用户函数不是在同一个任务里运行，建议不要在这里修改其他任务用到的全局变量，调用其他任务使用的函数
#if ShortcutSetting
struct Path ShortcutMenu[]=
{
	{SaveScreen,      	"SaveScreen",    		RED}, 
	{ShowScreen,     	"ShowScreen",			RED},
	{SC_SA_ST,			"SA ST",  	    		RED},
	{SC_Relay,  		"Relay",	    		RED},
	{SC_gps,     		"GPS",					RED},
	{SC_lightboard,   	"Lightboard",       	RED},
	{SC_SelfDef,   		"Self Def",       		RED},
	{SC_Manual,   		"Manual",       		RED},
};
#endif