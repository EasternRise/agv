#include "all.h"


//----供本框内部函数调用的子函数，本文件以外的函数不要调用------//															//
																//
void SelectPort( uint8 addr )									//
{																//
	SPI1_SendData(0x20| addr);									//
}																//
																//
void WriteRAM( uint8 RAM_addr,uint8 Data )						//
{																//
	SPI1_SendData( 0x80 | RAM_addr );							//	
	SPI1_SendData( Data );										//
}
																//
uint8 ReadRAM_FIFO( uint8 RAM_addr )							//
{																//
	return(SPI1_SendData( 0x40|RAM_addr ));						//
}
																//
uint8 ReadRAM_STATE( uint8 RAM_addr )							//
{																//
	return(SPI1_SendData( 0x60|RAM_addr ));						//
}																//	
																//
uint8 DisConnect( void )										//
{																//
	return(SPI1_SendData(0xC0));								//
}																//	
//--------------------------------------------------------------//

/*********************************************************************************************************
** 函数名称	:	EnableFPGA（）
** 功能描述	:   选通FPGA信息处理板
** 输　入 	:	FPGA信息处理板的ID
**				
** 输　出  :	无
** 注意事项：        
********************************************************************************************************/
void FGPA_Enable( uint8 ID )
{
	SPI1_SendData( ID );
}

/*********************************************************************************************************
** 函数名称	:	FPGA_ReadIR(  )
** 功能描述	:   读取红外值
** 输　入 	:	红外的端口地址：红外占用FPGA板上的5,6,7接口
**				
** 输　出  :	24位有符号数表示的红外值
** 注意事项：        
********************************************************************************************************/

uint32 FPGA_ReadIR( uint8 id )
{
	OS_CPU_SR  cpu_sr;
	
	union u8to32
	{
		uint32 value;
		uint8 datas[4];
	}IR;
	

	OS_ENTER_CRITICAL();

	switch(id)
	{
		/*
		case 1:
				IR.datas[3]=0;
			
				SelectPort(5);
				ReadRAM_STATE(0);
			
				IR.datas[2]=ReadRAM_STATE(1);
				IR.datas[1]=ReadRAM_STATE(2);
				IR.datas[0]=DisConnect();
				
				
		break;
		*/
		case 2:
				IR.datas[3]=0;
				SelectPort(6);
				ReadRAM_STATE(0);
			
				IR.datas[2]=ReadRAM_STATE(1);
				IR.datas[1]=ReadRAM_STATE(2);
				IR.datas[0]=DisConnect();
		break;
		case 3:
				IR.datas[3]=0;
				SelectPort(7);
				ReadRAM_STATE(0);
			
				IR.datas[2]=ReadRAM_STATE(1);
				IR.datas[1]=ReadRAM_STATE(2);
				IR.datas[0]=DisConnect();
		break;

		default:;
		
		
	}
	OS_EXIT_CRITICAL();
			
	return IR.value;
}


