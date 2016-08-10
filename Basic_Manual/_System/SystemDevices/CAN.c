/****************************************Copyright (c)**************************************************
**                                     XJTU Robot Team
**-----------------------------------------File Info----------------------------------------------------
** File name:			CAN.c
** Last modified Date:  2013-4-30
** Descriptions:		这个文件主要定义了ARM通过STM32的CAN总线与外设通信的一些软件接口。
** Created by:			dong		
********************************************************************************************************/
#include "basic.h"

/************************************************************************************************
**写在前面：可以调用的一些函数：
**			1.void STM32_Reset(void)   重启STM32，一般是开机初始化时用
**
**			2.void  Relay(uint32 set_low ,uint32 set_high,uint32 clear_low,  uint32  clear_high)
**			    上面这个函数可以同时置位或清零某几个继电器，具体用法看下面函数定义时的介绍
**				使用示例：想set 12  13 口和clear 15 和44，62口，可以这样用 
**              Relay(RELAY_LOW(3)|RELAY_LOW(7),0,RELAY_LOW(15),RELAY_HIGH(44)|RELAY_HIGH(62));
**				参数只能用：RELAY_LOW(xx1) xx1从1到32   和  RELAY_HIGH(xx2)  xx2从33到64 
** 
**			3.void Relay_Clear(uint8 port)  参数：1~64
**			4.void Relay_Set(uint8 port)    参数：1~64
**			
**			5.uint8 Relay_Read(uint8 port)  这三个就不用介绍了吧，用法和以前一样  参数：1~64
**			6.void ISO_UNMASK(uint16 port)  使能光隔板的某些口，让其自动执行，参数:ISO(port),port :1~16
**											可选多个，如：ISO(1) | ISO(2) | ..也可以做参数
**			7.void ISO_MASK(uint16 port)    对光隔似乎没用，因为只要使能光隔，只要其执行完动作后，会自动屏蔽
**			8.uint8 ISO_Read(uint8 port)   只能读本地记录的ISO的状态，实际是什么不清楚，因为没有光隔的反馈回来
**											参数：1~16
**			9.void Ranging_Start(uint8 id)  某个仪器开始测距，具体看下面
**			10.void  Ranging_Stop(uint8 id) 停止测距
**							参数可多选 ，即  CONTROL_LASER | CONTROL_IR1 |CONTROL_SONIC1...也可以
**											9.10 可选的参数为：
**												 激光测距：   CONTROL_LASER， 	//激光 
**												 红外测距：   CONTROL_IR1   	//红外1
**															  CONTROL_IR2		//红外2
**												 超声波测距： CONTROL_SONIC1 	//超声波1
**											 				  CONTROL_SONIC2	//超声波2
**											 				  CONTROL_SONIC3	//超声波3
**															  CONTROL_SONIC4
**			11.uint8 Read_Device_Statu(uint16  id) 	 读取某个设备的状态，判断数据是否一直往上传
**												参数和9  10 一样，但参数不能多选
**			12.uint16 STM32_ReadDistance(uint8 id)   返回测距值
**									    可选的参数为：（参数只能唯一，不能"或"起来）
**													 激光测距：   ID_LASER_DATA，
**													 红外测距：   ID_IR1_DATA  
**																  ID_IR2_DATA
**													 超声波测距： ID_SUP_SONIC1_DATA 
**													 			  ID_SUP_SONIC2_DATA
**													 			  ID_SUP_SONIC3_DATA
**			13.void Motor_CAN(uint8 id , uint8 cmd)  控制两个电机 id 的取值为 1  2
**													cmd  的 取值为  STOP zheng 正  fan 反  改一下宏定义吧，我不知取什么名字好
**		上层调用的就这些函数了
*************************************************************************************************/


