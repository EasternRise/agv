#ifndef USERDEVICES_H
#define USERDEVICES_H

#ifndef WheelSelectSetting
extern void Speed_Motor(fp64 speed_FL,fp64 speed_FR,fp64 speed_BL,fp64 speed_BR);
#endif

#ifdef WheelSelectSetting
extern void Speed_Motor(fp64 speed_BL,fp64 speed_BR,fp64 speed_F);
#endif


//gyro.c

struct Gyro_Struct
{
	int32  value;
	uint8  count;
};

extern int32 Gyro_Float;

extern struct Gyro_Struct Gyro_Last;

extern struct Gyro_Struct Gyro_Now;

extern int64 Gyro_Total;

extern fp64  Gyro_Convert1;
extern fp64  Gyro_Convert2;

extern int64 Gyro_Roll;

extern fp64 Gyro_Error_Float;

extern void GYRO_NandInit( void );

extern void GYRO_NandSave( void );

extern void GYRO_CheckFloat( void );

extern void GYRO_Update( void );

extern void TestGyro( void );

//encoder.c
struct Encoder_Stat
{
	int64 Total;
	
	int64 Last;
	
	fp64 Convert1;		//正转系数
	
	fp64 Convert2;		//反转系数
	
	int32 dir_flag;		//判断正反转标志位
	
	fp64 Distance;
	
	int64 Now;
	
	fp64  Theta;
	
	int32 step;
	
	fp64 Radius;
	
};

extern struct Encoder_Stat Encoders[];

extern void ENCODER_NandSave( void );

extern void ENCODER_NandInit( void );

extern void ENCODER_Init(int index);

extern void ENCODER_Ac_Init(int index);

extern void Location_ENCODER_Init(int index);

extern void angle_error_init(int index);

extern void Encoder_Update( void );

extern void ENCODER_Clear(int index);


//manual.c
extern void TaskManual(void *pdata);
extern uint8 SA2_en;
extern uint8 Route_Num;
extern uint8 ManualData[];
extern uint8 FBonly_flag;
extern fp32 Manual_speed_FL;			//手动左前轮速度
extern fp32 Manual_speed_FR;			//手动右前轮速度
extern fp32 Manual_speed_BL;			//手动左后轮速度
extern fp32 Manual_speed_BR;			//手动右后轮速度

//小强手柄

#define Button1_Up ((ManualData[7]&0x0c)==0x04)
#define Button1_Down ((ManualData[7]&0x0c)==0x08)
#define Button1_Off ((ManualData[7]&0x0c)==0x0)
#define Button2_Up ((ManualData[7]&0x30)==0x10)
#define Button2_Down ((ManualData[7]&0x30)==0x20)
#define Button2_Off ((ManualData[7]&0x30)==0x0)
#define Button3_Up ((ManualData[8]&0x0c)==0x08)
#define Button3_Down ((ManualData[8]&0x0c)==0x04)
#define Button3_Off ((ManualData[8]&0x0c)==0x0)
#define Button4_Up ((ManualData[8]&0x30)==0x20)
#define Button4_Down ((ManualData[8]&0x30)==0x10)
#define Button4_Off ((ManualData[8]&0x30)==0x0)
#define Button5_Up ((ManualData[7]&0xc0)==0x40)
#define Button5_Down ((ManualData[7]&0xc0)==0x80)
#define Button5_Off ((ManualData[7]&0xc0)==0x0)
#define Button6_Left ((ManualData[9]&0x30)==0x20)
#define Button6_Right ((ManualData[9]&0x30)==0x10)
#define Button6_Off ((ManualData[9]&0x30)==0x0)
#define Button7_On ((ManualData[9]&0x02)==0x02)
#define Button7_Off ((ManualData[9]&0x02)==0x0)
#define Button8_On ((ManualData[9]&0x08)==0x08)
#define Button8_Off ((ManualData[9]&0x08)==0x0)
#define Button9_On ((ManualData[9]&0x04)==0x0)
#define Button9_Off ((ManualData[9]&0x04)==0x04)
#define Button10_On ((ManualData[7]&0x02)==0x02)
#define Button10_Off ((ManualData[7]&0x02)==0x0)

