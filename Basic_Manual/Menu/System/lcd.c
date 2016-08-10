#include "all.h"

uint8 LCD_Table[LCD_LEN];
uint8 LCD_Index=0;

#if LCDBufferSetting
uint8 LCDTable[20][8];		//存储当前液晶屏显示内容
uint8 LCDTableSave[20][8];	//存储进入shortcut时用户任务显示内容
uint8 LCDIndex=0;			//存储当前LCD引索值
uint8 LCDIndexSave;			//存储进入shortcut时用户任务引索值
#endif

#if HaltSetting
uint8 HaltFlag=0;			//暂停标志位，1暂停，0继续
uint8 HaltBuffer[5]="    ";	//右下角显示halt时，存储本应显示的LCD字符
#endif

void LCD_PushByte(uint8 data)		//发送一个字符
{
#if HaltSetting
	uint8 StringHalt[5]="halt";
	if(HaltFlag==1 && LCDIndex>75 && LCDIndex<80)
	{
		if(data>=0x20 && data<=0x7e)
		{
			UART1_TxBuffer[UART1_TxBufferTop++]=StringHalt[LCDIndex-76];
			HaltBuffer[LCDIndex-76]=data;
			LCDIndex++;
			if(LCDIndex==LCD_LEN)
				LCDIndex=0;
			return;
		}
	}
#endif
	LCD_Table[LCD_Index]=data;
	if(LCD_Index==LCD_LEN-1)
		LCD_Index=0;
	else
		LCD_Index++;
	//UART1_TxBuffer[UART1_TxBufferTop++]=data;
#if LCDBufferSetting
	if(data>=0x20 && data<=0x7e)	//不计入命令字符
	{
		LCDTable[LCDIndex%20][LCDIndex/20]=data;
		LCDIndex++;
		if(LCDIndex==LCD_LEN)
			LCDIndex=0;
	}
#endif
	//UNMASKSubIrq(BIT_SUB_TXD1);
}

void LCD_PushString(char *data)		//发送一个字符串
{
	while(*data)
	{
		LCD_PushByte(*data++);
	}
}

void LCD_Printf(const char *fmt,...)
{
    va_list ap;
    char string[LCD_LEN];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    va_end(ap);
	LCD_PushString(string);

}
void UART0_Printf(const char *fmt,...)
{
    va_list ap;
    char string[LCD_LEN];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    va_end(ap);
	UART0_SendString(string);

}
void LCD_SetXY(int x,int y)
{
	LCD_Index=x+y*20;
}
void LCD_WriteChar(char data)
{
	LCD_PushByte(data);
}

void LCD_WriteString(char *data)
{
	LCD_PushString(data);
}

void LCD_WriteInt(int data)//写整数，从低位向高位写
{
	char answer[100];
	int index=98;
	int pose=0;//正负号标志符
	if(data<0)
	{
		pose=1;
		data=-data;
	}
	answer[99]='\0';//最低位存储结束标志
	while(data>9)//存储整数位
	{
		answer[index--]=data%10+'0';
		data=data/10;
	}
	answer[index]=data+'0';
	if(pose)//存储正负号
	{
		answer[--index]='-';
	}
	LCD_PushString(answer+index);//写整个数组
}
void LCD_WriteDouble(double data,int m)//写double型数，m为小数点后位数，从高位向低位写
{
	if(data<0&&data>-1)
		LCD_WriteChar('-');//写负号
	LCD_WriteInt((int)data);//写整数部分
	if(m>0)
	{
		int i;
		LCD_WriteChar('.');//写小数点
		if(data<0)
			data=-data;
		data=data-(int)data;
		for(i=0;i<m;i++)//写小数部分
		{
			data=data*10;
			LCD_WriteChar((int)data+'0');
			data=data-(int)data;
		}
		
	}
	
}
void LCD_WriteNum(double data)//写double型数，小数点后保留6位，从高位向低位写
{
	char str[5];
	int flag=0,i;
	if(data<0)
	{
		LCD_WriteChar('-');//写负号
		data=-data;
	}
	LCD_WriteInt((int)data);//写整数部分
	data=data-(int)data;
	for(i=0;i<5;i++)//将小数部分保留在一个字符串内
	{
		data=data*10;
		str[i]=(int)data+'0';
		if(str[i]!='0')
		{
			flag=i+1;
		}
		data=data-(int)data;
		
	}
	if(flag!=0)
		LCD_WriteChar('.');//写小数点
	for(i=0;i<flag;i++)
	{
		LCD_WriteChar(str[i]);//写小数部分
	}	
	for(i=flag;i<6;i++)
	{
		LCD_WriteChar(' ');//小数点后不够6位则补空格
	}
}

void LCD_WriteCharXY(char data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_PushByte(data);
}

void LCD_WriteStringXY(char *data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_PushString(data);
}

void LCD_WriteIntXY(int data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteInt(data);
}

void LCD_WriteDoubleXY(double data,int m,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteDouble(data, m);
}

void LCD_WriteNumXY(double data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteNum(data);
}

#if LCDBufferSetting

void LCD_Save()//存储LCD页面
{
	int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<20;j++)
			LCDTableSave[j][i]=LCDTable[j][i];//将当前LCD显示内容储存
	LCDIndexSave=LCDIndex;//将当前LCD引索储存
#if HaltSetting
	if(HaltFlag==1)//暂停状态时，不存入右下角的标志"halt"，而是存入本应显示的值
	{
		LCDTableSave[16][3]=HaltBuffer[0];
		LCDTableSave[17][3]=HaltBuffer[1];
		LCDTableSave[18][3]=HaltBuffer[2];
		LCDTableSave[19][3]=HaltBuffer[3];
	}
#endif
}

void LCD_Load()//加载存储的LCD页面
{
	int i,j;
	LCD_Clear();
	for(i=0;i<8;i++)
		for(j=0;j<20;j++)
		{
			LCD_WriteChar(LCDTableSave[j][i]);//写存储的LCD页面内容
		}
	LCD_SetXY(LCDIndexSave%20,LCDIndex/20);//重新定位当前LCD引索位置
	//LCDIndex=LCDIndexSave;
}

#endif

void LCD_Clear()//清屏时会同时将光标置(0,0)
{
	//LCD_WriteChar(12);//不用命令清屏时为了LCD_PushByte()函数可以记录LCD内容
	LCD_WriteStringXY("                                                                                ",0,0); 
	LCD_WriteStringXY("                                                                                ",0,4); 

	LCD_SetXY(0,0);
}



