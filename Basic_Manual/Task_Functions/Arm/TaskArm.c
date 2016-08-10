#include "all.h" 

uint8  armaction_done=1;  

void TaskArm(void *pdata)
{ 
    uint8  err;
	uint8  *msg;
	pdata=pdata;
	
	for(;;)
	{
	msg = OSMboxPend(ArmMbox,0,&err);
	  switch(msg[0])
	  {
		  case 1:
		  		 armaction_done=0; 
		         armaction1();
		         armaction_done=1;
          break;
          
          case 2:
	      		 armaction_done=0;
	             armaction2();
	             armaction_done=1;
		  break;
          		  
	      case 3:
	      		 armaction_done=0;
	             armaction3();
	             armaction_done=1;
		  break;
		  
		  case 4: 
		  		 armaction_done=0;
		         armaction4();
		         armaction_done=1;
		  break;
		  
		  case 5:
		  	     armaction_done=0;
		         armaction5();
		         armaction_done=1;
		  break;
		  
		  case 6: 
		  		 armaction_done=0;
		         armaction6();
		         armaction_done=1;
		  break;
		  
		  case 7:
		  		 armaction_done=0;
		         armaction7();
		         armaction_done=1;
		  break;
		  
		   case 8:
		  		 armaction_done=0;
		         armaction8();
		         armaction_done=1;
		  break;
		  
		  default:;
		  break;
	  }
	  OSTimeDly(1);
	}	
}	