/*
**struct Data2STM32
**{
	uint8 	Data_ID;
	uint8*	Data_Ptr;
	uint8	Data_Length;
	uint8	Data_Type;
**}; 在basic.h中定义了

**
** 	该结构体基本的元素定义如下：
** 	Data_ID :	在STM32定义了一个结构体数组，ID号就是数组的下标
** 	Data_Ptr:	要发送或接收的数据缓冲区的首地址
**	Data_Length:数据的长度
**	Data_Type:	数据的类型，读或写，这里写出来只是和STM32的程序配合而已，没有用到
*/



/*
**下面的二维数组的意义：
**STM32_DATA_BUF_LEN  	代表的是一共有几组要传输的数据
**       9  			代表的是每组数据最长有9字节
**虽然有两个数组，但是结构体中只会记录用到的地址，
**例如：ID为0的数据，类型是 WRITE_ONLY，所以把 STM32_Send_Buf 的地址赋给ID0 的 Data_Ptr
**（读或写以ARM为主语）
*/
uint8  STM32_Send_Buf[STM32_DATA_BUF_LEN][9]; 
uint8  STM32_Rece_Buf[STM32_DATA_BUF_LEN][9];


struct Data2STM32 STM32_Data[STM32_DATA_BUF_LEN]=
    {
		{ID_RELAY64,		(uint8*)STM32_Send_Buf[0 ],8,WRITE_ONLY},   //发送64位的继电器的数据
		{ID_RELAY8, 		(uint8*)STM32_Send_Buf[1 ],1,WRITE_ONLY},   //发送8位的继电器的数据
		{ID_LASER_CTR,		(uint8*)STM32_Send_Buf[2 ],1,WRITE_ONLY},   //激光测距的开始或结束的控制字节
        {ID_LASER_DATA,		(uint8*)STM32_Rece_Buf[3 ],3,READ_ONLY },   //读取16位的激光测距的距离值，第3个字节为1则数据最新，为0则数据不是最新的
		{ID_ISO_MASK,		(uint8*)STM32_Send_Buf[4 ],1,WRITE_ONLY},   //向光隔板上的STM32发送使能信号
		{ID_IR_CTR,			(uint8*)STM32_Send_Buf[5 ],1,WRITE_ONLY},   //红外板的开始或结束的命令字节
		{ID_IR1_DATA,   	(uint8*)STM32_Rece_Buf[6 ],3,READ_ONLY },   //红外1的距离值，第3个字节为1则数据最新，为0则数据不是最新的
		{ID_IR2_DATA,		(uint8*)STM32_Rece_Buf[7 ],3,READ_ONLY },   //红外2的距离值，
		{ID_SUP_SONIC_CTR,	(uint8*)STM32_Send_Buf[8 ],1,WRITE_ONLY},   //超声波的开始或结束的命令字节，0 1 2位分别为超声波1 2 3的控制位
		{ID_SUP_SONIC1_DATA,(uint8*)STM32_Rece_Buf[9 ],3,READ_ONLY },   //超声波1的距离值，第3个字节为1则数据最新，为0则数据不是最新的
		{ID_SUP_SONIC2_DATA,(uint8*)STM32_Rece_Buf[10],3,READ_ONLY },   //超声波2的距离值，
		{ID_SUP_SONIC3_DATA,(uint8*)STM32_Rece_Buf[11],3,READ_ONLY },   //超声波3的距离值，
		{ID_SUP_SONIC4_DATA,(uint8*)STM32_Rece_Buf[12],3,READ_ONLY },   //超声波3的距离值，
		
				
		{ID_ISO_DELAY1,		(uint8*)STM32_Send_Buf[13],2,WRITE_ONLY}, 	//给光隔板发送的延时的数据，对于12届来说，需要放3个叶子，所以需要3个延时的数据
		{ID_ISO_DELAY2,		(uint8*)STM32_Send_Buf[14],2,WRITE_ONLY},	//这3个数据是16位的延时时长
		{ID_ISO_DELAY3,		(uint8*)STM32_Send_Buf[15],2,WRITE_ONLY},
		{ID_LIDAR_CTR	,	(uint8*)STM32_Send_Buf[16],1,WRITE_ONLY}, 	
		{ID_LIDAR		,	(uint8*)STM32_Rece_Buf[17],5,READ_ONLY }, 
		
		{ID_MOTOR1		,	(uint8*)STM32_Send_Buf[18],4,WRITE_ONLY}, 	//电机1 ,这3个电机只有正反转，停止这3个状态
		{ID_MOTOR2		,	(uint8*)STM32_Send_Buf[19],4,WRITE_ONLY}, 	//电机2
		{ID_MOTOR3		,	(uint8*)STM32_Send_Buf[20],4,WRITE_ONLY}, 	//电机3
		{ID_MOTOR4		,	(uint8*)STM32_Send_Buf[21],4,WRITE_ONLY},
		
		{ID_MOTOR5		,	(uint8*)STM32_Send_Buf[22],4,WRITE_ONLY},    //电机5		第一个8位：转过的圈数，后两个8位：转过的角度 0~3600度，精度0.1度
		{ID_MOTOR6		,	(uint8*)STM32_Send_Buf[23],4,WRITE_ONLY},
		
		{ID_MaxonConWord,   (uint8*)STM32_Send_Buf[24],2,WRITE_ONLY},   //Maxon电机控制字()
		{ID_MaxonModeSelect,(uint8*)STM32_Send_Buf[25],1,WRITE_ONLY},   //Maxon电机选模式
		{ID_Maxon1Speed,    (uint8*)STM32_Send_Buf[26],4,WRITE_ONLY},   //Maxon电机1改速度
		{ID_Maxon2Speed,    (uint8*)STM32_Send_Buf[27],4,WRITE_ONLY}    //Maxon电机2改速度
	};

