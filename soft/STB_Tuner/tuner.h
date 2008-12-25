#ifndef TUNER_H
#define TUNER_H


#define TUNER_0 0
#define TUNER_1 1
#define NUM_TUNER 2

TUNER_Status_t	*LastStatus =(TUNER_Status_t *)0x3F800100;
void (**NotifyCallBack_p)(TUNER_Status_t Status, ErrorCode_t Error) = (void (**)(TUNER_Status_t Status, ErrorCode_t Error))0x3F801000;


#endif
