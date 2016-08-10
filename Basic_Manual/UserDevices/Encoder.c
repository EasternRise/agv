#include "all.h"

int8 Test_Times;
fp64 sina=1,sinb=1,cosa=1,cosb=1,sinab=1;
fp64 angle_a;
fp64 angle_b;
fp64 Ac1=-192.5;
fp64 Ac2=0.1;
//warning:以下变量的初始值从nandflash中取得///////////////////////////
//Encoder_Stat结构{原始值初始值，校正系数，计算后距离值}
struct Encoder_Stat Encoders[6]=
{
	{0,0,-0.319001,-0.318843,0,0,0},
	{0,0,-0.320864,-0.320624,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0}
};
//////////////////////////////////////////////////////////////////////

void ENCODER_NandSave()
{
	uint8 data[2048];
	int i;
	for(i=0;i<4;i++)
	{
		*(fp64*)(data+i*32)=Encoders[i].Convert1;
		*(fp64*)(data+i*32+8)=Encoders[i].Convert2;
		*(fp64*)(data+i*32+16)=Encoders[i].dir_flag;
	}
	*(fp64*)(data+120)=sina;
	*(fp64*)(data+128)=sinb;
	*(fp64*)(data+136)=Ac1;
	*(fp64*)(data+144)=Ac2;
	*(fp64*)(data+152)=cosa;
	*(fp64*)(data+160)=cosb;
	*(fp64*)(data+168)=sinab;
	*(fp64*)(data+176)=angle_a;
	*(fp64*)(data+184)=angle_b;
	EraseBlock(ENCODER_NANDPAGE);
	if(WritePage(ENCODER_NANDPAGE,data)!=1)
	{
		Nand_Error=ENCODER_NANDPAGE;
	}
}

void ENCODER_NandInit()
{
	uint8 data[2048];
	int i;
	ReadPage(ENCODER_NANDPAGE,data);
	for(i=0;i<4;i++)
	{
		Encoders[i].Convert1=*(fp64*)(data+i*32);
		Encoders[i].Convert2=*(fp64*)(data+i*32+8);
		Encoders[i].dir_flag=*(fp64*)(data+i*32+16);
	}
    sina=*(fp64*)(data+120);
	sinb=*(fp64*)(data+128);
	Ac1=*(fp64*)(data+136);
	Ac2=*(fp64*)(data+144);
	cosa=*(fp64*)(data+152);
	cosb=*(fp64*)(data+160);
	sinab=*(fp64*)(data+168);
	angle_a=*(fp64*)(data+176);
	angle_b=*(fp64*)(data+184);
}

void ENCODER_Clear(int index)
{
	Encoders[index].Total=0;
	Encoders[index].Distance=0.0;
	Encoders[index].Last=Encoders[index].Now;//记录本次清零时码盘值，作为清零后的初始值
}

void Encoder_Update()
{
	int i;
	int64 now;
	int32 step;
	int dir[3];
	dir[0]=1;
	dir[1]=1;
	dir[2]=1;
	for(i=0;i<3;i++)
	{
		now=FPGA_ReadEncoder(i+1);
		step=now-Encoders[i].Now;
		if(step<2000&&step>-2000)
		{
		        Encoders[i].Now=now;
		        Encoders[i].Total=Encoders[i].Now-Encoders[i].Last;
	        
		       if (step==Int32Abs(step)) 
		       {
			    	Encoders[i].Distance+=Int32Abs(step)*Encoders[i].Convert1*dir[i];
		       }
		       else
		       {
		    		Encoders[i].Distance-=Int32Abs(step)*Encoders[i].Convert2*dir[i];
		       }
		
	    }
	}
	
}


/***************************差速轮码盘初始化***********************************/
void ENCODER_Init(int index)
{
   	uint8 err;
	uint8 *msg;
	fp64 con[2];
	int64 step;
	fp64 N[2];
	int i;
	
	for(i=0;i<2;i++)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_Printf("Set %d and ",i+1);
		LCD_Printf("press any key...");

		OSMboxPend(AppKey1Mbox,0,&err);
			
		step=Encoders[index].Total;

		for(;;)
		{
		    LCD_Clear();
			LCD_SetXY(0,0);
			LCD_Printf("Push 3 meters");
			LCD_SetXY(0,1);
			LCD_Printf("Count:%d",Encoders[index].Total-step);
				
			OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
				
			OSTimeDly(20);
				
				msg = OSMboxAccept(AppKey2Mbox);					
				
				if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
				{
					N[i]=Encoders[index].Total-step;
					break;
				}
	    }
		Encoders[index].Total=0;
	}
		
		con[0]= 3000.0/N[0];
		con[1]=-3000.0/N[1];
		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_Printf("New Convert1:");
		LCD_SetXY(0,1);
		LCD_WriteDouble(con[0],6);
		LCD_SetXY(0,2);
		LCD_Printf("New Convert2:");
		LCD_SetXY(0,3);
		LCD_WriteDouble(con[1],6);
		LCD_SetXY(0,4);
		LCD_Printf("1.Save Other.Exit");
		
	msg = OSMboxPend(AppKey1Mbox,0,&err);
	if(*msg==1)
	{
		Encoders[index].Convert1=con[0];
		Encoders[index].Convert2=con[1];
		ENCODER_NandSave();
	}
}


