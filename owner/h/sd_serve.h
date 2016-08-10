
#ifndef  __SD_SERVE_H
#define  __SD_SERVE_H

#include "stm32f10x.h"
#include "ff.h"
#include "MyTypeDef.h"

#define TOTAL_STORE  2000

extern	FIL File;
extern  FIL File_TestRead ;
extern 	FRESULT res;
extern 	uint32_t BytesWritten;
extern 	uint32_t BytesRead;
extern  uint32_t Total_Store;


union udouble
{		
		double  	value ;
		uint8_t  	data[8];
};

union  uint64
{
		int64_t  value;
		uint8_t		data[8];
};

union ufloat
{
		float   	value;
		uint8_t 	data[4];
};

union uint32
{
		int32_t  value ;
		uint8_t		data[4];
};

union uuint32
{
		uint32_t 	value ;
		uint8_t		data[4];
};

union uint16
{
		int16_t		value ;
		uint8_t		data[2];
};

union uuint16
{
		uint16_t		value ;
		uint8_t			data[2];
};


struct _SDData
{
		char 		name[20] ;
		void *  		value;
		SD_Value_Type 	value_type;
} ;


extern struct _SDData SDSave[];

/*  function  */

void 			InitNewCard(void) ;
void 			FatFsInit(void);
FRESULT 	DataStore(char *fmt,...);

extern FRESULT SD_Printf (FIL* fp, char *fmt,...);
FRESULT SD_Printf_MQ(FIL* fp, char *fmt,...);
extern FRESULT SD_Printf2(FIL* fp, char *fmt,...);


extern FRESULT f_timestamp(char *fname);
void set_dir(void);
void sd_test(void);
void f_nameDeal(char *p_char);

#endif


