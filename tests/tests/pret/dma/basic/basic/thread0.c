#define DEBUG 1
#define END_SIM \
  asm(".word 0x22222222");

#ifdef DEBUG 
#include <stdio.h>
#endif

#include "deadline.h"

int main (void)
{
  printf("Start of program\n");
  int mem_addr = 0x400011b0;
  DMAMV(mem_addr, mem_addr - 0x40000000);
  //STCMEM(mem_addr);
  //STCSPM(mem_addr+10);
  //  CPTRANS();
  printf("End of program\n");  
  END_SIM;
  return 0;
}

