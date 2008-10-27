#include "deadline.h"

int main()
{
  DEAD(13); 
  volatile unsigned int * buf = (unsigned int*)(0x3FFF0200);
  unsigned int i = 0;

  for (i = 0; i < 95; i++ ) {
    DEAD(26); 
  /* Begin critical section */
    WRITEFILE(*buf);
  /* Exit critical section */
  }

  END_SIMULATION;
  return 0;
}
