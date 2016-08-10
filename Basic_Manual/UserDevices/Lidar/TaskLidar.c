#include "all.h"

#if LidarSetting

extern void LidarRing(void);
extern void LidarBoard_NoRANSAC(void);
extern uint8 One_Board_Flag;     

void TaskLidar(void *pdata)
{
 	uint8 i;
    pdata=pdata; 
    
    for(;;)
    {
    	switch(Lidar_Stat)
    	{
			case Lidar_Route_1:
   				LidarRangesNum=1;
   				for (i=0;i<LidarRangesNum;i++)
   					LidarRanges[i]=LidarRangesBoard[i];
				//One_Board_Flag=1;
				//LidarBoard();
				LidarBoard_NoRANSAC();
				break;
			case Lidar_Route_2:
				break; 
			case Lidar_Route_3:
   				for (i=0;i<1;i++)
   					LidarRanges[i]=LidarRangesRing[i];
				LidarRangesNum=1; 
//				LidarRing();
				LidarCircle();
				break;				
			case Lidar_Route_4:
				break;
			case Lidar_Route_5:				 
				break;
	   	}
    	OSTimeDly(1);
    }
}
#endif