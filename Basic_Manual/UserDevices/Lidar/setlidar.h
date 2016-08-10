#ifndef SETRADAR_H
#define SETRADAR_H

#if LidarSetting
//1 Step  <  -   > 1 Laser Point
//step=44~725  < - >  Angle=-120~120

typedef struct{
	int16 start_step;
	int16 end_step;
	int16 max_distance;
	int16 min_distance;
}_LidarRange;


#define URG_SCAN_INTERVAL 19  	//Blue Urg Lidar

//#define URG_SCAN_INTERVAL 66.7 	// White Urg Lidar

#define URG_LEN 	307		   	//Length toward the center of Robot

#define LineLength 	1000  	   	//Used in Communication 

#define Lidar_Waiting 0			//Lidar_System status

#define Lidar_Route_1 1

#define Lidar_Route_2 2

#define Lidar_Route_3 3

#define Lidar_Route_4 4

#define Lidar_Route_5 5

#define Lidar_Route_6 6

#define Lidar_Route_7 7

#define Lidar_Error -1

#define Lidar_Board 0

#define Lidar_Ring 1

#define Lidar_Board_Running 2

extern void LidarBoard(void);			//Scanning Boards

extern void LidarCircle(void);			//Scanning Circles

extern void LidarInit(void);	

extern void LidarTest(void);

extern void TaskLidar(void *pdata);

extern _LidarRange LidarRanges[];	//Seperate into several ranges to prevent communication error

extern _LidarRange LidarRangesBoard[];	//Range sets

extern _LidarRange LidarRangesRing[];	//Range sets

extern char LidarBuffer[];			//UART buffer

extern int16 LidarLength[];			//Distances acquired by Lidar

extern fp64 Lidar_ResultX[],Lidar_ResultY[],Lidar_ResultS[];

extern struct Pointfp64 Lidar_Pos;

extern int8 Lidar_Stat;

extern uint8 Lidar_Debug;

extern int8 LidarRangesNum;

extern uint16 LidarBufferTop;

extern int32 Lidar_NowTime,Lidar_LastTime;

extern fp64 Lidar_SpeedX,Lidar_SpeedY;

extern int16 Lidar_Num_Result;

extern int16 FCT_SPAN;

#endif
#endif