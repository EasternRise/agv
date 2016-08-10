	#include "lcd.h"
	#include "SysTick.h"
	#include "usart.h"

void Input_IntValue(int *address,char *name)//整型输入
{
//	u32 key_value;
    
	char str[20]={'\0'};
	int temp;
	int i,j;

	str[0]=' ';

	i=1;

	for(;;)
	{

		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString(name);
		LCD_SetXY(0,1);
		LCD_WriteString("old:");
		LCD_WriteInt(*address);
		LCD_SetXY(0,2);
		LCD_WriteString("new:");
		LCD_WriteString(str);
		refresh();
		
		if(i<=1)
		{
			LCD_SetXY(5,2);
			LCD_WriteString("Value=0!!!");
		}
		
		delay(LCD_DELAY);
		
        if(KeyReady)
		{
			KeyReady=0; //清按键标志位
			
            switch(KeyValue)
            {
                case 1://1
                    if(i<14)
                        str[i++]='1';
                break;
                case 2://2
                    if(i<14)
                        str[i++]='2';
                break;
                case 3://3
                    if(i<14)
                        str[i++]='3';
                break;
                case 4://4
                    if(i<14)
                        str[i++]='4';
                break;
                case 7://5
                    if(i<14)
                        str[i++]='5';
                break;
                case 8://6
                    if(i<14)
                        str[i++]='6';
                break;
                case 9://7
                    if(i<14)
                        str[i++]='7';
                break;
                case 10://8
                    if(i<14)
                        str[i++]='8';
                break;
                case 13://9
                    if(i<14)
                        str[i++]='9';
                break;
                case 14://0
                    if(i<14)
                        str[i++]='0';
                break;
				case 16://-
                    if(str[0] == ' ')
                        str[0] = '-';
                    else
                        str[0] = ' ';
                break;
                case 17://del
                    if(i>1)
                    {
                        str[--i]='\0';
                    }
                break;
                case 18://ok
                    temp=0;
                    
                    for(j=1;j<i;j++)//计算输入值
                    {
                        temp=temp*10+str[j]-'0';
                    }
                    if(str[0]=='-')
                        temp=-temp;
                    *address=temp;
                    return;
                
                case 5://cancel
                    return;
            }
        }
	}
}

void Input_DoubleValue(double * address,char *name)//浮点输入
{
//	u32 key_value;
    u32 point=0;
	char str[20]={'\0'};
	float temp;
	int i,j;

	str[0]=' ';
	
	i=1;
	point=0;
	while(1)
	{

		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString(name);
		LCD_SetXY(0,1);
		LCD_WriteString("old:");
		LCD_WriteNum(*address);
		LCD_SetXY(0,2);
		LCD_WriteString("new:");
		LCD_WriteString(str);
		refresh();		
		if(i<=1)
		{
			LCD_SetXY(5,2);
			LCD_WriteString("Value=0!!!");
		}
		
		delay(LCD_DELAY);
		
        if(KeyReady)
        {
			     KeyReady=0;	//清按键标志位
            switch(KeyValue)
            {
                case 1://1
                    if(i<14)
                        str[i++]='1';
                break;
                case 2://2
                    if(i<14)
                        str[i++]='2';
                break;
                case 3://3
                    if(i<14)
                        str[i++]='3';
                break;
                case 4://4
                    if(i<14)
                        str[i++]='4';
                break;
                case 7://5
                    if(i<14)
                        str[i++]='5';
                break;
                case 8://6
                    if(i<14)
                        str[i++]='6';
                break;
                case 9://7
                    if(i<14)
                        str[i++]='7';
                break;
                case 10://8
                    if(i<14)
                        str[i++]='8';
                break;
                case 13://9
                    if(i<14)
                        str[i++]='9';
                break;
                case 14://0
                    if(i<14)
                        str[i++]='0';
                break;
                case 15://.
                    if(point==0&&i<14)
                    {
                        str[i++]='.';
                        point=1;
                    }
                break;
				case 16://-
                    if(str[0] == ' ')
                        str[0] = '-';
                    else
                        str[0] = ' ';
                break;
                case 17://del
                    if(i>1)
                    {
                        if(str[i-1]=='.')
                            point=0;
                        str[--i]='\0';
                    }
                break;
                case 18://ok
                    temp=0;
                    point=0;
                    for(j=1;j<i;j++)// 
                    {
                        if(str[j]=='.')
                        {
                            point=10;
                        }
                        else if(point==0)
                        {
                            temp=temp*10+str[j]-'0';
                        }
						else
						{
							temp+=(str[j]-'0')/(float)point;
							point=point*10;
						}
                    }
                    if(str[0]=='-')
                        temp=-temp;
                    *address=temp;
                    return;
                
                case 5://cancel
                    return;
				default:	break;
            }
        }
	}
}

void Input_FloatValue(float * address,char *name)//浮点输入
{
	//u32 key_value;
    u32 point=0;
	char str[20]={'\0'};
	float temp;
	int i,j;

	str[0]=' ';
	
	i=1;
	point=0;
	for(;;)
	{

		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString(name);
		LCD_SetXY(0,1);
		LCD_WriteString("old:");
		LCD_WriteNum(*address);
		LCD_SetXY(0,2);
		LCD_WriteString("new:");
		LCD_WriteString(str);
		refresh();
		if(i<=1)
		{
			LCD_SetXY(5,2);
			LCD_WriteString("Value=0!!!");
		}
		
		delay(LCD_DELAY);
		
        if(KeyReady)
        {
			KeyReady=0;	//清按键标志位
            switch(KeyValue)
            {
                case 1://1
                    if(i<14)
                        str[i++]='1';
                break;
                case 2://2
                    if(i<14)
                        str[i++]='2';
                break;
                case 3://3
                    if(i<14)
                        str[i++]='3';
                break;
                case 4://4
                    if(i<14)
                        str[i++]='4';
                break;
                case 7://5
                    if(i<14)
                        str[i++]='5';
                break;
                case 8://6
                    if(i<14)
                        str[i++]='6';
                break;
                case 9://7
                    if(i<14)
                        str[i++]='7';
                break;
                case 10://8
                    if(i<14)
                        str[i++]='8';
                break;
                case 13://9
                    if(i<14)
                        str[i++]='9';
                break;
                case 14://0
                    if(i<14)
                        str[i++]='0';
                break;
                case 15://.
                    if(point==0&&i<14)
                    {
                        str[i++]='.';
                        point=1;
                    }
                break;
				case 16://-
                    if(str[0] == ' ')
                        str[0] = '-';
                    else
                        str[0] = ' ';
                break;
                case 17://del
                    if(i>1)
                    {
                        if(str[i-1]=='.')
                            point=0;
                        str[--i]='\0';
                    }
                break;
                case 18://ok
                    temp=0;
                    point=0;
                    for(j=1;j<i;j++)// 
                    {
                        if(str[j]=='.')
                        {
                            point=10;
                        }
                        else if(point==0)
                        {
                            temp=temp*10+str[j]-'0';
                        }
						else
						{
							temp+=(str[j]-'0')/(float)point;
							point=point*10;
						}
                    }
                    if(str[0]=='-')
                        temp=-temp;
                    *address=temp;
                    return;
                
                case 5://cancel
                    return;
            }
        }
	}
}
