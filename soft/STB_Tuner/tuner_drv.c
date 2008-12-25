/******************************************************************************************************* 
File Purpose  - Tuner Driver APIs are all defined in this file. These APIs get called from the tuner.c wrapper. 
*******************************************************************************************************/


#include "tuner_drv.h"
#include "error.h"
#include "message.h"
#include "dbg.h"

#include "Dem_X.c"
#include "Dem_Y.c"
#include "Tun_A.c"
#include "Tun_B.c"
#include "message.c"

void Notify_sub(TUNER_Status_t Status, ErrorCode_t Error);
TUNER_InstanceDbase_t *GetInstance(void);
ErrorCode_t VALIDATE_IsUniqueName(DeviceName_t DeviceName);
ErrorCode_t Validate_InitParams(Tuner_initparams_t *Params);
ErrorCode_t DRV_Install(TUNER_InstanceDbase_t *Ins,Tuner_initparams_t *InitParams);

static TUNER_Initalized = FALSE;


ErrorCode_t Tuner_Init(Tuner_initparams_t *InitParams, DeviceName_t DeviceName)
{
  ErrorCode_t error;
  TUNER_InstanceDbase_t    *Inst;
  int Index =0, i= 0;
  
  error = Validate_InitParams(InitParams);
  if(error != NO_ERROR){
    PRINT("Error in Tuner Init Params\n");
    return error;
    }
  else
    PRINT("Tuner Init Params OK\n");

  Inst =(TUNER_InstanceDbase_t *) GetInstance();
  if (FALSE == TUNER_Initalized){
  	Index = 0;
  	for (i=0; i < TUNER_MAX ;i++){
        Inst[Index].Device = TUNER_DEVICE_NONE;
        Inst[Index].Status  = TUNER_STATUS_FREE; 
        memset((char *)(Inst[Index].Name), '\0', MAX_DEVICE_NAME);

        memset((TUNER_Info_t *)(&Inst[Index].TunerInfo), 0, sizeof(TUNER_Info_t));
        memset((TUNER_demod_t *)(&Inst[Index].Demod),0,sizeof(TUNER_demod_t));
        memset((TUNER_tuner_t *)(&Inst[Index].Tuner),0, sizeof(TUNER_tuner_t));
  		}
	TUNER_Initalized = TRUE;
  	}
  else{
  	PRINT("Allready Initialized\n");
	error = ERROR_ALLREADYINITIALISED;
	return error;
	}

  error = VALIDATE_IsUniqueName(DeviceName);
  if(error != NO_ERROR){
    PRINT("DeviceName already used\n");
    return error;
    }
  else
    PRINT("DeviceName OK\n");

error = DRV_Install(Inst,InitParams);
  if(error != NO_ERROR){
    PRINT(" Error in DRV_Install\n");
    return error;
    }
  else
    PRINT("DRV_Install OK\n");

Inst->Device = InitParams->Device;
Inst->Status = TUNER_STATUS_INIT;

strcpy(Inst->Name, DeviceName);

return error;  
}

  
ErrorCode_t Tuner_Open(Tuner_openparams_t *Tuneropen_params, Tuner_Handle_t *Tuner_Handle, DeviceName_t DeviceName)
{
  ErrorCode_t error;
  TUNER_InstanceDbase_t    *Inst;
  int Index =0, i= 0;
  message_t msg;


  Inst  = (TUNER_InstanceDbase_t *)GetInstance();
  Index = 0;

    while(strcmp(Inst[Index].Name, DeviceName) != 0)
    {
        Index++;
        if (Index >= TUNER_MAX)
        {
            error = ERROR_UNKNOWN_DEVICE;
            return error;
        }
    }    

    if (Inst[Index].Status == TUNER_STATUS_OPEN)    /* handle already opened */
    {
       error = ERROR_OPEN_HANDLE;
       return error;
    }

    error = (Inst[Index].Demod.demod_Open)((DeviceName_t *)DeviceName);
  if(error != NO_ERROR){
    PRINT(" Error in demod_Open\n");
    return error;
    }
  else
    PRINT("demod_Open OK\n");

  error = (Inst[Index].Tuner.tuner_Open)((DeviceName_t *)DeviceName);
  if(error != NO_ERROR){
    PRINT(" Error in tuner_Open\n");
    return error;
    }
  else
    PRINT("tuner_Open OK\n");

*Tuner_Handle = Index;


  if(Tuneropen_params->NotifyFunction != NULL)
  {
    //PRINT("Set Notify Function properly\n");
	Inst[Index].NotifyFunction = Tuneropen_params->NotifyFunction;
  }
  else
   *(Inst[Index].NotifyFunction) = (void (*)(TUNER_Status_t , ErrorCode_t ))(Notify_sub);

  //PRINT("Address of Notify_sub is %x",Notify_sub);


/*****************IF ALL WENT WELL THEN START THE Thread1 form here which is same as the tuner Scan Task**********/

msg.Type = TUNER_OPEN;
msg.Index = Index;
if(message_Send(&msg))
  {
    //PRINT(" Message Sent\n");
  }
 
return error;
}



