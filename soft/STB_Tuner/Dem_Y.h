ErrorCode_t demod_Y_Init(DeviceName_t *DeviceName);
ErrorCode_t  demod_Y_Term(DeviceName_t *DeviceName);
ErrorCode_t demod_Y_Open(DeviceName_t *DeviceName);
ErrorCode_t demod_Y_Close(DeviceName_t *DeviceName);
ErrorCode_t demod_Y_GetTunerInfo(TUNER_Info_t *TunerInfo_p);
ErrorCode_t demod_Y_GetSignalQuality(int  *SignalQuality_p, int *Ber);
ErrorCode_t demod_Y_IsLocked(BOOL *IsLocked);
ErrorCode_t demod_Y_ScanFrequency(int  InitialFrequency, int   SymbolRate, TUNER_Polarization_t pol, BOOL *ScanSuccess);