/*********************************************************************************************************
** 函数名称	:	Read_Codewheel(  )
** 功能描述	:   读取某个码盘的计数值
** 输　入 	:	码盘的ID，FPGA板上面一共有3个码盘接口，所以ID range 1~3
**				
** 输　出  :	32位有符号数表示的码盘计数值
** 注意事项：        
********************************************************************************************************/
int32 FPGA_ReadEncoder( uint8 id )
{
	OS_CPU_SR  cpu_sr;
	union u8to32
	{
		int32 value;
		uint8 datas[4];
	}convert;
	
	OS_ENTER_CRITICAL();
	SelectPort(FPGA_ICUEN_ENCODER);
	switch(id)
	{
		case 1:
				ReadRAM_STATE(1);
				convert.datas[0]=ReadRAM_STATE(2);
				convert.datas[1]=ReadRAM_STATE(3);
				convert.datas[2]=ReadRAM_STATE(4);
				convert.datas[3]=DisConnect();
		break;
		case 2:
				ReadRAM_STATE(5);
				convert.datas[0]=ReadRAM_STATE(6);
				convert.datas[1]=ReadRAM_STATE(7);
				convert.datas[2]=ReadRAM_STATE(8);
				convert.datas[3]=DisConnect();
		break;
		case 3:
				ReadRAM_STATE(9);
				convert.datas[0]=ReadRAM_STATE(10);
				convert.datas[1]=ReadRAM_STATE(11);
				convert.datas[2]=ReadRAM_STATE(12);
				convert.datas[3]=DisConnect();
		break;
		case 4:
				ReadRAM_STATE(13);
				convert.datas[0]=ReadRAM_STATE(14);
				convert.datas[1]=ReadRAM_STATE(15);
				convert.datas[2]=ReadRAM_STATE(16);
				convert.datas[3]=DisConnect();
		break;
		case 5:
				ReadRAM_STATE(17);
				convert.datas[0]=ReadRAM_STATE(18);
				convert.datas[1]=ReadRAM_STATE(19);
				convert.datas[2]=ReadRAM_STATE(20);
				convert.datas[3]=DisConnect();
		break;
		case 6:
				ReadRAM_STATE(21);
				convert.datas[0]=ReadRAM_STATE(22);
				convert.datas[1]=ReadRAM_STATE(23);
				convert.datas[2]=ReadRAM_STATE(24);
				convert.datas[3]=DisConnect();
		break;
		case 7:
				ReadRAM_STATE(25);
				convert.datas[0]=ReadRAM_STATE(26);
				convert.datas[1]=ReadRAM_STATE(27);
				convert.datas[2]=ReadRAM_STATE(28);
				convert.datas[3]=DisConnect();
		break;
		default:;
		
		
	}
	OS_EXIT_CRITICAL();			
	return convert.value;
}


/*********************************************************************************************************
** 函数名称	:	Read_Gyro(  )
** 功能描述	:   读取陀螺仪的角位移值
** 输　入 	:	陀螺仪的端口地址：陀螺仪占用FPGA板上的UART1接口，两者端口地址相同，在spi0_FPGA.h中已经定义
**				
** 输　出  :	32位有符号数表示的陀螺仪的角位移值
** 注意事项：        
********************************************************************************************************/
struct Gyro_Struct FPGA_ReadGyro( void)
{
	OS_CPU_SR  cpu_sr;
	union u8to32
	{
		int32 value;
		uint8 datas[4];
	}gyro1;
	
	struct Gyro_Struct  gyro2;
	
	OS_ENTER_CRITICAL();
	SelectPort(FPGA_ICUEN_GYRO);
	ReadRAM_STATE(1);
	gyro1.datas[0]=ReadRAM_STATE(2);
	gyro1.datas[1]=ReadRAM_STATE(3);
	gyro1.datas[2]=ReadRAM_STATE(4);
	gyro1.datas[3]=ReadRAM_STATE(6);
	gyro2.count=DisConnect();
	OS_EXIT_CRITICAL();
	gyro2.value = gyro1.value;
	
	return gyro2;
}

/*********************************************************************************************************
** 函数名称	:	Set_PWM(  )
** 功能描述	:	设置某路PWM波的开启/关闭，频率，占空比
** 输　入 	:	1.PWM的ID号;
				2.PWM开启/关闭标志;
				3.PWM频率;
				4.PWM占空比
**				
** 输　出  :	无
** 注意事项: note1:ID range from 1 to 6
			 note2:	enable=1 代表 开启PWM输出, enable=0 代表 关闭PWM输出
			 note3:freq range from 10Hz to 500kHz 
**			 note4:duty1K range from 0 to 1000，1000 stand for 100%
********************************************************************************************************/
void FPGA_SetPWM( uint8 id,uint8 enable,uint32 freq,uint16 duty1K )
{  
    OS_CPU_SR  cpu_sr;
	union u8tou16
	{
		uint16 value;
		uint8 datas[2];
	}/*num1,*/num2;
	
	//uint8 	pre_div;
	uint16 	cycle_num;
	uint16 	width_num;
/*	
	if( freq<2000 )
	{
		pre_div=2000/freq;				 	// pre_div= (int) 50M/(25K*freq)
		cycle_num=FPGA_CLK/(pre_div*freq);	// cycle_num取值在25K附近，极端情况下为50K，仍然小于65535
	}
	else
	{
		pre_div=1;
		cycle_num=FPGA_CLK/freq;
	}
*/	
	cycle_num = 20000;
	width_num=cycle_num*duty1K/1000;
	
	
	//num1.value=cycle_num;
	num2.value=width_num;
	OS_ENTER_CRITICAL();
	SelectPort(FPGA_ICUEN_PWM);
	WriteRAM( 0,id-1);
	WriteRAM( 1,enable);
	//WriteRAM( 2,pre_div);
	//WriteRAM( 3,num1.datas[0] ); //发送cycle_num低8位
	//WriteRAM( 4,num1.datas[1] ); //发送cycle_num高8位
	WriteRAM( 2,num2.datas[0] ); //发送width_num低8位
	WriteRAM( 3,num2.datas[1] ); //发送width_num高8位
	DisConnect();	
	OS_EXIT_CRITICAL();
}


