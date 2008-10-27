#include "deadline.h"

int main()
{
  volatile unsigned int * buf = (unsigned int*)(0x3FFF0200);
  unsigned int i;
  for (i = ' '; ; i++ ) {
    DEAD(26);
  /* Begin critical section */
    *buf = i;
  /* Exit critical section */
  }

  return 0;
}
