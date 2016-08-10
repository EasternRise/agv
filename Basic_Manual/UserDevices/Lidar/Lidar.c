/****************************************************************
Lidar.c  ( Version 3.0 )
XJTU RobotTeam 12th		
Created by .Somnus 		
Date Modified:	2-17, 2013
****************************************************************/
#include "all.h"
#include "stdlib.h"

#if LidarSetting 
uint8 Lidar_Debug=0;						//Debug flag
uint8 Lidar_TimeControl=0;					//Timer enable/disable
uint8 Lidar_TypeSel=Lidar_Ring;			//Lidar mode, used in debug
int8 Lidar_Stat=Lidar_Waiting;				//Lidar status
int8 Lidar_ReadStat=0;						//URG read status
int8 One_Board_Flag=1;
int8 Lidar_CalibFlag=1;
//-------------------   UART read buffer   ---------------------//
uint16 LidarBufferTop;
uint16 LidarBufferEnd;
char LidarBuffer[5000];  			
//-------------   Original Length and XY data   ----------------//
int16 LidarLength[750];  								
int16 Lidar_x[800],Lidar_y[800],Result_n[50];	   		
//---------------------   Scan ranges   ------------------------//
_LidarRange LidarRanges[10];
_LidarRange LidarRangesBoard[2]={{300,460,4000,100},{370,570,4000,100}};
_LidarRange LidarRangesRing[2]={{240,440,4000,100},{370,570,4000,100}};
int8 LidarRangesNum=1;						
//------------------------   Output   --------------------------//
struct Pointfp64 Lidar_Pos;				//Position output
fp64 Lidar_Dist1,Lidar_Dist2;
fp64 Lidar_Phi;							//Angle output
fp64 Lidar_ResultX[50],Lidar_ResultY[50],Lidar_ResultS[50];   //Ring position output
int16 Lidar_Num_Result=0;
//---------------------   Timing Offset   -----------------------//
int32 Lidar_NowTime=0;					//Robot internal timer
int32 Lidar_LastTime;					//URG timer
int32 Lidar_TempTime;  					
fp64 Lidar_SpeedX,Lidar_SpeedY;			//Current speed
struct Pointfp64 Lidar_Pos_Orig;		//Original position when scanning
//---------------------   Distance Offset   -----------------------//
#define POLYSIZE 100

struct PPoly
{
	int  num;
	fp64 breaks[POLYSIZE];
	fp64 coefs[POLYSIZE][4];
};

struct PPoly LIDAR=
{4,
{0.60000,1.00000,1.40000,1.80000},
{{0.01400000,-0.09500000,0.01176471,0.00183824},
{-0.02200000,-0.08470588,-0.05473146,0.16624041},
{-0.05400000,-0.04869565,0.01222826,0.05502717}}};

extern void PPoly_interp(int n,double x[],double y[],struct PPoly *poly);
extern fp64 PPoly_Eval(fp64 data,struct PPoly poly);
//------------------------   Factors   -------------------------//

int32 Lidar_ScoreFactor	=	100000;
int16 FCT_SPAN			=	5;
int16 FCT_WIDTH		 	=	20;
int16 FCT_LENGTH 		=	70;
int16 FCT_COUNT 		=	40;

int16 FCT_LENGTH_FILT 	=	50;

int16 FCT_R_RING 		=	200;
int16 FCT_R_LEAF 		=	125;
int16 FCT_SPAN_R 		=	2;
int16 FCT_LENGTH_R		=	100;
int16 FCT_WIDTH_R	 	=	25;
int16 FCT_COUNT_R 		=	10;
int16 FCT_TOLERATE_R	=	100;	

/****************************************************************
Basic Functions
****************************************************************/

void URG_Send(char* tag) 			//Send message to Urg
{
  UART2_SendString(tag);
}

void URG_IgnoreLine()  				//Ignore a line from recieved data
{
	char ch;
	while (LidarBufferEnd<LidarBufferTop)
	{
		ch=LidarBuffer[LidarBufferEnd];
		LidarBufferEnd++;
		if ((ch == '\r') || (ch == '\n')) 
		  	break;		
	}
}


void URG_TimeDisplay() 				//Acquire Lidar self-timer
{
	int i,cnt,tmp; char ch; uint8*msg;
	int32 StaTime;

	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Please Wait..");

	URG_Send("TM0\n");
	OSTimeDly(400); LCD_Clear();

	LCD_SetXY(0,0);
	LCD_WriteString("Press Any Key..");

	for (cnt=0;;cnt++)			
	{
		LidarBufferTop=0; 	LidarBufferEnd=0;
		URG_Send("TM1\n");
		StaTime=Lidar_NowTime;
		tmp=-1;
		while (1)
		{
			if (LidarBufferTop==tmp)
				break;
			else
			{
				tmp=LidarBufferTop;
				OSTimeDly(2);
			}
		}
		URG_IgnoreLine();	URG_IgnoreLine();	Lidar_LastTime=0;
		for (i=0;i<4;i++)
		{
			ch=LidarBuffer[LidarBufferEnd];
			Lidar_LastTime=(Lidar_LastTime<<6)+ch-0x30;
			LidarBufferEnd++;
		}
	
		PC_WriteInt(StaTime);
		PC_WriteChar(',');
		PC_WriteInt(Lidar_LastTime);
		if (cnt>20)
		{
			PC_WriteChar(',');
			PC_WriteInt(Lidar_LastTime-StaTime);
		}
		
		PC_WriteChar('\n');		
		if (cnt==20)
			Lidar_NowTime=Lidar_LastTime+20;

    	OSMboxAccept(AppKey2Mbox);
    	OSTimeDly(20);
		msg = OSMboxAccept(AppKey2Mbox);							
		if(*msg!=*(uint8*)0)			
		{
			break;
		}	
	}

	URG_Send("TM2\n");  OSTimeDly(200);
	LCD_SetXY(0,1);
	LCD_WriteString("Done..           ");
}

