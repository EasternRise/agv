#include "all.h"
/*重要参数值,比赛程序一定要事先看一遍这里的变量！！！*/
struct Pointfp64 C_Ring1={-2142,-5829};
struct Pointfp64 C_Ring2={-2486,-4927};
struct Pointfp64 C_Ring3={-626,-3984.84};//{-650,-3984.84};
fp64 Board_ydis=7334.84;
fp64 DIS_LeafBoard=1427;
fp64 HalfGroundDis=7685;

/***************************************************/
void Info_NandSave()
{
		#if WalkGroundEnable
	uint8 data[2048];
	*(fp64*)(data)=Board_Shift;
	*(fp64*)(data+8)=Ring1_Shift.x;
	*(fp64*)(data+16)=Ring1_Shift.y;
	*(fp64*)(data+24)=Ring2_Shift.x;
	*(fp64*)(data+32)=Ring2_Shift.y;
	*(fp64*)(data+40)=Ring3_Shift.x;
	*(fp64*)(data+48)=Ring3_Shift.y;
	*(fp64*)(data+56)=DIS_Launch1Board;
	*(fp64*)(data+64)=DIS_Launch2Board;
	*(fp64*)(data+72)=DIS_Launch3Board;
	*(fp64*)(data+80)=DIS_Launch1Encoder;
	*(fp64*)(data+88)=DIS_Launch2Encoder;
	*(fp64*)(data+96)=Board_err_shift;
	EraseBlock(ROUTEINFO_NANDPAGE);
	if(WritePage(ROUTEINFO_NANDPAGE,data)!=1)
	{
		Nand_Error=ROUTEINFO_NANDPAGE;
	}
		#endif
}

void Info_NandInit()
{
		#if WalkGroundEnable
	uint8 data[2048];
	ReadPage(ROUTEINFO_NANDPAGE,data);
	Board_Shift=*(fp64*)(data);
	Ring1_Shift.x=*(fp64*)(data+8);
	Ring1_Shift.y=*(fp64*)(data+16);
	Ring2_Shift.x=*(fp64*)(data+24);
	Ring2_Shift.y=*(fp64*)(data+32);
	Ring3_Shift.x=*(fp64*)(data+40);
	Ring3_Shift.y=*(fp64*)(data+48);
	DIS_Launch1Board=*(fp64*)(data+56);
	DIS_Launch2Board=*(fp64*)(data+64);
	DIS_Launch3Board=*(fp64*)(data+72);
	DIS_Launch1Encoder=*(fp64*)(data+80);
	DIS_Launch2Encoder=*(fp64*)(data+88);
	Board_err_shift=*(fp64*)(data+96);
		#endif

}

void RouteInfo()
{
		#if WalkGroundEnable
	struct Value Values[]=
	{
		{(void*)&Board_Shift,		TYPE_FP64,	"shift1"},
		{(void*)&Ring1_Shift.x,		TYPE_FP64,	"shift2x"},
		{(void*)&Ring1_Shift.y,		TYPE_FP64,	"shift2y"},
		{(void*)&Ring2_Shift.x,		TYPE_FP64,	"shift3x"},
		{(void*)&Ring2_Shift.y,		TYPE_FP64,	"shift3y"},
		{(void*)&Ring3_Shift.x,		TYPE_FP64,	"shift4x"},
		{(void*)&Ring3_Shift.y,		TYPE_FP64,	"shift4y"},
		{(void*)&DIS_Launch1Board,	TYPE_FP64,	"Launch1"},
		{(void*)&DIS_Launch2Board,	TYPE_FP64,	"Launch2"},
		{(void*)&DIS_Launch3Board,	TYPE_FP64,	"Launch3"},
		{(void*)&DIS_Launch1Encoder,		TYPE_FP64,	"EN1"},
		{(void*)&DIS_Launch2Encoder,		TYPE_FP64,	"EN2"},
		{(void*)&Board_err_shift,		TYPE_FP64,	"errsft"},
	};
	LCD_Clear();
	DISPLAY_ValuesManager(Values,13,ROUTEINFO_NANDPAGE);
	Info_NandSave();
	
	LCD_WriteString("Save Done!");
		#else
	LCD_SetXY(0,0);
	LCD_WriteString("no enable");
		#endif
}

void Fix( void )
{
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteNum(fix_1.x);
	LCD_WriteString("*");
	LCD_WriteNum(fix_1.y);
	LCD_SetXY(0,1);
	LCD_WriteNum(fix_2.x);
	LCD_WriteString("*");
	LCD_WriteNum(fix_2.y);
	LCD_SetXY(0,2);
	LCD_WriteNum(fix_3.x);
	LCD_WriteString("*");
	LCD_WriteNum(fix_3.y);
	LCD_SetXY(0,3);
	LCD_WriteNum(fix_4.x);
	LCD_WriteString("*");
	LCD_WriteNum(fix_4.y);
	
}

void Fix_Init( void )
{
	Board_ydis+=fix_1.y;
	C_Ring1.x+=fix_1.x+fix_2.x;
	C_Ring1.y+=fix_1.y+fix_2.y;
	C_Ring2.x+=fix_1.x+fix_2.x+fix_3.x;
	C_Ring2.y+=fix_1.y+fix_2.y+fix_3.x;
	C_Ring3.x+=fix_1.x+fix_2.x+fix_3.x+fix_4.x;
	C_Ring3.y+=fix_1.y+fix_2.y+fix_3.y+fix_4.y;
}