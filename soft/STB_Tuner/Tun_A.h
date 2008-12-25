ErrorCode_t tuner_A_Init(DeviceName_t *DeviceName);
ErrorCode_t tuner_A_Term(DeviceName_t *DeviceName);
ErrorCode_t tuner_A_Open (DeviceName_t *DeviceName);
ErrorCode_t tuner_A_Close(DeviceName_t *DeviceName);
ErrorCode_t tuner_A_SetFrequency (int Frequency, int *NewFrequency);
ErrorCode_t tuner_A_GetStatus(TUNER_Status_t *Status);
ErrorCode_t tuner_A_IsTunerLocked(BOOL *Locked);
