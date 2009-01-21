/* this C file contains all the functions required by thread 0
Created by Devesh Dedhia
******************************************************************************************************************/

#include"delay.c"
volatile unsigned char Read_array[512];
volatile unsigned char Response_Readbuffer[6];
int Equal_buffers(volatile unsigned char*);
void Read_CMD(void);
void Send_Response(unsigned char);
volatile unsigned char CSD[]={0x00,0x26,0x00,0x2A,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x40,0x00,0x00};


/* Return 0 if the value of the 6 byte command received from thread 1 is 
	same as the required command*/ 
int Equal_buffers(volatile unsigned char *CMD){
	int i;		
	for (i=0;i<6;i++){
	if(Response_Readbuffer[i]!=CMD[i]){
		return 1;
		}
	}
	return 0;
}	

/* Read the 6 byte command sent by the thread 0*/
void Read_CMD(){
	int i=0;
	while(i<6){
	if(*Writeflag==1){
		delay();
		Response_Readbuffer[i]=*Writebuffer;
		i++;
		*Writeflag=0;
		}
	}
}

void Send_Response(volatile unsigned char response){
	if(*Readflag==1){	
		delay();
		(char)*Readbuffer=(char)response;
		*Readflag=0;		
		}
}


/* Send 16 bytes of CSD register*/
void Send_CSD(){
	volatile int i=0;
	while(i<16){			
		if(*Readflag==1){			
		(char)*Readbuffer=(char)CSD[i];
		i++;	
		*Readflag=0;	
		}
	}
}

/* Send the readblock */
void Send_Read_array(){
	volatile int i=0;
	while(i<512){			
		if(*Readflag==1){			
		(char)*Readbuffer=(char)Read_array[i];
		i++;	
		*Readflag=0;	
		}
	}
}
