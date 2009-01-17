#include "deadline.h"
#define buf ((volatile unsigned int*)0x3FFF0200)

int main()
{
  int i;
  for (i = 0; i < 2; ++i) {  
    DEAD(2000);
    int sum = 0;
    int j;
    for (j = 0; j < 9; j++) {
      sum = sum + j;
    }
    *buf = 'X';
  }
  WAIT_FOR_END_SIMULATION;
}