void URG_AdjustTime() 				//Acquire Lidar self-timer
{
	int i,tmp; char ch;
	int32 Fintime,StaTime,CalcTime; 


	LCD_SetXY(0,2);
	LCD_WriteString("Please Wait..");

	URG_Send("TM0\n");
	OSTimeDly(200);	LidarBufferTop=0; 	LidarBufferEnd=0;
	
	URG_Send("TM1\n");
	Lidar_TempTime=Lidar_NowTime;
	StaTime=Lidar_TempTime;
	tmp=-1;
	while (1)
	{
		if (LidarBufferTop==tmp)
			break;
		else
		{
			tmp=LidarBufferTop;
			OSTimeDly(2);
		}
	}
	URG_IgnoreLine();	URG_IgnoreLine();	Lidar_LastTime=0;
	for (i=0;i<4;i++)
	{
		ch=LidarBuffer[LidarBufferEnd];
		Lidar_LastTime=(Lidar_LastTime<<6)+ch-0x30;
		LidarBufferEnd++;
	}
	Fintime=Lidar_NowTime;
	Lidar_NowTime=Lidar_NowTime-Lidar_TempTime+Lidar_LastTime;
	Lidar_NowTime=(Lidar_NowTime+(1<<24))%(1<<24);
	CalcTime=Lidar_NowTime;
	URG_Send("TM2\n");  OSTimeDly(200);
}

void URG_CaptureByGS(int16 first_step, int16 last_step) //Capture data from urg
{
	static char send_message[LineLength],ch;
	int16 step,len,tmp;
	int i;
	
	
	LidarBufferTop=0; 	LidarBufferEnd=0;
	sprintf(send_message, "GS%04d%04d%02d\n", first_step, last_step, 0); 
	URG_Send(send_message);

	Lidar_TempTime=Lidar_NowTime;

	tmp=-1;
	while (1)
	{
		if (LidarBufferTop==tmp)
			break;
		else
		{
			tmp=LidarBufferTop;
			OSTimeDly(2);
		}
	}
	URG_IgnoreLine();	URG_IgnoreLine();	

	if (Lidar_TimeControl)
	{
		Lidar_LastTime=0;
		for (i=0;i<4;i++)
		{
			ch=LidarBuffer[LidarBufferEnd];
			Lidar_LastTime=(Lidar_LastTime<<6)+ch-0x30;
			LidarBufferEnd++;
		}
		LidarBufferEnd+=2;
	} else URG_IgnoreLine();
		

	Lidar_ReadStat=2;
		
	step=first_step; len=-1;
	while (LidarBufferEnd<LidarBufferTop)
	{
		if (step==last_step+1)
		{
			Lidar_ReadStat=0;
			break;
		}
		ch=LidarBuffer[LidarBufferEnd];
		LidarBufferEnd++;
		if ((ch == '\r') || (ch == '\n')) 
		{
			len=-1;
		  	continue;
		}
		if (len!=-1)
 		{
			len+=ch-0x30;	
			LidarLength[step]=len;
			len=-1; step++;
		}
		else
			len=64*(ch-48);
	}

}


void URG_Filter(void)				//Filter error points
{	
	int i;
	for (i=44;i<=725;i++) 
		if ((sqrt(pow(Lidar_x[i]-Lidar_x[i-1],2)+pow(Lidar_y[i]-Lidar_y[i-1],2))>100)&&(sqrt(pow(Lidar_x[i]-Lidar_x[i+1],2)+pow(Lidar_y[i]-Lidar_y[i+1],2))>100))
		{
			Lidar_x[i]=0;
			Lidar_y[i]=0;
			LidarLength[i]=0;
		}
}

void LidarInit(void)				//Lidar Initialization
{	
  	LidarBufferTop=0; LidarBufferEnd=0;
  	LCD_Clear();   	
	LCD_SetXY(0,0); LCD_WriteString("Protocol init...");
   	URG_Send("SCIP2.0\n"); 
  	while (LidarBufferTop < 12)  	
  		OSTimeDly(1);
  	if (Lidar_TimeControl)
  	{
		LCD_SetXY(0,1); LCD_WriteString("Adjust Timer....");
		URG_AdjustTime();
	}else
	{
		LCD_SetXY(0,1); LCD_WriteString("Timer disabled..");
	} 
  	LCD_SetXY(0,2); LCD_WriteString("Light up laser..");
  	URG_Send("BM\n"); 
	LCD_SetXY(0,3); LCD_WriteString("Done!");	
}

