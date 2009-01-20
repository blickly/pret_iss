#include "deadline.h"
#define buf ((volatile unsigned int*)0x3FFF0200)
#define trash ((volatile unsigned int*)0x3FFF0210)

int main()
{
  DEAD(1000);
  int i;
  for (i = 0; i < 2; ++i) {  
    DEAD(2000);
    *trash = 'O';
    int sum = 0;
    int j;
    for (j = 0; j < 10; j++) {
      sum = sum + j;
    }
    *buf = 'X';
    *trash = sum;
  }
  *trash = '\n';
  WAIT_FOR_END_SIMULATION;
}
