
#include "all.h"
#include "misc.h"
//TIME2_CH2_PA1
//TIME2_CH3_PA2

u32 temp1,temp2; 
u8 TIM2CH2_CAPTURE_STA=0;                            
u16 TIM2CH2_CAPTURE_VAL;    
u8 TIM2CH3_CAPTURE_STA=0;                            
u16 TIM2CH3_CAPTURE_VAL; 
 void colour(void)
{


    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM2_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
    
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_2); 
    
    TIM_TimeBaseStructure.TIM_Period = 0xffff; 
    TIM_TimeBaseStructure.TIM_Prescaler = 71; 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); 
            
   
    TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2;   
    TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
    TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
    TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
    TIM2_ICInitStructure.TIM_ICFilter = 0;                        //²»ÂË²¨
    TIM_ICInit(TIM2,&TIM2_ICInitStructure); 
    
		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_3;   
    TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
    TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
    TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
    TIM2_ICInitStructure.TIM_ICFilter = 0;                        //²»ÂË²¨
    TIM_ICInit(TIM2,&TIM2_ICInitStructure); 
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;   
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2|TIM_IT_CC3,ENABLE);
	//	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC3,ENABLE);
    TIM_Cmd(TIM2,ENABLE);    


}



void TIM2_IRQHandler(void)
{ 
    if((TIM2CH2_CAPTURE_STA&0X80)==0)   
    {
        if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
        {
            if(TIM2CH2_CAPTURE_STA&0X40)//
            {
                if((TIM2CH2_CAPTURE_STA&0X3F)==0X3F)
                {
                    TIM2CH2_CAPTURE_STA|=0X80;
                    TIM2CH2_CAPTURE_VAL=0XFFFF;
                }else TIM2CH2_CAPTURE_STA++;
            }
        }
        
        if(TIM_GetITStatus(TIM2,TIM_IT_CC2) !=RESET)
        {
            if(TIM2CH2_CAPTURE_STA & 0x40)
            {
                TIM2CH2_CAPTURE_STA|=0X80;        //????????????
                TIM2CH2_CAPTURE_VAL = TIM_GetCounter(TIM2);
                TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ????????
            }
            else
            {
                TIM2CH2_CAPTURE_STA=0;            //??
                TIM2CH2_CAPTURE_VAL=0;
                
                TIM_SetCounter(TIM2,0);
                TIM2CH2_CAPTURE_STA|=0X40;        //?????????
//                TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);        //CC1P=1 ????????
            }
        }
        
    }
		
		if((TIM2CH3_CAPTURE_STA&0X80)==0)   
    {
        if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
        {
            if(TIM2CH3_CAPTURE_STA&0X40)//
            {
                if((TIM2CH3_CAPTURE_STA&0X3F)==0X3F)
                {
                    TIM2CH3_CAPTURE_STA|=0X80;
                    TIM2CH3_CAPTURE_VAL=0XFFFF;
                }else TIM2CH3_CAPTURE_STA++;
            }
        }
        
        if(TIM_GetITStatus(TIM2,TIM_IT_CC3) !=RESET)
        {
            if(TIM2CH3_CAPTURE_STA & 0x40)
            {
                TIM2CH3_CAPTURE_STA|=0X80;        //????????????
                TIM2CH3_CAPTURE_VAL = TIM_GetCounter(TIM2);
                TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ????????
            }
            else
            {
                TIM2CH3_CAPTURE_STA=0;            //??
                TIM2CH3_CAPTURE_VAL=0;
                
                TIM_SetCounter(TIM2,0);
                TIM2CH3_CAPTURE_STA|=0X40;        //?????????
//                TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);        //CC1P=1 ????????
            }
        }
        
    }
    TIM_ClearITPendingBit(TIM2,TIM_IT_CC2|TIM_IT_Update); /*???????*/
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC3|TIM_IT_Update); 
						 if(TIM2CH2_CAPTURE_STA&0X80)
	{
			temp1=TIM2CH2_CAPTURE_STA&0X3F;
			temp1*=65536;
			temp1+=TIM2CH2_CAPTURE_VAL;
			TIM2CH2_CAPTURE_STA=0;
	}
					
	 if(TIM2CH3_CAPTURE_STA&0X80)
	{
			temp2=TIM2CH3_CAPTURE_STA&0X3F;
			temp2*=65536;
			temp2+=TIM2CH3_CAPTURE_VAL;
			TIM2CH3_CAPTURE_STA=0;
	}
}
