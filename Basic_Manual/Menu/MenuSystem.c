#include "all.h"
extern int16 time;

/*nightsky:这些函数是为了打开下一子菜单而加的空函数，没有
   实际含义，当menu里有一菜单函数的类型为FOLDER时，在这里
   加上函数定义*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void PATH_System(void)   //系统设备参数调整菜单
{}

void test(void)			 //各种测试函数
{}




void Owen(void)          //个人测试函数
{}

void owen_mecanum(void)
{
	uint8 err;
	uint8 *msg;
	fp64  speed_Vx=0,speed_Vy=0,speed_w=0;
	fp64 r,w;

/*->*/mecanum:
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_Printf("==owen's Function==");
	LCD_SetXY(0,1);
	LCD_WriteString("1.F 2.B 3.circle 4.R");
	LCD_SetXY(0,2);
	LCD_WriteString("5.Cir+ 6.Cir-");
    LCD_SetXY(0,3);
	LCD_WriteString("7.set");
	RouteForm=RouteFree;
	
	
	for(;;)
	{
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				goto F;
				break;
			case 2:
				goto B;
				break;
			case 3:
				goto circle;
				break;
			case 4:
				//goto R;
				break;
			case 7:
				goto Cir_P;
				break;
			case 8:
				goto Cir_N;
				break;
			case 9:
				goto set;
				break;
		}
	}
	
/*->*/F:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,3);
		LCD_Printf("=========F=========");
		
		RouteForm=RouteFree;
		
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		//Speed_Motor(-35,35,-35,35);
		Speed_F =0;
		Speed_FR=0;
		Speed_BL=0.09095*100;
		Speed_BR=0.09095*100;		
;			
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
	}
	goto mecanum;
	
/*->*/B:
	while(1)
	{
	 	LCD_Clear();
		LCD_SetXY(0,3);
		LCD_Printf("=========B=========");
		
		RouteForm=RouteFree;
		
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		//Speed_Motor(-35,35,-35,35);
		Speed_F =0;
		Speed_FR=0;
		Speed_BL=-0.09095*100;
		Speed_BR=-0.09095*100;			
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
	}
	goto mecanum;

/*->*///L:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,3);
		LCD_Printf("=========L=========");
		
		RouteForm=RouteFree;
		
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		//Speed_Motor(-35,35,-35,35);
		Speed_F =-50*14000;
		Speed_FR=50*14000;
		Speed_BL=50*14000;
		Speed_BR=-50*14000;			
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
	}
	goto mecanum;

/*->*///R:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,3);
		LCD_Printf("=========R=========");
		
		RouteForm=RouteFree;
		
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		//Speed_Motor(-35,35,-35,35);
		Speed_F =50*14000;
		Speed_FR=-50*14000;
		Speed_BL=-50*14000;
		Speed_BR=50*14000;			
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
	}
	goto mecanum;

/*->*/Cir_P:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,3);
		LCD_Printf("========CIR+========");
		
		RouteForm=RouteFree;
		
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		//Speed_Motor(-35,35,-35,35);
		Speed_F =0;
		Speed_FR=0;
		Speed_BL=-0.09095*100;
		Speed_BR=-0.09095*100;			
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
	}
	goto mecanum;

/*->*/Cir_N:
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,3);
		LCD_Printf("========CIR-========");
		
		RouteForm=RouteFree;
		
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		//Speed_Motor(-35,35,-35,35);
		Speed_F =0;
		Speed_FR=0;
		Speed_BL=-0.09095*100;
		Speed_BR=-0.09095*100;			
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
	}
	goto mecanum;

/*->*/circle:
    for(;;)
    {
        LCD_Clear();
        DISPLAY_Fp64Value(&r,"r");
        DISPLAY_Fp64Value(&w,"w");
        
        Speed_F =0;
		Speed_FR=0;
		Speed_BL=60*r*w/PI/210-(30*640/PI/210)*w;
		Speed_BR=60*r*w/PI/210+(30*640/PI/210)*w;
		
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
		OSTimeDly(20);
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
    }
    


/*->*/set:
	for(;;)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("1.Vx=");
		LCD_WriteNum(speed_Vx);
		LCD_SetXY(0,1);
		LCD_WriteString("2.Vy=");
		LCD_WriteNum(speed_Vy);
		LCD_SetXY(0,2);
		LCD_WriteString("3.w=");
		LCD_WriteNum(speed_w);
		
		RouteForm=RouteFree;
		
		Speed_F = 1754.23*speed_Vx+1754.23*speed_Vy-855.189*speed_w;
		Speed_FR=-1754.23*speed_Vx+1754.23*speed_Vy+855.189*speed_w;
		Speed_BL=-1754.23*speed_Vx+1754.23*speed_Vy-855.189*speed_w;
		Speed_BR= 1754.23*speed_Vx+1754.23*speed_Vy+855.189*speed_w;
		
		
		
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			case 1:
				//goto Vx;
				DISPLAY_Fp64Value(&speed_Vx,"speed_Vx");
				break;
			case 2:
				//goto Vy;
				DISPLAY_Fp64Value(&speed_Vy,"speed_Vy");
				break;
			case 3:
				//goto w;
				DISPLAY_Fp64Value(&speed_w,"speed_w");
				break;
		}
		
	}
}	



void owen_maxon(void)
{
    uint8 err;
	uint8 *msg;
    double speed;

	for(;;)
	{

	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_Printf("==Maxon Function==");
	LCD_SetXY(0,1);
	LCD_WriteString("1.Enable");
	LCD_SetXY(0,2);
	LCD_WriteString("2.Disable");
	LCD_SetXY(0,3);
	LCD_WriteString("3.QuickStop");
	LCD_SetXY(0,4);
	LCD_WriteString("4.SpeedMode");
	LCD_SetXY(0,5);
	LCD_WriteString("5.Maxon1_ChangeSpeed");
	LCD_SetXY(0,6);
	LCD_WriteString("6.Maxon2_ChangeSpeed");
	
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
			    MaxonDisable();
			    OSTimeDly(10);
			    MaxonEnable();
			
			    //OSTaskDel(ManualPrio);	
			    
				//OSTaskCreate (TaskManual,	(void *)0, &TaskManualStk[TaskStkLengh  - 1], ManualPrio );
				break;
			case 2:
				MaxonDisable();
				break;
			case 3:
				MaxonQuickstop();
				break;
			case 4:
				MaxonSpeedMode();
				break;
			case 7:
				DISPLAY_Fp64Value(&speed,"speed");
				Maxon1SetSpeed(speed);
				break;
			case 8:
				DISPLAY_Fp64Value(&speed,"speed");
				Maxon2SetSpeed(speed);
				break;
		}
	}
}



