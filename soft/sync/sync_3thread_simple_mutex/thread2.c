#include "deadline.h"

/*

*/

int main()
{
  DEAD(48); 
  volatile unsigned int * buf = (unsigned int*)(0x50000200);
  volatile unsigned int * fd = (unsigned int*)(0x80000600);

  unsigned int i = 0;

  // Loop takes 29 cycles
  // Critical sectino takes 10
  for (i = 0; ; i++ ) {
    DEAD(28); 
  /* Begin critical section */
    *fd = *buf;
  /* Exit critical section */

  }
  
  return 0;
}
