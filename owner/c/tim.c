#include "all.h"
#include "tim.h"

void leave_timer_cfg(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1 ; 
//	TIM_TimeBaseStructure.TIM_Period = 100; // 5000 - 1ms
    TIM_TimeBaseStructure.TIM_Period = 100; // 5000 - 1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM6,TIM_IT_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	//TIM_Cmd(TIM6,ENABLE);

}


u32 leave_sec=0;

extern float move_distance,AGV_speed;

void TIM6_IRQHandler(void)
{
static	u32 count = 0;
	
	TIM_ClearFlag(TIM6,TIM_IT_Update);
	count ++;
	if(count <= leave_sec)
	{
		return;
	}
	//将机器人的状态转换为on_road
	if(my_robot != back_slow)
			my_robot=on_road;
	
	TIM_Cmd(TIM6,DISABLE);
	
	//快速前进1s
	move_sec=ceil(move_distance/AGV_speed*1000*0.72);
	TIM_Cmd(TIM5,ENABLE);
	count = 0;
}



void move_timer_cfg(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1 ; 
//	TIM_TimeBaseStructure.TIM_Period = 50000; // 5000 - 5ms
    TIM_TimeBaseStructure.TIM_Period = 100; // 5000 - 5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM5,TIM_IT_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	//TIM_Cmd(TIM6,ENABLE);

}


u32 move_sec=0;

void TIM5_IRQHandler(void)
{
	static	 u32 count = 0;
	
	TIM_ClearFlag(TIM5,TIM_IT_Update);
	count ++;
	if(count <= move_sec)
	{
		return;
	}
	//将机器人的状态转换为on_road
	if(my_robot != back_slow)
	my_robot=on_road_slow;
	
	TIM_Cmd(TIM5,DISABLE);
	count = 0;
}