/****************************************************************
void URG_GetXY()

Function:	Generate point cloud
Special :	Offset time delay (motor speed required)
*****************************************************************/

void URG_GetXY(void)				
{	
	uint32 time0;
	int32 dtime;
	struct Pointfp64 pos0;
	int i,step,rangenum;
	fp64 rad,vx,vy;
	
	time0=Lidar_NowTime; pos0=Gps_List[0].position;
	
	for (i=0;i<750;i++)
		LidarLength[i]=0;

	vx=Lidar_SpeedX; vy=Lidar_SpeedY;			
	for (rangenum=0;rangenum<LidarRangesNum;rangenum++)
	{		
		for (;;)
		{
			Lidar_ReadStat=-1;
			URG_CaptureByGS(LidarRanges[rangenum].start_step,LidarRanges[rangenum].end_step);
		  	if (Lidar_ReadStat==0)
				break;
		}
		dtime=Lidar_LastTime-time0;
		if (dtime<0) dtime+=1<<24;

		for(step=LidarRanges[rangenum].start_step; step<=LidarRanges[rangenum].end_step; step++)
		{
			rad=-(step-384)*270.0/768.0+90.0;
		    rad=rad/180.0*PI;
			Lidar_x[step]=LidarLength[step]*cos(rad);
			Lidar_y[step]=LidarLength[step]*sin(rad); 
			if (Lidar_Stat==Lidar_Board_Running)
			{
				Lidar_x[step]-=vx/1000.0*((fp64)dtime+(fp64)(step-44.0)/681.0*URG_SCAN_INTERVAL);
				Lidar_y[step]+=vy/1000.0*((fp64)dtime+(fp64)(step-44.0)/681.0*URG_SCAN_INTERVAL);
			}
		}
	}
	if (Lidar_Debug&&Lidar_Stat!=Lidar_Board_Running)
	{
		for (i=44;i<=725;i++)
		{
			PC_WriteInt(LidarLength[i]);
			PC_WriteChar(',');
		}
		PC_WriteChar('\n');
	}
}
/****************************************************************
void URG_LineFilter()

Function:Filter lines from Urg point cloud
		 (Using the same method as Lidar_Gps )
*****************************************************************/

