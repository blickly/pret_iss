#include"mmc_response.c"
void Wait_for_delay(unsigned int);
void Intialize_Read_block();

int main() {
//////////////////////////////////////
/*volatile loop = 1;
printf("Going to infinite loop 1\n");
while(loop == 1){


DEAD(50);
DEAD(00);
//printf("thread0\n");
}
*/
/////////////////////////////////////

	Intialize_Read_block();	
	Read_CMD();
	if(Equal_buffers(CMD0)!=0){	
		printf("Error in GO_TO_IDLE_STATE command \n\n");
		return (1);
	}
	Wait_for_delay(10);
	Send_Response(0x01);

	Read_CMD();
	if(Equal_buffers(CMD1)!=0){	
		printf("Error in SEND_OP_CONDITIONS command \n");
		return (1);
	}
	Wait_for_delay(10);
	Send_Response(0x00);
	
   Read_CMD();
	if(Equal_buffers(CMD9)!=0){	
		printf("Error in READ_CSD command \n");
		return (1);
	}
	Wait_for_delay(5);
	Send_Response(0xFE);
	Wait_for_delay(2);
	Send_CSD();
	
	Read_CMD();
	if(Equal_buffers(CMD16)!=0){	
		printf("Error in Set_block_len command \n");
		return (1);
	}
	Wait_for_delay(3);
	Send_Response(0x00);

	Read_CMD();
	if(Equal_buffers(CMD17)!=0){	
		printf("Error in READ_block command \n");
		return (1);
	}
	Send_Response(0xFE);
	Send_Read_array();

printf("Write Complete\n");

volatile char i = 1;

	while(i);
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

printf("Read_array[%d] = %d\n",i-1,Read_array[i-1]);

}		
