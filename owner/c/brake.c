#include "all.h"

int stopcount=0;//刹车第二情况判断标志
int release=0;

float stop_engine_angle=96;
float stop_engine=97;



int stopCount=0;
int stopStatus=0;

void stop(void)
{
    if(stopStatus)
    {
        SET_ANGLE(stop_engine,2);  
        return;
    }
    else if((Point_NowNum>Route[20-1]+1 &&Point_NowNum<Route[21-1] ))//山道2弯道
         {
//              if(stopCount < 1)
//              {
//                  brakesec=1;
//                  TIM_Cmd(TIM6,ENABLE);
//                  stopCount=1;
//                  stopStatus=1;
//                  return;
//              }
//              
             if(Gps_List[0].speed>1.0)
             {
                 stop_engine_angle=stop_engine_angle+1.5;
             }
             else if(Gps_List[0].speed<0.8)
             {
                 stop_engine_angle=stop_engine_angle-1.2;
             }	
         }
        else if( (Point_NowNum>Route[22-1]+1 &&Point_NowNum<Route[23-1]) )//山道3直线
         {
             if(stopCount < 2)
             {
                 brakesec=2;
                 TIM_Cmd(TIM6,ENABLE);
                 stopCount=2;
                 stopStatus=1;
                 return;
             }
             if(Gps_List[0].speed>0.7)
             {
                 stop_engine_angle=stop_engine_angle+0.5;
             }
             if(Gps_List[0].speed<0.6)
             {
                 stop_engine_angle=stop_engine_angle-1.0;
             }
         }
         else if( (Point_NowNum>Route[23-1]+1 &&Point_NowNum<Route[24-1]) )//山道3直线
         {
             
             
             
                 if(stopCount < 3)
                 {
                     brakesec=2;
                     TIM_Cmd(TIM6,ENABLE);
                     stopCount=3;
                     stopStatus=1;
                     return;
                 }
                  stop_engine_angle=stop_engine-20;
                 
             if(Gps_List[0].speed>0.8)
             {
                 stop_engine_angle=stop_engine_angle+1.3;
             }
             if(Gps_List[0].speed<0.7)
             {
                 stop_engine_angle=stop_engine_angle-1.5;
             }    
         
     }
 
        else if(Point_NowNum>=Route[24-1]+3&& Point_NowNum<=Route[24-1]+70)//最后一段
        {
            if(Point_NowNum>=Route[24-1]+30)
            {
                if(! release )
                 {
                     release=1;
                     SET_ANGLE(100,3);
                     
                 }
            }
             
            if(Gps_List[0].speed>0.4)
            {
                stop_engine_angle=stop_engine_angle+1.3;
            }
            if(Gps_List[0].speed<0.3)
            {
                stop_engine_angle=stop_engine_angle-0.3;
            }        

        }
        else if(Route_Num>=19)//进入山道
        {
            stop_engine_angle=stop_engine-10;
        }
         else
         {
             stop_engine_angle=0;
         }
         
         
         if(stop_engine_angle>stop_engine)
             stop_engine_angle=stop_engine;
         else if(stop_engine_angle<0)
             stop_engine_angle=0;
         SET_ANGLE(stop_engine_angle,2); 
         
}

void brake_timer_cfg(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 1440 - 1 ; 
//	TIM_TimeBaseStructure.TIM_Period = 50000; // 5000 - 5ms
    TIM_TimeBaseStructure.TIM_Period = 50000; // 5000 - 5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM6,TIM_IT_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	//TIM_Cmd(TIM6,ENABLE);

}

static char count = 0;
int brakesec=0;

void TIM6_IRQHandler(void)
{
	TIM_ClearFlag(TIM6,TIM_IT_Update);
	count ++;
	if(count <= brakesec)
	{
		return;
	}
	     SET_ANGLE(stop_engine-20,2); 
	stopStatus = 0;
	TIM_Cmd(TIM6,DISABLE);
}