void URG_LineFilter()		
{
	int16 step,num_in,rangenum,i;
	fp64 xy,xx,yy,sx,sy;
	int16 X1,X2,Y1,Y2;
	fp64 k0,b0,A,B,C,dist,k1,b1;
	
	for (rangenum=0;rangenum<LidarRangesNum;rangenum++)
	{		 
		for(step=LidarRanges[rangenum].start_step; step+10<=LidarRanges[rangenum].end_step; step+=FCT_SPAN)
		{
			for (i=step+1;i<=LidarRanges[rangenum].end_step;i++)
				if (sqrt((Lidar_x[step]-Lidar_x[i])*(Lidar_x[step]-Lidar_x[i])+(Lidar_y[step]-Lidar_y[i])*(Lidar_y[step]-Lidar_y[i]))>FCT_LENGTH_FILT)
					break;
			if (LidarLength[step]<LidarRanges[rangenum].min_distance||LidarLength[step]>LidarRanges[rangenum].max_distance)
					continue;
			if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
			X1=Lidar_x[step]; 	Y1=Lidar_y[step];
			X2=Lidar_x[i];		Y2=Lidar_y[i];

            k0=(fp64)((fp64)Y2-(fp64)Y1)/((fp64)X2-(fp64)X1);
            b0=((fp64)Y1-k0*(fp64)X1);
            A=k0; B=-1; C=b0;
            num_in=0;
			for (i=LidarRanges[rangenum].start_step;i<=LidarRanges[rangenum].end_step;i++)
			{
				if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
	            dist=fabs((A*Lidar_x[i]+B*Lidar_y[i]+C)/sqrt(A*A+B*B));
				if (dist<FCT_WIDTH)
					num_in++;
			}
			
			if (num_in<80)
				continue;
			
			//----------------------
			xx=0; yy=0; xy=0; sx=0; sy=0;
			for (i=LidarRanges[rangenum].start_step;i<=LidarRanges[rangenum].end_step;i++)
			{
				if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
				if (fabs((A*Lidar_x[i]+B*Lidar_y[i]+C)/sqrt(A*A+B*B))>FCT_WIDTH)
					continue;
				xx+=Lidar_x[i]*Lidar_x[i];  yy+=Lidar_y[i]*Lidar_y[i]; xy+=Lidar_x[i]*Lidar_y[i];
				sx+=Lidar_x[i]; sy+=Lidar_y[i];
			}	
			k1=((num_in*xy-sx*sy))/(num_in*xx-sx*sx);
			b1=((xx*sy-sx*xy))/(num_in*xx-sx*sx);
			
			A=k0; B=-1; C=b0;
            for (i=LidarRanges[rangenum].start_step;i<=LidarRanges[rangenum].end_step;i++)
			{
				if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
	            dist=fabs((A*Lidar_x[i]+B*Lidar_y[i]+C)/sqrt(A*A+B*B));
				if (dist<10*FCT_WIDTH)
				{
					Lidar_x[i]=0; Lidar_y[i]=0; LidarLength[i]=0;
				}	;
			}
		}
	}
	if (Lidar_Debug&&Lidar_Stat!=Lidar_Board_Running)
	for (i=44;i<=725;i++)
	{
		PC_WriteInt(LidarLength[i]);
		PC_WriteChar(',');
	}
}
/****************************************************************
void LidarCircle()

Function:	Get local position of scanned circles
			(similar RANSAC algorithm as Lidar_Gps)
*****************************************************************/
void LidarCircle(void)
{
	int16 i,step,num_in,num_result,rangenum;
	fp64 X1,X2,Y1,Y2;
	fp64 x_mid,y_mid,k,b,x_center,y_center,rad,score;
	fp64 tdist,dist,x_fit,y_fit,r_fit;
//	fp64 AA,AB,AC,BB,BC,CC,A,B,C;
	static fp64 x_in[300],y_in[300];
	
	num_result=0; Lidar_Num_Result=0;
	Lidar_Pos_Orig=Gps_List[0].position;
	
	URG_GetXY();
	URG_LineFilter();
	URG_Filter();

	for (rangenum=0;rangenum<LidarRangesNum;rangenum++)
	{	
		for(step=LidarRanges[rangenum].start_step; step+10<=LidarRanges[rangenum].end_step; step+=FCT_SPAN_R)
		{
			for (i=step+1;i<=LidarRanges[rangenum].end_step;i++)
				if (sqrt((Lidar_x[step]-Lidar_x[i])*(Lidar_x[step]-Lidar_x[i])+(Lidar_y[step]-Lidar_y[i])*(Lidar_y[step]-Lidar_y[i]))>FCT_LENGTH_R)
					break;
			if (LidarLength[step]<LidarRanges[rangenum].min_distance||LidarLength[step]>LidarRanges[rangenum].max_distance)
					continue;
			if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
			X1=Lidar_x[step]; 	Y1=Lidar_y[step];
			X2=Lidar_x[i];		Y2=Lidar_y[i];
			
			x_mid=(fp64)(X1+X2)/2.0; y_mid=(fp64)(Y1+Y2)/2.0;
			k=-(fp64)(X2-X1)/(fp64)(Y2-Y1);
            b=(fp64)y_mid-k*(fp64)x_mid;
            
            rad=atan(k); if (sin(rad)<0) rad=PI+rad; 
            tdist=sqrt(pow(x_mid-X1,2)+pow(y_mid-Y1,2));
            if (tdist>FCT_R_RING)
            	continue;
            x_center=x_mid+cos(rad)*sqrt(pow(FCT_R_RING,2)-pow(tdist,2)); //edited;
            y_center=y_mid+sin(rad)*sqrt(pow(FCT_R_RING,2)-pow(tdist,2));

            
            num_in=0;
   			for (i=LidarRanges[rangenum].start_step;i<=LidarRanges[rangenum].end_step;i++)
			{
				if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
	            dist=fabs(sqrt((Lidar_x[i]-x_center)*(Lidar_x[i]-x_center)+(Lidar_y[i]-y_center)*(Lidar_y[i]-y_center))-FCT_R_RING);
				if (dist<FCT_WIDTH_R)
				{
					x_in[num_in]=Lidar_x[i];
					y_in[num_in]=Lidar_y[i];
					num_in++;
				}
			}
			
            if (num_in<FCT_COUNT_R)
                continue;
             
/*            AA=0; AB=0; AC=0; BB=0; BC=0; CC=0; r_fit=0;
   			for (i=1;i<num_in;i++)
			{
				A=2*(x_in[i]-x_in[i-1]);
	            B=2*(y_in[i]-y_in[i-1]);
	            C=x_in[i]*x_in[i]+y_in[i]*y_in[i]-x_in[i-1]*x_in[i-1]-y_in[i-1]*y_in[i-1];

				AA+=A*A; AB+=A*B; AC+=A*C; BB+=B*B; BC+=B*C; CC+=C*C;
				
			}
            x_fit=(BB*AC-BC*AB)/(AA*BB-AB*AB);
            y_fit=(AA*BC-AC*AB)/(AA*BB-AB*AB);
    		        
            for (i=0;i<num_in;i++)
				r_fit+=sqrt((x_in[i]-x_fit)*(x_in[i]-x_fit)+(y_in[i]-y_fit)*(y_in[i]-y_fit));
			r_fit/=num_in;

            if (fabs(r_fit-FCT_R_RING)>FCT_TOLERATE_R)
               continue;
  */             
  			r_fit=FCT_R_RING; x_fit=x_center; y_fit=y_center;
            score=num_in-abs(r_fit-FCT_R_RING);
            if ((num_result!=0)&&(sqrt((x_fit-Lidar_ResultX[num_result-1])*(x_fit-Lidar_ResultX[num_result-1])+(y_fit-Lidar_ResultY[num_result-1])*(y_fit-Lidar_ResultY[num_result-1]))<500))
            {
            	if (score>Lidar_ResultS[num_result-1])
            	{
            	
					Lidar_ResultS[num_result-1]=score;
					Lidar_ResultX[num_result-1]=x_fit;
					Lidar_ResultY[num_result-1]=y_fit;
           		}
           	}
           	else 
           	{
	           	Lidar_ResultS[num_result]=score;
				Lidar_ResultX[num_result]=x_fit;
				Lidar_ResultY[num_result]=y_fit;
				num_result++;
           	}
		}
	}

	if (Lidar_CalibFlag)
		for(i=0;i<num_result;i++)
			Lidar_ResultY[i]+=1000*PPoly_Eval(Lidar_ResultY[i]/1000.0,LIDAR);


	Lidar_Stat=Lidar_Waiting;
	Lidar_Num_Result=num_result;
	
	if (Lidar_Debug)
	for (i=0;i<num_result;i++)
	{
	   	LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("Ring  No,");
		LCD_WriteInt(i);
		LCD_SetXY(0,1);
		LCD_WriteString("x:");
		LCD_WriteNum(Lidar_ResultX[i]);
		LCD_SetXY(0,2);
		LCD_WriteString("y:");
		LCD_WriteNum(Lidar_ResultY[i]);
		PressAnyKey();

	}
	LCD_SetXY(0,3);
	LCD_WriteString("Done!");

}

