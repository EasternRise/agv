#include "all.h"

void ISR_Handle (void);

uint8 Flag_Recv=0;

//中断服务函数初始化
void ISR_Init(void)
{
	pISR_TIMER4		= (uint32)ISR_Handle;
    pISR_UART0      = (uint32)ISR_Handle;
    pISR_UART2		= (uint32)ISR_Handle;
    pISR_IIC		= (uint32)ISR_Handle;
#if CANSetting
	pISR_EINT1		= (uint32)ISR_Handle;
#endif

    rINTMOD			= 0x0;	  			//All=IRQ mode
    
    MASKIrq( BIT_ALLMSK );	  			//All interrupt is masked.
    MASKSubIrq( BIT_SUB_ALLMSK );
    
    rGPFCON &= ~0xc;					//设置外部中断1引脚
    rGPFCON |=   0x8;
    rGPFUP  |=   0x2; 
    
    rEXTINT0 &= ~0x70;					//外部中断1下降沿触发
    rEXTINT0 |= 0x20;

	ClearPending( BIT_ALLMSK );
	ClearSubPending( BIT_SUB_ALLMSK );
	
#if CANSetting
	UNMASKIrq( BIT_EINT1 );				//开外部中断1
#endif
}

//中断服务函数
//由OS_CPU_IRQ_ISR函数直接导向本函数，添加中断时直接添加偏移量即可
void ISR_Handle (void)
{
	rINTOFFSET=0;
	
   	switch(rINTOFFSET)
   	{
#if CANSetting
   		case 1:							//外部中断1，MCP2515中断
   			Flag_Recv=1;
   			ClearPending( BIT_EINT1 );
   		break;
#endif
   		case 14:						//定时器4中断，每1ms一次
			OSTimeTick();				//节拍服务，使任务进入就绪态
			Expection_FLS();			//全场信息刷新
			ClearPending( BIT_TIMER4 ); 
   		break;
   		case 27:
   			Expection_I2C();			//IIC中断
   			ClearPending( BIT_IIC ); 
   		break;
   		case 28:
   			Expection_UART0();			//UART0中断
   		break;
   		case 15:
   			Expection_UART2();			//UART2中断
   			ClearPending( BIT_UART2 ); 
   			ClearSubPending(BIT_SUB_ERR2);
   			ClearSubPending( BIT_SUB_RXD2 );
   		break;
   		default:
   			ClearPending(BIT_ALLMSK);
   		break;
	}
}
