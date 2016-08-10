#ifndef _ENCODER_H
#define _ENCODER_H
#include "stm32f10x.h"

struct Encoder_Stat
{	
	int32_t Last;		//
	int32_t Total;		//码盘脉冲数
	double Convert;		//正转系数
	signed char dir;	//改变正反转方向（如果是需要的方向则置1，否则置-1）
	double Distance;		//码盘走过位移	
};

  extern struct Encoder_Stat Encoders[2];
  void Encoder_Update(void);
  void ENCODER_Clear(int index);
  void Encoder_Clear(int index);
  void Encoder_Init(void);
  void Encoder_InitXY(uint8_t i);
  void Encoder_InitR(void);
  void E_Init_Con(void);
#endif
