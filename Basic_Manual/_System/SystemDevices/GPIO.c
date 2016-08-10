/****************************************Copyright (c)**************************************************
**                                     XJTU Robot Team
**-----------------------------------------File Info----------------------------------------------------
** File name:			gpio.c
** Last modified Date:  2013-4-10
** Descriptions:		
** Created by:		   	dong		
********************************************************************************************************/
#include "basic.h"


/*********************************************************************************************************
** ��������: IO_Init����
** ��������: 
			 GPC0-4��8-14������ȡ�Ӵ����أ��ӽ����صȿ�����������
			 GPF4�Ǹ������ϵ�STM32���ж���
***************************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     *****************************************************************/
void IO_Init(void)
{    
    rGPDDAT &= (~(0xffff));						//��ʼ�����ݶ˿������Ϊ�͵�ƽ
   
    rGPDCON &= (~(0xffffffff));					//��ʼ��ȫ���˿�Ϊ�����,��λ����һ���˿ڵ�״̬,32λ��16���˿�
	rGPDCON |= 0x55555555;						
    //rGPDUP  &= (~(0xffff));					//ʹ�ܸ�����������
   
   
   	/* 00�����룬01�����
	** GPCCON 	0	1	2	3	4	8	9	10	11	12	13	14
	**			O	I	O	I	O	I	O	 O	 O	 O	 O	O
	*/
   
    rGPCCON &= (~(0xffffffff));
 	rGPCCON |= 1<<0 | 0<<2 | 1<<4 | 0<<6 | 1<<8  | 0<<16 | 1<<18 | 1<<20 | 1<<22 | 1<<24 | 1<<26 | 1<<28;
    //1<<0����0λ��1��1<<4������4λ��1��0<<2û��ʵ�����壬ֻ��Ϊ��˵����2λ��0��
    
    rGPJDAT=0x0;
    rGPJCON &= (~(0xffffffff));                //��ʼ��ȫ���˿�Ϊ����˿�
	rGPJCON |= 0x55555555;					
	rGPJUP  &= (~(0xffff));
	
	rGPGCON &= (~(0xffffffff));           //��ʼ��Ϊ����˿�
	rGPGCON |= 0x55555555; 				
	
	 //GPF4��ARM��STM32���ж��ߣ�����Ϊ������,GPF4��ʼ��Ϊ���
    rGPFCON &= ~(0x3<<8);
    rGPFCON |= 0x1<<8;		//��8λ��1
	
    rGPFDAT &= ~(0x1<<4);
	
}



/******************************************************
**	�ӽ����غͽӽ���������:
**    ������Ϲ���3��4051 ���Զ�ȡ24����,������6������ST�Ӵ�����
**	  13�����ڽӽ�����,ÿ�ζ�ȡ��ʱ��ѡͨ��Ӧ�ĵ�ַ��ɽ�ͨ 
**
** 
*******************************************************/



/*************************************************************
**�������ƣ�SA_Read()
**����������port  Ҫ��ȡ�Ľӽ����ض˿ں� ��1~13
**������������ȡ�ӽ����ص�ֵ
**************************************************************/

uint32 SA_ENCODE[13] ={0x0,0x6010,0x4010,0x6000,0x4000,0x2010,0x0010,0x2000,0x0000,  0x1804,0x1004,0x1800,0x1000};
		          //S3:000,  111 ,  110 , 101  , 100  , 011  , 010  , 001  , 000 ��S2��111 �� 110��  101��  100
			//        :      000 ,  001 , 010  , 011  , 100  , 101  , 110  , 111 ��    000 �� 001��  010��  011
uint8 SA_Read(uint8 port)
{
	U32 state,i;
	uint8  value;
	if( port<9  &&  port>0 ) 
	{
		rGPCDAT &= ~(0x6010 );  
		rGPCDAT |= SA_ENCODE[port]; //���ѡ���˿ڵ�ַ
		for(i=0;i<100;i++);			//��ʱ�ȴ���ֵ
		state = rGPCDAT;			//ȡ��ֵ��ļĴ���ֵ
		if(state & (1<<8))			//ȡ��S3_comλ��ֵ�����ж��Ƿ�Ϊ0
			value = 1;
		else 
			value = 0;
	}
	else{
		rGPCDAT &= ~(0x1804);  
		rGPCDAT |= SA_ENCODE[port];
		for(i=0;i<100;i++); 
		state = rGPCDAT;
		if(state & (1<<3))
			value = 1;
		else 
			value = 0;		
	} 		
	return value;
}


/*************************************************************
**�������ƣ�ST_Read()
**����������port  Ҫ��ȡ�ĽӴ����ض˿ں� ��1~6
**������������ȡ�Ӵ����ص�ֵ
**************************************************************/

uint32 ST_ENCODE[7]={0x0, 0x804,  0x4, 0x800, 0x0000,  0x601,0x401};
			    //S2��      011,  010,  001,   000, S1��111 ,  110
			    //          100,  101,  110,   111,     000 ,  001
 					  
 					  
uint8 ST_Read(uint8 port)
{
	U32 state,i;
	uint8  value;
	if(port>0 && port<5)
	{
		rGPCDAT &= ~(0x1804);
		rGPCDAT |= ST_ENCODE[port]; 
		for(i=0;i<100;i++);
		state = rGPCDAT;
		if(state & (1<<3))
			value = 1;
		else 
			value = 0;
	}
	else{
		rGPCDAT &= ~(0x601);
		rGPCDAT |= ST_ENCODE[port];
		for(i=0;i<100;i++); 		
		state = rGPCDAT;
		if(state & (1<<1))
			value = 1;
		else 
			value = 0;		
	} 		
	return value;
}




/********************************************************
*
*����������LED����ʾ ��Ϩ��  LED��ARM������һ��Ҫ�ӵ͵�ƽ
*********************************************************/


void LED_Set(U8  LED_num)
{
	switch(LED_num){
		
		case 1 :
			rGPGDAT  |= (1<<12);
			break ;
			
		case 2:
			rGPJDAT  |= (1<<10);
			break;
			
		case 3:
			rGPJDAT  |= (1<<9);
			break;
		case 4:
			rGPJDAT  |= (1<<8);
		default: break;						
	}
}



void LED_Clear(U8  LED_num)
{
	switch(LED_num){
		
		case 1 :
			rGPGDAT  &= (~(1<<12));
			break ;
			
		case 2:
			rGPJDAT  &= (~(1<<10));
			break;
			
		case 3:
			rGPJDAT  &= (~(1<<9));
			break;
		case 4:
			rGPJDAT  &= (~(1<<8));
		default: break;						
	}
}
