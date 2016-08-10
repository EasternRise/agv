/* 
* @Author: obsidian
* @Date:   2015-12-19 08:55:11
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-31 21:13:32
*/
#ifndef CAN_H

#include "stm32f10x_can.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void Can1_Init(void);
void Can_SendData(CanTxMsg* TxM);
void Can_IT_Config(void);
extern unsigned char Can_Data_Num;

#define CAN_H
#endif
