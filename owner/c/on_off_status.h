#ifndef _RELAY_INIT_H
#define _RELAY_INIT_H
	#include "stm32f10x.h"

	void RCC_Configuration(void);
	void GPIO_Configuration(void);
	void Can_Init(void);
	void USB_LP_CAN1_RX0_IRQHandler(void);
	void CAN1_RX1_IRQHandler(void);
	void relay_clear(void);
	void READ(void);
	void IO_IT_config(void);
  void EXTI2_IRQHandler(void);
	void EXTI15_10_IRQHandler(void);
	void TIM3_Configuration(void);
	void TIM3_NVIC_Configuration(void);
	void TIM3_DO(void);
	void TIM_Config(void);
	void play(void);
	void get_time(void);
extern 	float Factor;
extern float Length;
	extern u8 sa_status,st_status;
	extern u8 start_stepper_flag;
	
#endif
