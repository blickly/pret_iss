#ifndef MESSAGE_H
#define MESSAGE_H
#include "tuner_drv.h"
BOOL *New_msg = (BOOL *)0x3F800100;

typedef enum {TUNER_OPEN = 0, SCAN_FREQ, TUNER_CLOSE}msg_type;

//#define TUNER_OPEN 0
//#define SCAN_FREQ 1
//#define TUNER_CLOSE 2


typedef struct message {
    msg_type Type;
    int Index;
    TUNER_Info_t TunerInfo;
}message_t;

message_t *Global_msg = (message_t *)0x3F800200;

TUNER_Status_t *Status = (TUNER_Status_t *)0x3F800400;
int *currentfreq = (int *)0x3F800600;
int *symbol_rate = (int *)0x3F800700;
int *cur_index = (int *)0x3F800800;
TUNER_Polarization_t *Pol = (TUNER_Polarization_t *)0x3F800900;

#endif
