#include "all.h"

#include "stm32f10x_exti.h"

/*scan the status of the on-offs,from num 1 to num 13.num-0-to-num-8 stands for 8 touching on_offs
 *num-8-to-num-13 stands for 5 untouching on-offs;
 * 1 - off
 - 0 - on
 use the bit 0 to bit 12,leave alone bit 13,bit 14 and bit 15;
 */

u8 sa_status=0x00;
//高位 对应 高序号
void READ(void)
{
	u8 sa_temp=0x00;
	sa_temp|=!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)<<0;  //GSA1
	sa_temp|=!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)<<1;  //GSA2
    
	sa_temp|=!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)<<2;  //GSA3
	sa_temp|=!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)<<3;  //GSA4
	sa_temp|=!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)<<4;   //GSA5
//     
	if((sa_status != sa_temp))
	{
		sa_status = sa_temp;
	}
}



/*************************************************
??: void GPIO_Configuration(void)
??: GPIO??
??: ?
??: ?
**************************************************/
void SA_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;        //??GPIO??????

  //  ??GPIO A B C D??? 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

 /* Configure PA as Output push-pull ***********GPIO_Pin_0 | GPIO_Pin_1 |  GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_15 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;   //GSA1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

 /* Configure PC as Output push-pull ***********GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12 ; //GSA2 GSA3 GSA4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;  //GSA5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void IO_IT_config(void)
{
	EXTI_InitTypeDef EXTI_initstruct;
	NVIC_InitTypeDef NVIC_initstruct;
	EXTI_DeInit();

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
	
	EXTI_initstruct.EXTI_Line=EXTI_Line12|EXTI_Line11|EXTI_Line10|EXTI_Line15;
	EXTI_initstruct.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_initstruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_initstruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_initstruct);

	EXTI_initstruct.EXTI_Line=EXTI_Line2;
	EXTI_initstruct.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_initstruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_initstruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_initstruct);

	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_initstruct.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_initstruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_initstruct.NVIC_IRQChannelSubPriority=1;
	NVIC_initstruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_initstruct);
	
	NVIC_initstruct.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_initstruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_initstruct.NVIC_IRQChannelSubPriority=2;
	NVIC_initstruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_initstruct);
	
	NVIC_initstruct.NVIC_IRQChannel=EXTI2_IRQn;
	NVIC_initstruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_initstruct.NVIC_IRQChannelSubPriority=0;
	NVIC_initstruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_initstruct);
}

//为1表示接近了
//初始位置为接近

// u8 state1=1;
// u8 state2=1;


void EXTI2_IRQHandler(void)
{
    EXTI->PR = EXTI_Line2;          // EXTI_ClearITPendingBit(EXTI_Line2)    SA5
    READ();
    
}

//接近物体触发中断
//由高电平变为低电平

extern u8 col_error;

void EXTI15_10_IRQHandler(void)
{
    READ();    
	
    if((EXTI->PR & EXTI_Line15) != (uint32_t)RESET)   // EXTI_GetITStatus(EXTI_Line15)    SA1
	{
		EXTI->PR = EXTI_Line15;      //EXTI_ClearITPendingBit(EXTI_Line15)
       // sa_status|=state1<<0;  //GSA1
	}
	
	if((EXTI->PR & EXTI_Line12) != (uint32_t)RESET)   
 	{
		EXTI->PR = EXTI_Line12;		//EXTI_ClearITPendingBit(EXTI_Line12)       SA4
	}
	if((EXTI->PR & EXTI_Line11) != (uint32_t)RESET)  // EXTI_GetITStatus(EXTI_Line11)   SA3
	{
		EXTI->PR = EXTI_Line11;   //EXTI_ClearITPendingBit(EXTI_Line11)
		if((sa_status & 0x04) )
		{
				col_error=1;
				my_robot=back_slow;
				
				switch(move_angle){
						case 0:
							my_edge[edge_cnt].dirction=1;
							my_edge[edge_cnt].start_p=start_pos.x+ start_pos.y*X_SIZE;
							my_edge[edge_cnt].stop_p =start_pos.x+(start_pos.y+1)*X_SIZE;
							edge_cnt++;
						
							add_edge(my_edge[edge_cnt-1]);
							break;
						case 180:
							my_edge[edge_cnt].dirction=1;
							my_edge[edge_cnt].start_p=start_pos.x+(start_pos.y-1)*X_SIZE;
							my_edge[edge_cnt].stop_p=start_pos.x+start_pos.y*X_SIZE;
							edge_cnt++;
							
							add_edge(my_edge[edge_cnt-1]);
						
							break;
						case 90:
							my_edge[edge_cnt].dirction=0;
							my_edge[edge_cnt].start_p=start_pos.x+start_pos.y*X_SIZE;
							my_edge[edge_cnt].stop_p=start_pos.x+1+start_pos.y*X_SIZE;
							edge_cnt++;
						
							add_edge(my_edge[edge_cnt-1]);
							break;
						case -90:
							my_edge[edge_cnt].dirction=0;
							my_edge[edge_cnt].start_p=start_pos.x-1+start_pos.y*X_SIZE;
							my_edge[edge_cnt].stop_p=start_pos.x+start_pos.y*X_SIZE;
							edge_cnt++;
						
							add_edge(my_edge[edge_cnt-1]);
							break;
			
				}
				
		}
		else
		{
				col_error=0;
		}
	}
	
	if((EXTI->PR & EXTI_Line10) != (uint32_t)RESET)  // EXTI_GetITStatus(EXTI_Line10)   SA2
	{
		EXTI->PR = EXTI_Line10; //EXTI_ClearITPendingBit(EXTI_Line10)
      // sa_status|=state2<<1;  //GSA1
	}
}

