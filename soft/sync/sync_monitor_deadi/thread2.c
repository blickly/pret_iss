#include "deadline.h"

/*

*/

int main()
{
  // Deadline of 42 makes thread2 synchronized to start every loop iteration
  // in sync with thread1 (since thread 1 has an extra instuction at start of
  // main to offset stack pointer)
  DEADI("02A"); 
  // Deadline of 45 would make thread2 and thread1 perform their loads in sync
  //DEADI("02D")
  volatile unsigned int * buf = (unsigned int*)(0x3F800200);
  volatile unsigned int * fd = (unsigned int*)(0x80000600);

  unsigned int i = 0;
  /*
  */

  for (i = 0; ; i++ ) {
    DEADI("00B"); 
  /* Begin critical section */
    *fd = *buf;
  /* Exit critical section */

    // with fd DEAD(55);    
  }
  
  return 0;
}