void owen_route()
{
	uint8 err;
	uint8 *msg;
	struct Pointfp64 aim;
	struct Pointfp64 keep_aim;
	fp64 keep_radian;

	struct Pointfp64 owen_test={0,0};
	//fp64  speed_Vstart=0,speed_Vmax=300,speed_Vend=200;
	
	
	struct Value Values[]=
     {   
        {(void*)&Pid_List[6].PROP,	TYPE_FP64,	"DP"}, 
        {(void*)&Pid_List[6].Ti,	TYPE_FP64,	"DI"}, 
        {(void*)&Pid_List[6].Td,	TYPE_FP64,	"DD"},
        {(void*)&Pid_List[7].PROP,	TYPE_FP64,	"AP"}, 
        {(void*)&Pid_List[7].Ti,	TYPE_FP64,	"AI"}, 
        {(void*)&Pid_List[7].Td,	TYPE_FP64,	"AD"},

      };
	
	
	

	
	
	

/*->*/route:
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_Printf("==Route Function==");
	LCD_SetXY(0,1);
	LCD_WriteString("1.Line");
	LCD_SetXY(0,2);
	LCD_WriteString("2.curve");
	LCD_SetXY(0,3);
	LCD_WriteString("3.keep");
	LCD_SetXY(0,4);
	LCD_WriteString("4.databack");
 
	//RouteForm=RouteFree;
	
	
	for(;;)
	{
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				goto owen_line;
				break;
			case 2:
				goto owen_curve;
				break;
			case 3:
				goto owen_keep;
				break;
			case 4:
				//goto owen_databack;
				DataBackTest();
				break;
		}
	}
	
/*->*/owen_line:
	
	aim.x=0;
	aim.y=1000;
	
	
	
	DISPLAY_ValuesManager(Values,6,PID_NANDPAGE);
	
	
	
	
	GPS_Clear();
	PID_Clear();
	
	
	//Gps_List[0].position.x=100;
	//Gps_List[0].position.y=0;
	//Gps_List[0].angle=0;
	//GPS_Init(Gps_List[0].position,Gps_List[0].angle);
	
	
	
	
	//DISPLAY_Fp64Value(&speed_Vstart,"speed_Vstart");
	//DISPLAY_Fp64Value(&speed_Vmax,"speed_Vmax");
	//DISPLAY_Fp64Value(&speed_Vend,"speed_Vend");
	//DISPLAY_Fp64Value(&owen_test.x,"X");
	
	
	
	BackDataInit();
	
	SetLine(100,300,100,0.2,0.2,aim,100,0);
	
	
	//GPS_Init(owen_test,Gps_List[0].angle);
	
	while(MATH_Distance(Gps_List[0].position,aim)>20)
	//while(Gps_List[0].position.y<aim.y)
	{OSTimeDly(1);}
	RouteForm=RouteStop;
	
	
	goto route;	
	
/*->*/owen_curve:

	DISPLAY_ValuesManager(Values,6,PID_NANDPAGE);

	GPS_Clear();
	PID_Clear();
	//Gps_List[0].angle=-45;
	
	BackDataInit();
	
	owen_test.x=166.18;
	owen_test.y=-392.46;
	
	//aim.x=-497.86839;
	//aim.y=1841.43593;
	
	GPS_Init(PointRoute[Route[0]].position,(PointRoute[Route[0]].lineangle-PI/2)/PI*180);
	
	SetPointPath(Route[0],Route[9],PointRoute[Route[9]].lineangle,PointRoute[Route[9]].position);
	
	while(Point_NowNum<Route[9]-2)
	{OSTimeDly(1);}
	RouteForm=RouteStop;
	
	goto route;


/*->*/owen_keep:
	
	
	GPS_Clear();
	PID_Clear();
	
	DISPLAY_Fp64Value(&keep_aim.x,"AimX");
	DISPLAY_Fp64Value(&keep_aim.y,"AimY");
	DISPLAY_Fp64Value(&keep_radian,"AimRadian");
	
	
	BackDataInit();
	
	SetKeep(keep_aim,keep_radian);
	
	//while(Gps_List[0].position.x<keep_aim.x && Gps_List[0].position.y<keep_aim.y && Gps_List[0].radian<keep_radian)
	while(MATH_Distance(Gps_List[0].position,keep_aim)>20)
	
	{OSTimeDly(20);}
	RouteForm=RouteStop;
	
	LCD_Clear();;
	LCD_WriteString("done");
	
	goto route;	
}




void Error_Show(void)
{
	uint8 *msg;

	LCD_Clear();
	for(;;)
	{
		LCD_SetXY(0,0);
		LCD_WriteString("M_FL:");
		LCD_WriteNum(error_convert[0]);
		LCD_WriteString("        ");
		LCD_SetXY(0,1);
		LCD_WriteString("A_FL:");
		LCD_WriteNum(error_convert[1]);
		LCD_WriteString("        ");
		LCD_SetXY(0,2);
		LCD_WriteString("M_FR:");
		LCD_WriteNum(error_convert[2]);
		LCD_WriteString("        ");
		LCD_SetXY(0,3);
		LCD_WriteString("A_FR:");
		LCD_WriteNum(error_convert[3]);
		LCD_WriteString("        ");
		LCD_SetXY(0,4);
		LCD_WriteString("M_BL:");
		LCD_WriteNum(error_convert[4]);
		LCD_WriteString("        ");
		LCD_SetXY(0,5);
		LCD_WriteString("A_BL:");
		LCD_WriteNum(error_convert[5]);
		LCD_WriteString("        ");	
		LCD_SetXY(0,6);
		LCD_WriteString("M_BR:");
		LCD_WriteNum(error_convert[6]);
		LCD_WriteString("        ");
		LCD_SetXY(0,7);
		LCD_WriteString("A_BR:");
		LCD_WriteNum(error_convert[7]);
		LCD_WriteString("        ");
		LCD_SetXY(9,7);
		LCD_WriteString("autoflag2:");
		LCD_WriteNum(error_convert[8]);
		
		LCD_SetXY(9,6);
		LCD_WriteString("bp1:");
		LCD_WriteNum(error_convert[9]);
		LCD_SetXY(9,5);
		LCD_WriteString("autoflag1:");
		LCD_WriteNum(error_convert[11]);
		LCD_SetXY(9,4);
		LCD_WriteString("bp2:");
		LCD_WriteNum(error_convert[10]);



		
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			
			case 4:
				GPS_Clear();
			break;
		}
	}

 

}

void Retry(void)
{
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Keep away!");
	Motor_CAN(1,zheng);
	Motor_CAN(3,fan);
	Motor_CAN(4,zheng);
	OSTimeDly(4000);
	Motor_CAN(1,STOP);
	Motor_CAN(3,STOP);
	Motor_CAN(4,STOP);
	LCD_SetXY(0,1);
	LCD_WriteString("Done!");
}

