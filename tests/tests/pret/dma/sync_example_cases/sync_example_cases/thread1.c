#include "deadline.h"

int main()
{
  DEAD(15);
  volatile unsigned int * buf = (unsigned int*)(0x3FFF0200);
  unsigned int i = 0;
  int arr[8];

  for (i = 0; i < 95 ; i++) {
    DEAD(30);
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
