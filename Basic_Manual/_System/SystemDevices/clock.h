#ifndef __CLOCK_H
#define __CLOCK_H

//常量定义
#define SYS_FIN  (12000000)
#define SYS_MEGA (1000000)
#define CLKMODE_200M   0
#define CLKMODE_300M   1
#define CLKMODE_400M   2
#define CLKMODE_440M   3

//变量定义
extern int Sys_Fclk;
extern int Sys_Hclk;
extern int Sys_Pclk;
extern int Sys_Fin;//晶振频率

extern void CLOCK_Init(int mode);

#endif