/*
**要点：ARM通过12.5M的SPI给板子上的STM32发送数据或命令，然后STM32再把这些命令通过CAN总线往外送			
**通信规则：1.每次都是ARM做主机发送命令
**			2.先发Data_ID,告诉STM32将要传输的是什么设备的数据
**			3.再根据具体的情况把要发送或接收的数据处理掉
*/

/*********************************************************************************************
**函数名称：STM32_RW(id) STM32读写
**函数参数：id   
**性能参数：运行一次需要20us左右，也就是说传输一个32字节的数需要20us的时间
**			其中大部分时间用在延时等待上，如果你有时间再次验证，可能会发现
**			能改进的地方，那就改吧（对应的SPI速度为12.5M）
**********************************************************************************************/

void STM32_RW(uint8 id)
{
	OS_CPU_SR  cpu_sr;
	int i=0;
	uint8  Length_Temp;
	uint8* rece_data_ptr,*send_data_ptr;
	Length_Temp = STM32_Data[id].Data_Length;
	
	send_data_ptr = (uint8*)STM32_Send_Buf[id];
	rece_data_ptr = (uint8*)STM32_Rece_Buf[id];
		
	/*
	**先发送数据的ID过去，让对方判断接下来的动作
	*/
	OS_ENTER_CRITICAL();
	
	SPI1_SendData_stm32(STM32_Data[id].Data_ID);
	for(i=0;i<250;i++);   	//延时一下，让STM32处理数据，i的值改为70也可以，为了更加稳定，才用150
	while(Length_Temp--)
	{
		(*rece_data_ptr++)  =  SPI1_SendData_stm32(*send_data_ptr++);
		for(i=0;i<250;i++); //延时
	}
	
	OS_EXIT_CRITICAL();
	for(i=0;i<250;i++);		//延时	
}

/*****************************************************************************************
**函数名称：STM32_Send8
**函数参数：id 要发送的设备ID  data：要发送的数据   
**功能描述：可以发送8位或64位的数据，但是对应的ID号要和发送的数据长度配合好
**           主要是给继电器调用的。上层函数最好别调用，因为STM32只会对对应的命令
**           起作用，随便调用可能会引起不必要的混乱
**           
******************************************************************************************/
void STM32_Send8(uint8 id  ,  uint8 data)
{
	uint8* dptr;
	dptr  =  STM32_Send_Buf[id];
	*dptr =  data;
	STM32_RW(id);
}



