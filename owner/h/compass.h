#ifndef _COMPASS_H
#define _COMPASS_H

void Compass_Init(void);
void UART1_DMA_init(unsigned int  baud);
void USART1_Config(void);   

extern float angle_average;

#endif
