#ifndef _RELAY_INIT_H
#define _RELAY_INIT_H
	#include "stm32f10x.h"

	
	void IO_Configuration(void);
	
	
	
	
	void READ(void);
	void IO_IT_config(void);
  void EXTI2_IRQHandler(void);
	void EXTI15_10_IRQHandler(void);
	extern u8 sa_status,st_status;
	extern u8 start_stepper_flag;
	
#endif