void ENCODER_Ac_Init(int index)
{
    uint8 err;
	uint8 *msg;
	
	fp64 N[2]={0};
	int64 step1=0;
    
		for(;;)
		{
		    LCD_Clear();
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
		step1=Encoders[index].Total;

		
		for(;;)
		{
		    LCD_Clear();
			LCD_SetXY(0,0);
			LCD_Printf("Count:%d",Encoders[index].Total-step1);
			LCD_SetXY(0,1);
			LCD_WriteString("A:");
	        LCD_WriteNum(Gps_List[0].angle);
			
			OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
			OSTimeDly(20);
			
			msg = OSMboxAccept(AppKey2Mbox);					
			
			RouteForm=RouteFree;
			
			Speed_F= -5.53859*50;
			Speed_FR=-5.53859*50;
			Speed_BL= 2.8648*5;
			Speed_BR=-2.8648*5;
			
			if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
			{
				Speed_F= 0;
			    Speed_FR=0;
			    Speed_BL=0;
			    Speed_BR=0;
				N[0]=Encoders[index].Total-step1;
				break;
			}
		}
		
	//经测试码盘偏右，即在中心右侧	
    Ac1=N[0]*Encoders[index].Convert2/(10*2*PI+Gps_List[0].angle);

    LCD_Clear();
	LCD_SetXY(0,0);
	LCD_Printf("New Convert:");
    LCD_SetXY(0,1);
	LCD_WriteDouble(Ac1,1);
	LCD_SetXY(0,2);
	LCD_Printf("1.Save Other.Exit");

	msg = OSMboxPend(AppKey1Mbox,0,&err);
	if(*msg==1)
	{
		ENCODER_NandSave();
	}

}






/***********************************全向码盘初始化算法******************************************/
void Location_ENCODER_Init(int index)
{
   	uint8 err;
	uint8 *msg;

 	int i;
 	fp64 angle_a1,angle_b1,angle_a2,angle_b2;
	fp64 N[8]={0};
	int64 step1=0,step2=0;
	fp64 C11,C12,C21,C22;    //C11,1码盘正转，C12，1码盘反转，C21，C22，2码盘正反转
	fp64 C11_1,C11_2,C21_1,C21_2,C12_1,C12_2,C22_1,C22_2;
	
    ///说明：当码盘与某一底盘边缘平行，沿着该边缘推执行算法1；否则按车体y，x顺序推执行算法2	
    
//**********************************算法2***************************************************/
//                            
//  四轮车码盘校正算法(修正后)
//
//******************************************************************************************/
    
    for(i=0;i<2;i++)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_Printf("Set %d and ",i+1);
		LCD_Printf("press any key...");

		OSMboxPend(AppKey1Mbox,0,&err);
		
		step1=Encoders[index].Total;
		step2=Encoders[index+1].Total;

		LCD_Clear();

		for(;;)
		{
			LCD_SetXY(0,0);
			LCD_Printf("Push 3 meters");
			LCD_SetXY(0,1);
			LCD_Printf("Count:%d",Encoders[index].Total-step1);
		    LCD_WriteString("        ");		
			LCD_SetXY(0,2);
			LCD_Printf("Count:%d",Encoders[index+1].Total-step2);
			LCD_WriteString("        ");	
			LCD_SetXY(0,3);
			LCD_WriteString("A:");
	        LCD_WriteNum(Gps_List[0].angle);
			
			OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
			OSTimeDly(20);
			
			msg = OSMboxAccept(AppKey2Mbox);					
			
			if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
			{
				N[i]=Encoders[index].Total-step1;
				N[i+2]=Encoders[index+1].Total-step2;
				break;
			}
		}
		Encoders[index].Total=0;
		Encoders[index+1].Total=0;
	}
    Encoders[index].dir_flag=N[0]/Fp64Abs(N[0]);
    Encoders[index+1].dir_flag=N[2]/Fp64Abs(N[2]);

    
    for(i=4;i<6;i++)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_Printf("Set %d and ",i-1);
		LCD_Printf("press any key...");

		OSMboxPend(AppKey1Mbox,0,&err);
		
		step1=Encoders[index].Total;
		step2=Encoders[index+1].Total;

		LCD_Clear();

		for(;;)
		{
			LCD_SetXY(0,0);
			LCD_Printf("Push 3 meters");
			LCD_SetXY(0,1);
			LCD_Printf("Count:%d",Encoders[index].Total-step1);
		    LCD_WriteString("        ");		
			LCD_SetXY(0,2);
			LCD_Printf("Count:%d",Encoders[index+1].Total-step2);
			LCD_WriteString("        ");	
			LCD_SetXY(0,3);
			LCD_WriteString("A:");
	        LCD_WriteNum(Gps_List[0].angle);
			
			OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
			OSTimeDly(20);
			
			msg = OSMboxAccept(AppKey2Mbox);					
			
			if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
			{
				N[i]=Encoders[index].Total-step1;
				N[i+2]=Encoders[index+1].Total-step2;
				break;
			}
		}
		Encoders[index].Total=0;
		Encoders[index+1].Total=0;
	}
	
	angle_a1=atan(N[1]/N[0]);
	angle_a2=atan(N[5]/N[4]);
	angle_a=(angle_a1+angle_a2)/2;
	
    angle_b1=atan(N[3]/N[6]);
    angle_b2=atan(N[7]/N[2]);
    angle_b=(angle_b1+angle_b2)/2;
    
    sina=sin(angle_a);
	sinb=sin(angle_b);
	cosa=cos(angle_a);
	cosb=cos(angle_b);
	sinab=sin(angle_a+angle_b);
	
    C11_1=-3000*cosa/N[4];
    C11_2=-3000*sina/N[5];
    C11 = (C11_1+C11_2)/2;
    
    C21_1=-3000*cosb/N[6];
    C21_2=-3000*sinb/N[3];
    C21 = (C21_1+C21_2)/2;
    
	C12_1= 3000*sina/N[1];
	C12_2= 3000*cosa/N[0];
	C12 = (C12_1+C12_2)/2;
	
	C22_1=3000*cosb/N[2];
	C22_2=3000*sinb/N[7];
    C22 = (C22_1+C22_2)/2;