void Ringtest(void)
{
	uint8 *msg;
	for(;;)
	{
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("1.on");
	LCD_SetXY(0,1);
	LCD_WriteString("2.off");
	OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			case 1:
				rGPJDAT|=0x1000;     //开蜂鸣器
			break;
			case 2:
				rGPJDAT&=~0x1000;    //关蜂鸣器
			break;
			
		}
	OSTimeDly(1);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//系统设备参数函数////////////////////////////////////////
////////////////GYRO/////////////////////////////////////
void sys_Gyro( void )
{
	uint8 err;
	uint8 *msg;
	int32 i;
	extern int64 temp_data[2000];//,temp_data_num;
	struct Gyro_Struct temp_Now,temp_Last;

/*->*/gyro:
	
	LCD_Clear();
	
	for(;;)//for循环的作用是，如果msg没得到需要的值，则重新等判断
	{
	
		LCD_SetXY(0,0);
		LCD_WriteString("Ang:");
		LCD_WriteNum(Gps_List[0].angle);
		LCD_SetXY(0,1);
		LCD_WriteString("Con1:");
		LCD_WriteDouble(Gyro_Convert1,13);
		LCD_SetXY(0,2);
		LCD_WriteString("Con2:");
		LCD_WriteDouble(Gyro_Convert2,13);
		LCD_SetXY(0,3);
		LCD_Printf("1.Init 2.SetC 3.float 4.Tes");
	
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			case 1:
				goto initgyro;
			break;
			case 2:
				DISPLAY_Fp64Value(&Gyro_Convert1,"Gyro Convert1");
				DISPLAY_Fp64Value(&Gyro_Convert2,"Gyro Convert2");
				Gyro_Roll=(3600.0)*10000000.0/((Gyro_Convert1+Gyro_Convert2)/2);
				if(Gyro_Roll<0)
				   Gyro_Roll=-Gyro_Roll;
				GYRO_NandSave();
			break;
			case 3:
				goto checkfloat;
			break;
			case 4:
			    TestGyro();
			break;
			case 7:
			    DISPLAY_Fp64Value(&Gyro_Error_Float,"Gyro_Error_Float");
			break;
			case 8:				
				for (i=0;i<1000;i++)
				{
					temp_Now=FPGA_ReadGyro();
					temp_data[i]=temp_Now.value-temp_Last.value;//+Gyro_Error_Float;//1152 往大调是负偏
					temp_Last=temp_Now;	
					OSTimeDly(1);
				}
				for (i=0;i<1000;i++)
				{
					PC_WriteInt(temp_data[i]);
					PC_WriteChar(',');
				}
				PC_WriteChar(';');
				PC_WriteChar('\n');
			break;
		}
	}
	
/*->*/initgyro:
	LCD_Clear();
	Gyro_Total=0;
	GPS_Clear();
	
	for(;;)
	{
		
	    LCD_SetXY(0,0);
	    LCD_WriteNum(Gps_List[0].angle);
	    LCD_SetXY(0,1);
		LCD_Printf("Set Direction and ");
		LCD_Printf("press any key...");
		
	    
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);					
			
		if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
		{
			break;
		}
	}
	
	
	for(;;)
	{
	    LCD_Clear();
		LCD_SetXY(0,0);
		LCD_Printf("Please Turn +3600 degrees and press any key...");
		LCD_SetXY(0,3);
		LCD_Printf("total:%d",Gyro_Total);
			
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		
		RouteForm=RouteFree;
					
		Speed_F= -5.53859*50;
		Speed_FR=-5.53859*50;
		Speed_BL= 0.28648*50;
		Speed_BR=-0.28648*50;
		
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
	}
	
    for(;;)
	{
		LCD_Clear();
	    LCD_SetXY(0,0);
		LCD_Printf("press any key...");
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		
		Speed_F= 0;
		Speed_FR=0;
		Speed_BL=0;
		Speed_BR=0;
			
		msg = OSMboxAccept(AppKey2Mbox);					
			
		if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
		{
			break;
		}
	}
	LCD_Clear();

	LCD_SetXY(0,0);
	LCD_Printf("Old_C1:");
	LCD_WriteDouble(Gyro_Convert1,13);
	LCD_SetXY(0,1);
	LCD_Printf("New:");
	LCD_WriteDouble(3600.0*10000000.0/(fp64)Gyro_Total,13);
	LCD_SetXY(0,3);
	LCD_Printf("1:Save   Other:Exit");
			
	msg = OSMboxPend(AppKey1Mbox,0,&err);

	if(*msg==1)
	{
		Gyro_Convert1=(3600.0)*10000000.0/(fp64)Gyro_Total;
		Gyro_Roll=Gyro_Total/10;
		if(Gyro_Roll<0)
			Gyro_Roll=-Gyro_Roll;
		GYRO_NandSave();
	}
	
	Gyro_Total=0;
	GPS_Clear();
	LCD_Clear();
	for(;;)
	{
		
	    LCD_SetXY(0,0);
		LCD_Printf("Set Direction and ");
		LCD_Printf("press any key...");
		LCD_SetXY(0,2);
	    LCD_WriteNum(Gps_List[0].angle);
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);					
			
		if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
		{
			break;
		}
	}
	
	LCD_Clear();
	for(;;)
	{
		LCD_SetXY(0,0);
		LCD_Printf("Please Turn -3600 degrees and press any key...");
		LCD_SetXY(0,3);
		LCD_Printf("total:%d",Gyro_Total);
			
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
		//Speed_Motor(35,-35,35,-35);
		
		RouteForm=RouteFree;
			
			
		Speed_F=  5.53859*50;
		Speed_FR= 5.53859*50;
		Speed_BL=-0.28648*50;
		Speed_BR= 0.28648*50;
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
		{
			Speed_F= 0;
			Speed_FR=0;
			Speed_BL=0;
			Speed_BR=0;
			break;
		}
	}
    for(;;)
	{
		LCD_Clear();
	    LCD_SetXY(0,0);
		LCD_Printf("press any key...");
        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);					
			
		if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
		{
			break;
		}
	}

	LCD_Clear();

	LCD_SetXY(0,0);
	LCD_Printf("Old_C2:");
	LCD_WriteDouble(Gyro_Convert2,13);
	LCD_SetXY(0,1);
	LCD_Printf("New:");
	LCD_WriteDouble(-3600.0*10000000.0/(fp64)Gyro_Total,13);
	LCD_SetXY(0,3);
	LCD_Printf("1:Save   Other:Exit");
			
	msg = OSMboxPend(AppKey1Mbox,0,&err);

	if(*msg==1)
	{
		Gyro_Convert2=(-3600.0)*10000000.0/(fp64)Gyro_Total;
		Gyro_Roll=Gyro_Total/10;
		if(Gyro_Roll<0)
			Gyro_Roll=-Gyro_Roll;
		GYRO_NandSave();
	}
	goto gyro;
	
	
/*->*/checkfloat:
		
	LCD_Clear();
	GYRO_CheckFloat();

	goto gyro;
}
//////////////////////////////////////////////////////////////
/////////////////ENCODER//////////////////////////////////////
void sys_encoder(void)
{
	uint8 err;
	uint8 *msg;

/*->*/encoder:
	LCD_Clear();
//              12345678901234567890
//              ||||||||||||||||||||
	LCD_SetXY(0,0);
	LCD_Printf("======ENCODER=======");
	LCD_SetXY(0,1);
	LCD_Printf("1.Two Encoders init");
	LCD_SetXY(0,2);
	LCD_Printf("2.View  3.Set");
	LCD_SetXY(0,3);
	LCD_Printf("4.EN_angle");
	LCD_SetXY(0,4);
	LCD_Printf("5.Encoders3-6");
		
	for(;;)
	{		
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				goto two_encoders;
			break;
			case 2:
				goto  View;
			break;
			case 3:
				goto  Set;
			break;
			case 4:
				goto  EN_angle;
			break;
			case 7:
				goto other;
			break;
			
		}

	}
	