/****************************************************************
void LidarBoard_NoSCIENCE!()

Function:	Lidar GPS by scanning boards
*****************************************************************/
void LidarBoard_NoRANSAC(void)
{
	int32 step,rangenum,dist;

	URG_GetXY();
	URG_Filter();
	
	dist=100000;
	for (rangenum=0;rangenum<LidarRangesNum;rangenum++)
	{	
		for(step=LidarRanges[rangenum].start_step; step<=LidarRanges[rangenum].end_step; step++)
			if (LidarLength[step]>1000&&LidarLength[step]<2000&&LidarLength[step]<dist)
				dist=LidarLength[step];
	}
	Lidar_Dist1=dist;
 	if (Lidar_Debug)
	{
	 	LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("dist:");
		LCD_WriteInt((int16)Lidar_Dist1);
	}

	if (Lidar_Dist1<2500&&dist>1000)
		Lidar_Stat=Lidar_Waiting;
   	else 
   		Lidar_Stat=Lidar_Error;
}


/****************************************************************
void LidarBoard()

Function:	Lidar GPS by scanning boards
*****************************************************************/
void LidarBoard(void)					
{
	int16 step,num_in,num_result,rangenum,ans1,ans2,i,j;
	fp64 xy,xx,yy,sx,sy;
	int16 X1,X2,Y1,Y2;
	fp64 k0,b0,A,B,C,dist,k1,b1,dtheta,score,maxscore=0;
	fp64 theta1,theta2;
	static fp64 Result_k[50],Result_b[50],angle[800];

	num_result=0;
	Lidar_Pos_Orig=Gps_List[0].position;
	URG_GetXY();
	
	for (rangenum=0;rangenum<LidarRangesNum;rangenum++)
	{	
		for(step=LidarRanges[rangenum].start_step; step+10<=LidarRanges[rangenum].end_step; step+=FCT_SPAN)
		{
			for (i=step+1;i<=LidarRanges[rangenum].end_step;i++)
				if (sqrt((Lidar_x[step]-Lidar_x[i])*(Lidar_x[step]-Lidar_x[i])+(Lidar_y[step]-Lidar_y[i])*(Lidar_y[step]-Lidar_y[i]))>FCT_LENGTH)
					break;
			if (LidarLength[step]<LidarRanges[rangenum].min_distance||LidarLength[step]>LidarRanges[rangenum].max_distance)
					continue;
			if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
			X1=Lidar_x[step]; 	Y1=Lidar_y[step];
			X2=Lidar_x[i];		Y2=Lidar_y[i];

            k0=(fp64)((fp64)Y2-(fp64)Y1)/((fp64)X2-(fp64)X1);
            b0=((fp64)Y1-k0*(fp64)X1);
            A=k0; B=-1; C=b0;
            num_in=0;
			for (i=LidarRanges[rangenum].start_step;i<=LidarRanges[rangenum].end_step;i++)
			{
	            dist=fabs((A*Lidar_x[i]+B*Lidar_y[i]+C)/sqrt(A*A+B*B));
				if (dist<FCT_WIDTH)
					num_in++;
			}			
			if (num_in<FCT_COUNT)
				continue;			
			//----------------------
			xx=0; yy=0; xy=0; sx=0; sy=0;
			for (i=LidarRanges[rangenum].start_step;i<=LidarRanges[rangenum].end_step;i++)
			{
				if (fabs((A*Lidar_x[i]	+B*Lidar_y[i]+C)/sqrt(A*A+B*B))>FCT_WIDTH)
					continue;
				xx+=Lidar_x[i]*Lidar_x[i];  yy+=Lidar_y[i]*Lidar_y[i]; xy+=Lidar_x[i]*Lidar_y[i];
				sx+=Lidar_x[i]; sy+=Lidar_y[i];
			}	
			k1=((num_in*xy-sx*sy))/(num_in*xx-sx*sx);
			b1=((xx*sy-sx*xy))/(num_in*xx-sx*sx);
			Result_n[num_result]=num_in;
			Result_k[num_result]=k1;
			Result_b[num_result]=b1;
			angle[num_result]=atan(k1);
			num_result++;
		}
	}
	if (One_Board_Flag)
	{
		for (i=0;i<num_result;i++)
		{
			score=Result_n[i];
			if (score>maxscore)
			{
				ans1=i; 
				maxscore=score;
			}
		}
		if (maxscore>40)
			Lidar_Dist1=fabs(Result_b[ans1]/sqrt(Result_k[ans1]*Result_k[ans1]+1));
	 	if (Lidar_Debug)
		{
		 	LCD_Clear();
			LCD_SetXY(0,0);
			LCD_WriteString(" maxscore:");
			LCD_WriteNum(maxscore);
			LCD_SetXY(0,1);
			LCD_WriteString("dist:");
			LCD_WriteInt((int16)Lidar_Dist1);
		}
	
    	Lidar_Stat=Lidar_Waiting;
		return;
	}
	
	for (i=0;i<num_result;i++)
		for (j=0;j<num_result;j++)
		{
			dtheta=angle[i]-angle[j]+2*PI-PI/4;
			while (dtheta>PI)
				dtheta-=PI;
			if (dtheta>0.1)
				continue;
			score=Result_n[i]*Result_n[j]-Lidar_ScoreFactor*pow(10*dtheta,10);
			if (score>maxscore)
			{
				ans1=i; ans2=j;
				maxscore=score;
			}
		}
	if (maxscore>100)
	{
		if (angle[ans1]<0) angle[ans1]+=PI;
		if (angle[ans2]<0) angle[ans2]+=PI;
		
		theta1=fabs(angle[ans1]-PI/2);
		theta2=fabs(angle[ans2]-PI/2);
		if (theta2>theta1)	{			i=ans1; ans1=ans2; ans2=i;	}
		Lidar_Dist1=fabs(Result_b[ans1]/sqrt(Result_k[ans1]*Result_k[ans1]+1));
        Lidar_Dist2=fabs(Result_b[ans2]/sqrt(Result_k[ans2]*Result_k[ans2]+1));

	 	if (Lidar_Debug)
		{
			LCD_Clear();
			LCD_SetXY(0,0);
			LCD_WriteNum(maxscore);
			LCD_SetXY(10,0);
			LCD_WriteString(" A");
			LCD_WriteInt(ans1);
			LCD_WriteString("B");
			LCD_WriteInt(ans2);
			LCD_SetXY(0,1);
			LCD_WriteString("d1:");
			LCD_WriteInt((int16)Lidar_Dist1);
			LCD_SetXY(10,1);
			LCD_WriteString("d2:");
			LCD_WriteInt((int16)Lidar_Dist2);
			
			LCD_SetXY(0,2);
			LCD_WriteNum(Result_k[ans1]);
			LCD_SetXY(10,2);
			LCD_WriteString(" ");
			LCD_WriteNum(Result_b[ans1]);
			
			LCD_SetXY(0,3);
			LCD_WriteNum(Result_k[ans2]);
			LCD_SetXY(10,3);
			LCD_WriteString(" ");
			LCD_WriteNum(Result_b[ans2]);

			PressAnyKey();
		}
		
	 	LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString(" maxscore:");
		LCD_WriteNum(maxscore);
		LCD_SetXY(0,1);
		LCD_WriteString("d1:");
		LCD_WriteInt((int16)Lidar_Dist1);
		LCD_SetXY(10,1);
		LCD_WriteString("d2:");
		LCD_WriteInt((int16)Lidar_Dist2);
		
		LCD_SetXY(0,2);
		LCD_WriteInt((int16)Lidar_Pos.x);
		LCD_SetXY(10,2);
		LCD_WriteInt((int16)Lidar_Pos.y);

		LCD_SetXY(0,3);
		LCD_WriteString("Phi:");
		LCD_WriteNum(Lidar_Phi);

	//		PressAnyKey();

	
    	Lidar_Stat=Lidar_Waiting;
    	return;
	}
   	Lidar_Stat=Lidar_Error;
}

