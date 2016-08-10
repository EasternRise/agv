#include "all.h"

void TaskRetry(void *pdata)
{
	uint8 retryflag=1;
	struct Pointfp64 position;
	pdata=pdata;
	
	for(;;)
	{
		if(Button10_Off&&(retryflag==1))
		{
			retryflag=0;
			if(Route_Num<=6)
			{
				RouteForm=RouteStop;
				OSTaskDel(AutoPrio);
	    		OSTaskDel(ArmPrio);
				Relay(RELAY_LOW(7)|RELAY_LOW(4)|RELAY_LOW(2),0,RELAY_LOW(8)|RELAY_LOW(3)|RELAY_LOW(1)|RELAY_LOW(6),0);
				Motor_CAN(1,zheng);
				Motor_CAN(3,fan);
				Motor_CAN(4,zheng);
				position.x=-1000;
				position.y=-1000;
				SetLine(100,3000,0,0.2,0.2,position,100,0);
	    		OSTimeDly(2000);
	    		Relay(0,0,RELAY_LOW(7)|RELAY_LOW(4)|RELAY_LOW(2),0);
	    		while(Getlength(Gps_List[0].position,position)>50)
	    			OSTimeDly(1);
	    		RouteForm=RouteStop;
	    		LCD_Clear();
	    	}
	    	if(Route_Num==7)
	    	{	
	    		OSTaskDel(AutoPrio);
	    		OSTaskDel(ArmPrio);
	    	//	SetLine(100,3000,0,0.2,0.2,PointRoute[Route[1]].position,100,PointRoute[Route[1]].selfangle);
	    		Relay(RELAY_LOW(7)|RELAY_LOW(4)|RELAY_LOW(2),0,RELAY_LOW(8)|RELAY_LOW(3)|RELAY_LOW(1),0);
				Motor_CAN(3,zheng);
				Motor_CAN(4,fan);
				while(Getlength(Gps_List[0].position,PointRoute[Route[1]].position)>50)
	    			OSTimeDly(1);
	    		RouteForm=RouteStop;
	    		LCD_Clear();
	    	}
	    	
	    }
	    
	    if(Button10_On&&(retryflag==0))
	    {
	    	retryflag=1;
	    	if(Route_Num<=6)
	    	{
	    		Route_Num=0;
	    		OSTaskCreate (TaskArm   ,   (void *)0,&TaskArmStk[TaskStkLengh -1],ArmPrio);
   				OSTaskCreate (TaskAuto  ,   (void *)0,&TaskAutoStk[TaskStkLengh-1],AutoPrio);
   			}
   			if(Route_Num==7)
   			{   				
   				Motor_CAN(3,fan);  				
	    		OSTaskCreate (TaskArm   ,   (void *)0,&TaskArmStk[TaskStkLengh -1],ArmPrio);
   				OSTaskCreate (TaskAuto  ,   (void *)0,&TaskAutoStk[TaskStkLengh-1],AutoPrio);
   			//	SetLine(100,3000,0,0.2,0.2,PointRoute[Route[6]].position,100,PointRoute[Route[6]].selfangle);
				while(Getlength(Gps_List[0].position,PointRoute[Route[6]].position)>50)
	    			OSTimeDly(1);
	    		RouteForm=RouteStop;
	    		LCD_Clear();
   			}
   		}
   		
   		OSTimeDly(1);
   	}
}
	    		
				