void STM32_Send16(uint8 id , uint16 data)
{
//    uint8 *dptr;
    union
    {
    	uint8 	data[2];
    	uint16	value;
    }Send16;
    Send16.value = data;
    
	STM32_Send_Buf[id][0]  = Send16.data[0];//低八位
	STM32_Send_Buf[id][1]  = Send16.data[1];//高八位
	
//	dptr = (uint16*)STM32_Send_Buf[2];     //用指针好像不行
//	*(uint16*)dptr =  data;

	STM32_RW(id);
}

void STM32_Send32(uint8 id , uint32 data)
{
    union
    {
    	uint8 	data[4];
    	uint32	value;
    }Send32;
    Send32.value = data;
    
	STM32_Send_Buf[id][0]  = Send32.data[0];
	STM32_Send_Buf[id][1]  = Send32.data[1];
	STM32_Send_Buf[id][2]  = Send32.data[2];
	STM32_Send_Buf[id][3]  = Send32.data[3];

	STM32_RW(id);
}

//注意这个编译器好像对64位数据不怎么支持，最好不要用64位的数据类型
//所以这里用了32位的数组
void STM32_Send64(uint8 id  ,  uint32* data)   
{
	uint8* dptr,i;
	union 
	{
		uint32 data[2];
		uint8  value[8];
	}Send64;
	dptr = STM32_Data[id].Data_Ptr;
	Send64.data[0] = *data;
	Send64.data[1] = *(++data);
	
	for(i=0;i<8;i++)
	{
		*(dptr++) = Send64.value[i];
	}

	STM32_RW(id);
}



/****************************************************************************
**函数名称：STM32_Reset
**函数参数：无
**函数返回：无   
**功能描述：会给主控板上的STM32一个中断，STM32会重启自己
*****************************************************************************/                                                                      
void STM32_Reset(void)
{
    int i;
    rGPFDAT &= ~(0x1<<4);       //先拉低
    rGPFDAT |= (0x1<<4);        //再拉高
    for(i=0;i<300;i++);         //延时一小会
    rGPFDAT &= ~(0x1<<4);       //拉低
}



/******************************************************************************************
** 以下是关于继电器的描述
**      		继电器有三种控制的方式：UART  CAN总线8位，CAN总线64位
**                      1.UART:每次只控制一个口，
                          格式：第7位是0表示清除 ，1表示置位
**	                      0到5 共6位表示对应端口的号码，第6位没有用到
**		                2.CAN总线8位：   
**                        格式：和UART一样，只是传输方式不同
**                      3.CAN总线64位：
**                        每改变一个继电器口的状态都会把所有的状态都发过去
**            不管是哪一种方式是把状态记录在一个64位的变量中。	
********************************************************************************************/

#define  RELAY_CAN  1   //根据实际情况选择用UART还是CAN传输,一般用CAN每次传输8位

uint32 Relay_Value[2];  //记录继电器的状态
//=========================================================
//用UART传输数据
void Relay_Set_Uart(uint8 port)
{
	uint8 temp;
	Relay_Value[(port-1)/32] |= (uint32)1<<((port-1)%32);  //把状态保存在Relay_Value[]中	
	temp = 1<<7 | (port-1);
	UART0_SendByte(temp);			
}

void Relay_Clear_Uart(uint8 port)
{
	uint8 temp;
	Relay_Value[(port-1)/32] &= ~((uint32)1<<((port-1)%32));	
	temp = 0<<7 | (port-1);
	UART0_SendByte(temp);
}
//==========================================================
//用CAN传输1字节数据	        
void Relay_Set_Can8(uint8 port)
{
	uint8 temp;
	Relay_Value[(port-1)/32] |= (uint32)1<<((port-1)%32); 	
	temp = 1<<7 | (port-1);
	STM32_Send8(ID_RELAY8,temp);			
}

