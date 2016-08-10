#include "all.h"
#define sign(a) (a>0?1:-1)
//角度=Gyro_Total*0.0001*Gyro_Convert=Gyro_Total*360/Gyro_Roll

//warning:以下变量的初始值从nandflash中取得///////////////////////////
fp64  Gyro_Convert1=-0.001602243155;    //正转系数
fp64  Gyro_Convert2=-0.001602521789;	//反转系数

////Gyro_Roll是转一圈时对应的陀螺仪的原始值
int64 Gyro_Roll=0;

//陀螺仪零飘
int32 Gyro_Float;
/////////////////////////////////////////////////////////////////////
fp64 Gyro_Error_Float=0;

//陀螺仪上次采样值
struct Gyro_Struct Gyro_Last={0,0};

//陀螺仪本次采样值
struct Gyro_Struct Gyro_Now=0;

//陀螺仪积分值
int64 Gyro_Total=0;

struct Pointfp64 Start_RotatePosition;

int64 Gyro_Step=0;

fp64 Gyro_Radian_Step=0;

fp64 Gyro_Multiplier;

#define POLYSIZE 100

struct PPoly
{
	int  num;
	fp64 breaks[POLYSIZE];
	fp64 coefs[POLYSIZE][4];
};



void GYRO_NandInit()
{
	uint8 data[2048];
	ReadPage(GYRO_NANDPAGE,data);
	Gyro_Convert1=*(fp64*)(data);
	Gyro_Convert2=*(fp64*)(data+16);
	Gyro_Roll=*(int64*)(data+32);
	Gyro_Float=*(int32*)(data+40);
}

void GYRO_NandSave()
{
	uint8 data[2048];
	*(fp64*)(data)=Gyro_Convert1;
	*(fp64*)(data+16)=Gyro_Convert2;
	*(int64*)(data+32)=Gyro_Roll;
	*(int32*)(data+40)=Gyro_Float;
	WritePartBlock(GYRO_NANDPAGE,data);
}

	int64 temp_data[2000],temp_data_num=0;
	extern void GYRO_CheckSpeed(void);

void GYRO_CheckFloat()
{
	int i,Gyro_Count_Tmp=0;
	struct Gyro_Struct Gyro_Last_Tmp, Gyro_Now_Tmp;
	int64 Gyro_Total_Tmp=0;
	uint8 *msg , count_step;  //判断两次都回来的值是不是新的

	LCD_SetXY(0,1);
	LCD_WriteString("check and set float");
	LCD_SetXY(0,3);
	for(i=0;i<20;i++)
	{
		LCD_WriteChar('=');
	}		
	for(;;)
	{	
		Gyro_Now_Tmp=FPGA_ReadGyro();
		count_step = Gyro_Now_Tmp.count - Gyro_Last_Tmp.count;  //有新的值才进入零漂的累加值中
		if(count_step){
			//前256次值不计
			if(Gyro_Count_Tmp<256)
			{
				Gyro_Last_Tmp=Gyro_Now_Tmp;
				Gyro_Count_Tmp  += count_step;
			}
			//1280-256=1024次值启动时检查零漂用
			else if(Gyro_Count_Tmp<1280)
			{
				Gyro_Total_Tmp  +=  Gyro_Now_Tmp.value-Gyro_Last_Tmp.value;
				Gyro_Last_Tmp=Gyro_Now_Tmp;
				Gyro_Count_Tmp += count_step ;
				//Data[4]=Gyro_Total_Tmp;
				//SaveMovingData();
			}
			//开始检测零漂，结果储存在Gyro_Count中，由TaskLcdDisplay()处理
			else if(Gyro_Count_Tmp==1280)
			{
				if(Gyro_Total_Tmp==0)//零漂为0，则初始化不成功
				{ 
					Gyro_Count_Tmp=5001;	
				}
				else if(Int64Abs(Gyro_Total_Tmp-Gyro_Float*1024)>40960)//零漂过大，更改设置
				{
					Gyro_Count_Tmp=5002; 
	//				Gyro_Float=(Gyro_Total_Tmp+512)/1024;	//0.018(10)  不知道这个512从哪里来
					Gyro_Float=(Gyro_Total_Tmp)/1024;	//0.018(10)
				}
				else//零漂正常
				{
					Gyro_Count_Tmp=5000;
				}
				Gyro_Total_Tmp=0;
				GYRO_NandSave();		
			}
		}
		
		
		LCD_SetXY(0,3);	
		for(i=0;i<Gyro_Count_Tmp/64;i++)
		{
			LCD_WriteChar('>');
		}
		//Gyro_Count值在GYRO_Update()
		if(Gyro_Count_Tmp>4999)
		{
			if(Gyro_Count_Tmp==5001)
			{
				DISPLAY_Message("Error:Gyro Init Error!!");
			}
			if(Gyro_Count_Tmp==5002)
			{
				DISPLAY_Message("Warning:Gyro Zero Float Changed!!");
			}
			break;

		}
		
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(2);
			
		msg = OSMboxAccept(AppKey2Mbox);
		
		if(*msg!=*(uint8*)0)
			break;
	}
}