/*->*/two_encoders:
	
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_Printf("======ENCODER=======");
	LCD_SetXY(0,1);
	LCD_Printf("Dis1:");
	LCD_SetXY(0,2);
	LCD_Printf("Dis2:");
	LCD_SetXY(0,3);
	LCD_Printf("1.Init_C 2.Init_R");
	for(;;)
	{
		LCD_SetXY(5,1);
	    LCD_WriteDouble(Gps_List[0].distance1,6);
	    LCD_SetXY(5,2);
	    LCD_WriteDouble(Gps_List[0].distance2,6);
	
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			case 1:
			 	ENCODER_Init(0);
			    break;
			
			case 2:
			 	ENCODER_Ac_Init(0);
			    break;
			
			case 4:
				goto EN_angle;
			    break;
			
			case 5:
				goto encoder;
			    break;
			
		}
	}
	
/*->*/other:
	
	LCD_Clear();
	
	for(;;)
	{
		LCD_SetXY(0,0);
		LCD_WriteDouble(Encoders[2].Total,3);
		LCD_SetXY(0,1);
	    LCD_WriteDouble(Encoders[3].Total,3);
		LCD_SetXY(0,2);	
		LCD_WriteDouble(Encoders[4].Total,3);
		LCD_SetXY(0,3);	
	    LCD_WriteDouble(Encoders[5].Total,3);
		
		OSTimeDly(1);
	}
	

/*->*/View:

        LCD_Clear();
		LCD_SetXY(0,0);
		LCD_Printf("C11:");
    	LCD_WriteDouble(Encoders[0].Convert1,6);
	   	LCD_SetXY(0,1);
		LCD_Printf("C21:");
		LCD_WriteDouble(Encoders[1].Convert1,6);
		LCD_SetXY(0,2);
		LCD_Printf("C12:");
	    LCD_WriteDouble(Encoders[0].Convert2,6);
	    LCD_SetXY(0,3);
		LCD_Printf("C22:");
		LCD_WriteDouble(Encoders[1].Convert2,6);
	    LCD_SetXY(0,4);
        LCD_Printf("Angle_a:");
		LCD_WriteDouble(angle_a,6);
		LCD_SetXY(0,5);
        LCD_Printf("Angle_b:");
		LCD_WriteDouble(angle_b,6);
		LCD_SetXY(0,6);
		LCD_Printf("Ac1:");
	    LCD_WriteDouble(Ac1,6);
	    LCD_SetXY(0,7);
		LCD_Printf("Ac2:");
		LCD_WriteDouble(Ac2,6);
	    for(;;)
	    {
			
	        OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		    OSTimeDly(20);
			
			msg = OSMboxAccept(AppKey2Mbox);					
			
			if(*msg!=*(uint8*)0)
			goto encoder;
	    }
	
/*->*/Set:
		LCD_Clear();
		DISPLAY_Fp64Value(&(Encoders[0].Convert1),"Convert11");
		DISPLAY_Fp64Value(&(Encoders[1].Convert1),"Convert21");
		DISPLAY_Fp64Value(&(Encoders[0].Convert2),"Convert12");
		DISPLAY_Fp64Value(&(Encoders[1].Convert2),"Convert22");
		DISPLAY_Fp64Value(&(Ac1),"Ac1");
		DISPLAY_Fp64Value(&(Ac2),"Ac2");
		DISPLAY_Fp64Value(&(angle_a),"Angle_a");
		DISPLAY_Fp64Value(&(angle_b),"Angle_b");

		ENCODER_NandSave();
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);					
			
		if(*msg!=*(uint8*)0)
		goto encoder;
		
/*->*/EN_angle:
	for(;;)     
   {     
        LCD_SetXY(0,0);
		LCD_Printf("======EN_Angle=======");
		LCD_SetXY(0,1);
		LCD_Printf("angle_a");
	    LCD_WriteDouble(angle_a,3);
	    LCD_WriteString("        ");
		LCD_SetXY(0,2);
		LCD_Printf("angle_b");
		LCD_WriteDouble(angle_b,3);
		LCD_WriteString("        ");
		LCD_SetXY(0,3);
		LCD_Printf("1.seta 2.setb");
		
	OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			case 2:
			    DISPLAY_Fp64Value(&(angle_b),"angle_b");
			    sinb=sin(angle_b);
			    cosb=cos(angle_b);
			    sinab=sin(angle_a+angle_b);
				ENCODER_NandSave();
			break;
			case 1:
			 	DISPLAY_Fp64Value(&(angle_a),"angle_a");
			 	sina=sin(angle_a);
			 	cosa=cos(angle_a);
			 	sinab=sin(angle_a+angle_b);
				ENCODER_NandSave();
			break;
			case 5:
				goto encoder;
			break;
		}
   }			

}
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////GPS/////////////////////////////////////
void sys_gps(void)
{
	uint8 *msg;
	fp64 d;

	LCD_Clear();
	for(;;)
	{
		LCD_SetXY(0,0);
		LCD_WriteString("1A:");
		LCD_WriteNum(Gps_List[0].angle);
		LCD_WriteString("        ");
		LCD_SetXY(0,1);
		LCD_WriteString("2X:");
		LCD_WriteNum(Gps_List[0].position.x);
		LCD_WriteString("        ");
		LCD_SetXY(0,2);
		LCD_WriteString("3Y:");
		LCD_WriteNum(Gps_List[0].position.y);
		LCD_WriteString("        ");
		LCD_SetXY(0,3);
		LCD_Printf("4Clear");
		LCD_SetXY(0,4);
		LCD_Printf("5Modulation");
		

		
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			case 1:
				DISPLAY_Fp64Value(&Gps_List[0].angle,"A");
				GPS_Init(Gps_List[0].position,Gps_List[0].angle);
			break;
			case 2:
				DISPLAY_Fp64Value(&Gps_List[0].position.x,"X");
				GPS_Init(Gps_List[0].position,Gps_List[0].angle);
			break;
			case 3:		
				DISPLAY_Fp64Value(&Gps_List[0].position.y,"Y");
				GPS_Init(Gps_List[0].position,Gps_List[0].angle);
			break;
			case 4:
				GPS_Clear();
			break;
			case 7:
			    DISPLAY_Fp64Value(&d,"d");
			    GPSModulation(d);
			break;
		}
	}
}
////////////////////////////////////////////////////////////////
/////////////////////////MECANUM////////////////////////////////
void sys_mecanum()
{
   	uint8 err;
	uint8 *msg;
	
/*->*/mecanum:
	LCD_Clear();

	LCD_SetXY(0,0);
	LCD_Printf("=======MOTOR========");
	LCD_SetXY(0,1);
	LCD_Printf("1FL port%d",Motor_F);
	LCD_SetXY(10,1);
	LCD_Printf("2FR port%d",Motor_FR);
	LCD_SetXY(0,2);
	LCD_Printf("3BL port%d",Motor_BL);
	LCD_SetXY(10,2);
	LCD_Printf("4BR port%d",Motor_BR);
		
	for(;;)
	{		
	    msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				goto FL;
			break;
			case 2:
				goto FR;
			break;
			case 3:
				goto BL;
			break;
			case 4:
				goto BR;
			break;
		}
	}
	