void FPGA_SetPWM1( uint8 id,uint8 enable,uint32 freq,uint16 duty0x6000 )
{   
    OS_CPU_SR  cpu_sr;
	union u8tou16
	{
		uint16 value;
		uint8 datas[2];
	}num1,num2;
	
	uint8 	pre_div;
	uint16 	cycle_num;
	uint16 	width_num;
	
	if( freq<2000 )
	{
		pre_div=2000/freq;				 	// pre_div= (int) 50M/(25K*freq)
		cycle_num=FPGA_CLK/(pre_div*freq);	// cycle_num取值在25K附近，极端情况下为50K，仍然小于65535
	}
	else
	{
		pre_div=1;
		cycle_num=FPGA_CLK/freq;
	}
	
	width_num=cycle_num*duty0x6000/0x6000;
	
	
	num1.value=cycle_num;
	num2.value=width_num;
	OS_ENTER_CRITICAL();
	SelectPort(FPGA_ICUEN_PWM);
	WriteRAM( 0,id-1);
	WriteRAM( 1,enable);
	//WriteRAM( 2,pre_div);
	//WriteRAM( 3,num1.datas[0] ); //发送cycle_num低8位
	//WriteRAM( 4,num1.datas[1] ); //发送cycle_num高8位
	WriteRAM( 2,num2.datas[0] ); //发送width_num低8位
	WriteRAM( 3,num2.datas[1] ); //发送width_num高8位
	DisConnect();	
	OS_EXIT_CRITICAL();
}