void TestGyro()
{
	uint8 *msg;
	
    LCD_Clear();
	Gyro_Total=0;
	GPS_Clear();
	
	for(;;)
	{
		
	    LCD_SetXY(0,0);
		/*
		LCD_Printf("Set Direction and ");
		LCD_Printf("press any key...");
		*/
		LCD_Printf("Test Rotate");
		
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
		LCD_WriteNum(Encoders[0].Total);
		LCD_SetXY(0,1);
		LCD_WriteNum(Encoders[1].Total);
		LCD_SetXY(0,2);                                                                                                         
		LCD_WriteDouble(Gps_List[0].angle,2);
			 
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(1);
		//Speed_Motor(-70,70,-70,70);	
		Speed_F=-70*14000;
		Speed_FR=70*14000;
		Speed_BL=-70*14000;
		Speed_BR=70*14000;	
	}
		
}

void GYRO_CheckSpeed()  
{
	static uint8 flag=0;            //超速置1，不超速置0
	static uint8 count=1;     //超速后每5ms加1直至400
	if(count==400)
	{
		rGPJDAT&=~0x1000;    //关蜂鸣器
		count=1;
		flag=0;
	}
	if(flag==1)
		count++;
	if(((Gyro_Now.value-Gyro_Last.value-Gyro_Float)*0.0000001*Gyro_Convert1)>=1.45)
	{
		rGPJDAT|=0x1000;     //开蜂鸣器
		count=1;
		flag=1;
	}
}

