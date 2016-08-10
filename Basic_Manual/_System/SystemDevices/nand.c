#include "all.h"

int32 Nand_Error=0;
int8  Nand_2G=0;

void Nand_Reset(void)
{
    volatile int i;
   
	NFChipEn();
	NF_CLEAR_RB();
	for (i=0; i<10; i++);
	WrNFCmd(0xFF);	//reset command
	NF_DETECT_RB();
	NFChipDs();
}


uint32 WaitNFBusy(void)
{
	uint8 stat;
	
	WrNFCmd(QUERYCMD);
	do 
	{
		stat = RdNFDat();
	}while(!(stat&0x40));
	
	WrNFCmd(READCMD0);
	return stat&1;
}

void MarkBadBlk(uint32 addr)
{
	int i;
	EraseBlock(addr);
	NFChipEn();
	WrNFCmd(0x80);
	NF_ADDR(0);
	NF_ADDR(8);
	NF_ADDR(addr&0xff);
	NF_ADDR((addr>>8)&0xff);
	if(Nand_2G)
	{
		NF_ADDR(1);
	}
	for(i=0; i<64; i++)
	{
		WrNFDat(0);
	}
	WrNFCmd(0x10);
	WaitNFBusy();											//needn't check return status										//point to area a
		
	NFChipDs();
}

uint32 ReadChipId(void)
{
	uint32 id;
	
	NFChipEn();
	WrNFCmd(RdIDCMD);
	WrNFAddr(0);
	
	while(NFIsBusy());
	
	id = RdNFDat()<<8;
	id |= RdNFDat();
	NFChipDs();
	
	return id;
}


void ReadPage( uint32 sector, uint8 *buf)
{
	uint16 i;
	NFChipEn();
	WrNFCmd(0x00);
	WrNFAddr(0);
	WrNFAddr(0);
	WrNFAddr((sector)&0xff);
	WrNFAddr((sector>>8)&0xff);
	if(Nand_2G)
	{
		NF_ADDR(1);
	}
	WrNFCmd(0x30);
	WaitNFBusy();
	for(i=0; i<2048; i++)
		buf[i] = RdNFDat();
	NFChipDs();
}


uint32 Check_Page(uint32 addr,uint8 *num)
{
	uint32 i;
	U8 RdDat[2048];
	ReadPage( addr, RdDat );
	for(i=0;i<2048;i++)
	{
		if(RdDat[i]!=num[i])
			return 0;
	}
	return 1;
}

void InitNandCfg( void )
{
	rNFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);//8位总线
	//nand使能，禁止片选，初始化ECC，开锁MECC，开锁SECC，检测上升沿，禁止RnB中断，禁止非法中断，禁止上锁，禁止紧锁
	rNFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(0<<6)|(0<<5)|(1<<4)|(1<<1)|(1<<0);
	rNFSTAT = 0;
	
	Nand_Reset();
	if(ReadChipId()==0xecda)	
	{
		Nand_2G=1;
	}
	else							//ID还可能为ecf1
	{
		Nand_2G=0;
	}
	//return NF8_CheckId();
}

uint32 EraseBlock(uint32 sector)
{
	uint8 stat;		
	NFChipEn();	
	WrNFCmd(ERASECMD0);	
	WrNFAddr(sector&0xff);
	WrNFAddr((sector>>8)&0xff);
	if(Nand_2G)
	{
		WrNFAddr(1);
	}
	WrNFCmd(ERASECMD1);
	stat = WaitNFBusy();
	NFChipDs();

	return stat;
}

uint32 WritePage(U32 sector, U8 *buf)
{
	
	uint32 i,stat;
	
	NFChipEn();
	WrNFCmd(0x80);
	NF_ADDR(0);
	NF_ADDR(0);
	NF_ADDR(sector&0xff);
	NF_ADDR((sector>>8)&0xff);		//地址对齐block
	if(Nand_2G)
	{
		NF_ADDR(1);
	}
	for(i=0; i<2048;  i++)
	{
		WrNFDat(buf[i]);
	}
	WrNFCmd(0x10);
	stat = WaitNFBusy();
	NFChipDs();
	if(stat)						//program error, 返回-1
	{
		return -1;
	}
	else 
	{	
		if(Check_Page(sector,buf))	//没错误，返回1
		{
		}
		else						//program error, 返回0
		{
			//MarkBadBlk(sector);
			return 0;
		}
	}
	return 1;	
}

void WritePartBlock(uint32 sector, uint8 *buf)
{
	uint32 blockaddr;
	uint32 i;
	static uint8 data[2048*64];
	blockaddr=sector&0xffc0;
	for(i=0;i<64;i++)				//先读出整个block的值
	{
		ReadPage(blockaddr+i,data+2048*i);
	}
	EraseBlock(blockaddr);			//擦除block
	for(i=0;i<2048;i++)				//修改读到的block
	{
		data[(sector-blockaddr)*2048+i]=buf[i];
	}
	for(i=0;i<64;i++)				//将整个block存入
	{
		WritePage(blockaddr+i,data+2048*i);
	}
}

#if RestorationSetting
void RestorationNandInit()
{
	uint8 data[2048];
	uint8 i;
	if(!(rGSTATUS2 & 0x4))
	{
		return;
	}
	ReadPage(RESTORATION_NANDPAGE,data);
	for(i=0;i<5;i++)
	{
		//nightsky  Encoders[i].Last=*(fp64*)(data+i*8);
	}
	for(i=0;i<16;i++)
	{
		if(*(fp64*)(data+40+i*8)==1)
		{
			Relay_Set(i+1);
		}
		else
		{
			Relay_Clear(i+1);
		}
	}
	// nightsky  Action_Num=*(fp64*)(data+168);
}
#endif