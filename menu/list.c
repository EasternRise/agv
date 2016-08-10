
/* Includes ------------------------------------------------------------------*/
#include "all.h"
#include "M_SA.h"
#include "M_camer.h"
#include "M_MOTOR.h"
#include "M_COMPASS.h"
#include "M_ADC.h"
#include "M_move.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*菜单*/
struct M_Type MainMenu[M_NUM];
struct M_Type M_VIEW[M_NUM];
struct M_Type M_PID[M_NUM];
struct M_Type M_TEST[M_NUM];
struct M_Type *CurrentMenu = MainMenu;//初始化当前菜单为主菜单

//主菜单下的子菜单
/*struct M_Type MainMenu[M_NUM]=
{  //Name          菜单属性    	函数			节点

	{"1.STOP",       M_UFunc,        stop_car,		   	  {MainMenu, NULL} },
  {"2.MOTOR",     M_UFunc,        sys_Mecanum,		 			{MainMenu, NULL} },
	{"3.MOVE",      M_UFunc,				move_test,	        {MainMenu, M_VIEW} },//看大车行驶方向
  {"4.",      M_UFunc,        NULL,					{MainMenu, M_TEST} },
	{"5.VIEW",	M_SMenu,        NULL,     	 	{MainMenu, M_VIEW} },
	{"6.TEST",  M_SMenu,        NULL,		     	{MainMenu, M_TEST} },
	{"7.",     M_Empty,        NULL,			  	{MainMenu, NULL} },
	{"8.",     M_Empty,        NULL,			    {MainMenu, NULL} },
};*/
struct M_Type MainMenu[M_NUM]=
{  //Name          菜单属性    	函数			节点

	{"1.PARAMETER SETTING",           M_UFunc,        parameter_setting,		  {MainMenu, NULL}   },
  {"2.TEST",                        M_SMenu,        NULL,		 			{MainMenu, M_VIEW} }, //对应于上个菜单的5.view
	{"3.LOAD OPTIMIZED DEAFULTS",     M_UFunc,				move_test,	  {MainMenu, NULL} },
  {"4.TEMP ",                       M_SMenu,        NULL,					{MainMenu, M_TEST} },
	{" ",	     M_SMenu,        NULL,     	 	  {MainMenu, NULL} },
	{" ",      M_SMenu,        NULL,		     	{MainMenu, NULL} },
	{" ",      M_Empty,        NULL,			  	{MainMenu, NULL} },
	{" ",      M_Empty,        NULL,			    {MainMenu, NULL} },
};
/*
struct M_Type M_VIEW[M_NUM]=
//{   //Name			菜单属性		函数			节点(上下级指针)
	{"1.CAMER",       M_UFunc,      CamerShow,   	     {MainMenu, NULL} },
	{"2.COMPASS",       M_UFunc,    ComShow,			     {MainMenu, NULL} },
	{"3.SA",       M_UFunc,         SaShow,			     {MainMenu, NULL} },
	{"4.ADC ",      M_UFunc,        EleShow,			     {MainMenu, NULL} },
	{"4.MOVE",      M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
  {" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};
*/
struct M_Type M_VIEW[M_NUM]=                                          //对应于TEST
{   //*Name			             菜单属性		     函数			           节点(上下级指针)
	{"1.CAMER",                M_UFunc,        CamerShow,   	     {MainMenu, NULL} },
	{"2.COMPASS",              M_UFunc,        ComShow,			       {MainMenu, NULL} },
	{"3.PROXIMITY SWITCH",     M_UFunc,        SaShow,			       {MainMenu, NULL} },
	{"4.MOTOR",                M_UFunc,        sys_Mecanum,			   {MainMenu, NULL} },
	{"5.BATTLE",               M_UFunc,        EleShow,			       {MainMenu, NULL} },
	{"6.PID ",                 M_SMenu,        NULL,			         {MainMenu, M_PID} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
  {" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};


struct  M_Type M_PID[M_NUM]=
{   /*Name			菜单属性		函数			节点(上下级指针)*/
	{"1.LINE_PID",  M_Empty,        NULL,					 {MainMenu, NULL} },
	{"2.BIG_PID",   M_Empty,        NULL,					 {MainMenu, NULL} },
	{"3.ADP_PID",   M_Empty,        NULL,		    	 {MainMenu, NULL} },
	{"4.OTHER",     M_Empty,        NULL,	      	 {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};


struct  M_Type M_TEST[M_NUM]=
{   /*Name          菜单属性		函数		 	节点(上下级指针)*/
	{"1.Keep",      M_UFunc,        Keep_Test,			 	{MainMenu, NULL} },
	{"2.Encoder",   M_Empty,        NULL,			 	{MainMenu, NULL} },
	{"3.Colour",    M_Empty,        NULL,			 	{MainMenu, NULL} },
	{"4.SD",        M_Empty,        NULL,				{MainMenu, NULL} },
	{"5.Engine",   	M_Empty,        NULL,		 		{MainMenu, NULL} },
	{"",  					M_Empty,        NULL ,			{MainMenu, NULL} },
	{"",           	M_Empty,        NULL ,			{MainMenu, NULL} },
  {"",       		M_Empty,        NULL,			    {MainMenu, NULL} },
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/********************************    END OF FILE    ***************************/
