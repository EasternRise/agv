#ifndef __MEASURE_H
#define	__MEASURE_H

#include "stm32f10x.h"


void TIM2_Cap_Init(void);
void TIM2_IRQHandler(void);
void colour(void);
	 
extern u32 temp1,temp2;
extern u32 temp1,temp2; 
extern u8 TIM2CH2_CAPTURE_STA;                            
extern u16 TIM2CH2_CAPTURE_VAL;    
extern u8 TIM2CH3_CAPTURE_STA;                            
extern u16 TIM2CH3_CAPTURE_VAL; 
#endif /* __PWM_OUTPUT_H */

