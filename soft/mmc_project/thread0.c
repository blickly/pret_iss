/* This Thread sends commands to MMC card(which here in is emulated by thread 1).

	Created by Devesh Dedhia

**********************************************************************************/

 
#include"mmc_init.c"
#include<stdio.h>
#include"error.h"

#define END_SIM \
asm(".word 0x22222222");

unsigned int Transaction_speed;
unsigned int Access_time;
void intialize_global_parameters();

int main(){
	intialize_global_parameters();
	ErrorCode_t Error= NO_ERROR;
	printf("GO_TO_IDLE_STATE\n");
	__asm( "LABEL:" );
	Error=SEND_GO_TO_IDLE_STATE();
	if(Error!=NO_ERROR){		
	printf("ERROR in response for GO_TO_IDLE_STATE command\n");
	return (1);
	}	
	//printf("SUCCESS for GO_TO_IDLE_STATE command\n\n");					
	
	Error=SEND_OP_CONDITIONS();
	if(Error!=NO_ERROR){		
	printf("ERROR in response for SEND_OP_CONDITIONS command\n");
	return (1);
	}	
	//printf("SUCCESS for SEND_OP_CONDITIONS command\n\n");					
	printf("MMC INITIALISED\n\n");

	Error=SEND_READ_CSD_CMD(8);
	if(Error!=NO_ERROR){		
	printf("ERROR in response for SEND_READ_CSD command\n");
	return (1);
	}	
	//printf("SUCCESS for SEND_READ_CSD command\n\n");
	
	Error=Retrive_CSD_parameters();
	if(Error!=NO_ERROR){		
	printf("CSD parameters not retrived\n");
	return (1);
	}	

	Transaction_speed=Cal_Trans_speed();
	if(Transaction_speed<20000)	// check if the Transfer speed required by the card is less than 20000 KHz
	{printf("MMC card with a transfer speeds less than 20Mhz are not supported");
	}	
	
	Frequency_MMC=HIGH;				// set the frequency to 20MHz
	
	Access_time=Cal_Access_time(20); //send the set frequency as parameter
	if(Access_time>150000){
	printf("MMC card with access times greater than 150000 clock cycles are not supported");
	}	
	
	Error=SET_BLOCK_LEN_CMD();
	if(Error!=NO_ERROR){		
	printf("ERROR in response for SET_BLOCK_LEN_CMD command\n");
	return (1);
	}	

	Error=SEND_READ_BLOCK_CMD(10);	// the actual read access time is 10/8*150000 clock cycles the value 
												//	10 is used only for simulation	
	if(Error!=NO_ERROR){		
	printf("ERROR in response for SEND_READ_BLOCK_CMD command\n");
	return (1);
	}	

	printf("A block of 512 bytes read\n");	

	END_SIM;
	return(0);
}

void intialize_global_parameters(){
	*Readflag=1;
	*Writeflag=0;
	Frequency_MMC=LOW;
}
