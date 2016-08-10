
//TaskI2cDisplay.c
extern void TaskI2cKeys(void *pdata);


extern void PressAnyKey(void);

//TaskPathSel.c
extern void TaskPathSel(void *pdata);

//TaskLcdDisplay.c
extern void TaskLcdDisplay(void *pdata);

//TaskLcdRefresh.c
extern void TaskLcdRefresh(void *pdata);

//lcd.c

#define LCD_LEN  160

extern uint8 LCD_Table[160];

extern uint8 LCD_Index;

#if LCDBufferSetting

extern uint8 LCDTable[20][4];

extern uint8 LCDTableSave[20][4];

extern uint8 LCDIndex;

extern uint8 LCDIndexSave;

#endif

#if HaltSetting

extern uint8 HaltFlag;

extern uint8 HaltBuffer[5];

#endif

extern void LCD_PushByte(uint8 data);

extern void LCD_PushString(char *data);	

extern void LCD_Printf(const char *fmt,...);

extern void UART0_Printf(const char *fmt,...);

extern void LCD_SetXY(int x,int y);

extern void LCD_WriteChar(char data);

extern void LCD_WriteString(char *data);

extern void LCD_WriteDouble(double data,int m);

extern void LCD_WriteNum(double data);

extern void LCD_WriteInt(int data);

extern void LCD_WriteCharXY(char data,int x,int y);

extern void LCD_WriteStringXY(char *data,int x,int y);

extern void LCD_WriteIntXY(int data,int x,int y);

extern void LCD_WriteDoubleXY(double data,int m,int x,int y);

extern void LCD_WriteNumXY(double data,int x,int y);

#if LCDBufferSetting

extern void LCD_Save( void );

extern void LCD_Load( void );

#endif

extern void LCD_Clear( void );



//display.c
//主菜单(未使用)
#define GENERAL				0

//设备菜单(未使用)
#define DEVICES				2

//参数菜单(未使用)
#define VALUES				3

//运行中
#define RUNNING				4

//应用程序自定义(显示任务不再起作用)
#define APPLICATION			5

//快捷方式程序
#define SHORTCUTAPP			6



//路径菜单
#define MENU				10

#define SUBMENU1			11
#define SUBMENU2			12
#define SUBMENU3			13
#define SUBMENU4			14
#define SUBMENU5			15
#define SUBMENU6			16
#define SUBMENU7			17
#define SUBMENU8			18
#define SHORTCUTMENU		19

extern int Display_Page;

extern int Last_Page;

extern uint8 GyroCheckEnable;

extern uint8 StartPage;

extern void DISPLAY_ShowMenu(void);

extern void DISPLAY_Message(char *message);

extern uint8 DISPLAY_Fp64Value(fp64 * address,char *name);

extern uint8 DISPLAY_INT16Value(uint16 * address,char *name);

extern void DISPLAY_ValuesManager(struct Value * address,uint16 length,uint16 nandaddress);

extern void NAND_SaveValues(struct Value * address,uint16 length,uint16 nandaddress);

extern void NAND_LoadValues(struct Value * address,uint16 length,uint16 nandaddress);


//list.c
extern int Menu_Count;	

extern int SubMenu1_Count;	

extern int SubMenu2_Count;

extern int SubMenu3_Count;

extern int SubMenu4_Count;

extern int SubMenu5_Count;

extern int SubMenu6_Count;

extern int SubMenu7_Count;

extern int SubMenu8_Count;

extern int ShortcutMenu_Count;

extern struct Path Menu[];

extern struct Path SubMenu1[];

extern struct Path SubMenu2[];

extern struct Path SubMenu3[];

extern struct Path SubMenu4[];

extern struct Path SubMenu5[];

extern struct Path SubMenu6[];

extern struct Path SubMenu7[];

extern struct Path SubMenu8[];

#if ShortcutSetting

extern struct Path ShortcutMenu[];

#endif

#define CONFIG	0

#define FOLDER	1


//system.c
extern void PATH_System( void );

extern void test ( void );


extern void Owen(void);
extern void owen_mecanum(void);
extern void owen_maxon(void);
extern void owen_route(void);

extern void Error_Show(void);

extern void Retry(void);

extern void Ringtest(void);

extern void sys_Gyro(void);

extern void sys_encoder(void);

extern void sys_gps(void);

extern void sys_mecanum(void);

extern void sys_SA_ST(void);

extern void sys_Relay(void);

extern void manualdata ( void );

extern void steertest ( void );

extern void sys_Laser(void); 

extern void sys_can(void);
//shortcut.c

#if ShortcutSetting

extern void SaveScreen(void);

extern void ShowScreen(void);

extern void SC_SA_ST(void);

extern void SC_Relay(void);

extern void SC_gps(void);

extern void SC_lightboard(void);

extern void SC_SelfDef(void);

extern void SC_Manual(void);

#endif

//test.c
extern void Sonictest(void);
extern void Shootest1(void);
extern void Shootest(void);
extern void LidarUse(void);

//WalkGround.c

extern void Info_NandSave( void );
extern void Info_NandInit( void );
extern void RouteInfo( void );
extern void Fix( void );
extern void Fix_Init( void );
extern struct Pointfp64 C_Ring1;
extern struct Pointfp64 C_Ring2;
extern struct Pointfp64 C_Ring3;
extern fp64 Board_ydis;
extern fp64 DIS_LeafBoard;
extern fp64 DIS_Launch1Board;
extern fp64 DIS_Launch2Board;
extern fp64 DIS_Launch3Board;
extern fp64 DIS_Launch1Encoder;
extern fp64 DIS_Launch2Encoder;
extern fp64 HalfGroundDis;

extern struct Pointfp64 fix_1;
extern struct Pointfp64 fix_2;
extern struct Pointfp64 fix_3;
extern struct Pointfp64 fix_4;
extern fp64 Board_err_shift;
extern fp64 Board_Shift;
extern struct Pointfp64 Ring1_Shift;
extern struct Pointfp64 Ring2_Shift;
extern struct Pointfp64 Ring3_Shift;

//Kalman.c

#define di1    6
#define di2    2
#define di3    3