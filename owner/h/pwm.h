#ifndef TIM_PWM_H
#define TIM_PWM_H
#include "all.h"

extern u16 pwm1_2_Freqz;//pwm波1，2输出口 的频率
extern u16 pwm3_4_Freqz;//pwm波3，4输出口的频率

void TIM_PWM_Init(void);
void  PWM_GPIO_Config(void);
void  PWM_Mode_Config(void);
int PWM_SetDuty(u8 pwmx, float duty);
// void PWM1_SetDuty(u16 rate);
// void PWM2_SetDuty(u16 rate);

void	DIR_GPIO_Config(void);
void DIR_IO_Init(void);

void set_mecf(float speed);
void set_mecbr(float speed);
void set_mecbl(float speed);

#endif
