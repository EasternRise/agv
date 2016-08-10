#include "all.h"
#include "compass.h"

void ReadyMeasure(void);

void Compass_Init(void)
{
		UART1_DMA_init(9600);
		//ReadyMeasure();   //标定用，只需标定一次，标定完成注释掉即可
	
}

float angle;
float angle_average;

volatile float angle_last[4]={0};
volatile uint8_t USART1_RxBuffer[8];

#define USART1_DR_Address ((u32)0x40013804)     //USART1-DR 地址

void UART1_DMA_init(u32 baud)
{
	
  GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Usart1 DMA 配置
	/* USARTy RX DMA1 Channel --DMA1_Channel5(triggered by USART1 Rx event) Config */
	DMA_DeInit(DMA1_Channel5); 												// 
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Address;			//DMA通道1的地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RxBuffer;		//DMA传送地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//传送方向 USART是外设
	DMA_InitStructure.DMA_BufferSize = 8;						//传送内存大小 ,注意内存大小为DMA传送地址的数组大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//传送源地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//传送内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//源地址的数据长度是8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//传送的目的地址是8位宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//传送模式循环
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//优先级设置
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							//DMA通道没有设置为内存到内存传送 
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);							//
	
		//USART 初始化设置

	USART_InitStructure.USART_BaudRate = baud;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USARTy DMA Rx request */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	/* 允许DMA1通道1传输结束中断 */
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC, ENABLE);
	/* Enable USARTy RX DMA1 Channel */
	DMA_Cmd(DMA1_Channel5, ENABLE);

  USART_Cmd(USART1, ENABLE);                    //使能串口 

}


void DMA1_Channel5_IRQHandler(void)
{   
 
	 uint8_t SUM;
   //uint8_t i;
	if(DMA_GetITStatus(DMA1_IT_GL5)==SET)
	{

    if(USART1_RxBuffer[0]==0X0D&&USART1_RxBuffer[1]==0X0A)
		{
			SUM=USART1_RxBuffer[6]+USART1_RxBuffer[5]+USART1_RxBuffer[4]+USART1_RxBuffer[3]+USART1_RxBuffer[2]+USART1_RxBuffer[1]+USART1_RxBuffer[0];
		  if(USART1_RxBuffer[7]==SUM)
			{
			//angle=1;
				angle=(USART1_RxBuffer[2]-'0')*100+(USART1_RxBuffer[3]-'0')*10+(USART1_RxBuffer[4]-'0')+(USART1_RxBuffer[6]-'0')*0.1;
			}
    }
		
//				for( i = 0;i<=2;i++)
//		{
//			angle_last[i] = angle_last[i+1];
//		}
		angle_last[0]=angle_last[1];
		angle_last[1]=angle_last[2];
		angle_last[2]=angle_last[3];
		angle_last[3]=angle;
		
		angle_average=(angle_last[3]+angle_last[2]+angle_last[1]+angle_last[0]*10)/13;
		DMA_ClearITPendingBit(DMA1_IT_GL5);

	}
  
}

void UART1_SendByte(uint8_t ch)
{
  USART_SendData(USART1, (uint8_t) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void ReadyMeasure(void)
{
int i;
  UART1_SendByte(0xC0);
	
 for(i=0;i<5;i++)
  {
		delay(1000);
	}
	
  UART1_SendByte(0xC1);
  delay(50);
	
//  UART1_SendByte(0X31);
	
  UART1_SendByte(0X03);
  UART1_SendByte(0X00);
  UART1_SendByte(0X04);
  UART1_SendByte(0X00);
}
