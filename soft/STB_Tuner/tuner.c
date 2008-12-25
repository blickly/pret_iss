/******************************************************************************************************* 
File Purpose  - Wrapper file for application to call driver APIs
*******************************************************************************************************/


#include "tuner_drv.h"
#include "error.h"
#include "tuner.h"
#include "tuner_drv.c"
#include "dbg.h"

Tuner_Handle_t Tuner_Handle[NUM_TUNER];
DeviceName_t Tuner_Name[NUM_TUNER] = {"TUNER_0","TUNER_1"};


static void SetInitParams(Tuner_initparams_t *Initparams_p);
void SetOpenParams(Tuner_openparams_t *Openparams_p);
void NotifyCallBack(TUNER_Status_t Status, ErrorCode_t Error);

ErrorCode_t Tuner_Setup (unsigned int instance)
{
  ErrorCode_t error;
  Tuner_initparams_t InitParams;
  Tuner_openparams_t OpenParams;
 
  SetInitParams(&InitParams);
  SetOpenParams(&OpenParams);

  error = Tuner_Init(&InitParams, Tuner_Name[instance]);
  if(error != NO_ERROR){
    PRINT("Error in Tuner Init\n");
    return error;
    }
  else
    PRINT("Tuner Init Success\n");

  error = Tuner_Open(&OpenParams, &Tuner_Handle[instance], Tuner_Name[instance]);
  if(error != NO_ERROR){
    PRINT("Error in Tuner Open\n");
    return error;
    }
  else
    PRINT("Tuner Open Success\n");
return error;
}

void SetInitParams(Tuner_initparams_t *Initparams_p)
{

  /****** Here goes all the init params ********/
  Initparams_p->Device = TUNER_DEVICE_SATELLITE;
  Initparams_p->TunerType = TUNER_TYPE_A;
  Initparams_p->DemodType = DEMODE_TYPE_X;

}

void SetOpenParams(Tuner_openparams_t *Openparams_p)
{
  /****** Here goes all the open params ********/
 
  *NotifyCallBack_p = NotifyCallBack;

  /*void (*NotifyCallBack_p)(TUNER_Status_t Status, ErrorCode_t Error) =
{
  
    PRINT("NotifyCallBack Called\n");
	if(Error == NO_ERROR)
	  {
		*LastStatus = Status;
                PRINT("Lock Status is %d\n",Status);
		}
		};*/
    /*  PRINT("Address of NotifyCallBack is %x",NotifyCallBack);
	PRINT("Address of NotifyCallBack_p is %x",NotifyCallBack_p);*/

    //memcpy( NotifyCallBack_p , NotifyCallBack, 2000);

  Openparams_p->NotifyFunction = NotifyCallBack_p;
}

void NotifyCallBack(TUNER_Status_t Status, ErrorCode_t Error) 
{
  PRINT("NotifyCallBack Called\n");
	if(Error == NO_ERROR)
	  {
		*LastStatus = Status;
                PRINT("Lock Status is %d\n",Status);
	  }
}
/*
void (*NotifyCallBack_p)(TUNER_Status_t Status, ErrorCode_t Error) 
{
  PRINT("NotifyCallBack Called\n");
	if(Error == NO_ERROR)
	  {
		*LastStatus = Status;
                PRINT("Lock Status is %d\n",Status);
	  }
}
*/
ErrorCode_t TunetoFrequency (unsigned int instance, int frequency, int SymRate)
{
  ErrorCode_t Error;
  TUNER_Info_t TunerInfo;

  //PRINT("tuner.c -> %d\n",frequency);
  TunerInfo.Frequency = frequency;
  TunerInfo.ScanInfo.SymbolRate = SymRate;
  TunerInfo.ScanInfo.Polarization = TUNER_PLR_ALL;
  Error = TUNER_SetFrequency(Tuner_Handle[instance] , &TunerInfo);
  if(Error != NO_ERROR)
    PRINT("Error in TUNER_SetFrequency\n");
  else
    PRINT("TUNER_SetFrequency Success\n");

  return Error;
}
