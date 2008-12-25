/******************************************************************************************************* 
File Purpose  - Functions defined for sending and receiving messages.
*******************************************************************************************************/


#include "message.h"
#include "types.h"
#include "deadline.h"
#include "dbg.h"
//#include "tool.h"

BOOL Ismessage_Rec(void)
{
if(*New_msg == 1){
	switch(Global_msg->Type)
	{
            case TUNER_OPEN:
	      //PRINT("TUNER_OPEN Ismessage_Rec\n");
             *Status = TUNER_STATUS_OPEN;
	     *cur_index = Global_msg->Index;
	    break;

	    case SCAN_FREQ:
	      //PRINT("SCAN_FREQ Ismessage_Rec\n");
             *currentfreq = Global_msg->TunerInfo.Frequency;
	     *symbol_rate = Global_msg->TunerInfo.ScanInfo.SymbolRate; 
	     *Pol = Global_msg->TunerInfo.ScanInfo.Polarization;
             *Status = TUNER_STATUS_SCANNING;
	    break;
	    case TUNER_CLOSE:
	      //PRINT("TUNER_CLOSE Ismessage_Rec\n");
             *Status = TUNER_STATUS_NOT_OPEN_YET;
	     *cur_index = -1;
	    break;
	}

*New_msg = 0;
	return 1;
}
 else
   {
	return 0;
   }
}
BOOL message_Send(message_t *msg_p)
{
static int retry = 0;
BOOL ret_val;
if(*New_msg == 1){
	/***OLD Message not received yet. So, can't send this message through...Can put this in the message queue here***/
	if(retry < 5){
	  //PRINT("Waiting for the message to be cleared\n");
		retry += 1;
		DEAD(5000);
		DEAD(0);
        	ret_val = message_Send(msg_p);
		return ret_val;
	}
	return 0;
}
else
{
	/*****Copy the contents of msg_p to Global message struct*******/
  	*Global_msg = *msg_p;
 	*New_msg = 1;
	return 1;
}
}