void FPGA_PWM_INIT(void)
{
	OS_CPU_SR  cpu_sr;
   	//int i;
   	OS_ENTER_CRITICAL();
   	//for(i=1;i<=6;i++)
   	//{
     	FPGA_SetPWM(1,1,50,100);

     	FPGA_SetPWM(2,1,50,42);
   	//}
   	OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** 函数名称	:	Set_FPGA_UART_BaudRate()
** 功能描述	:   设置FPGA板上某个UART接口的波特率
** 输　入 	:	1.UART接口对应的端口地址(spi0_FPGA.h中有定义);
				2.要设置的波特率
**				
** 输　出  :	无
** 注意事项：  波特率 range 300~115200      
********************************************************************************************************/
void FPGA_UARTSetBaudRate( uint8 port_addr,uint32 baudrate )
{   
    OS_CPU_SR  cpu_sr;
	union u16tou8
	{
		uint16 value;
		uint8 datas[2];
	}num;

	num.value= (uint16)(6250000/baudrate);	 
	OS_ENTER_CRITICAL();
	SelectPort(port_addr);
	WriteRAM( 0,0x01);
	WriteRAM( 1,num.datas[0]);
	WriteRAM( 2,num.datas[1]);
	DisConnect();
	OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** 函数名称	:	FPGA_UART_txd( )
** 功能描述	:	通过FPGA板上的某个UART发送出一个字节
** 输　入 	:	1.UART接口的对应的端口地址（spi0_FPGA.h中有定义）;
				2.要发送的字节数据
**				
** 输　出  :	无
** 注意事项：        
********************************************************************************************************/
void FPGA_UartSendByte(uint8 port_addr, uint8 data )
{	
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();
	SelectPort(port_addr);
	WriteRAM( 0,0x00);
	WriteRAM( 1,data);
	DisConnect();
	OS_EXIT_CRITICAL();
}


/*********************************************************************************************************
** 函数名称	:	FPGA_UART_rxd()
** 功能描述	:   接收某个UART接口收到的数据缓冲队列
** 输　入 	:	1.UART接口的对应的端口地址(spi0_FPGA.h中有定义);
				2.存数据的17*byte数组
**				
** 输　出  :	无
** 注意事项：   该函数修改了数组data的数据
				data[0]表示缓冲队列的有效长度，0~16为正常值，若等于100 说明缓冲区溢出(丢帧)
				data[1]~data[16] :16个字节的数据缓冲队列，其中data[1]为最后收到的数据，其它依次类推     
********************************************************************************************************/
void FPGA_UartRead(uint8 port_addr,uint8 * data)
{   
    OS_CPU_SR  cpu_sr;
	int i;
	OS_ENTER_CRITICAL();
	SelectPort(port_addr);
	ReadRAM_FIFO(0);
	for(i=0;i<16;i++)
	{
		data[i]=ReadRAM_FIFO(i+1);
	}
	data[16]=DisConnect();
	OS_EXIT_CRITICAL();
}

#if LightBoardSetting
  /*********************************************************************************************************
** 函数名称	:	FPGA_UART_rxd_LB()
** 功能描述	:   接收某个UART接口收到的灯板数据
** 输　入 	:	1.UART接口的对应的端口地址(spi0_FPGA.h中有定义);
				2.存数据的4*byte数组
**				
** 输　出  :	无
** 注意事项：   该函数修改了数组data的数据
				灯板共32个灯，每8个灯的状态存放在一个data中    
********************************************************************************************************/
void FPGA_UartReadLB(uint8 port_addr,uint8 * data)
{   
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();
	SelectPort(port_addr);
	ReadRAM_STATE(0);
	data[0]=ReadRAM_STATE(1);
	data[1]=ReadRAM_STATE(2);
	data[2]=ReadRAM_STATE(3);
	data[3]=DisConnect();
	OS_EXIT_CRITICAL();
}

#endif

#if ManualSetting
  /*********************************************************************************************************
** 函数名称	:	Read_Manual
** 功能描述	:   接收FPGA接口收到的手柄数据
** 输　入 	:	存数据的16*byte数组
**				
** 输　出  :	无
** 注意事项：  
********************************************************************************************************/
void FPGA_ReadManual(uint8 * data)
{   
   OS_CPU_SR  cpu_sr;
	uint8 i;
	OS_ENTER_CRITICAL();
	SelectPort(5);
	ReadRAM_STATE(0);
	for(i=1;i<10;i++)
	{
		data[i]=ReadRAM_STATE(i);
	}
	data[10]=DisConnect();
	/*
	data[0]=0;
	for(i=11;i<16;i++)
		data[i]=0;
	*/	
	OS_EXIT_CRITICAL();
}

#endif

/*********************************************************************************************************
** 函数名称	:	FPGA_ReadUltrasonic
** 功能描述	:   接收FPGA接口收到的超声波的数据
** 输　入 	:	超声波用到的UART口
**				
** 输　出  :	返回的16位的超声波测距的距离值
** 注意事项：  如果要换FPGA的UART口，要修改函数里面SelectPort();的值
**				FIFO(1)  和  FIFO(2) 的值分别是低8位和高8位
********************************************************************************************************/
//#define 	ULTRASONIC_1   	8
//#define	ULTRASONIC_2	9

uint16 FPGA_ReadUltrasonic(uint8 port)
{
	OS_CPU_SR  cpu_sr;
	union u8to16
	{
		int16 value;
		uint8 datas[2];
	}Supersonic;
	OS_ENTER_CRITICAL();
	SelectPort(port);
	ReadRAM_FIFO(0);
	ReadRAM_FIFO(1);	
	Supersonic.datas[0] = ReadRAM_FIFO(2);
	Supersonic.datas[1] = ReadRAM_FIFO(3);
	OS_EXIT_CRITICAL();
	return Supersonic.value;
}