/****************************************************************
void LidarRing()

Function:	Get local position of scanned circles
			(RANSAC+SA)
***************************************************************/
void LidarRing(void)
{
	int16 i,step,num_in,num_result,rangenum;
	fp64 X1,X2,Y1,Y2;
	fp64 x_mid,y_mid,k,b,x_center,y_center,rad;
	fp64 dist,x_tmp,y_tmp,x_max,y_max,tdist,score,score_min;
	int16 dep,times;
	static fp64 x_in[300],y_in[300];
	
	num_result=0; Lidar_Num_Result=0;
	Lidar_Pos_Orig=Gps_List[0].position;
	
	URG_GetXY();
	URG_LineFilter();
	URG_Filter();

	for (rangenum=0;rangenum<LidarRangesNum;rangenum++)
	{	
		for(step=LidarRanges[rangenum].start_step; step+10<=LidarRanges[rangenum].end_step; step+=2*FCT_SPAN_R)
		{
			for (i=step+1;i<=LidarRanges[rangenum].end_step;i++)
				if (sqrt((Lidar_x[step]-Lidar_x[i])*(Lidar_x[step]-Lidar_x[i])+(Lidar_y[step]-Lidar_y[i])*(Lidar_y[step]-Lidar_y[i]))>FCT_LENGTH_R)
					break;
			if (LidarLength[step]<LidarRanges[rangenum].min_distance||LidarLength[step]>LidarRanges[rangenum].max_distance)
					continue;
			if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
			X1=Lidar_x[step]; 	Y1=Lidar_y[step];
			X2=Lidar_x[i];		Y2=Lidar_y[i];
			
			x_mid=(fp64)(X1+X2)/2.0; y_mid=(fp64)(Y1+Y2)/2.0;
			k=-(fp64)(X2-X1)/(fp64)(Y2-Y1);
            b=(fp64)y_mid-k*(fp64)x_mid;
            
            rad=atan(k); if (sin(rad)<0) rad=PI+rad; 
            tdist=sqrt(pow(x_mid-X1,2)+pow(y_mid-Y1,2));
            if (tdist>FCT_R_RING)
            	continue;
            x_center=x_mid+cos(rad)*sqrt(pow(FCT_R_RING,2)-pow(tdist,2)); //edited;
            y_center=y_mid+sin(rad)*sqrt(pow(FCT_R_RING,2)-pow(tdist,2));

            
            num_in=0;
   			for (i=LidarRanges[rangenum].start_step;i<=LidarRanges[rangenum].end_step;i++)
			{
				if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
					continue;
	            dist=fabs(sqrt((Lidar_x[i]-x_center)*(Lidar_x[i]-x_center)+(Lidar_y[i]-y_center)*(Lidar_y[i]-y_center))-FCT_R_RING);
				if (dist<FCT_WIDTH_R)
				{
					x_in[num_in]=Lidar_x[i];
					y_in[num_in]=Lidar_y[i];
					num_in++;
				}
			}
			
            if (num_in<FCT_COUNT_R)
                continue;
             
            score=num_in;
            if ((num_result!=0)&&(sqrt((x_center-Lidar_ResultX[num_result-1])*(x_center-Lidar_ResultX[num_result-1])+(y_center-Lidar_ResultY[num_result-1])*(y_center-Lidar_ResultY[num_result-1]))<500))
            {
            	if (score>Lidar_ResultS[num_result-1])
            	{
            	
					Lidar_ResultS[num_result-1]=score;
					Lidar_ResultX[num_result-1]=x_center;
					Lidar_ResultY[num_result-1]=y_center;
           		}
           	}
           	else 
           	{
	           	Lidar_ResultS[num_result]=score;
				Lidar_ResultX[num_result]=x_center;
				Lidar_ResultY[num_result]=y_center;
				num_result++;
           	}
		}
	}
	
	srand((uint32)LidarRing);
	for (i=0;i<num_result;i++)
	{
		x_max=Lidar_ResultX[i]; y_max=Lidar_ResultY[i]; score=100000;
		dep=200;

        num_in=0;
		for (i=LidarRanges[rangenum].start_step;i<=LidarRanges[rangenum].end_step;i++)
		{
			if (LidarLength[i]<LidarRanges[rangenum].min_distance||LidarLength[i]>LidarRanges[rangenum].max_distance)
				continue;
            dist=fabs(sqrt((Lidar_x[i]-x_max)*(Lidar_x[i]-x_max)+(Lidar_y[i]-y_max)*(Lidar_y[i]-y_max))-FCT_R_RING);
			if (dist<2*FCT_WIDTH_R)
			{
				x_in[num_in]=Lidar_x[i];
				y_in[num_in]=Lidar_y[i];
				num_in++;
			}
		}

		while(dep>5)
		{
		    score_min=1000000;
		    for (times=0;times<20;times++)
		    {
		        x_center=x_max+((fp64)rand()-16384.0)/32768.0*dep; y_center=y_max+((fp64)rand()-16384.0)/32768.0*dep;        
		        score=0;
		        for (i=0;i<num_in;i++)
			        score+=pow(sqrt(pow(x_in[i]-x_center,2)  +  pow(y_in[i]-y_center,2))- FCT_R_RING,2);
			    score/=num_in;
		        if (score<score_min)
		        {
		            x_tmp=x_center; y_tmp=y_center;
		            score_min=score;
		        }
			}
		    x_max=x_tmp; y_max=y_tmp;
		    dep=dep/2;
		}
		Lidar_ResultX[i]=x_max; 
		Lidar_ResultY[i]=y_max; 
		Lidar_ResultS[i]=score_min;

	}
	if (Lidar_CalibFlag)
		for(i=0;i<num_result;i++)
			Lidar_ResultY[i]+=1000*PPoly_Eval(Lidar_ResultY[i]/1000.0,LIDAR);


	Lidar_Stat=Lidar_Waiting;
	Lidar_Num_Result=num_result;
	
	if (Lidar_Debug)
	for (i=0;i<num_result;i++)
	{
	   	LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("Ring  No,");
		LCD_WriteInt(i);
		LCD_SetXY(0,1);
		LCD_WriteString("x:");
		LCD_WriteNum(Lidar_ResultX[i]);
		LCD_SetXY(0,2);
		LCD_WriteString("y:");
		LCD_WriteNum(Lidar_ResultY[i]);
		PressAnyKey();

	}
	LCD_SetXY(0,3);
	LCD_WriteString("Done!");

}


