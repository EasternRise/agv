#if DatabackSetting
extern void SendStartData(void);
extern void SendEndData(void);
extern void BackDataInit(void);
extern void DataBackTest(void);

extern void PC_WriteNumNew(double data);
extern void PC_WriteIntNew(int data);

extern void PC_WriteChar(char data);
extern void PC_WriteInt(int data);
extern void PC_WriteNum(double data);
extern void SaveMovingData(void);


extern fp64 BackData[30000][16];	//10的数据量 30000个数据点 5ms一个点 能有150s
//extern fp64 *pData[30000];
extern fp64 Data[16];
extern int32 BackDataCount;

#endif