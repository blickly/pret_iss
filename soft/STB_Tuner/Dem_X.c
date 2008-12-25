/******************************************************************************************************* 
File Purpose  - Driver function calls to communicate with the hardware directly using I2C.
*******************************************************************************************************/


#include "error.h"
#include "tuner_drv.h"
#include "deadline.h"

ErrorCode_t demod_X_Init(DeviceName_t *DeviceName)
{

return NO_ERROR;

}
ErrorCode_t  demod_X_Term(DeviceName_t *DeviceName)
{

return NO_ERROR;

}


ErrorCode_t demod_X_Open (DeviceName_t *DeviceName)
{

return NO_ERROR;

}

ErrorCode_t demod_X_Close(DeviceName_t *DeviceName)
{

return NO_ERROR;

}

ErrorCode_t demod_X_GetTunerInfo(TUNER_Info_t *TunerInfo_p)
{

return NO_ERROR;

}

ErrorCode_t demod_X_GetSignalQuality(int  *SignalQuality_p, int *Ber)
{

return NO_ERROR;

}
	
ErrorCode_t demod_X_IsLocked(BOOL *IsLocked)
{
  //*IsLocked = 0;
return NO_ERROR;

}

ErrorCode_t demod_X_ScanFrequency (int  InitialFrequency, int   SymbolRate, TUNER_Polarization_t pol, BOOL *ScanSuccess)
{
  //PRINT("%d\n",InitialFrequency);
  
  switch(InitialFrequency)
    {
    case 10800:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 1;
      break;
    case 11020:
      //PRINT("%d\n",InitialFrequency);
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 0;
      break;
    case 11600:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 1;
      break;
    case 11360:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 0;
      break;
    case 12729:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 1;
      break;
    case 11172:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 0;
      break;
    case 12634:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 1;
      break;
    case 12500:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 0;
      break;
    case 12200:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 1;
      break;
    case 15400:
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 0;
      break;
    default:
      //PRINT("%d\n",InitialFrequency);
      DEAD(3000);
      DEAD(0);
      *ScanSuccess = 0;
      break;
    }
return NO_ERROR;

}

