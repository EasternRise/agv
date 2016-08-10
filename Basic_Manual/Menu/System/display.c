#include "all.h"


int Display_Page;
int Last_Page;

uint8 GyroCheckEnable=1;
uint8 StartPage=MENU;


//路径菜单显示函数
void DISPLAY_ShowMenu(void)
{
	int TmpCount;
	int i,j,k,l=0;
	char *linename;
	switch(Display_Page)
	{
		case MENU:
			TmpCount=Menu_Count;
		break;
		case SUBMENU1:
			TmpCount=SubMenu1_Count;
		break;
		case SUBMENU2:
			TmpCount=SubMenu2_Count;
		break;
		case SUBMENU3:
			TmpCount=SubMenu3_Count;
		break;
		case SUBMENU4:
			TmpCount=SubMenu4_Count;
		break;
		case SUBMENU5:
			TmpCount=SubMenu5_Count;
		break;
		case SUBMENU6:
			TmpCount=SubMenu6_Count;
		break;
		case SUBMENU7:
			TmpCount=SubMenu7_Count;
		break;
		case SUBMENU8:
			TmpCount=SubMenu8_Count;
		break;
#if ShortcutSetting
		case SHORTCUTMENU:
			TmpCount=ShortcutMenu_Count;
		break;
#endif
	}
	for(l=0;l<2;l++)
	{
		for(i=0;i<4;i++)
		{
			LCD_SetXY(l*10,i);
			if(i+l*4<TmpCount)
			{
				k=0;
				LCD_WriteInt(i+l*4+1);
				switch(Display_Page)
				{
					case MENU:
						linename=Menu[i+l*4].name;
					break;
					case SUBMENU1:
						linename=SubMenu1[i+l*4].name;
					break;
					case SUBMENU2:
						linename=SubMenu2[i+l*4].name;
					break;
					case SUBMENU3:
						linename=SubMenu3[i+l*4].name;
					break;
					case SUBMENU4:
						linename=SubMenu4[i+l*4].name;
					break;
					case SUBMENU5:
						linename=SubMenu5[i+l*4].name;
					break;
					case SUBMENU6:
						linename=SubMenu6[i+l*4].name;
					break;
					case SUBMENU7:
						linename=SubMenu7[i+l*4].name;
					break;
					case SUBMENU8:
						linename=SubMenu8[i+l*4].name;
					break;
#if ShortcutSetting
					case SHORTCUTMENU:
						linename=ShortcutMenu[i+l*4].name;
					break;
#endif
				}
				while(*linename!=0&&k<=8)
				{
					LCD_PushByte(*linename++);
					k++;
				}
				for(j=k;j<9;j++)
				{
					LCD_WriteChar(' ');
				} 
			}
			else
			{
				LCD_WriteString("          ");
			}
		}
	}
}


uint8 DISPLAY_INT16Value(uint16 * address,char *name)
{
	//uint8 err;
	uint8 *msg;
	
	char str[20];
	int16 temp;
	int i,j;
	
	LCD_Clear();
	OSTimeDly(1);
	LCD_Clear();
	
	for(i=1;i<20;i++)
	{
		str[i]='\0';
	}
	str[0]=' ';
	
	LCD_Clear();
	i=1;

	for(;;)
	{
		LCD_SetXY(0,0);
		LCD_WriteString(name);
		LCD_WriteString(":      ");
		LCD_SetXY(0,1);
		LCD_WriteString("old:");
		LCD_WriteInt(*address);
		LCD_WriteString("  ");
		
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		

			
		switch(*msg)
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
			case 16://-
				if(str[0]==' ')
					str[0]='-';
				else
					str[0]=' ';
			break;
			case 13://9
				if(i<14)
					str[i++]='9';
			break;
			case 14://0
				if(i<14)
					str[i++]='0';
			break;

			case 17://del
				if(i>1)
				{
					str[i--]='\0';
				}
			break;
			case 18://ok
				temp=0;
				
				for(j=1;j<i;j++)// 计算输入值
				{
					temp=temp*10+str[j]-'0';

				}
				if(str[0]=='-')
					temp=-temp;
				*address=temp;
				return  1;
			break;
			
			case 5://cancel
				return  0;
			break;
		}

		LCD_SetXY(0,2);
		LCD_WriteString("new:");
		LCD_WriteString(str);
		LCD_SetXY(i+4,2);
		LCD_WriteString("             ");
		LCD_SetXY(0,3);
		if(i<=1)
		{
			LCD_WriteString("Value=0!!!");
		}
		else
		{
			LCD_WriteString("            ");
		}
	}
}




uint8 DISPLAY_Fp64Value(fp64 * address,char *name)
{
	uint8 *msg;
	
	char str[20]={'\0'};
	fp64 temp;
	int i,j;
	fp64 point=0;
	
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
		if(i<=1)
		{
			LCD_WriteDouble(*address,6);
		}
		
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		

			
		switch(*msg)
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
				if(str[0]==' ')
					str[0]='-';
				else
					str[0]=' ';
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
				if(i>1)
				{
					for(j=1;j<i;j++)// 计算输入值
					{
						if(str[j]=='.')//当该位为点时
						{
							point=10;
						}
						else if(point==0)//当没有遇到小数点时
						{
							temp=temp*10+str[j]-'0';
						}
						else
						{
							temp+=(str[j]-'0')/point;
							point=point*10;
						}
						
					}
					if(str[0]=='-')
						temp=-temp;
					*address=temp;
				}
				return 1;
			break;
			
			case 5://cancel
				return 0;
			break;
		}

		
	}
}

