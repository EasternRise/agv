//===================================================================
// File Name : 2440lib.h
// Function  : S3C2440 
// Date      : February 26, 2002
// Version   : 0.0
// History
//  0.0 :Feb.20.2002:SOP     : Programming start
//  0.01:Mar.29.2002:purnnamu: For POWEROFF_wake_up, the START... label is added
//===================================================================
 
#ifndef __2440lib_h__
#define __2440lib_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"

#define DebugOut Uart_Printf

#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))

#define ONESEC0 (62500)	             //16us resolution, max 1.04 sec
#define ONESEC1 (31250)	             //32us resolution, max 2.09 sec
#define ONESEC2 (15625)	             //64us resolution, max 4.19 sec
#define ONESEC3 (7812)	             //128us resolution, max 8.38 sec
#define ONESEC4 (PCLK/128/(0xff+1))  //@60Mhz, 128*4us resolution, max 32.53 sec

#define NULL 0

#define EnterPWDN(clkcon) ((void (*)(int))0x20)(clkcon)
void StartPointAfterPowerOffWakeUp(void); //purnnamu:Mar.29.2002


// 2440lib.c

void outportw(U16, U32);
U16 inportw(U32);
//void ExitCritical(int *i);

#ifdef __cplusplus
}
#endif

#endif  //__2440lib_h__
