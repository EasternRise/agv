#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"

void move_timer_cfg(void);
extern u32 move_sec;

void leave_timer_cfg(void);
extern u32 leave_sec;

#endif
