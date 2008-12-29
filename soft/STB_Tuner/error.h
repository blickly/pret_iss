
#ifndef ERROR_H
#define ERROR_H

typedef enum {
    NO_ERROR = 0,
    ERROR_ALLREADYINITIALISED,
    ERROR_BAD_PARAMETER,
    ERROR_UNKNOWN_DEVICE,
    ERROR_OPEN_HANDLE,
    ERROR_MSG_SEND
}ErrorCode_t;


#endif