/*->*/BL:
	for(;;)
	{	
	
		LCD_Clear();
	
		LCD_SetXY(0,0);
		LCD_Printf("1.SpeedMode");
		LCD_SetXY(0,1);
		LCD_Printf("2.ChangeArgu");
		LCD_SetXY(0,2);
		LCD_Printf("3.SetPort now:%d",Motor_BL);
		
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				Mecanum_SpeedMode(Motor_BL,MotorBL_Argument);
			break;
			case 2:
				DISPLAY_Fp64Value(&(MotorBL_Argument),"MotorBL_Argument");
				Mecanum_NandSave();
			break;
			case 3:
				LCD_Clear();
				
				for(;;)
				{
					LCD_SetXY(0,0);
					LCD_Printf("Now Port:%d",Motor_BL);
					LCD_SetXY(0,1);
					LCD_Printf("Input Port:");
									
					msg = OSMboxPend(AppKey1Mbox,0,&err);
					if(*msg<=3)
					{
						Motor_BL=*msg;
						break;
					}
					else if(*msg==14)
					{
						Motor_BL=0;
						break;
					}
				}
				Mecanum_NandSave();
				LCD_WriteNum(Motor_BL);
				LCD_SetXY(0,3);
				LCD_WriteString("Press a key to skip.");
				OSMboxPend(AppKey1Mbox,0,&err);
			break;
			case 5:
				goto mecanum;
			break;
		}
	}
	
/*->*/BR:
	for(;;)
	{
		LCD_Clear();

		LCD_SetXY(0,0);
		LCD_Printf("1.SpeedMode");
		LCD_SetXY(0,1);
		LCD_Printf("2.ChangeArgu");
		LCD_SetXY(0,2);
		LCD_Printf("3.SetPort now:%d",Motor_BR);
		
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				Mecanum_SpeedMode(Motor_BR,MotorBR_Argument);
			break;
			case 2:
				DISPLAY_Fp64Value(&(MotorBR_Argument),"MotorBR_Argument");
				Mecanum_NandSave();
			break;
			case 3:
				LCD_Clear();
				
				for(;;)
				{
					LCD_SetXY(0,0);
					LCD_Printf("Now Port:%d",Motor_BR);
					LCD_SetXY(0,1);
					LCD_Printf("Input Port:");
						
					msg = OSMboxPend(AppKey1Mbox,0,&err);			
					if(*msg<=3)
					{
						Motor_BR=*msg;
						break;
					}
					else if(*msg==14)
					{
						Motor_BR=0;
						break;
					}
				}
				Mecanum_NandSave();
				LCD_WriteNum(Motor_BR);
				LCD_SetXY(0,3);
				LCD_WriteString("Press a key to skip.");
				OSMboxPend(AppKey1Mbox,0,&err);
			break;
			case 5:
				goto mecanum;
			break;
		}
	}
	
/*->*/FL:
	for(;;)
	{	
		LCD_Clear();

		LCD_SetXY(0,0);
		LCD_Printf("1.SpeedMode");
		LCD_SetXY(0,1);
		LCD_Printf("2.ChangeArgu");
		LCD_SetXY(0,2);
		LCD_Printf("3.SetPort now:%d",Motor_F);
		
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				Mecanum_SpeedMode(Motor_F,MotorF_Argument);
			break;
			case 2:
				DISPLAY_Fp64Value(&(MotorF_Argument),"MotorF_Argument");
				Mecanum_NandSave();
			break;
			case 3:
				LCD_Clear();
				
				for(;;)
				{
					LCD_SetXY(0,0);
					LCD_Printf("Now Port:%d",Motor_F);
					LCD_SetXY(0,1);
					LCD_Printf("Input Port:");
									
					msg = OSMboxPend(AppKey1Mbox,0,&err);
					if(*msg<=3)
					{
						Motor_F=*msg;
						break;
					}
					else if(*msg==14)
					{
						Motor_F=0;
						break;
					}
				}
				Mecanum_NandSave();
				LCD_WriteNum(Motor_F);
				LCD_SetXY(0,3);
				LCD_WriteString("Press a key to skip.");
				OSMboxPend(AppKey1Mbox,0,&err);
			break;
			case 5:
				goto mecanum;
			break;
		}
	}
/*->*/FR:
	for(;;)
	{	
		LCD_Clear();

		LCD_SetXY(0,0);
		LCD_Printf("1.SpeedMode");
		LCD_SetXY(0,1);
		LCD_Printf("2.ChangeArgu");
		LCD_SetXY(0,2);
		LCD_Printf("3.SetPort now:%d",Motor_FR);
		
		msg = OSMboxPend(AppKey1Mbox,0,&err);
		switch(*msg)
		{
			case 1:
				Mecanum_SpeedMode(Motor_FR,MotorFR_Argument);
			break;
			case 2:
				DISPLAY_Fp64Value(&(MotorFR_Argument),"MotorFR_Argument");
				Mecanum_NandSave();
			break;
			case 3:
				LCD_Clear();
				
				for(;;)
				{
					LCD_SetXY(0,0);
					LCD_Printf("Now Port:%d",Motor_FR);
					LCD_SetXY(0,1);
					LCD_Printf("Input Port:");
									
					msg = OSMboxPend(AppKey1Mbox,0,&err);
					if(*msg<=3)
					{
						Motor_FR=*msg;
						break;
					}
					else if(*msg==14)
					{
						Motor_FR=0;
						break;
					}
				}
				Mecanum_NandSave();
				LCD_WriteNum(Motor_FR);
				LCD_SetXY(0,3);
				LCD_WriteString("Press a key to skip.");
				OSMboxPend(AppKey1Mbox,0,&err);
			break;
			case 5:
				goto mecanum;
			break;
		}
	}
}
//////////////////////////////////////////////////////////////////////
////////////////////////SA_ST/////////////////////////////////////////
//进入函数可看到全部接近开关和接触开关的状态

