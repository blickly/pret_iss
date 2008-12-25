/******************************************************************************************************* 
Thread Name - Tuner scan task thread.
Purpose     - Waits for message from thread0 (application thread) and then does work accordingly.
              Gets invoked every 50*10 = 500 clock cycles if there is no message.
*******************************************************************************************************/

#define END_SIM \
  asm(".word 0x22222222");
#define COUNT 10

#include <stdio.h>
#include "deadline.h"
#include "message.h"
#include "tuner_drv.c"
#include "tuner.h"
#include "Dem_X.h"
#include "Dem_Y.h"
#include "Tun_A.h"
#include "Tun_B.h"

//#include "message.c"

void Initglobalparams(void);
void CallBack_replacement(void);

int main() 
{
volatile int i =0;
message_t msg;
TUNER_InstanceDbase_t    *Inst;
ErrorCode_t error = NO_ERROR;
int  retries = 0;
BOOL Lock_st = 0;
BOOL retstatus = 0;
//void (*Notify_p)(TUNER_Status_t Status, ErrorCode_t Error);


// DEAD(100);

 Initglobalparams();


while(1)
{

	for(i=0; i<COUNT; i++)
	{
	  DEAD(50);
          DEAD(0);
	  retstatus = Ismessage_Rec();
	  if(retstatus == 1)
	    {
             retries  = 3;
	     break;
	    }
	}
	//if(i == COUNT){
	if((TUNER_STATUS_NOT_OPEN_YET == *Status) || (TUNER_STATUS_OPEN == *Status) || (TUNER_STATUS_SCANFAIL == *Status))
		continue;
	else if(TUNER_STATUS_LOCKED == *Status)
	{
	  Inst  = (TUNER_InstanceDbase_t *)GetInstance();
	  error = (Inst[*cur_index].Demod.demod_IsLocked)(&Lock_st);
	  if(error == NO_ERROR){
	    if(!Lock_st){
	      *Status = TUNER_STATUS_UNLOCKED;
	      CallBack_replacement();
	      //Notify_p = *(Inst[*cur_index].NotifyFunction);
	      //Notify_p(*Status,error);
	      //(void (*)(TUNER_Status_t , ErrorCode_t ))(*(Inst[*cur_index].NotifyFunction))(*Status,error);//called Notify but is of no use as we can not put the function inside the global address space
	      retries  = 3;
	    }
	  }
	}
	else if(TUNER_STATUS_UNLOCKED == *Status)
	{
	  *Status = TUNER_STATUS_SCANNING;
	  error = NO_ERROR;
	  retries  = 3;
	  CallBack_replacement();
	  //Notify_p = *(Inst[*cur_index].NotifyFunction);
	  //Notify_p(*Status,error);
	  //*(Inst[*cur_index].NotifyFunction)(*Status,error);//called Notify but is of no use as we can not put the function inside the global address space
	}
	else if(TUNER_STATUS_SCANNING == *Status)
	{
          retries = 3;
	  while(retries != 0)
	    {
	      retries -= 1;
	      //error = (Inst[*cur_index].Demod.demod_ScanFrequency)((int) *currentfreq,(int) *symbol_rate,*Pol,&Lock_st);
	      error = demod_X_ScanFrequency((int) *currentfreq,(int) *symbol_rate,*Pol,&Lock_st);
	      if(error == NO_ERROR){
		if(Lock_st){
		  *Status  = TUNER_STATUS_LOCKED;
		  error = NO_ERROR;//called Notify but is of no use as we can not put the function inside the global address space
		  CallBack_replacement();
		  break;
		  //Notify_p = *(Inst[*cur_index].NotifyFunction);
		  //Notify_p(*Status,error);
		  //*(Inst[*cur_index].NotifyFunction)(*Status,error);
		}
	      }
	    }
	  if((retries == 0) && (*Status != TUNER_STATUS_LOCKED)){
	    *Status  = TUNER_STATUS_SCANFAIL;
	    error = NO_ERROR;
	    //PRINT("Calling NotifyFunction\n");
	    CallBack_replacement();
	    //Notify_p = *(Inst[*cur_index].NotifyFunction);
	    //Notify_p(*Status,error);
	    
	    //*(Inst[*cur_index].NotifyFunction)(*Status,error);//called Notify but is of no use as we can not put the function inside the global address space
	  }
	}
	
}
   return 0;
}


void Initglobalparams(void)
{
*Status = (TUNER_Status_t )TUNER_STATUS_NOT_OPEN_YET ;
*currentfreq = (int ) 0;
*symbol_rate = (int ) 0;
*Pol = (TUNER_Polarization_t )TUNER_PLR_ALL;
*New_msg = (BOOL )0;
*cur_index = (int )-1;
}

void CallBack_replacement(void)
{
  *LastStatus = *Status;
}
