#include "deadline.h"

int main()
{
  
  volatile unsigned int * buf = (unsigned int*)(0x3FFF0200);
  unsigned int i;
  for (i = ' '; ; i++ ) {
    DEAD(30);
  /* Begin critical section */
    *buf = i;
  /* Exit critical section */
  }

  WAIT_FOR_END_SIMULATION;
}
