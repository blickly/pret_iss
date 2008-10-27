#include "deadline.h"

int main()
{
  DEAD(480); 
  volatile unsigned int * buf = (unsigned int*)(0x3FFF0200);
  volatile unsigned int * fd = (unsigned int*)(0x80000100);

  unsigned int i = 0;

  // Loop takes 29 cycles
  // Critical section takes 10
  for (i = 0; i < 95; i++ ) {
    DEAD(280); 
  /* Begin critical section */
    *fd = *buf;
  /* Exit critical section */
  }

  *fd = '\n';
  asm(".word 0x22222222");
  return 0;
}