void DISPLAY_Message(char *message)
{
	int i;
	uint8 err;
	Last_Page=Display_Page;
	Display_Page=APPLICATION;
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("======MESSAGE=======");
	for(i=0;i<40;i++)
	{
		if(*message)
			LCD_PushByte(*message++);
		else
			break;
	}
	LCD_SetXY(0,3);
	LCD_WriteString("Press a key to skip.");
	OSMboxPend(AppKey1Mbox,0,&err);
	Display_Page=Last_Page;
}

void DISPLAY_ValuesManager(struct Value * address,uint16 length,uint16 nandaddress)
{
	//uint8 err;
	uint8 *msg;
	int i;
	int allpage=(length-1)/3;
	int nowpage=0;
	int index;
	fp64 out;
	LCD_Clear();
	for(;;)
	{
		for(i=0;i<3;i++)
		{
			index=nowpage*3+i;
			if(index<length)
			{
				LCD_SetXY(0,i);
				LCD_WriteInt(i+1);
				LCD_SetXY(1,i);
				LCD_WriteString(address[index].name);
				LCD_WriteChar(':');
				switch(address[index].type)
				{
					case TYPE_INT8:
						LCD_WriteNum(*(int8*)(address[index].enter));
					break;
					case TYPE_UINT8:
						LCD_WriteNum(*(uint8*)(address[index].enter));
					break;
					case TYPE_INT16:
						LCD_WriteNum(*(int16*)(address[index].enter));
					break;
					case TYPE_UINT16:
						LCD_WriteNum(*(uint16*)(address[index].enter));
					break;
					case TYPE_INT32:
						LCD_WriteNum(*(int32*)(address[index].enter));
					break;
					case TYPE_UINT32:
						LCD_WriteNum(*(uint32*)(address[index].enter));
					break;
					case TYPE_INT64:
						LCD_WriteNum(*(int64*)(address[index].enter));
					break;
					case TYPE_UINT64:
						LCD_WriteNum(*(uint64*)(address[index].enter));
					break;
					case TYPE_FP32:
				        LCD_WriteNum(*(fp32*)(address[index].enter));
					break;
					case TYPE_FP64:
						LCD_WriteNum(*(fp64*)(address[index].enter));
					break;
				}
			}
			else
			{
				LCD_SetXY(0,i);
				LCD_WriteString("                    ");
			}
		}
		LCD_SetXY(0,3);
		if(nandaddress>29999)
		{
			//LCD_Printf("5save 6load 7go %d/%d ",nowpage+1,allpage+1);
			LCD_Printf("5save 7go %d/%d ",nowpage+1,allpage+1);
		}
		else
		{
			LCD_Printf("                %d/%d ",nowpage+1,allpage+1);
		}
		
		OSMboxAccept(AppKey2Mbox);			//这一句是在清除pevent->OSEventPtr中可能存在的数据
			
		OSTimeDly(20);
			
		msg = OSMboxAccept(AppKey2Mbox);
		switch(*msg)
		{
			case 7:
				if(nandaddress>29999)
				{
					//NAND_SaveValues(address,length,nandaddress);
					PID_NandSave();
					DISPLAY_Message("Save Values");
				}
			break;
			
			case 8:
				if(nandaddress>29999)
				{
					//NAND_LoadValues(address,length,nandaddress);
					PID_NandInit();
					DISPLAY_Message("Load Values");
				}
			break;
			
			case 9:
				if(nandaddress>29999)
					return;
			break;
			case 5:
				return;
			break;
			case 13:
			case 15:
				if(nowpage>0)
					nowpage--;
				else
					nowpage=allpage;
			break;	
			case 14:
			case 16:
				if(nowpage<allpage)
					nowpage++;
				else
					nowpage=0;
			break;	
			case 1:
			case 2:
			case 3:
				index=*msg+nowpage*3-1;
				switch(address[index].type)
				{
					case TYPE_INT8:
						out=(*(int8*)(address[index].enter));
					break;
					case TYPE_UINT8:
						out=(*(uint8*)(address[index].enter));
					break;
					case TYPE_INT16:
						out=(*(int16*)(address[index].enter));
					break;
					case TYPE_UINT16:
						out=(*(uint16*)(address[index].enter));
					break;
					case TYPE_INT32:
						out=(*(int32*)(address[index].enter));
					break;
					case TYPE_UINT32:
						out=(*(uint32*)(address[index].enter));
					break;
					case TYPE_INT64:
						out=(*(int64*)(address[index].enter));
					break;
					case TYPE_UINT64:
						out=(*(uint64*)(address[index].enter));
					break;
					case TYPE_FP32:
						out=(*(fp32*)(address[index].enter));
					break;
					case TYPE_FP64:
						out=(*(fp64*)(address[index].enter));
					break;
				}
				DISPLAY_Fp64Value(&out,address[index].name);
				switch(address[index].type)
				{
					case TYPE_INT8:
						(*(int8*)(address[index].enter))=out;
					break;
					case TYPE_UINT8:
						(*(uint8*)(address[index].enter))=out;
					break;
					case TYPE_INT16:
						(*(int16*)(address[index].enter))=out;
					break;
					case TYPE_UINT16:
						(*(uint16*)(address[index].enter))=out;
					break;
					case TYPE_INT32:
						(*(int32*)(address[index].enter))=out;
					break;
					case TYPE_UINT32:
						(*(uint32*)(address[index].enter))=out;
					break;
					case TYPE_INT64:
						(*(int64*)(address[index].enter))=out;
					break;
					case TYPE_UINT64:
						(*(uint64*)(address[index].enter))=out;
					break;
					case TYPE_FP32:
						(*(fp32*)(address[index].enter))=out;
					break;
					case TYPE_FP64:
						(*(fp64*)(address[index].enter))=out;
					break;
				}
			break;
		}
	}
}

