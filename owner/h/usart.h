#ifndef __USART_H
#define __USART_H
	#include "stm32f10x.h"
	
	#define RxBufferSize 8
	#define TxBufferSize 12
	extern volatile uint8_t USART1_RxBuffer[RxBufferSize];
	extern	volatile float USART3_TxBuffer[TxBufferSize];
	extern int64_t Gyro_Float_Total1;
	extern int64_t Gyro_Float_Total2;
	
	extern u32 KeyValue;
	extern u8	KeyReady;
	
	void UART1_DMA_init(u32 bound);
	void DMA_check(void);
	void UART1_DMA_EN(void);
	void DMA_check(void);
	void UART1_SendByte(uint8_t ch);
	
	void UART3_init(u32 bound);
	void UART3_SendByte(uint8_t ch);
	
	void USART3_Init(void);
	void DMA_Enable(void);

#endif