/********************************************************************************************************
Function:	Piecewise Heimite Polynomial Interpolation
Created by 	.Somnus
Version 	1.0
*********************************************************************************************************/
void PPoly_interp(int n,double x[],double y[],struct PPoly poly)
{
	int i,pos[POLYSIZE],num=0;
	double diff_x[POLYSIZE],diff_y[POLYSIZE],divdif[POLYSIZE],slope[POLYSIZE];
	double hs[POLYSIZE],w1[POLYSIZE],w2[POLYSIZE],dmax[POLYSIZE],dmin[POLYSIZE];
	double dzzdx[POLYSIZE],dzdxdx[POLYSIZE];

	poly.num=n;
	for (i=0;i<n-1;i++)
	{
		diff_x[i]=x[i+1]-x[i];
		diff_y[i]=y[i+1]-y[i];
		divdif[i]=diff_y[i]/diff_x[i];
		slope[i]=0;
	}

	
	for (i=0;i<n-1;i++)
		if ((divdif[i]<0&&divdif[i+1]<0)||(divdif[i]>0&&divdif[i+1]>0))
		{
			pos[num]=i;
			hs[num]=diff_x[i]+diff_x[i+1];
			w1[num]=(diff_x[i]+hs[num])/(3*hs[num]);
			w2[num]=(diff_x[i+1]+hs[num])/(3*hs[num]);
			dmax[num]=max(fabs(divdif[i]),fabs(divdif[i+1]));
			dmin[num]=min(fabs(divdif[i]),fabs(divdif[i+1]));
			slope[i+1]=dmin[num]/(w1[num]*(divdif[i]/dmax[num])+w2[num]*(divdif[i+1]/dmax[num]));
			num++;
		}
		
	slope[0] = ((2*diff_x[0]+diff_x[1])*divdif[0] - diff_x[0]*divdif[1])/(diff_x[0]+diff_x[1]);
	if (sign(slope[0]) != sign(divdif[0]))
		slope[0] = 0;
	else if ((sign(divdif[0]) != sign(divdif[1])) && (fabs(slope[0]) > fabs(3*divdif[0])))
		slope[0] = 3*divdif[0];

	slope[n-1] = ((2*diff_x[n-2]+diff_x[n-3])*divdif[n-2] - diff_x[n-2]*divdif[n-3])/(diff_x[n-2]+diff_x[n-3]);
	if (sign(slope[n-1]) != sign(divdif[n-2]))
		slope[n-1] = 0;
	else if ((sign(divdif[n-2]) != sign(divdif[n-3])) && (fabs(slope[n-1]) > fabs(3*divdif[n-2])))
		slope[n-1] = 3*divdif[n-2];


	for (i=0;i<n-1;i++)
	{
		dzzdx[i] =(divdif[i]-slope[i])/diff_x[i];
		dzdxdx[i]=(slope[i+1]-divdif[i])/diff_x[i];
		poly.coefs[i][3]=(dzdxdx[i]-dzzdx[i])/diff_x[i];
		poly.coefs[i][2]=2*dzzdx[i]-dzdxdx[i];
		poly.coefs[i][1]=slope[i];
		poly.coefs[i][0]=y[i];
		poly.breaks[i]=x[i];
	}
		poly.breaks[n-1]=x[n-1];
}
 
/********************************************************************************************************
Function:	Evaluate Piecewise Heimite Polynomial 
Created by 	.Somnus
Version 	1.0
*********************************************************************************************************/
fp64 PPoly_Eval(fp64 data,struct PPoly poly)
{
	fp64 result=1.13;
	int i;
	
	for (i=0;i<poly.num-1;i++)
	{
		if (data>poly.breaks[i]&&data<poly.breaks[i+1])
		{
			data=data-poly.breaks[i];
			result=poly.coefs[i][3]*pow(data,3)+poly.coefs[i][2]*
				pow(data,2)+poly.coefs[i][1]*pow(data,1)+poly.coefs[i][0];
			break;
		}
	}

	return result;
}


/********************************************************************************************************
Polynomials
Date acquired by Zhou Changxing
*********************************************************************************************************/
struct PPoly WHITEGYRO=
{17,
{0.00000,0.37400,0.74800,1.12200,1.49600,1.87000,2.24399,2.61799,2.99199,3.36599,3.73999,4.11399,4.48799,4.86199,5.23599,5.60999,5.98399,6.35799},
{{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.01576,-0.01437},
{1.00145,0.00576,0.03865,-0.06497},
{1.00561,0.00740,-0.01917,0.03799},
{1.00769,0.00900,0.09920,-0.15853},
{1.01664,0.01668,-0.01796,0.02029},
{1.02142,0.01176,-0.01731,0.03996},
{1.02549,0.01559,0.06806,-0.09704},
{1.03577,0.02577,-0.00681,0.00751},
{1.04484,0.02382,-0.01136,0.02728},
{1.05359,0.02678,0.02438,-0.03282},
{1.06530,0.03124,-0.00815,0.02135},
{1.07696,0.03410,-0.03582,0.12099},
{1.07696,0.03410,-0.03582,0.12099}}
};