void NAND_SaveValues(struct Value * address,uint16 length,uint16 nandaddress)
{
	int i;
	int j;
	int pagelength=256;
	int index1=0,index2=0;
	uint8 data[2048];
	if(nandaddress<30000)
		return;
	for(i=0;i<=length/256;i++)
	{
		if(i==length/256)
		{
			pagelength=length%256;
		}
		for(j=0;j<pagelength;j++)
		{
			switch(address[index2].type)
			{
				case TYPE_INT8:
					*(int8*)(data+index1*8)=*(int8*)(address[index2].enter);
				break;
				case TYPE_UINT8:
					*(uint8*)(data+index1*8)=*(uint8*)(address[index2].enter);
				break;
				case TYPE_INT16:
					*(int16*)(data+index1*8)=*(int16*)(address[index2].enter);
				break;
				case TYPE_UINT16:
					*(uint16*)(data+index1*8)=*(uint16*)(address[index2].enter);
				break;
				case TYPE_INT32:
					*(int32*)(data+index1*8)=*(int32*)(address[index2].enter);
				break;
				case TYPE_UINT32:
					*(uint32*)(data+index1*8)=*(uint32*)(address[index2].enter);
				break;
				case TYPE_INT64:
					*(int64*)(data+index1*8)=*(int64*)(address[index2].enter);
				break;
				case TYPE_UINT64:
					*(uint64*)(data+index1*8)=*(uint64*)(address[index2].enter);
				break;
				case TYPE_FP32:
					*(fp32*)(data+index1*8)=*(fp32*)(address[index2].enter);
				break;
				case TYPE_FP64:
					*(fp64*)(data+index1*8)=*(fp64*)(address[index2].enter);
				break;
			}
			index1++;
			index2++;
		}
		index1=0;
		WritePartBlock(nandaddress+i,data);
		UART0_Printf("Send:%d %d %d %d to %d\n",*(uint16*)(data),*(uint16*)(data+8),*(uint16*)(data+16),*(uint16*)(data+24),nandaddress+i);
	}
} 

void NAND_LoadValues(struct Value * address,uint16 length,uint16 nandaddress)
{
	int i;
	int j;
	int pagelength=256;
	int index1=0,index2=0;
	uint8 data[2048];
	if(nandaddress<30000)
		return;
	for(i=0;i<=length/256;i++)
	{
		ReadPage(nandaddress+i,data);
		if(i==length/256)
		{
			pagelength=length%256;
		}
		for(j=0;j<pagelength;j++)
		{
			switch(address[index2].type)
			{
				case TYPE_INT8:
					*(int8*)(address[index2].enter)=*(int8*)(data+index1*8);
				break;
				case TYPE_UINT8:
					*(uint8*)(address[index2].enter)=*(uint8*)(data+index1*8);
				break;
				case TYPE_INT16:
					*(int16*)(address[index2].enter)=*(int16*)(data+index1*8);
				break;
				case TYPE_UINT16:
					*(uint16*)(address[index2].enter)=*(uint16*)(data+index1*8);
				break;
				case TYPE_INT32:
					*(int32*)(address[index2].enter)=*(int32*)(data+index1*8);
				break;
				case TYPE_UINT32:
					*(uint32*)(address[index2].enter)=*(uint32*)(data+index1*8);
				break;
				case TYPE_INT64:
					*(int64*)(address[index2].enter)=*(int64*)(data+index1*8);
				break;
				case TYPE_UINT64:
					*(uint64*)(address[index2].enter)=*(uint64*)(data+index1*8);
				break;
				case TYPE_FP32:
					*(fp32*)(address[index2].enter)=*(fp32*)(data+index1*8);
				break;
				case TYPE_FP64:
					*(fp64*)(address[index2].enter)=*(fp64*)(data+index1*8);
				break;
			}
			index1++;
			index2++;
		}
		index1=0;
	}
} 
