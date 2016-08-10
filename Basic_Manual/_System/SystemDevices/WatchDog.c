#include "all.h"

#if WatchDogSetting

//100ms系统无响应则重启
void WatchDog_Init()
{
	rWTCNT=0x500;//计数初值
	rWTCON=0xff<<8|1<<5|0<<3|0<<2|1;
}

#endif