struct PPoly BLACKGYRO=
{17,
{0.00000,0.37400,0.74800,1.12200,1.49600,1.87000,2.24399,2.61799,2.99199,3.36599,3.73999,4.11399,4.48799,4.86199,5.23599,5.60999,5.98399,6.35799},
{{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.03856,-0.05956},
{1.00228,0.00385,-0.00598,0.00859},
{1.00333,0.00298,-0.00531,0.01554},
{1.00452,0.00553,0.10353,-0.16144},
{1.01262,0.01523,-0.02226,0.03457},
{1.01701,0.01308,-0.00129,0.01556},
{1.02254,0.01865,0.04372,-0.06959},
{1.03199,0.02215,-0.02226,0.04214},
{1.03937,0.02319,0.02497,-0.03145},
{1.04988,0.02866,0.00090,0.00159},
{1.06081,0.03000,-0.01226,0.03867},
{1.07234,0.03706,-0.00875,0.09057},
{1.07234,0.03706,-0.00875,0.09057}}
};


struct PPoly BLUEGYRO=
{17,
{0.00000,0.37400,0.74800,1.12200,1.49600,1.87000,2.24399,2.61799,2.99199,3.36599,3.73999,4.11399,4.48799,4.86199,5.23599,5.60999,5.98399,6.35799},
{{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.02323,-0.02863},
{1.00175,0.00537,0.00081,0.00436},
{1.00410,0.00780,0.00617,0.00136},
{1.00795,0.01298,0.03752,-0.06754},
{1.01452,0.01271,-0.02435,0.04541},
{1.01824,0.01355,0.03744,-0.04528},
{1.02618,0.02256,0.00739,-0.00894},
{1.03518,0.02434,0.00443,-0.00991},
{1.04439,0.02349,-0.01478,0.03224},
{1.05279,0.02596,0.02765,-0.03981},
{1.06429,0.02994,-0.04080,0.10369},
{1.07520,0.04294,0.15261,-0.13468},
{1.07520,0.04294,0.15261,-0.13468}}
};

struct PPoly GREENGYRO=
{17,
{0.00000,0.37400,0.74800,1.12200,1.49600,1.87000,2.24399,2.61799,2.99199,3.36599,3.73999,4.11399,4.48799,4.86199,5.23599,5.60999,5.98399,6.35799},
{{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.08772,-0.13579},
{1.00517,0.00863,-0.01692,0.02840},
{1.00751,0.00789,0.00335,0.01065},
{1.01149,0.01487,0.06468,-0.10271},
{1.02073,0.02015,-0.01587,0.02005},
{1.02709,0.01670,-0.01539,0.03892},
{1.03322,0.02151,0.06183,-0.09520},
{1.04493,0.02781,-0.01648,0.02404},
{1.05429,0.02557,-0.01052,0.03231},
{1.06407,0.03126,-0.00895,0.07814},
{1.07860,0.05736,0.24189,-0.27338},
{1.11959,0.12357,0.09803,-0.13286},
{1.11959,0.12357,0.09803,-0.13286}}
};


fp64 time_interval=1;

void GYRO_Update()
{
	uint8  count_step;
	
	Gyro_Now=FPGA_ReadGyro();
	count_step = Gyro_Now.count - Gyro_Last.count;
	
	if(count_step)
	{
		Gyro_Step = Gyro_Now.value-Gyro_Last.value - Gyro_Float*count_step;//1152 往大调是负偏		
		Gyro_Radian_Step=Gyro_Step*0.0000001*Gyro_Convert1*PI/180;

		//Gyro_Multiplier=PPoly_Eval(fabs(1000*Gyro_Radian_Step/time_interval),GREENGYRO);
		Gyro_Multiplier=1.0;
		
		
		Gyro_Total+=Gyro_Step*Gyro_Multiplier;
		Gyro_Radian_Step*=Gyro_Multiplier;
		time_interval=1;		
	} else time_interval++;

	Gyro_Last=Gyro_Now;	
	GYRO_CheckSpeed();
}



