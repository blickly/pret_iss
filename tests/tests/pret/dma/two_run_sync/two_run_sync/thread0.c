#include "deadline.h"
#define buf ((volatile unsigned int*)0x3FFF0200)

// This address is includes the instructions of the j-indexed loop that
// calculates the sum.  By putting it on the scratchpad, this thread
// is able to execute faster than thread1 and win the race condition
// on buf.
#define loop_mem_addr 0x40000060

int main()
{
  DEAD(1000);
  int i;
  for (i = 0; i < 2; ++i) {  
    DEAD(2000);
    *buf = 'O';
    int sum = 0;
    int j;
    for (j = 0; j < 10; j++) {
      sum = sum + j;
    }
    WRITE(*buf);
    WRITE(sum);
    DMAMV(loop_mem_addr, 0);
  }
  WRITE('\n');
  END_SIMULATION;
}
