#include "deadline.h"

/*

*/

int main()
{
  DEAD(41); 
  volatile unsigned int * buf = (unsigned int*)(0x50000200);

  unsigned int i = 0;
  /*
  */

  //  Loop takes 28 cycles
  //  Critical section takes 7 
  for (i = 0; ; i++ ) {
    DEAD(28);
  /* Begin critical section */
    *buf = i;
  /* Exit critical section */
  }
  
  return 0;
}