/****************************************************************
void LidarTest()

Function:	Test function..
*****************************************************************/

void LidarTest()
{	
	int i;
	struct Value Values_Basic[]=
   	{    
        {(void*)&Lidar_TypeSel				,TYPE_UINT8	,"TypeSel"},
        {(void*)&Lidar_Debug				,TYPE_UINT8	,"Debug"},
        {(void*)&Lidar_CalibFlag			,TYPE_UINT8	,"Lidar_Calib"},
	};
	struct Value Values_Ranges[]=
   	{    
        {(void*)&LidarRangesNum				,TYPE_INT8	,"RangesNum"},
        {(void*)&LidarRanges[0].start_step	,TYPE_INT16	,"Range1.sta"},
        {(void*)&LidarRanges[0].end_step	,TYPE_INT16	,"Range1.end"},
        {(void*)&LidarRanges[1].start_step	,TYPE_INT16	,"Range2.sta"},
        {(void*)&LidarRanges[1].end_step	,TYPE_INT16	,"Range2.end"},
        {(void*)&LidarRanges[2].start_step	,TYPE_INT16	,"Range3.sta"},
        {(void*)&LidarRanges[2].end_step	,TYPE_INT16	,"Range3.end"},
   	};

	struct Value Values_Ring[]=
   	{    
		{(void*)&FCT_TOLERATE_R				,TYPE_INT16	,"FCT_TOL_R"},
		{(void*)&FCT_R_RING 				,TYPE_INT16	,"FCT_R"},
		{(void*)&FCT_SPAN_R 				,TYPE_INT16	,"FCT_SPN_R"},
		{(void*)&FCT_WIDTH_R				,TYPE_INT16	,"FCT_WID_R"},
		{(void*)&FCT_COUNT_R 				,TYPE_INT16	,"FCT_CNT_R"},
  	};
	struct Value Values_Board[]=
   	{    
        {(void*)&FCT_SPAN				,TYPE_INT16	,"FCT_SPAN"},
        {(void*)&FCT_WIDTH				,TYPE_INT16	,"FCT_WIDTH"},
        {(void*)&FCT_LENGTH 			,TYPE_INT16	,"FCT_LENGTH"},
        {(void*)&FCT_COUNT 				,TYPE_INT16	,"FCT_COUNT"},
	};
   	
   	DISPLAY_ValuesManager(Values_Basic,3,40000+64*6);
   	
   	if (Lidar_TypeSel==Lidar_Ring)
   	{
   		for (i=0;i<1;i++)
   			LidarRanges[i]=LidarRangesRing[i];
   		LidarRangesNum=1;
	   	DISPLAY_ValuesManager(Values_Ranges,7,40000+64*6);
	   	DISPLAY_ValuesManager(Values_Ring,5,40000+64*6);
	}
	else if (Lidar_TypeSel==Lidar_Board)
	{
		for (i=0;i<1;i++)
   			LidarRanges[i]=LidarRangesBoard[i];
   		LidarRangesNum=1;
	   	DISPLAY_ValuesManager(Values_Ranges,7,40000+64*6);
	  	DISPLAY_ValuesManager(Values_Board,4,40000+64*6);	
	}
  	else 
  	{
   		for (i=0;i<1;i++)
   			LidarRanges[i]=LidarRangesRing[i];
   		LidarRangesNum=1;
	   	DISPLAY_ValuesManager(Values_Ranges,7,40000+64*6);
	   	DISPLAY_ValuesManager(Values_Ring,5,40000+64*6);
  	}		
	  	
   	Display_Page=APPLICATION;
	for(;;)
	{
		switch (Lidar_TypeSel)
		{
			case Lidar_Board:
			
				LidarBoard_NoRANSAC();
				break;
				
				
			case Lidar_Ring:
			
				LidarRing();
				break;
				
				
			case Lidar_Board_Running:
			
				LidarCircle();
				break;
		}
		break;
	}
}





#endif 