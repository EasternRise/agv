#ifndef _FLASH_H
#define _FLASH_H
	#include "stm32f10x.h"

	#define PAGE_ADDRESS 0x8070000
	#define SAVEDATA_NUM 5
	
	void Data_Load(void);
	void Data_Save(void);
	

#endif
