#include "all.h"
#include "stm32f10x_can.h"

void Can_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Configure CAN1 IOs **********************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         //PA11:CAN1-RX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			   //ÊäÈëÉÏÀ­
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					 //PA12:CAN1-TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 //žŽÓÃÄ£Êœ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void CAN_config_init(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
 
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
    // 36M 250k速率
  CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=ENABLE;
  //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
  CAN_InitStructure.CAN_Prescaler=12;
  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_OperatingModeRequest( CAN1, CAN_OperatingMode_Normal);
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure); 
}
 
void Can1_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
      // 首先打开电源及时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  
   
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
   
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 然后配置pin
		Can_GPIO_Config();
    CAN_config_init();
    CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE);  // fifo0中断
    CAN_ITConfig(CAN1, CAN_IT_FMP1 | CAN_IT_FF1 | CAN_IT_FOV1, ENABLE);  // fifo1中断
    //CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);                // 发送中断
}
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg   RxMessage;
	
     CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    if(RxMessage.StdId == 0x1)
    {
        guide_ang_error =*(float *)RxMessage.Data[0];
				guide_dis_error =*(float *)RxMessage.Data[4];
//        camerError=a;
    }
    else if(RxMessage.StdId == 0x1)
    {
			;
        //b=*(u8 *)RxMessage.Data;
//        lightError=b;
    }
}

//Ö÷¿ØÏòGPS°å·¢ËÍµÄÊýŸÝ
void CAN1_RX1_IRQHandler(void)
{
	CanRxMsg   RxMessage;
	
	CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
}

uint8_t Can_State = 0;
void Can_SendData(CanTxMsg* TxM)
{
	if(CAN_Transmit(CAN1,TxM)==CAN_NO_MB)
    {
			Can_State = 1;
    }
		else 
		{
			Can_State = 0;
		}
}
