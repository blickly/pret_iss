#include "deadline.h"

int main()
{
  DEAD(410); 
  volatile unsigned int * buf = (unsigned int*)(0x3FFF0200);

  unsigned int i;

  //  Loop takes 28 cycles
  //  Critical section takes 7 
  for (i = ' '; ; i++ ) {
    DEAD(280);
  /* Begin critical section */
    *buf = i;
  /* Exit critical section */
  }
  
  return 0;
}
