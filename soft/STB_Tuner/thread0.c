/******************************************************************************************************* 
Thread Name - Application thread. 
Purpose     - Initialise the System (tuner here) and get the handle(s). Then call the APIs(TUNER here) as 
              per requirement.
*******************************************************************************************************/

#define DEBUG 1
#define END_SIM \
  asm(".word 0x22222222");

#ifdef DEBUG 
#include <stdio.h>
#endif

#include "error.h"
#include "dbg.h"
#include "tuner.c"

void PrintLockStatus(void);

volatile unsigned int * _fd = ( unsigned int *)( 0x80000600 );
void file_write( char * buf, int len )
{
        int i = 0;
        for( i = 0; i < len; i++ )
        {
                *_fd = *( buf + i );
        }
}

int main (void)
{
  ErrorCode_t Error = NO_ERROR; 
  volatile int i=0;
  volatile int symrate[]={27250,27500,26000,20000,28000,20400,23100,25600,24500,26300};
  volatile int freq[] = {10800,11020,11600,11360,12729,11172,12634,12500,12200,15400};
  Error = Tuner_Setup(TUNER_0);
  if(Error != NO_ERROR)
    PRINT("Error in Tuner Setup\n");
  else
    PRINT("Tuner Setup Success\n");

  for(i = 0; i < 10;i++)
    {
      //PRINT("thread0 -> %d %d %d\n",i,*(freq+i),symrate[i]);
      Error = TunetoFrequency(TUNER_0, freq[i], symrate[i]);
      if(Error != NO_ERROR)
	PRINT("Error in TunetoFrequency\n");
      else
	PRINT("TunetoFrequency Success\n");
      DEAD(10000);
      DEAD(0);
      PrintLockStatus();
    }
  
  END_SIM;
  return (0);
}
void PrintLockStatus(void)
{
  switch(*LastStatus)
    {
    case TUNER_STATUS_INIT :
      PRINT("TUNER_STATUS_INIT\n");
      break;
    case TUNER_STATUS_NOT_OPEN_YET :
      PRINT("TUNER_STATUS_NOT_OPEN_YET\n");
      break;
    case TUNER_STATUS_OPEN :
      PRINT("TUNER_STATUS_OPEN\n");
      break;
    case TUNER_STATUS_CLOSE :
      PRINT("TUNER_STATUS_CLOSE\n");
      break;
    case TUNER_STATUS_FREE :
      PRINT("TUNER_STATUS_FREE\n");
      break;
    case TUNER_STATUS_SCANNING :
      PRINT("TUNER_STATUS_SCANNING\n");
      break;
    case TUNER_STATUS_LOCKED :
      PRINT("TUNER_STATUS_LOCKED\n");
      break;
    case TUNER_STATUS_UNLOCKED :
      PRINT("TUNER_STATUS_UNLOCKED\n");
      break;
    case TUNER_STATUS_TIMEOUT :
      PRINT("TUNER_STATUS_TIMEOUT\n");
      break;
    case TUNER_STATUS_SCANFAIL :
      PRINT("TUNER_STATUS_SCANFAIL\n");
      break;
    }
}
