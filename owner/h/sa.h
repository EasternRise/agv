#ifndef _SA_H
#define _SA_H

#include "all.h"

#define SA1_ON      GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 1
#define SA1_OFF     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) != 1
#define SA2_ON      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) == 1
#define SA2_OFF     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) != 1
#define SA3_ON      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == 1
#define SA3_OFF     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) != 1
#define SA4_ON      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == 1
#define SA4_OFF     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) != 1
#define SA5_ON      GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) == 1
#define SA5_OFF     GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) != 1

extern u8 sa_status,st_status;
extern u8 start_stepper_flag;

void IO_IT_config(void);
void SA_Init(void);
	



#endif
