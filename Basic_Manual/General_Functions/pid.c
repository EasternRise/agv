 #include "all.h"

fp64 A_EP1=0.0;
fp64 A_EP2=0.0;
fp64 D_EP1=0.0;
fp64 D_EP2=0.0;
fp64 D_EP1_IR=0.0;
fp64 D_EP2_IR=0.0;
fp64 EP1 = 0.0;
fp64 EP2 = 0.0;
fp64 AmendV = 0;
fp64 Vout[2]={0.0,0.0};

fp64 A_AmendV=0;
fp64 D_AmendV=0;
fp64 D_AmendV_IR=0;

uint8 PID_ListLength=11;

///////////////////////////////////////////////////////////////////////////////////
 struct Value Pid_Values[]=
      {    
        {(void*)&Pid_List[0].PROP,TYPE_FP64,   "AP"}, 
        {(void*)&Pid_List[0].Ti,  TYPE_FP64,   "AI"}, 
        {(void*)&Pid_List[0].Td,  TYPE_FP64,   "AD"},
        {(void*)&Pid_List[1].PROP,TYPE_FP64,   "DP"}, 
        {(void*)&Pid_List[1].Ti,  TYPE_FP64,   "DI"}, 
        {(void*)&Pid_List[1].Td,  TYPE_FP64,   "DD"},
      };
 
//warning:以下变量的初始值从nandflash中取得///////////////////////////////////////////
struct PID Pid_List[]=
{
	{20.0,100000,200.0},             //直线位移PID
	{2000.0,100000.0,40.0},          ///直线位移角度PID
	{1.5,5000000,3}, //KeepPID--2
	{6,5000000,3},    //Keep角度PID--3
	
	{0.15,	 7000,  0.214},   //Stay_Point--4
	{-120.0, 10000, 30},   //直线自转位移--5           
	
	{15,   5000000,  8},    //6 pointpath disPID   
	{0.05, 5000000,  1.8},	//7.pointpath angPID
	                        //7.goline    angPID
	{6.5,  5000000,  0},  //8.goline    disPID
	
	{5,100000.0,1.0},  //自转角度(-180)PID--9
	{0.5714,2000000,0.0714},   //Stay_Point
	{0.5714,2000000,0.0714},   //IR
	
};

//////////////////////////////////////////////////////////////////////////////////////

void PID_NandInit()
{
	int i;
	uint8 data[2048];
	ReadPage(PID_NANDPAGE,data);
	PID_ListLength=*(uint8*)(data);
	for(i=0;i<PID_ListLength;i++)
	{
		Pid_List[i].PROP=*(fp64*)(data+24*i+8);
		Pid_List[i].Ti=*(fp64*)(data+24*i+16);
		Pid_List[i].Td=*(fp64*)(data+24*i+24);
	}
}
void PID_NandSave()
{
	int i;
	uint8 data[2048];
	*(uint8*)(data)=PID_ListLength;
	for(i=0;i<PID_ListLength;i++)
	{
		*(fp64*)(data+24*i+8)=Pid_List[i].PROP;
		*(fp64*)(data+24*i+16)=Pid_List[i].Ti;
		*(fp64*)(data+24*i+24)=Pid_List[i].Td;
	}
	EraseBlock(PID_NANDPAGE);
	if(WritePage(PID_NANDPAGE,data)!=1)
	{
		Nand_Error=PID_NANDPAGE;
	}
}



/////PID清除//////
uint8 PID_Clear( void )
{
	 A_EP1=0.0;
     A_EP2=0.0;
     D_EP1=0.0;
     D_EP2=0.0;
     D_EP1_IR=0.0;
     D_EP2_IR=0.0;
	 EP1=0.0;
 	 EP2=0.0;
	 AmendV=0;
     A_AmendV=0;
     D_AmendV=0;
     D_AmendV_IR=0;
     
	 Vout[1]=0;
	 Vout[0]=0;	
	
	return TRUE;
}




//角度PID
fp64 AnglePID(fp64 AngleError,fp64 A_prop,fp64 A_ti,fp64 A_td)
{
    
    fp64  A_a0 = 0.0;
	fp64  A_a1 = 0.0;
	fp64  A_a2 = 0.0;
	fp64  A_Vd=0.0;
	fp64 a_amendv=0;	    
    
    A_a0 = (A_prop * ( 1 + 1 / A_ti + A_td ));     
	A_a1 = (-A_prop * (1 + 2 * A_td));         
	A_a2 = (A_prop * A_td );    	
    A_Vd = A_a0 *AngleError + A_a1 *A_EP1 + A_a2 *A_EP2; 
    A_AmendV = A_AmendV + A_Vd;
    A_EP2 = A_EP1;
	A_EP1 = AngleError;
	
	a_amendv=(fp64)A_AmendV;
	
	
		
	return a_amendv;
}

//距离PID
fp64 DistancePID(fp64 DistanceError,fp64 D_prop,fp64 D_ti,fp64 D_td)
{
    
    fp64  D_a0 = 0.0;
	fp64  D_a1 = 0.0;
	fp64  D_a2 = 0.0;
	fp64  D_Vd=0.0;
	fp64 d_amendv=0;	    
    
    D_a0 = ( D_prop * ( 1 + 1 / D_ti + D_td ));     
	D_a1 = ( -D_prop * (1 + 2 * D_td));         
	D_a2 = ( D_prop * D_td );    	
    D_Vd = D_a0 *DistanceError + D_a1 *D_EP1 + D_a2 *D_EP2; 
    D_AmendV = D_AmendV + D_Vd;
    D_EP2 = D_EP1;
	D_EP1 = DistanceError;
	
	d_amendv=(fp64)D_AmendV;
	
	return d_amendv;
}
