ErrorCode_t tuner_B_Init(DeviceName_t *DeviceName);
ErrorCode_t tuner_B_Term(DeviceName_t *DeviceName);
ErrorCode_t tuner_B_Open (DeviceName_t *DeviceName);
ErrorCode_t tuner_B_Close(DeviceName_t *DeviceName);
ErrorCode_t tuner_B_SetFrequency (int Frequency, int *NewFrequency);
ErrorCode_t tuner_B_GetStatus(TUNER_Status_t *Status);
ErrorCode_t tuner_B_IsTunerLocked(BOOL *Locked);

