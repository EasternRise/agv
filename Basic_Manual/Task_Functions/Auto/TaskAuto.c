#include "all.h"

void TaskAuto (void *pdata)
{
	//static struct Pointfp64 position;
	//static fp64 angle;
	
	uint8 err;
	uint8 *msg;
	pdata=pdata;
	 	 
	for(;;)
	{
		msg = OSMboxPend(AutoMbox,0,&err);
		switch (msg[0])
		{
			case 1:
					LCD_Clear();
					GPS_Clear(); 
					RouteForm=RouteStop;
					
					PID_Clear();
					GoRoute1();

    		case 2:  				
										
    				PID_Clear(); 							
	  				GoRoute2();
	  		        break;
	  		
    		case 3:
    								
    				PID_Clear(); 							
	  				GoRoute3();
	  				
    		case 4:  
    				 
    				PID_Clear();
    				GoRoute4();

    		case 5:  
 
    				PID_Clear();    				 
    				GoRoute5();

    		case 6:    			 
		 		rGPJDAT&=~0x1000;    //¹Ø·äÃùÆ÷
				LCD_Clear();
				PID_Clear();    				
				//GoRoute6();
				Route_Num=6;
				SetKeep(PointRoute[Route[6]].position,0.0);
				while(RouteFinish) 
				{
					if(((Fp64Abs(Aim_Angle-Gps_List[0].angle)<1)&&
						(Getlength(AimPoint,Gps_List[0].position)<10))||(Route_Num==7))
					{
						RouteFinish=0;
						RouteForm=RouteStop;
						LCD_SetXY(0,2);
						LCD_WriteString("route6 done");
					}
					OSTimeDly(1);
				}
    		break;
    		
    		case 7:    			 
    				 GPS_Init(PointRoute[Route[6]].position,0.0);
    				 LCD_Clear();
    				 PID_Clear();

    				 GoRoute7();
    		break;
    		
    		case 8:    			 
    				 GPS_Init(PointRoute[Route[7]+1].position,Gps_List[0].angle);
    				 LCD_Clear();
    				 PID_Clear();
    				 GoRoute8();
    				 Shootest();
    		break; 
    		
    		default:;
    		break;
    	}
    	OSTimeDly(1);
    	
    }
    
}