void Relay_Clear_Can8(uint8 port)
{
	uint8 temp;
	Relay_Value[(port-1)/32] &= ~((uint32)1<<((port-1)%32));	
	temp = 0<<7 | (port-1);
	STM32_Send8(ID_RELAY8,temp);
}

//=================================================================
/************************************************************************************************
** 函数名称	:	Relay
** 功能描述	:   set  或 clear 某个继电器端口，可以同时操作几个口
** 输　入 	    :	四个参数：1.set_low  ：要set的端口，如果端口号小于32则用这个参数
**				              2.set_high ：要set的端口，如果端口号大于32小于64则用这个参数
**                            3.clear_low：要clear的端口，如果小于于32则用这个参数
**                            4.clear_low：要clear的端口，如果大于32小于64则用这个参数
**                  low 与 high 以32为分界线
**          使用示例：想set 12  13 口和clear 15 和44，62口，
**          可以这样用 Relay(RELAY_LOW(3)|RELAY_LOW(7),0,RELAY_LOW(15),RELAY_HIGH(44)|RELAY_HIGH(62))
** 输　出       :	
** 注意事项    :选择端口最好用这个宏定义： RELAY_LOW(port) 
**************************************************************************************************/
void  Relay(uint32 set_low ,uint32 set_high,uint32 clear_low,  uint32  clear_high)
{
	
	Relay_Value[0] |=  set_low;
    Relay_Value[1] |=  set_high;

	Relay_Value[0] &=  ~clear_low;
    Relay_Value[1] &=  ~clear_high;

	STM32_Send64(ID_RELAY64,Relay_Value);
}

void Relay_Set_Can64(uint8 port)
{
    if(port<33 && port >0)
        Relay(RELAY_LOW(port),0,0,0);
    else if(port>32 && port < 65)
        Relay(0,RELAY_HIGH(port),0,0);			
}

void Relay_Clear_Can64(uint8 port)
{
    if(port<33 && port >0)
        Relay(0,0,RELAY_LOW(port),0);
    else if(port>32 && port < 65)
        Relay(0,0,0,RELAY_HIGH(port));	
}
//==========================================================
//读继电器数据
uint8 Relay_Read(uint8 port)
{
	uint8 value;
    if(port >0 && port <33)
    {
        if(Relay_Value[0] & (uint32)1<<(port-1))
            value = 1;
        else 
            value = 0;
    }else if(port >32 && port<65){
        if(Relay_Value[1] & (uint32)1<<(port-33))
            value = 1;
        else 
            value = 0;
    }
    return  value;
}

/********************************************************************
** 函数名称：Relay_Set()  和  Relay_Clear()
** 输入参数：对应的继电器板端口号 从1到64
** 输出参数：无
** 注意事项：这个是通用的函数接口，最常用的是这个
**********************************************************************/

void Relay_Clear(uint8 port)
{
#if RELAY_CAN
     Relay_Clear_Can8(port);
#else 
     Relay_Clear_Uart(port);
#endif
}

void Relay_Set(uint8 port)
{
#if RELAY_CAN
     Relay_Set_Can8(port);
#else 
     Relay_Set_Uart(port);
#endif
}



/********************************************************************
** 函数名称：UNMASK_Relay()  和  MASK_Relay()
** 输入参数：光隔板上的接近开关的端口号
** 输出参数：无
** 功能描述：可以控制光隔板的接近开关是否打开，用来自动对接近开关的
**            功能进行反应
** 用法     ：例如要屏蔽 2,3口，则 MASK_ISO(ISO(2) | ISO(3))   //ISO(port)是个宏定义
**********************************************************************/
uint16  ISO_Mask_Statu = 0;

