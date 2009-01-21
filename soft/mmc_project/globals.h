
volatile unsigned char *Readflag=(unsigned char *)0x3f800001;
volatile unsigned char *Writeflag=(unsigned char *)0x3f800002;
volatile unsigned char *Readbuffer=(unsigned char *)0x3f800003;
volatile unsigned char *Writebuffer=(unsigned char *)0x3f800004;
volatile unsigned char *CS=(unsigned char *)0x3f800005;
volatile unsigned char Bus_high=0xFF;
volatile unsigned char CMD0[]={0x40,0x00,0x00,0x00,0x00,0x95};
volatile unsigned char CMD1[]={0x41,0x00,0x00,0x00,0x00,0xFF};
volatile unsigned char CMD9[]={0x49,0x00,0x00,0x00,0x00,0x00};
volatile unsigned char CMD16[]={0x50,0x00,0x00,0x02,0xFF,0xFF}; //set block length to 512
volatile unsigned char CMD17[]={0x51,0x00,0x00,0x00,0x00,0xFF}; //read block 0

volatile unsigned char CMD0_response=0x01;
volatile unsigned char CMD1_response=0x00;
volatile unsigned char CMD9_response=0xFE;
volatile unsigned char CMD16_response=0x00;
volatile unsigned char CMD17_response=0xFE;

typedef enum {HIGH,LOW}Frequency ;
Frequency Frequency_MMC; 

