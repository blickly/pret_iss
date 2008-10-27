#include "deadline.h"

/*

*/

int main()
{
  // Deadline of 42 makes thread2 synchronized to start every loop iteration
  // in sync with thread1 (since thread 1 has an extra instuction at start of
  // main to offset stack pointer)
  DEAD(42); 
  // Deadline of 45 would make thread2 and thread1 perform their loads in sync
  //DEAD(45)
  volatile unsigned int * buf = (unsigned int*)(0x80000200);
  volatile unsigned int * fd = (unsigned int*)(0x80000600);

  unsigned int i = 0;
  /*
  */

  //76 - 10 
// with fd  DEAD(97); //51
  /* Begin critical section */
  for (i = 0; ; i++ ) { //(i = 10; i > 0; i--) {
    DEADI("00A"); 
    *fd = *buf;

    // with fd DEAD(55);    
  }
  /* Exit critical section */
  
  return 0;
}