//使能自主动作，只要接近或接触开关的某个条件达到，
//光隔板上的STM32可以自主反应，而不需要经过主控
void  ISO_UNMASK(uint8 port)
{
    ISO_Mask_Statu  |=  port;
    STM32_Send16(ID_ISO_MASK,ISO_Mask_Statu);
}

//屏蔽，即不使能自主动作
void ISO_MASK(uint8 port)
{
    ISO_Mask_Statu  &=  ~port;
    STM32_Send16(ID_ISO_MASK,ISO_Mask_Statu);
}
//如果是1则使能，即光隔板可以自己跳过主控做出反应
uint8 ISO_Read(uint8 port)
{
	if(ISO_Mask_Statu & (uint16)1<<(port-1))
		return  1;
	else 
		return  0;
}


//这个函数是设置ISO的延时时间的，
//port 的取值范围是 1  2  3，delay_time 是延时的时间

uint16 ISO_Delay_Time[3];

void ISO_Delay(uint8 port , uint16 delay_time)
{
	STM32_Send16(ID_ISO_DELAY1+port-1 , delay_time);
}

/********************************************************************
** 函数名称：STM32_ReadDistance
** 输入参数：要读取的设备的ID   
			 可选的参数为：
			 激光测距：   ID_LASER_DATA，
			 红外测距：   ID_IR1_DATA  ID_IR2_DATA
			 超声波测距： ID_SUP_SONIC1_DATA 
			 			  ID_SUP_SONIC2_DATA
			 			  ID_SUP_SONIC3_DATA									
** 输出参数：返回距离值
** 功能描述：
** 注意事项：每次通信会返回3个字节的数，
**           其中，前两个字节为uint16的距离值，第3个字节为0或1 ；
**           为0则表示这个距离不是最新的，但是或许有用，因为要是距离变化不大的话就可以用
**           为1则表示距离值是最新的，函数返回值就是uint16的距离值
**			这里暂时没有使用第三个标志值
**********************************************************************/
uint16 STM32_ReadDistance(uint8 id)
{
    uint8   flag ,*ptr;
    union 
    {
        uint8   data[2];
        uint16  value;
    }distance;

	STM32_RW(id);                            //读数据
    ptr = STM32_Data[id].Data_Ptr;
    distance.data[0] = *ptr ;
    distance.data[1] = *(ptr+1) ;

    flag =   *(ptr+2);   //把第三个字节的值赋给flag

//    if (flag ==1)      //这个标志位暂时不管，根据需要使用
        return distance.value;
//    else 
//        return 0;
}


/********************************************************************
** 函数名称：Ranging_Start   Ranging_Stop  意思是：测距开始，ranging是从有道查的  ==！
** 输入参数：要控制的设备的ID   
			 可选的参数为：
			 激光测距：   CONTROL_LASER，
			 红外测距：   CONTROL_IR1  CONTROL_IR2
			 超声波测距： CONTROL_SONIC1 
			 			  CONTROL_SONIC2
			 			  CONTROL_SONIC3									
** 输出参数：返回距离值
** 功能描述：在需要测距的时候才开始测距，减少CAN总线的占用率
**     注意：这个函数的ID 和上面的是不一样的
**********************************************************************/
volatile uint16 	Device_Statu;  	//记录几个设备的状态，只是在ARM上用，不会传到STM32
									//这个变量和下面的数组其实表示的是同一个东西，多写是为了处理方便
volatile uint8  	CAN_Device_Control[3] = {0};  //这个数组每个字节记录一类设备的状态：正在运行或停止
												  //现在有三类设备：激光测距，红外，超声波

struct Struct_CAN_Device
{
	uint8  	Device_DataID;		//设备的ID，这样做是因为传输数据的设备的ID号不连续
	uint8	Device_ControlID;
	uint8* 	Control_Ptr;    //指向上面的控制状态的指针，
};

