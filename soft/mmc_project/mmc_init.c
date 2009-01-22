/* this C file contains all the functions required by thread 0 to send commands and receive response from thread 1
* Created by Devesh Dedhia
******************************************************************************************************************/

#include "error.h"
#include "deadline.h"
#include "delay.c"
#include "CSD_parameters.h"

volatile unsigned char READregister[512];
int SEND_OP_CONDITIONS();
int WriteSPI(volatile unsigned char);
int get_response(volatile unsigned char,int);
int SEND_READ_CSD_CMD(unsigned int);
int Read_CSD();
int Retrive_CSD_parameters();
unsigned int Cal_TAAC_speed();
unsigned int Cal_Trans_speed();
unsigned int Cal_Access_time(unsigned int);
void Send_CMD(volatile unsigned char*);
int SET_BLOCK_LEN_CMD();
int SEND_READ_BLOCK_CMD(unsigned int);

//send command 0 (Go_to_idle state)
int SEND_GO_TO_IDLE_STATE(){
	printf("Writing cmd0 to writebuffer \n");
	Send_CMD(CMD0);
	if(get_response(0x01,10)==1) //check for 80 clock cycles
	{ return 1;}
	return 0;
}

//send command 1(send_OP_conditions)
int SEND_OP_CONDITIONS(){
	printf("Writing cmd1 to writebuffer \n");
	Send_CMD(CMD1);
	if(get_response(0x00,100)==1)	//the actual delay is 1 second so the value should be 50000
		{ return 1;}
	return 0;
}

int SET_BLOCK_LEN_CMD(){
	printf("Writing cmd16 to writebuffer \n");
	Send_CMD(CMD16);
	if(get_response(0x00,8)==1)
		{ return 1;}
	return 0;
}

//Read the CSD register values
int SEND_READ_CSD_CMD(unsigned int count){
	//read 16 bytes
	printf("Writing cmd9 to writebuffer \n");
	Send_CMD(CMD9);	
	volatile int i=0;
   volatile int Responseflag=1;
	for(i=0;i<count;i++){
		if(Responseflag!=0){
			if (*Readflag==0){		
					if((char)*Readbuffer==(char)0xFE){	
					printf(" DESIRED RESPONSE RECEIVED\n");
					Responseflag=0;
					}
					*Readflag=1;
			}
		}
		else{	
			if (*Readflag==0){
				int j=0;
				while(j<16){
					if(*Readflag==0){	
					(char)CSDregister[j]=(char)*Readbuffer;
					j++;	
					*Readflag=1;
					} 
				}
			}		
		}	
	}
	return 0;
}


int SEND_READ_BLOCK_CMD(unsigned int count){
	//read 16 bytes
	printf("Writing cmd17 to writebuffer \n");
	Send_CMD(CMD17);	
	volatile int i=0;
   volatile int Responseflag=1;
	for(i=0;i<count;i++){
		if(Responseflag!=0){	
			if (*Readflag==0){		
					if((char)*Readbuffer==(char)0xFE){	
					printf(" DESIRED RESPONSE RECEIVED \n");
					Responseflag=0;
					}
					*Readflag=1;
			}
		}
		else{
			if (*Readflag==0){
				volatile int j=0;
				while(j < 512){
					if(*Readflag==0){	
					(char)READregister[j]=(char)*Readbuffer;
					j++;	
					*Readflag=1;
					} 
				}
			}		
		}	
	}
	return 0;
}

int Retrive_CSD_parameters(){
	MMC_CSD_REGISTER_TAAC_MANTISSA=(CSDregister[1] & 0x78) >> 3; 
	MMC_CSD_REGISTER_TAAC_EXPONENT=(CSDregister[1] & 0x07);
	MMC_CSD_REGISTER_NSAC=CSDregister[2];
	MMC_CSD_REGISTER_R2W_FACTOR=(CSDregister[12] & 0x1C) >> 2;
	MMC_CSD_REGISTER_TRAN_SPEED_MANTISSA=(CSDregister[3] & 0x78) >> 3;
	MMC_CSD_REGISTER_TRAN_SPEED_EXPONENT=(CSDregister[3] & 0x07);
	return 0;	
}

//Calculating TAAC in unit:1us
unsigned int Cal_TAAC_speed(){	
	TAAC_speed=taac_mantissa_speeds_x10[MMC_CSD_REGISTER_TAAC_MANTISSA]*
				  taac_exponent_speeds_div10[MMC_CSD_REGISTER_TAAC_EXPONENT];
	TAAC_speed=TAAC_speed/100000;
return TAAC_speed;
}

//Calculating Trans speed in khz
unsigned int Cal_Trans_speed(){
	Trans_speed=trans_speed_mantisaa_x10[MMC_CSD_REGISTER_TRAN_SPEED_MANTISSA]*
				   trans_speed_exponent[MMC_CSD_REGISTER_TRAN_SPEED_EXPONENT];
	Trans_speed=Trans_speed/10;
	return Trans_speed;
}

//calculating access time in cycles
unsigned int Cal_Access_time(unsigned int OPERATING_FREQUENCY_MHZ){
	unsigned int NSAC;
	NSAC=MMC_CSD_REGISTER_NSAC;
	Read_Access_time=(Cal_TAAC_speed()*OPERATING_FREQUENCY_MHZ+100*NSAC);
	R2W_factor=r2w_factor_val[MMC_CSD_REGISTER_R2W_FACTOR];
	Write_Access_time = Read_Access_time*R2W_factor;
}

//Writing a value 
int WriteSPI(volatile unsigned char buff){
	delay();
	if (*Writeflag==0) {	
		*Writebuffer=(char)buff;
		*Writeflag=1;
		return 0;
	}
	else {
	return 1;	
	}
}


int get_response(volatile unsigned char buff,int count){ 
	int i;
   int Responseflag=1;
	for(i=0;i<count;i++)
	{	delay();					// to ensure data is read at MMC clock frequency
			if (*Readflag==0)			//to ensure that data is written by thread 2
			{
				if((char)*Readbuffer!=Bus_high)
				{		
					if((char)*Readbuffer==(char)buff){	
					printf(" DESIRED RESPONSE RECEIVED \n");
					Responseflag=0;
					}
					
				}
				*Readflag=1;
			}
	}
	return Responseflag;
}					

void Send_CMD(volatile unsigned char*CMD){
	volatile int i;
	int ret=-1;
	//Read the CSD register 
	i=0;	
	while(i<6){	
			ret = (int) WriteSPI(CMD[i]);	
			if(ret==0)
			{	i++;
			}
	}		
}
		
