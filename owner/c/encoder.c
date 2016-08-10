#include "all.h"


//encoders[0]Îª×óÂÖ encoders[1]ÎªÓÒÂÖ 
struct Encoder_Stat Encoders[2]=
{
	// Last;Total;Convert;dir;Distance (mm) ;	
    {0,0,0.0835747715,-1,0.0},
 	{0,0,0.1000233387,1,0.0},
};


int32_t GET_ENCODER(int8_t i)
{
	int32_t temp;
	assert_param((i>=0)&&(i<2));
	temp = (int32_t)(((int32_t)(r_now[i]))*CPR+(int32_t)(Tim_S[i]->CNT));
	return temp;
}

void Encoder_Update(void)
{
	char i;
	int32_t now;
	int32_t step;
	
	for(i=0;i<2;i++)
	{
		now = GET_ENCODER(i);
		step=now-Encoders[i].Last;
		if(step<2000&&step>-2000)
		{
				Encoders[i].Last=now;
				//Encoders[i].Total+=step;
				Encoders[i].Total+=step*Encoders[i].dir;
				Encoders[i].Distance+=step*Encoders[i].Convert*Encoders[i].dir;	  
	    }
	}
}

void Encoder_Clear(int index)
{
	Encoders[index].Distance=0.0;
	Encoders[index].Last=GET_ENCODER(index);
	Encoders[index].Total=0;
}


void Encoder_InitXY(uint8_t i)
{

}

void Encoder_InitR(void)
{

}

void Encoder_Init(void)
{ 
	
}

void E_Init_Con(void)
{ 
	Encoders[0].Convert=3000.0/Encoders[0].Total;
    Encoders[1].Convert=3000.0/Encoders[1].Total;
}
