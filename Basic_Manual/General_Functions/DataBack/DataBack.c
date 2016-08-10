#include "all.h"

#if DatabackSetting
int32 BackDataCount=0;
fp64 BackData[30000][16]={0};
//pData[0]=BackData[0];

fp64 Data[16];

extern void PC_WriteNum(double data);
extern void PC_WriteChar(char data);

///////////////////////////////////////////////
//数据统一转成 拥有4位有效数字【科学计数法】 然后，4位有效数字以【int16】格式发送 如【1.234】按照【1234】发送
//科学计数位数以【int8】格式发送 如【10^12】以【12】发送
void PC_WriteNumNew(double data)
{
	union i16tou8
	{
		int16 value;
		uint8 datas[2];
	}coef;
	int8 cnt=floor(log10(fabs(data)));
	coef.value=(int16)(data/pow(10,cnt-3));
	UART0_SendByte(coef.datas[0]);
	UART0_SendByte(coef.datas[1]);
	UART0_SendByte((uint8)(cnt-3));	
}

void PC_WriteIntNew(int data)
{
	PC_WriteNum((double)data);
}


///////////////////////////////////////////////
void BackDataInit(void)
{
	BackDataCount=0;
}


void SaveMovingData(void)
{
	if(BackDataCount<30000)
	{
/*	fp64 MovingData[10];
	BackData[DataCount][0] = Gps_List[0].position.x;
	BackData[DataCount][1] = Gps_List[0].position.y;
	BackData[DataCount][2] = Gps_List[0].radian;
	BackData[DataCount][3] = error_angle;
	BackData[DataCount][4] = error_dis;
	BackData[DataCount][5] = L_speed;
	BackData[DataCount][6] = R_speed;
	BackData[DataCount][7] = F_speed;
	BackData[DataCount][8] = v_err_x;
	BackData[DataCount][9] = v_err_y;
	pData=MovingData;
	pData=pData+1;
	DataCount++;
	*/
	BackData[BackDataCount][0]  = Point_NowNum;//
	BackData[BackDataCount][1]  = Gps_List[0].position.x;//
	BackData[BackDataCount][2]  = Gps_List[0].position.y;//
	BackData[BackDataCount][3]  = Gps_List[0].radian;//
	BackData[BackDataCount][4]  = Data[4];//pointpath error_angle
	BackData[BackDataCount][5]  = Vout[0];//
	BackData[BackDataCount][6]  = Data[6];//pointpath error_dis
	BackData[BackDataCount][7]  = Vout[1];//
	BackData[BackDataCount][8]  = 0;//PRE_Count1;
	BackData[BackDataCount][9]  = 0;//Point_EndNum-PRE_Count1;
	BackData[BackDataCount][10] = 0;//supersonic_temp1;
	BackData[BackDataCount][11] = 0;//Gps_Speed; 
	BackData[BackDataCount][12] = Data[12];//goline error_angle
	BackData[BackDataCount][13] = Data[13];//goline error_dis
	BackData[BackDataCount][14] = 0;
	
	BackDataCount++;
	}
}

void SendBackData(void)
{
		
	int i , j ;
	for(i=0 ; i<=BackDataCount ; i++)
		for(j=0 ; j<=14 ; j++)
			PC_WriteNumNew( BackData[i][j] );
}

/*void SendEndData(void)
{
	
	for( ; ; )
	{
		PC_WriteChar('*');
	}
	
}*/

void SendStartData(void)
{
	int i ;
	for( i=0 ; i<100 ; i++ )
	{
		PC_WriteChar('*');
	}
	
}

void DataBackTest(void)
{
	//double i ;
	uint8 *msg;
	
	for(;;)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_Printf("Data saved %d" , BackDataCount);
		LCD_SetXY(0,1);
		LCD_Printf("Press Enter to Send" );	
				
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
		OSTimeDly(20);	
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			/*case 1:
				BackDataInit();
				for(i=0 ; i<10000 ; i++) 
				{
					SaveMovingData();
				}
				LCD_SetXY(0,3);
				LCD_Printf("Save data %d" , BackDataCount);
				break;*/
			
			case 18:
			
			    LCD_Clear();
		        LCD_SetXY(4,3);
		        LCD_Printf("Waiting....." );	
				//PC_WriteIntNew(BackDataCount);
				PC_WriteNumNew(BackDataCount);
				
				//PC_WriteInt(BackDataCount);
				//PC_WriteChar('\n');
				
				SendBackData();
				PC_WriteChar('$');
				 
				//SendEndData();
				
				LCD_Clear();
				LCD_SetXY(4,3);
		        LCD_Printf("Done~!" );
				break;
		}
	}
}


#endif