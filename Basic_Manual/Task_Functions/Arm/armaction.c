#include"all.h"


void armaction1(void)		
{
	Relay(0,0,0xffffffff,0xffffffff);
	OSTimeDly(1);
	//爪子张开
	Relay(RELAY_LOW(7)|RELAY_LOW(4)|RELAY_LOW(5)|RELAY_LOW(2),0,0,0);
	//while(PointRoute[Point_NowNum].selfangle>-0.53)
		OSTimeDly(1);
	Motor_CAN(3,zheng);
//	while(PointRoute[Point_NowNum].selfangle>-0.6)
		OSTimeDly(1);
	Relay_Set(6);	
	Motor_CAN(4,fan);
	while(ST_Read(2)==1)
		OSTimeDly(1);
	Motor_CAN(3,STOP);
	LCD_SetXY(0,1);
	LCD_WriteString("arm1 done");
	
}
 

void armaction2(void)
{
	struct Pointfp64 point;
	point.x=PointRoute[Route[2]+1].position.x+70;
	point.y=PointRoute[Route[2]+1].position.y;
	
  	Motor_CAN(3,STOP);
  	
  	while(Fp64Abs(Gps_List[0].position.y-PointRoute[Route[2]-10].position.y)>5)
  		OSTimeDly(1);
  	//架子下降
  	
  	Motor_CAN(4,zheng);
  	OSTimeDly(200);
  	while(SA_Read(1)==1)
  		OSTimeDly(1);
  	GPS_Init(point,Gps_List[0].angle);

	Relay(0,0,RELAY_LOW(7)|RELAY_LOW(4)|RELAY_LOW(5)|RELAY_LOW(2),0);
  	OSTimeDly(200);
  	Motor_CAN(4,fan);
  	Relay_Clear(6);

  	Route_Num++;		
	OSMboxPost(AutoMbox,&Route_Num);
	OSMboxPost(ArmMbox,&Route_Num);

	LCD_SetXY(0,1);
	LCD_WriteString("arm2 done");
	
}
 

void armaction3(void)		
{
	OSTimeDly(800);
	Motor_CAN(1,fan);
	while(Point_NowNum<Point_EndNum-PRE_Count1)
		OSTimeDly(1);
	Relay(RELAY_LOW(7)|RELAY_LOW(8),0,0,0);
	
	LCD_SetXY(0,1);
	LCD_WriteString("arm3 done");
}
 

void armaction4(void)		
{	
	OSTimeDly(50);
	while(Point_NowNum<Point_EndNum-PRE_Count2)
		OSTimeDly(1);
	Relay(RELAY_LOW(2)|RELAY_LOW(1),0,0,0);
	
	LCD_SetXY(0,1);
	LCD_WriteString("arm4 done");   
}


void armaction5(void)		
{
	OSTimeDly(100);
	while(Point_NowNum<Point_EndNum-PRE_Count3)
		OSTimeDly(1);
	Relay(RELAY_LOW(4)|RELAY_LOW(3),0,0,0);
	
	LCD_SetXY(0,1);
	LCD_WriteString("arm5 done");
}

void armaction6(void)		
{
	Relay_Set(6);
	OSTimeDly(200);
	Motor_CAN(1,zheng);
	Motor_CAN(3,fan);	
	OSTimeDly(500);
	Motor_CAN(4,zheng);	
	
    LCD_SetXY(0,1);
	LCD_WriteString("arm6 done");
	  
}

void armaction7(void)		
{
   
    Relay_Set(5);
	OSTimeDly(50);
	Relay_Clear(6);
	Motor_CAN(4,fan);	
    OSTimeDly(2000);
    Motor_CAN(4,zheng);
    
   
    
    LCD_SetXY(0,1);
	LCD_WriteString("arm7 done");
	  
}

void armaction8(void)		
{
	
	uint16 count;	
    Relay_Set(1);
    Motor_CAN(2,zheng);
    for(;;)						
    {
    	count=0;
	    while(Button6_Left==0)
	    	OSTimeDly(1);
	    SA2_en=0;
	   	OSTimeDly(500);
	   	for(;;)					////扳机打开后500ms玻璃升降器动，800ms后恢复接近开关控制
	   	{
	   		Relay_Set(1);				
			Motor_CAN(2,zheng);
			count++;
			if(count>=800)
				break;
			OSTimeDly(1);
		}
		SA2_en=1;
		OSTimeDly(2000);
   	}	
   		
    LCD_SetXY(0,1);
	LCD_WriteString("arm8 done");
	  
}

  
