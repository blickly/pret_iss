#include "deadline.h"

/*
*/

int main()
{
  DEADI("026"); 
  volatile unsigned int * buf = (unsigned int*)(0x3F800200);

  unsigned int i = 0;
  /*
  */

  for (i = 0; ; i++ ) { //(i = 10; i > 0; i--) {
    DEADI("00B"); 
  /* Begin critical section */
    *buf = i;
  /* Exit critical section */
  }
  
  return 0;
}
