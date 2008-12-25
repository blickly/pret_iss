#ifndef TUNER_DRV_H
#define TUNER_DRV_H

#include "error.h"
#include "types.h"

#define MAX_DEVICE_NAME 30
#define TUNER_MAX 2
typedef char DeviceName_t[MAX_DEVICE_NAME];
typedef int Tuner_Handle_t;

typedef enum {
        TUNER_DEVICE_NONE      = 0,
        TUNER_DEVICE_SATELLITE = 1
}TUNER_Dev_t;


typedef enum{
        TUNER_STATUS_INIT = 0,
	TUNER_STATUS_NOT_OPEN_YET,
        TUNER_STATUS_OPEN,
        TUNER_STATUS_CLOSE,
        TUNER_STATUS_FREE,
        TUNER_STATUS_SCANNING,
        TUNER_STATUS_LOCKED,
        TUNER_STATUS_UNLOCKED,
        TUNER_STATUS_TIMEOUT,
        TUNER_STATUS_SCANFAIL
    }TUNER_Status_t;

typedef enum{
        TUNER_PLR_ALL        = 0x07, 
        TUNER_PLR_HORIZONTAL = 1,
        TUNER_PLR_VERTICAL   = (1 << 1),
        TUNER_LNB_OFF        = (1 << 2) 
}TUNER_Polarization_t;

typedef struct {
        TUNER_Polarization_t  Polarization;
        int 		           SymbolRate;	
}TUNER_Scan_t;

typedef struct {
        TUNER_Scan_t     ScanInfo;
        int                    Frequency;
        int                    SignalQuality;
        int                    BitErrorRate;
        int                    FrequencyFound;
    } TUNER_Info_t;


typedef struct {

        /* ---------- API to demod driver ---------- */

        /* global for the driver */
        ErrorCode_t (*demod_Init)(DeviceName_t *DeviceName);
        ErrorCode_t (*demod_Term)(DeviceName_t *DeviceName);

        /* instance (was init(), now called Open() )*/
        ErrorCode_t (*demod_Open) (DeviceName_t *DeviceName);
        ErrorCode_t (*demod_Close)(DeviceName_t *DeviceName);
        ErrorCode_t (*demod_GetTunerInfo) (TUNER_Info_t *TunerInfo_p);

        ErrorCode_t (*demod_GetSignalQuality)(int  *SignalQuality_p, int *Ber);
	
        ErrorCode_t (*demod_IsLocked)(BOOL *IsLocked);

        ErrorCode_t (*demod_ScanFrequency)   (int  InitialFrequency, int   SymbolRate,TUNER_Polarization_t pol,BOOL *ScanSuccess);
}TUNER_demod_t;

typedef struct{

       
        /* ---------- API to tuner ---------- */
        ErrorCode_t (*tuner_Init)(DeviceName_t *DeviceName);
        ErrorCode_t (*tuner_Term)(DeviceName_t *DeviceName);

        ErrorCode_t (*tuner_Open) (DeviceName_t *DeviceName);
        ErrorCode_t (*tuner_Close)(DeviceName_t *DeviceName);
        
        ErrorCode_t (*tuner_SetFrequency) (int Frequency, int *NewFrequency);
        ErrorCode_t (*tuner_GetStatus)    (TUNER_Status_t *Status);
        ErrorCode_t (*tuner_IsTunerLocked)(BOOL *Locked);
} TUNER_tuner_t;

typedef struct {
        TUNER_Dev_t        Device;
        TUNER_Status_t        Status;
        char                      Name[MAX_DEVICE_NAME];
        TUNER_Info_t         TunerInfo;
        TUNER_demod_t    Demod;
        TUNER_tuner_t      Tuner;
        void (**NotifyFunction)(TUNER_Status_t Status, ErrorCode_t Error);
}TUNER_InstanceDbase_t;

/**********************/
TUNER_InstanceDbase_t Tuner_drvInst[TUNER_MAX];
/**********************/

typedef enum{
	DEMODE_TYPE_NONE =0,
	DEMODE_TYPE_X ,
       DEMODE_TYPE_Y
}DemodType_t;

typedef enum{
	TUNER_TYPE_NONE =0,
	TUNER_TYPE_A ,
       TUNER_TYPE_B
}TunerType_t;

typedef struct {
        TUNER_Dev_t       Device;
        DemodType_t       DemodType;
	 TunerType_t         TunerType;
}Tuner_initparams_t;

typedef struct {
        void (**NotifyFunction)(TUNER_Status_t Status, ErrorCode_t Error);
}Tuner_openparams_t;


/*
ErrorCode_t Tuner_Init(Tuner_initparams_t *InitParams, DeviceName_t DeviceName);
ErrorCode_t Tuner_Open(Tuner_openparams_t *Tuneropen_params, Tuner_Handle_t *Tuner_Handle, DeviceName_t DeviceName);
TUNER_InstanceDbase_t *GetInstance(void);
ErrorCode_t VALIDATE_IsUniqueName(DeviceName_t DeviceName);
ErrorCode_t DRV_Install(TUNER_InstanceDbase_t *Ins,Tuner_initparams_t *InitParams);
void Notify_sub(TUNER_Status_t Status, ErrorCode_t Error);
ErrorCode_t TUNER_SetFrequency(Tuner_Handle_t Tuner_Handle , TUNER_Info_t *TunerInfo);
*/
#endif