//陈曲手柄
/*
#define Button1_Up ((ManualData[7]&0x0c)==0x08)
#define Button1_Down ((ManualData[7]&0x0c)==0x04)
#define Button1_Off ((ManualData[7]&0x0c)==0x0)
#define Button2_Up ((ManualData[7]&0x30)==0x20)
#define Button2_Down ((ManualData[7]&0x30)==0x10)
#define Button2_Off ((ManualData[7]&0x30)==0x0)
#define Button3_Up ((ManualData[8]&0x0c)==0x08)
#define Button3_Down ((ManualData[8]&0x0c)==0x04)
#define Button3_Off ((ManualData[8]&0x0c)==0x0)
#define Button4_Up ((ManualData[8]&0x30)==0x20)
#define Button4_Down ((ManualData[8]&0x30)==0x10)
#define Button4_Off ((ManualData[8]&0x30)==0x0)
#define Button5_Up ((ManualData[7]&0xc0)==0x80)
#define Button5_Down ((ManualData[7]&0xc0)==0x40)
#define Button5_Off ((ManualData[7]&0xc0)==0x0)
#define Button6_Left ((ManualData[9]&0x30)==0x20)
#define Button6_Right ((ManualData[9]&0x30)==0x10)
#define Button6_Off ((ManualData[9]&0x30)==0x0)
#define Button7_On ((ManualData[9]&0x08)==0x08)
#define Button7_Off ((ManualData[9]&0x08)==0x0)
#define Button8_On ((ManualData[9]&0x02)==0x02)
#define Button8_Off ((ManualData[9]&0x02)==0x0)
#define Button9_On ((ManualData[7]&0x02)==0x02)
#define Button9_Off ((ManualData[7]&0x02)==0x0)
#define Button10_On ((ManualData[9]&0x04)==0x04)
#define Button10_Off ((ManualData[9]&0x04)==0x0)
*/





//fpga.c

extern void FGPA_Enable( uint8 ID );

#define  FPGA_UART1     3
#define  FPGA_UART2     4
#define  FPGA_UART3     5
#define  FPGA_UART4     6
#define  FPGA_UART5     7

//	the part of pwm
#define  FPGA_ICUEN_PWM 		1	//ICU_EN[1]
#define  FPGA_PWM_ENABLE       1
#define  FPGA_PWM_FREQM         500	//for moto
#define  FPGA_PWM_FREQ            50	//for rd
#define  FPGA_CLK		         50000000

extern void FPGA_SetPWM( uint8 id,uint8 enable,uint32 freq,uint16 duty1K );	//舵机用的。ID从1~6,enable=1，freq=FPGA_PWM_FREQ
extern void FPGA_SetPWM1( uint8 id,uint8 enable,uint32 freq,uint16 duty6w );	//FOR MOTO
extern void FPGA_PWM_INIT(void);

//	the part of encoder
#define  FPGA_ICUEN_ENCODER 0	
		
extern int32 FPGA_ReadEncoder( uint8 id );		//id从1~5

//	the part of gyro
#define  FPGA_ICUEN_GYRO	3

extern struct Gyro_Struct FPGA_ReadGyro(void );	

//	the part of LTBD
#define  FPGA_ICUEN_LTBD1	4
#define  FPGA_ICUEN_LTBD2	5

extern void FPGA_UartReadLB(uint8 port_addr,uint8 * data);			//port_addr	共有2个，1是FPGA_ICUEN_LTBD1 	2是FPGA_ICUEN_LTBD2

//	the part of uart
#define  FPGA_ICUEN_UART1	6
#define  FPGA_ICUEN_UART2	7

extern void FPGA_UARTSetBaudRate( uint8 port_addr,uint32 baudrate );		//port_addr 	共有2个，1是FPGA_ICUEN_UART1	2是FPGA_ICUEN_UART2
extern void FPGA_UartSendByte(uint8 port_addr, uint8 data );
extern void FPGA_UartRead(uint8 port_addr,uint8 * data);

// the part of Manual
extern void FPGA_ReadManual(uint8 * data);

// the part of Ultrasonic
extern uint16 FPGA_ReadUltrasonic(uint8 port);

//IR_sensor.c

extern fp32 IR_dis[3];
extern uint32 FPGA_ReadIR( uint8 id );
void IR_Update(void);



#endif