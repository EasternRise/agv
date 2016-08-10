#include "all.h"

static __IO uint32_t TimingDelay;

void SysTick_init(void)
{
	if (SysTick_Config(SystemCoreClock/1000))   //Setup SysTick Timer for  1ms interrupts
	{
		/* Capture error */ 
		while (1);
	}

}

void delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	
	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
		static unsigned int cnt=0;
		
		if(++cnt >= 999)			cnt=0;
		
		TimingDelay_Decrement();
		
		if(cnt%5 == 0)
				;
		if(cnt%50==0)
		{
			UART1_SendByte(0X31);
		}
		if(cnt%100 == 0)
			//move();
			;
		if(cnt%1000==0)
				LED1_on;
		if(cnt%1000==500)
				LED1_off;
}
