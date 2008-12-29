ErrorCode_t demod_X_Init(DeviceName_t *DeviceName);
ErrorCode_t  demod_X_Term(DeviceName_t *DeviceName);
ErrorCode_t demod_X_Open(DeviceName_t *DeviceName);
ErrorCode_t demod_X_Close(DeviceName_t *DeviceName);
ErrorCode_t demod_X_GetTunerInfo(TUNER_Info_t *TunerInfo_p);
ErrorCode_t demod_X_GetSignalQuality(int  *SignalQuality_p, int *Ber);
ErrorCode_t demod_X_IsLocked(BOOL *IsLocked);
ErrorCode_t demod_X_ScanFrequency(int  InitialFrequency, int   SymbolRate, TUNER_Polarization_t pol, BOOL *ScanSuccess);