void sys_SA_ST(void)
{
	uint32 st_count,sa_count;
    LCD_Clear();
	LCD_SetXY(0,0);
	LCD_Printf("====SA_ST_Test====");
	for(;;)
	{
	    for(st_count = 1; st_count<7 ;st_count ++){
			LCD_SetXY(((st_count-1)%4)*5,1+(st_count-1)/4);
			LCD_Printf("T%d:%d",st_count,ST_Read(st_count));
		}
		
		for(sa_count = 1; sa_count<13 ;sa_count ++){
			LCD_SetXY(((sa_count-1)%4)*5,4+(sa_count-1)/4);
			LCD_Printf("A%d:%d",sa_count,SA_Read(sa_count));
		}
		OSTimeDly(20);
	}
}
///////////////////////////////////////////////////////////////////////
///////////////////MANUAL DATA////////////////////////////////
void manualdata( void )
{
	#if ManualSetting
	for(;;)
	{	
      
	  LCD_WriteStringXY("                   ",0,0);
	  LCD_SetXY(0,0);
	  LCD_Printf("%x",ManualData[1]);
	  LCD_SetXY(4,0);
	  LCD_Printf("%x",ManualData[2]);
	  LCD_SetXY(8,0);
	  LCD_Printf("%x",ManualData[3]);
	  LCD_SetXY(12,0);
	  LCD_Printf("%x",ManualData[4]);
	  LCD_SetXY(16,0);
	  LCD_Printf("%x",ManualData[5]);
	  
	  LCD_WriteStringXY("                   ",0,1);
	  LCD_SetXY(0,1);
	  LCD_Printf("%x",ManualData[6]);
	  LCD_SetXY(4,1);
	  LCD_Printf("%x",ManualData[7]);
	  LCD_SetXY(8,1);
	  LCD_Printf("%x",ManualData[8]);
	  LCD_SetXY(12,1);
	  LCD_Printf("%x",ManualData[9]);
	  LCD_SetXY(16,1);
	  LCD_Printf("%x",ManualData[10]);
	  
	  OSTimeDly(20);
  	}
#else
	LCD_SetXY(0,0);
	LCD_WriteString("no manual");
#endif
}
///////////////////////////////////////////////////////////////
//////////////////////RELAY///////////////////////////////////
void sys_Relay(void)
{
	uint8 *msg;
	uint8 i,l,page=0; 
	
	
	
	for(;;)
	{
		LCD_Clear();
		if(Relay_Value[0]==0 && Relay_Value[1]==0){
			LCD_SetXY(0,6);
			LCD_Printf("ClearAll");
		}
		if(Relay_Value[0]==0xffffffff && Relay_Value[1]==0xffffffff){
			LCD_SetXY(0,6);
			LCD_Printf("SetAll  ");
		}
	    if(page < 6){
			//l表示列数，共有2列
			for(l=0;l<2;l++)
			{           
				for(i=1;i<5;i++)
				{
					LCD_SetXY(l*10,i);
					LCD_WriteInt(i+l*4+8*page);
					if(Relay_Read(i+l*4+8*page))
						LCD_WriteString("set  ");
					else
						LCD_WriteString("clear");
				}
			}
		}
	    else {
	        LCD_SetXY(0,2);
	        LCD_Printf("1.SetALL  2.ClearAll");
	    }	
	    LCD_SetXY(0,0);
	    LCD_Printf("=====Relay_Test=====");
		LCD_SetXY(17,4);
		LCD_WriteInt(page+1);
		LCD_WriteString("/7");

		OSMboxAccept(AppKey2Mbox);		
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);		   		    
 
		switch(*msg)
		{     
			case 1:
            if(page < 6){
				if(Relay_Read(1+8*page))
					Relay_Clear(1+8*page);
				else
					Relay_Set(1+8*page);
            }
            else {
                Relay(0xffffffff,0xffffffff,0,0);
            }
			break;
			case 2:
            if(page < 6){
				if(Relay_Read(2+8*page))
					Relay_Clear(2+8*page);
				else
					Relay_Set(2+8*page);
            }
            else {
                Relay(0,0,0xffffffff,0xffffffff);
            }
			break;
			case 3:
				if(Relay_Read(3+8*page))
					Relay_Clear(3+8*page);
				else
					Relay_Set(3+8*page);
			break;
			case 4:
				if(Relay_Read(4+8*page))
					Relay_Clear(4+8*page);
				else
					Relay_Set(4+8*page);
			break;
			case 7:
				if(Relay_Read(5+8*page))
					Relay_Clear(5+8*page);
				else
					Relay_Set(5+8*page);
			break;
			case 8:
				if(Relay_Read(6+8*page))
					Relay_Clear(6+8*page);
				else
					Relay_Set(6+8*page);
			break;
			case 9:
				if(Relay_Read(7+8*page))
					Relay_Clear(7+8*page);
				else
					Relay_Set(7+8*page);
			break;
			case 10:
				if(Relay_Read(8+8*page))
					Relay_Clear(8+8*page);
				else
					Relay_Set(8+8*page);
			break;
			case 13:
			case 14:
			case 15:
				if(page==0)
					page=7;
				page--;
			break;
			case 16:
				page++;
				if(page==7)
					page=0;
			break;
		}
		if(*msg ==5){   //如果等于上一页，则跳出循环
			break;
		}
	}	
}
//////////////////////////////////////////////////////////////
///////////////////steer/////////////////////////////////////////
void steertest(void)
{
	uint8 *msg;
	uint8 err;
	LCD_WriteString("Please input port number:");
	msg = OSMboxPend(AppKey1Mbox,0,&err);
	Steer_SpeedMode(*msg);

}



/*************************************************************************************
** 从这里开始往下都是有关CAN总线测试的函数，
** 分别为：激光测距 1个，红外测距 2个，超声波测距3个，还有继电器  和 光隔板的屏蔽
**		   共五个内容 
**	几个测距的仪器可以选择开始或停止向CAN总线传送数据
**  很容易就看懂了，注释就不写了哈
***************************************************************************************/	


void sys_Laser(void)
{
	uint8  *msg;
		
	while(1){
		LCD_Clear();
		LCD_Printf("====Laser_Test=====");
		LCD_SetXY(2,1);
		LCD_Printf("1.Laser:");	
		LCD_SetXY(3,2);
		LCD_Printf("Dist: %d",STM32_ReadDistance(ID_LASER_DATA));

		
		
		if(Read_Device_Statu(CONTROL_LASER))
		{	
			LCD_SetXY(10,1);
			LCD_Printf("BEGIN");
		}
		else{
			LCD_SetXY(10,1);
			LCD_Printf("STOP  ");
		}
		
		LCD_SetXY(2,4);
		LCD_Printf("2.ResetSTM32");
	
		
		OSMboxAccept(AppKey2Mbox);					
		OSTimeDly(20);			
		msg = OSMboxAccept(AppKey2Mbox);
		switch (*msg)
		{
			case 1: if(Read_Device_Statu(CONTROL_LASER) == 0){
						Ranging_Start(CONTROL_LASER);
					}else {
						Ranging_Stop(CONTROL_LASER);
					}
			break;
			case 2:
					STM32_Reset();
			break;
			default:
					LCD_SetXY(0,5);
					LCD_Printf("err");
			break;
		}
		if(*msg ==5){
			break;
		}
	}
}


void sys_IR_test(void)
{
	uint8  *msg;
		
	while(1){
		LCD_Clear();
		LCD_Printf("=====IR_Test======");
		LCD_SetXY(0,1);
		LCD_Printf("1.IR1 :");
	
		LCD_SetXY(2,2);
		LCD_Printf("Dist:%d",STM32_ReadDistance(ID_IR1_DATA));
		if(Read_Device_Statu(CONTROL_IR1))
		{	
			LCD_SetXY(8,1);
			LCD_Printf("begin");
		}
		else{
			LCD_SetXY(8,1);
			LCD_Printf("stop  ");
		}
		LCD_SetXY(0,3);
		LCD_Printf("2.IR2:");	
		LCD_SetXY(2,4);	
		
		if(Read_Device_Statu(CONTROL_IR2))
		{	
			LCD_SetXY(8,3);
			LCD_Printf("begin");
		}
		else{
			LCD_SetXY(8,3);
			LCD_Printf("stop  ");
		}
		
		LCD_SetXY(0,5);
		LCD_Printf("3.ResetSTM32");	
		
		
		LCD_SetXY(2,4);
		LCD_Printf("Dist:%d",STM32_ReadDistance(ID_IR2_DATA));

		
		OSMboxAccept(AppKey2Mbox);					
		OSTimeDly(20);			
		msg = OSMboxAccept(AppKey2Mbox);
		switch (*msg)
		{
			case 1: if(Read_Device_Statu(CONTROL_IR1)  == 0){
						Ranging_Start(CONTROL_IR1);
					}else{
						Ranging_Stop(CONTROL_IR1);
					}
			break;
			case 2:	if(Read_Device_Statu(CONTROL_IR2)  == 0){
						Ranging_Start(CONTROL_IR2);
					}else{
						Ranging_Stop(CONTROL_IR2);
					}
					
			break;
			case 3:
					STM32_Reset();
			break;
			default:
					LCD_SetXY(0,7);
					LCD_Printf("err");
			break;
		}
		if(*msg ==5){
			break;
		}
	}
}