//////////////////////////
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_Printf("New Convert:");
    LCD_SetXY(0,1);
	LCD_WriteDouble(C11,1);
	LCD_Printf("+");
	LCD_WriteDouble(C21,1);
	LCD_Printf("+");
	LCD_WriteDouble(Encoders[0].dir_flag,0);
	LCD_Printf("+");
	LCD_WriteDouble(Encoders[1].dir_flag,0);
	LCD_SetXY(0,2);
	LCD_WriteDouble(angle_a*180/PI,1);
	LCD_Printf("+");
	LCD_WriteDouble(angle_b*180/PI,1);
	LCD_SetXY(0,3);
	LCD_Printf("1.Save Other.Exit");

	msg = OSMboxPend(AppKey1Mbox,0,&err);
	if(*msg==1)
	{
		Encoders[index].Convert1=C11;
		Encoders[index].Convert2=C12;
		Encoders[index+1].Convert1=C21;
		Encoders[index+1].Convert2=C22;
		ENCODER_NandSave();
	}
}


/*************************码盘旋转半径*算法一**********************************/
void angle_error_init( int index )
{
	uint8 err;
	uint8 *msg;
	
	fp64 N[2]={0};
	int64 step1=0,step2=0;   
	    
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
		step1=Encoders[index].Total;
		step2=Encoders[index+1].Total;

		LCD_Clear();

		for(;;)
		{
			LCD_SetXY(0,0);
			LCD_Printf("Count:%d",Encoders[index].Total-step1);
			LCD_SetXY(0,1);
			LCD_Printf("Count:%d",Encoders[index+1].Total-step2);
			LCD_SetXY(0,2);
			LCD_WriteString("A:");
	        LCD_WriteNum(Gps_List[0].angle);
			
			OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
			OSTimeDly(20);
			
			msg = OSMboxAccept(AppKey2Mbox);					
			
			//Speed_Motor(-30,30,-30,30);
			
			RouteForm=RouteFree;
			
			Speed_F=-35*14000;
			Speed_FR=35*14000;
			Speed_BL=-35*14000;
			Speed_BR=35*14000;
			
			if(*msg!=*(uint8*)0)				//注意*(void *)0不等于0，这里等于168
			{
				//MOTOR_SetSpeed(0,0,0);
				N[0]=Encoders[index].Total-step1;
				N[1]=Encoders[index+1].Total-step2;
				break;
			}
		}
    Ac1=N[0]*Encoders[index].Convert1/(10*PI);
    Ac2=N[1]*Encoders[index+1].Convert2/(10*PI);
    LCD_Clear();
	LCD_SetXY(0,0);
	LCD_Printf("New Convert:");
    LCD_SetXY(0,1);
	LCD_WriteDouble(Ac1,1);
	LCD_SetXY(0,2);
	LCD_WriteDouble(Ac2,1);
	LCD_SetXY(0,3);
	LCD_Printf("1.Save Other.Exit");

	msg = OSMboxPend(AppKey1Mbox,0,&err);
	if(*msg==1)
	{
		ENCODER_NandSave();
	}
}





