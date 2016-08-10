#ifndef _LCD_H
#define _LCD_H
	#include "stm32f10x.h"
    
	#define LCD_LEN  160
    #define LCD_DELAY 20
	extern uint8_t LCD_Table[160];
	extern uint8_t LCD_Index;
	
	void LCD_PushByte(uint8_t data);
	void LCD_PushString(char *data);	
	void LCD_SetXY(int x,int y);
	void LCD_WriteChar(char data);
	void LCD_WriteString(char *data);
	void LCD_WriteDouble(double data,int m);
	void LCD_WriteNum(double data);
	void LCD_WriteInt(int data);
	void LCD_WriteCharXY(char data,int x,int y);
	void LCD_WriteStringXY(char *data,int x,int y);
	void LCD_WriteIntXY(int data,int x,int y);
	void LCD_WriteDoubleXY(double data,int m,int x,int y);
	void LCD_WriteNumXY(double data,int x,int y);
	void LCD_Clear( void );
	void LCD_Clear_Line(u8 C_line);
	void LCD_Clear_Forward(u8 L_count);
	
	
    void WriteString(char *data);
    void WriteDouble(double data,int m);
    void WriteInt(int data);

	void refresh(void);
    void refresh_slow(void);
    void RefreshDma(void);
	
#endif
