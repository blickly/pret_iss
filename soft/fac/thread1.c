/* This Thread emulates the response of the MMC card and 
	gives a response to thread 1 within a time range 
	specified by the MMC card data sheets. 

	Created by Devesh Dedhia

*/

#include"mmc_response.c"
void Wait_for_delay(unsigned int);
void Intialize_Read_block();

int main(){

	Intialize_Read_block();	
	Read_CMD();
	if(Equal_buffers(CMD0)!=0){	
		printf("Error in GO_TO_IDLE_STATE command \n\n");
		return (1);
	}
	Wait_for_delay(8);	// sends a response after 8*8 clock cycles.
	Send_Response(CMD0_response);

	Read_CMD();
	if(Equal_buffers(CMD1)!=0){	
		printf("Error in SEND_OP_CONDITIONS command \n");
		return (1);
	}
	Wait_for_delay(200);
	Send_Response(CMD1_response); // sends a response after 200*8 clock cycles
	
   Read_CMD();
	if(Equal_buffers(CMD9)!=0){	
		printf("Error in READ_CSD command \n");
		return (1);
	}
	Wait_for_delay(5);	// sends a response after 5*8 clock cycles
	Send_Response(CMD9_response);
	Wait_for_delay(8);
	Send_CSD();				// sends the CSD after 8*8 clock cycles
	
	Read_CMD();
	if(Equal_buffers(CMD16)!=0){	
		printf("Error in Set_block_len command \n");
		return (1);
	}
	Wait_for_delay(6);	
	Send_Response(CMD16_response);// sends a response after 6*8 clock cycles

	Read_CMD();
	if(Equal_buffers(CMD17)!=0){	
		printf("Error in READ_block command \n");
		return (1);
	}
	Send_Response(CMD17_response);// sends a response after 200*8 clock cycles
	Send_Read_array();

	return (0);
	
}

Wait_for_delay(unsigned int count){
	int i;		
	for(i=0;i<count;i++){
	delay();	
	}		
}

void Intialize_Read_block(){
	volatile int i;	
	for(i=0;i<512;i++){
		Read_array[i]=i;
	}

}		