void sys_sonic_test(void)
{
	uint8  *msg,i;
		
	while(1){
		LCD_Clear();
		LCD_Printf("=====Sonic_Test=====");
		
		for(i=0;i<3;i++){
			LCD_SetXY(0,1+i*2);
			LCD_Printf("%d.Statu:",i+1);
		
			LCD_SetXY(2,2+i*2);
			LCD_Printf("Dist:%d",STM32_ReadDistance(ID_SUP_SONIC1_DATA+i));
		}
		
		
		LCD_SetXY(9,1);		
		if(Read_Device_Statu(CONTROL_SONIC1)){	
			LCD_Printf("Begin");
		}
		else{
			LCD_Printf("Stop ");
		}
		
		LCD_SetXY(9,3);
		if(Read_Device_Statu(CONTROL_SONIC2)){	
			LCD_Printf("Begin");
		}
		else{
			LCD_Printf("Stop ");
		}
		
		LCD_SetXY(9,5);
		if(Read_Device_Statu(CONTROL_SONIC3)){	
			LCD_Printf("Begin");
		}
		else{
			LCD_Printf("Stop ");
		}
		
		LCD_SetXY(0,7);
		LCD_Printf("4.ResetSTM32");

		OSMboxAccept(AppKey2Mbox);					
		OSTimeDly(20);			
		msg = OSMboxAccept(AppKey2Mbox);
		switch (*msg)
		{
			case 1: if(Read_Device_Statu(CONTROL_SONIC1) == 0){
						Ranging_Start(CONTROL_SONIC1);
					}else{
						Ranging_Stop(CONTROL_SONIC1);
					}
			break;
			case 2:	if(Read_Device_Statu(CONTROL_SONIC2) == 0){
						Ranging_Start(CONTROL_SONIC2);
					}else{
						Ranging_Stop(CONTROL_SONIC2);
					}
					
			break;
			case 3:	if(Read_Device_Statu(CONTROL_SONIC3) == 0){
						Ranging_Start(CONTROL_SONIC3);
					}else{
						Ranging_Stop(CONTROL_SONIC3);
					}
					
			break;
			case 4:
					STM32_Reset();
			break;
			default:
					LCD_SetXY(0,7);
					LCD_Printf("press 1  2  3");
			break;
		}
		if(*msg ==5){
			break;
		}
	}
}

void sys_MASK_ISO(void)
{
	uint8  *msg,l,page=0 ,i;
	static uint8  iso_send_count[16];//这个数组没什么意思，是用来统计enable 的次数并显示出来，
									 //让我们知道程序运行了
	extern void ISO_Delay(uint8 port , uint16 delay_time);
	for(;;)
	{	
		LCD_Clear();
		LCD_Printf("======ISO_MASK======");
				
	    if(page < 2){
			//l表示列数，共有2列,和继电器测试类似
	        
			for(l=0;l<2;l++)
			{           
				for(i=0;i<4;i++)
				{
					LCD_SetXY(l*10,i+1);
					LCD_Printf("SA%d %d",l*4+i+1+8*page,iso_send_count[l*4+i+1+8*page]);  
				}
			}
		}
	    else if(page == 2){
	    	LCD_SetXY(0,1);
	    	LCD_Printf("Delay Time:");
	    	LCD_SetXY(0,2);
	    	LCD_Printf("1. Leaves 1");
	    	LCD_SetXY(0,3);
	    	LCD_Printf("2. Leaves 2");
	    	LCD_SetXY(0,4);
	    	LCD_Printf("3. Leaves 3");
	    }	
		LCD_SetXY(17,4);
		LCD_WriteInt(page+1);
		LCD_WriteString("/3");

		OSMboxAccept(AppKey2Mbox);		
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);		   		    
 
		switch(*msg)
		{     
			case 1:
            if(page <2){
				ISO_UNMASK(ISO(1+8*page));
				iso_send_count[1+8*page] ++;
            }
            else if(page == 2){
                if(DISPLAY_INT16Value(&ISO_Delay_Time[0],"ISO_Delay1"))
                	ISO_Delay(1,ISO_Delay_Time[0]);
            }
			break;
			case 2:
             if(page < 2){
				ISO_UNMASK(ISO(2+8*page));
				iso_send_count[2+8*page] ++;
            }
            else if(page == 2){
                if(DISPLAY_INT16Value(&ISO_Delay_Time[1],"ISO_Delay2"))
                	ISO_Delay(2,ISO_Delay_Time[1]);
            }
			break;
			case 3:
				 if(page < 2){
				ISO_UNMASK(ISO(3+8*page));
				iso_send_count[3+8*page] ++;
            }
            else if(page == 2){
                if(DISPLAY_INT16Value(&ISO_Delay_Time[2],"ISO_Delay3"))
                	ISO_Delay(3,ISO_Delay_Time[2]);
            }
			break;
			case 4:
				
				ISO_UNMASK(ISO(4+8*page));
				iso_send_count[4+8*page] ++;
			break;
			case 7:
				
					ISO_UNMASK(ISO(5+8*page));
					iso_send_count[5+8*page] ++;
			break;
			case 8:
				
					ISO_UNMASK(ISO(6+8*page));
					iso_send_count[6+8*page] ++;
			break;
			case 9:
				
					ISO_UNMASK(ISO(7+8*page));
					iso_send_count[7+8*page] ++;
			break;
			case 10:
				
					ISO_UNMASK(ISO(8+8*page));
					iso_send_count[8+8*page] ++;
			break;
			case 13:
			case 14:
			case 15:
				if(page==0)
					page=3;
				page--;
			break;
			case 16:
				page++;
				if(page==3)
					page=0;
			break;
		}
		if(*msg ==5){   //如果等于上一页，则跳出循环
			break;
		}
		
	}	
}



