#include "all.h"


void TaskRoute(void *pdata)
{
    
    //uint8 err;
    pdata=pdata;
    
    //OSMboxPend(RouteMbox,0,&err);
    
    for(;;)
    {

        switch(RouteForm)
        {
            case RouteStop:
	            Acc_Limit_enable=0;
	            #ifdef WheelSelectSetting
	            	Speed_Motor(0,0,0);
	            #else
	            	//Speed_Motor(0,0,0,0);
	            	Speed_F=0;
	            	Speed_FR=0;
	            	Speed_BL=0; 
	            	Speed_BR=0;
	            #endif
	            Acc_Limit_enable=1;
	            
            	break;
            
            case PointLine:
	            GoPointPath();
	            break;
            
            case RouteLine:
	            GoLine();
	            break;
            
            case RouteKeep:
	            GoKeep();
	            break;
            
            case RouteFree:
            
            	break;
         }
         OSTimeDly(1);
     }
        
}