TUNER_InstanceDbase_t *GetInstance(void){
	return Tuner_drvInst;
}

ErrorCode_t Validate_InitParams(Tuner_initparams_t *Params){

/**************???*****************/
return NO_ERROR;
}

ErrorCode_t VALIDATE_IsUniqueName(DeviceName_t DeviceName)
{
    ErrorCode_t error = NO_ERROR;
    TUNER_InstanceDbase_t    *Inst;
    unsigned int index;

    Inst = GetInstance();

    for(index = 0; index < TUNER_MAX; index++)
    {
        if( strcmp(Inst[index].Name, DeviceName) == 0 )
        {
	    PRINT("Device Name In use = %s\n",DeviceName);            
            return(ERROR_BAD_PARAMETER);
        }
    }
    return(error);
}

ErrorCode_t DRV_Install(TUNER_InstanceDbase_t *Ins,Tuner_initparams_t *InitParams)
{
    ErrorCode_t error = NO_ERROR;
	
	switch (InitParams->DemodType)
	{
	case DEMODE_TYPE_X :
		Ins->Demod.demod_Init = demod_X_Init;
		Ins->Demod.demod_Open = demod_X_Open;
		Ins->Demod.demod_GetTunerInfo = demod_X_GetTunerInfo;
		Ins->Demod.demod_GetSignalQuality = demod_X_GetSignalQuality;
		Ins->Demod.demod_IsLocked = demod_X_IsLocked;
		Ins->Demod.demod_ScanFrequency = demod_X_ScanFrequency;
		Ins->Demod.demod_Close = demod_X_Close;
		Ins->Demod.demod_Term = demod_X_Term;
	break;
	case DEMODE_TYPE_Y :
		Ins->Demod.demod_Init = demod_Y_Init;
		Ins->Demod.demod_Open = demod_Y_Open;
		Ins->Demod.demod_GetTunerInfo = demod_Y_GetTunerInfo;
		Ins->Demod.demod_GetSignalQuality = demod_Y_GetSignalQuality;
		Ins->Demod.demod_IsLocked = demod_Y_IsLocked;
		Ins->Demod.demod_ScanFrequency = demod_Y_ScanFrequency;
		Ins->Demod.demod_Close = demod_Y_Close;
		Ins->Demod.demod_Term = demod_Y_Term;
	break;
	default :
		error = ERROR_BAD_PARAMETER;
	break;
	}
		switch (InitParams->TunerType)
	{
	case TUNER_TYPE_A :
		Ins->Tuner.tuner_Init = tuner_A_Init;
		Ins->Tuner.tuner_Open = tuner_A_Open;
		Ins->Tuner.tuner_SetFrequency =tuner_A_SetFrequency;
		Ins->Tuner.tuner_GetStatus = tuner_A_GetStatus;
		Ins->Tuner.tuner_IsTunerLocked = tuner_A_IsTunerLocked;
		Ins->Tuner.tuner_Close =tuner_A_Close;
		Ins->Tuner.tuner_Term = tuner_A_Term;
	break;
	case TUNER_TYPE_B :
		Ins->Tuner.tuner_Init = tuner_B_Init;
		Ins->Tuner.tuner_Open = tuner_B_Open;
		Ins->Tuner.tuner_SetFrequency =tuner_B_SetFrequency;
		Ins->Tuner.tuner_GetStatus = tuner_B_GetStatus;
		Ins->Tuner.tuner_IsTunerLocked = tuner_B_IsTunerLocked;
		Ins->Tuner.tuner_Close =tuner_B_Close;
		Ins->Tuner.tuner_Term = tuner_B_Term;
	break;
	default :
		error = ERROR_BAD_PARAMETER;
	break;
	}

return error;
}

void Notify_sub(TUNER_Status_t Status, ErrorCode_t Error)
{
  PRINT("Inside Notify_sub\n");
}

ErrorCode_t TUNER_SetFrequency(Tuner_Handle_t Tuner_Handle , TUNER_Info_t *TunerInfo)
{
  message_t msg;
  ErrorCode_t error = NO_ERROR;
  msg.Type = SCAN_FREQ;
  msg.Index = Tuner_Handle;
  msg.TunerInfo = *TunerInfo;
    
    if(message_Send(&msg))
    {
      //PRINT(" Message Sent\n");
    }
    else
    {
        PRINT(" Message Sent Failed in TunerSetFrequency\n");
	error = ERROR_MSG_SEND;
    }

  return error;
}
