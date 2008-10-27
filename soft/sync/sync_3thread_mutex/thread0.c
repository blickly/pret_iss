#include "deadline.h"

/*

*/

int main()
{
  DEAD(43); 
  volatile unsigned int * buf = (unsigned int*)(0x80000200);

  unsigned int i = 0;
  /*
  */

  //  Loop takes 29 cycles
  //  Critical section takes 7
  for (i = 0; ; i++ ) {
    DEAD(29); 
  /* Begin critical section */
    *buf = i;
  /* Exit critical section */
  }
  
  return 0;
}
