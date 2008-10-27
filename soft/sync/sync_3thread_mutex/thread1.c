#include "deadline.h"

/*

*/
int main()
{
  DEAD(42);
  volatile unsigned int * buf = (unsigned int*)(0x80000200);

  unsigned int i = 0;
  int arr[8];

  for (i =0; i<8; i++)
    arr[i] = 0;

  // Loop takes 29 cycles
  // Critical section takes 11
  for (i = 0; ; i++) {
    DEAD(7);
    DEAD(22);
  /* Enter critical section */
    register int tmp = *buf;
    arr[i%8] = tmp;
  /* Exit critical section */
  }

/*
  for (i = 0; i< 10; i++ )
    printf("%d ", arr[i]);
*/
  return 0;
}
