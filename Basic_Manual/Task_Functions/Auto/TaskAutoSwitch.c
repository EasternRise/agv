#include "all.h"

void TaskAutoSwitch(void *pdata)
{
	uint8 autoflag=1;
	uint8 i;
	uint16	count_delta=1;
	//uint8 err;
    pdata=pdata;

    
    for(;;)
    {	
    	
    	if(Button9_Off&&(autoflag==1))		//切换至手动，只执行一次
    	{
    		autoflag=0;
    		error_convert[9]+=1;//breakpoint1
    		RouteForm=RouteStop;
    		if(Route_Num<=2)
    		{
	    		OSTaskDel(AutoPrio);
	    		OSTaskDel(ArmPrio);
	    	}
	    	if((Route_Num>2)&&(Route_Num<7))
	    	{
	    		OSTaskSuspend(AutoPrio);
	    		OSTaskSuspend(ArmPrio);
	    	}
    		switch(Route_Num)
    		{
    			case 1:
    			case 2:
    				//抓叶子的一切准备
    			break;
    			case 3:
    				//Relay_Set(6);		//小臂下
    			break;
    			case 4:
    				Relay(RELAY_LOW(7),0,RELAY_LOW(8),0);		//前爪开
    			break;
    			case 5:
    				Relay(RELAY_LOW(7)|RELAY_LOW(2),0,RELAY_LOW(8)|RELAY_LOW(1),0);//前后爪开
    				
    			break;
    			case 6:
    				Relay(RELAY_LOW(7)|RELAY_LOW(2)|RELAY_LOW(4),0,RELAY_LOW(8)|RELAY_LOW(1)|RELAY_LOW(3),0);//前中后爪开
    		 	break;
    		 	default:
    		 	break;
    		 }
    	}
    	error_convert[11]=autoflag;
    	if(Button9_On&&(autoflag==0))		//切换回自动，只执行一次
    	{
    		autoflag=1;
    		error_convert[10]+=1;
    		
    		if(Route_Num<=2)
    		{
	    		Relay(0,0,RELAY_LOW(7)|RELAY_LOW(4)|RELAY_LOW(5)|RELAY_LOW(2),0);
			  	OSTimeDly(300);
			  	Motor_CAN(4,fan);
			  	Relay_Clear(6);
			  	OSTaskCreate (TaskArm   ,   (void *)0,&TaskArmStk[TaskStkLengh -1],ArmPrio);
   				OSTaskCreate (TaskAuto  ,   (void *)0,&TaskAutoStk[TaskStkLengh-1],AutoPrio);
			  	Route_Num=3;		
				OSMboxPost(AutoMbox,&Route_Num);
				OSMboxPost(ArmMbox,&Route_Num);
			}
    		if(Route_Num>2&&Route_Num<7)	
    		{
	    		if(Route_Num!=6)
    				Relay_Clear(6);
//	    		SetLine(100,1000,100,0.1,0.1,PointRoute[Point_NowNum].position,100,PointRoute[Point_NowNum].selfangle);
//	    		while((Getlength(Gps_List[0].position,PointRoute[Point_NowNum].position)>50)||(Fp64Abs(Gps_List[0].radian-PointRoute[Point_NowNum].selfangle)>PI*0.5))
	    			OSTimeDly(1);
//	    		SetPointPath(Point_NowNum,Route[Route_Num],PointRoute[Route[Route_Num]].selfangle,PointRoute[Route[Route_Num]].position);
				OSTaskResume(AutoPrio);
				OSTaskResume(ArmPrio);
			}
			
		}
		error_convert[8] = autoflag;
		
		//寻点模块
		if(Button9_Off&&(Route_Num<=6))		//手动过程中，重复执行
		{
			for(i=0;i<Count_Step;i++)
			{
	             
				if((Point_NowNum+i)>=(Point_EndNum-1))
				{
					break;
				}
	              
				if((Getlength(PointRoute[Point_NowNum+i+1].position,Gps_List[0].position))
				<=(Getlength(PointRoute[Point_NowNum+i].position,Gps_List[0].position)))
	                 
				{
					count_delta=i+1;
				}
	          
			}
		
			if((Point_NowNum+count_delta)<=(Point_EndNum)&&count_delta!=0)
			{
				if(Getlength(PointRoute[Point_NowNum+count_delta+1].position,Gps_List[0].position)
		 		 <=Getlength(PointRoute[Point_NowNum+count_delta-1].position,Gps_List[0].position))
				{
					Point_NowNum=Point_NowNum+count_delta;
				}
				else
					Point_NowNum=Point_NowNum+count_delta-1;
			}
		}
		////////////////////
		
		OSTimeDly(1);
		
	}
}