#include "deadline.h"

/*

*/
int main()
{
  DEADI("029"); // 0x29 = 41
  volatile unsigned int * buf = (unsigned int*)(0x3F800200);

  unsigned int i = 0;
  int arr[8];

  for (i =0; i<8; i++)
    arr[i] = 0;

  /* T0 loop takes 5 cycles to get to sync point */
  
  /* Enter critical section */
  for (i = 0; ; i++) { //(i = 0; i<10 ; i++) {
    DEADI("00B"); 
    register int tmp = *buf;
    arr[i%8] = tmp;
  }
  /* Exit critical section */

/*
  for (i = 0; i< 10; i++ )
    printf("%d ", arr[i]);
*/
  return 0;
}
