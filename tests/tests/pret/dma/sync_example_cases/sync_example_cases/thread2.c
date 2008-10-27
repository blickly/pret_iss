#include "deadline.h"

int main()
{
  DEAD(15); 
  volatile unsigned int * buf = (unsigned int*)(0x3FFF0200);
  unsigned int i = 0;

  for (i = 0; i < 95; i++ ) {
    DEAD(30); 
  /* Begin critical section */
    WRITE(*buf);
  /* Exit critical section */
  }
  WRITE('\n');

  END_SIMULATION;
  return 0;
}