void sys_motor(void)
{
	uint8 *msg,temp;
	fp64 Rotation;
	
	while (1){
	
		LCD_Clear();
		LCD_Printf("======MOTOR_Test======");	
		LCD_SetXY(0,1);
		LCD_Printf("1.Test motor 1");	
		LCD_SetXY(0,2);
		LCD_Printf("2.Test motor 2");	
		LCD_SetXY(0,3);
		LCD_Printf("3.Test motor 3");	
		LCD_SetXY(0,4);
		LCD_Printf("4.Test motor 4");	
		LCD_SetXY(0,5);
		LCD_Printf("5.Test motor 5");	
		LCD_SetXY(0,6);
		LCD_Printf("5.Test motor 6");
	
		OSMboxAccept(AppKey2Mbox);					
		OSTimeDly(20);			
		msg = OSMboxAccept(AppKey2Mbox);
		
		switch(*msg)
		{
			case 1:temp = DISPLAY_Fp64Value(&Rotation,"Set MOTOR1");
					if(temp ){
						arm_motor(ID_MOTOR1 , (fp32)Rotation);
					}
					for(;;)
						{
						    //%d int; %c char; %f float;
						    LCD_Clear();
							LCD_Printf("======MOTOR_Test======");	
							LCD_SetXY(0,1);
							LCD_Printf("Roatation");	
							LCD_SetXY(0,2);
							LCD_WriteNum(temp);
							LCD_SetXY(0,3);
							LCD_WriteNum(Rotation);
							
						    OSMboxAccept(AppKey2Mbox);					
		                    OSTimeDly(20);			
		                    msg = OSMboxAccept(AppKey2Mbox);
		                    
		                    if(*msg!=*(uint8*)0)
			                break;
						}
			break;
			case 2:DISPLAY_Fp64Value(&Rotation,"Set MOTOR2");
					
						arm_motor(ID_MOTOR2 , (fp32)Rotation);
					
			break;
			case 3:
					temp = DISPLAY_Fp64Value((fp64*)&Rotation,"Set MOTOR3");
					if(temp ){
						arm_motor(ID_MOTOR3 , Rotation);
					}
			break ;
			case 4:
					temp = DISPLAY_Fp64Value((fp64*)&Rotation,"Set MOTOR4");
					if(temp ){
						arm_motor(ID_MOTOR4 , Rotation);
					}
			break;
			case 7:
					temp = DISPLAY_Fp64Value((fp64*)&Rotation,"Set MOTOR5");
					if(temp ){
						arm_motor(ID_MOTOR5 , Rotation);
					}
			break;
			case 8:
					temp = DISPLAY_Fp64Value((fp64*)&Rotation,"Set MOTOR6");
					if(temp ){
						arm_motor(ID_MOTOR6 , Rotation);
					}
			break;
			default :
			break;
		}
		if(*msg == 5)	
			break;		
	}	 
}


void sys_Lidar(void)
{
	uint8  *msg,i;
	char ARM_Command;
	extern volatile uint16 	Device_Statu;
	void STM32_Send8(uint8 id  ,  uint8 data);
		
	while(1)
	{
		LCD_Clear();
		LCD_Printf("==== Lidar_Can =====");
		LCD_SetXY(0,1);		
		if(Read_Device_Statu(CONTROL_LIDAR))
			LCD_Printf("1.Init");
		else
			LCD_Printf("1.DeInit");
		
		LCD_SetXY(0,2);		LCD_Printf("2.Board");
		LCD_SetXY(0,3);		LCD_Printf("3.Ring");
		LCD_SetXY(7,2);		LCD_Printf("4.Leaf");
		LCD_SetXY(7,3);		LCD_Printf("5.ResetSTM32");
	
		OSMboxAccept(AppKey2Mbox);					
		OSTimeDly(20);			
		msg = OSMboxAccept(AppKey2Mbox);
		switch (*msg)
		{
			case 1:
				if(Read_Device_Statu(CONTROL_LIDAR))
					ARM_Command='I';	//Init
				else
					ARM_Command='D';	//DeInit
			break;		

			case 2:
				ARM_Command='B';		//Board
			break;		
			
			case 3:
				ARM_Command='R';		//Ring
			break;
			
			case 4:
				ARM_Command='L';		//Leaf
			break;
			
			case 5:
				STM32_Reset();
			break;

			default:
				LCD_SetXY(0,4);
				LCD_Printf("err");
			break;
		}
		
		if (*msg<5)
		{
			STM32_ReadLidarData(0);
			STM32_Send8(ID_LIDAR_CTR,ARM_Command);
			if(ARM_Command=='I'||ARM_Command=='D')
			{
				OSTimeDly(20);
				STM32_ReadLidarData(0);
				if (Lidar_ResultX[0]!=5000)	//5000=Error
				{	
					if(ARM_Command=='I')
						Device_Statu |=  CONTROL_LIDAR;
					else
						Device_Statu &= ~CONTROL_LIDAR;
					LCD_SetXY(10,0);
					LCD_Printf("Done! ");

				}
				else
				{
					LCD_SetXY(10,0);
					LCD_Printf("Fail..");
				} 
					
			}
			if(ARM_Command=='R'||ARM_Command=='L')
				for (Lidar_Num_Result=0;Lidar_Num_Result<10;Lidar_Num_Result++)
				{
					while (1)
					{
						STM32_Send8(ID_LIDAR_CTR,'0'+Lidar_Num_Result);	//Ring Sel
						OSTimeDly(1);
						if (STM32_ReadLidarData(Lidar_Num_Result))	//SPI-Positive
							break;
					}
					if (Lidar_ResultX[Lidar_Num_Result]==5000)	//5000=Finish
						break;			
				}
			if(ARM_Command=='R'||ARM_Command=='L')
				for (i=0;i<Lidar_Num_Result;i++)
				{
				   	LCD_Clear();
					LCD_SetXY(0,0);
					LCD_Printf(" --- Result ----");
					LCD_SetXY(0,1);
					LCD_WriteString("Ring  No.");
					LCD_WriteInt(i);
					LCD_SetXY(0,2);
					LCD_WriteString("x:");
					LCD_WriteNum(Lidar_ResultX[i]);
					LCD_SetXY(0,3);
					LCD_WriteString("y:");
					LCD_WriteNum(Lidar_ResultY[i]);
					PressAnyKey();
				}
			if(ARM_Command=='B')
			{
			   	LCD_Clear();
				LCD_SetXY(0,0);
				LCD_Printf(" --- Result ----");
				LCD_SetXY(0,1);
				LCD_WriteString("Dist1:");
				LCD_WriteNum(Lidar_ResultX[0]);
				LCD_SetXY(0,2);
				LCD_WriteString("Dist2:");
				LCD_WriteNum(Lidar_ResultY[0]);
				PressAnyKey();
			}
		}
		
		if(*msg ==5){
			break;
		}
	}
}



void sys_can(void)
{
	uint8 *msg;
	
	while(1){
		LCD_Clear();
		LCD_Printf("======CAN_Test======");	
		LCD_SetXY(0,1);
		LCD_Printf("1.Relay    2.Laser");	
		LCD_SetXY(0,2);
		LCD_Printf("3.ISO_Mask 4.IR_test");
		LCD_SetXY(0,3);
		LCD_Printf("5.Sonic    6.Motor");
		LCD_SetXY(0,4);
		LCD_Printf("7.Lidar");
		
		OSMboxAccept(AppKey2Mbox);					
		OSTimeDly(20);			
		msg = OSMboxAccept(AppKey2Mbox);
		switch (*msg)
		{
			case 1: sys_Relay();
			break;
			case 2: sys_Laser();		
			break;
			case 3: sys_MASK_ISO();
			break;
			case 4: sys_IR_test();
			break;
			case 7: sys_sonic_test();
			break;
			case 8: sys_motor();
			break;
			case 9:  sys_Lidar(); 
			break;
			default:
					LCD_SetXY(0,7);
					LCD_Printf("err");
			break;
		}
	}
}



	