//上面是定义，下面是赋值，只在接下来的两个函数中使用
struct Struct_CAN_Device  
CAN_Device[DEVICE_NUM] = {
					{ID_LASER_DATA		,	ID_LASER_CTR, 		(uint8*)&CAN_Device_Control[0]	},
					{ID_IR1_DATA		,	ID_IR_CTR, 			(uint8*)&CAN_Device_Control[1]	},
					{ID_IR2_DATA		,	ID_IR_CTR,			(uint8*)&CAN_Device_Control[1]	},
					{ID_SUP_SONIC1_DATA	,	ID_SUP_SONIC_CTR,	(uint8*)&CAN_Device_Control[2]	},
					{ID_SUP_SONIC2_DATA	,	ID_SUP_SONIC_CTR,	(uint8*)&CAN_Device_Control[2]	},
					{ID_SUP_SONIC3_DATA	,	ID_SUP_SONIC_CTR,	(uint8*)&CAN_Device_Control[2]	},
					{ID_SUP_SONIC4_DATA	,	ID_SUP_SONIC_CTR,	(uint8*)&CAN_Device_Control[2]	}
				};



void  Ranging_Start(uint16 id)
{
	uint8  i;
	Device_Statu |= id;
	CAN_Device_Control[0] |= (uint8)(id & 1);
	CAN_Device_Control[1] |= (uint8)((id>>1) & 0x3);
	CAN_Device_Control[2] |= (uint8)((id>>3) & 0xf);
	
	for(i=0; i < DEVICE_NUM ; i++){
		if((id >> i) & (uint16)1 ){   //如果选择了对应的ID
		    STM32_Send8(CAN_Device[i].Device_ControlID , *(CAN_Device[i].Control_Ptr));
		    //每次发送开始命令，都读回来一个值，那个值没有用，是为了清空
		    //STM32的发送区的数据 
		    STM32_ReadDistance(CAN_Device[i].Device_DataID);                 
		}
	}
}


void  Ranging_Stop(uint16 id)
{
	uint8  i;
	Device_Statu &= ~id;
	CAN_Device_Control[0] &= ~(uint8)(id & 1);
	CAN_Device_Control[1] &= ~(uint8)((id>>1) & 0x3);
	CAN_Device_Control[2] &= ~(uint8)((id>>3) & 0xf);
	
	for(i=0; i < DEVICE_NUM ; i++){
		if((id >> i) & (uint16)1 ){   //如果清除了对应的ID
		    STM32_Send8(CAN_Device[i].Device_ControlID , *(CAN_Device[i].Control_Ptr));
		    //每次发送结束命令，都读回来一个值，那个值没有用，是为了清空
		    //STM32的发送区的数据 
		    STM32_ReadDistance(CAN_Device[i].Device_DataID);                 
		}
	} 
}


uint8 Read_Device_Statu(uint16  id)
{
	if((Device_Statu & id) >0 )
		return 1;
	else
		return 0;
}


/*********************************************************************************************
** 函数名称：Motor_CAN
** 输入参数：id  cmd  
			 可选的参数为id :1,2
						   cmd :STOP zheng  fan //zheng  和 fan 改个名字吧	
** 输出参数：返回距离值
**********************************************************************************************/
uint8  MOTOR_STATU[4];


void Motor_CAN(uint8 id , uint8 cmd)
{
	MOTOR_STATU[id-1]  =  cmd;
	STM32_Send8( ID_MOTOR1+id-1 , MOTOR_STATU[id-1]);
}

uint8 Motor_CAN_Statu(uint8 id )
{	
	return  MOTOR_STATU[id-1];
}

//这个电机可以控制电机转的圈数和角度
void Motor_Position(uint8 circ , uint16 radian)
{
	//uint8 *ptr;
	union
    {
    	uint8 	data[2];
    	uint16	value;
    }Send16;
    
	
	STM32_Send_Buf[ID_MOTOR5][0]  = circ;
	Send16.value = radian;
	STM32_Send_Buf[ID_MOTOR5][1]  = Send16.data[0];
	STM32_Send_Buf[ID_MOTOR5][2]  = Send16.data[1];
	
	STM32_RW(ID_MOTOR5); 
}



/**
  * @brief  由主控调用，控制某个电机转一定圈数
  * @note   
  * @param  motor:  ID_MOTOR1 ~ ID_MOTOR6
            rot:    要转的圈数，注意类型是浮点数
  * @retval None
  */    

void arm_motor(uint8 motor, float rot)
{
    #define  M_FHB 1  /* faulhaber motor */
    #define  M_COM 0  /* common motor */
        
    /* 电机的类型和电机的顺序要一一对应 */
    static uint8 m_type[] = {M_FHB, M_COM, M_COM, M_FHB,  M_COM,  M_FHB}; 
    
    union uPosType
    {
        uint8 d[4];    /* data */
        int v;      /* value */
    }uPos;          /* union position */
    
    if(motor>ID_MOTOR6 || motor<ID_MOTOR1)
    {
        /* add code here to report error */
        return;
    }
    
    if(m_type[motor-ID_MOTOR1] == M_FHB)
    {/* faulhaber motor */
        uPos.v = (int)(rot*2048*14);
    }
    else if(m_type[motor-ID_MOTOR1] == M_COM)
    {/* common motor */
        uPos.v = (int)(rot*2000);
    }
    
    /* send goal position data by can bus */
    STM32_Send_Buf[motor][0] = uPos.d[0];
    STM32_Send_Buf[motor][1] = uPos.d[1];
    STM32_Send_Buf[motor][2] = uPos.d[2];
    STM32_Send_Buf[motor][3] = uPos.d[3];
    
    STM32_RW(motor); 
}



/*********************************************************************************************
** 函数名称：STM32_ReadLidarData(int ring_id)
** 输入参数：
** 输出参数：
**********************************************************************************************/
uint16 STM32_ReadLidarData(int ring_id)
{
    uint8   *ptr;
    //uint8 flag;
    extern fp64 Lidar_ResultX[50],Lidar_ResultY[50];
    union 
    {
        uint8   data[2];
        uint16  value;
    }distance;

	STM32_RW(ID_LIDAR);
	
    ptr = STM32_Data[ID_LIDAR].Data_Ptr;
    distance.data[0] = *ptr ;
    distance.data[1] = *(ptr+1) ;
	Lidar_ResultX[ring_id]=distance.value;

    distance.data[0] = *(ptr+2) ;
    distance.data[1] = *(ptr+3) ;
	Lidar_ResultY[ring_id]=distance.value;	
	
	return *(ptr+4);
}



/*********************************************************************************************
** 函数名称：Maxon电机控制
** 输入参数：
** 输出参数：
**********************************************************************************************/
void MaxonEnable(void)
{
   STM32_Send16(ID_MaxonConWord,MAXONENALBE);
}

void MaxonDisable(void)
{
   STM32_Send16(ID_MaxonConWord,MAXONDISABLE);
}

void MaxonQuickstop(void)
{
   STM32_Send16(ID_MaxonConWord,MAXONQUICKSTOP);
}

void MaxonSpeedMode(void)
{
   STM32_Send16(ID_MaxonModeSelect,MAXONSPEEDMODE);
}

void MaxonFaultReset(void)
{
   STM32_Send16(ID_MaxonModeSelect,0x0080);
}

void Maxon1SetSpeed(double speed)
{
   int32 tempspeed;
   if(-5000<=speed<=5000)
   {
      tempspeed=(int32)speed;
      STM32_Send32(ID_Maxon1Speed,tempspeed);
   }
}

void Maxon2SetSpeed(double speed)
{
   int32 tempspeed;
   if(-5000<=speed<=5000)
   {
      tempspeed=(int32)speed;
      STM32_Send32(ID_Maxon2Speed,tempspeed);
   }
}


/***********************************************************************************************
**                            End Of File
*************************************************************************************************/