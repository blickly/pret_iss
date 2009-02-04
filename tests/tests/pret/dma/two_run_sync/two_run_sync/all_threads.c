#include "deadline.h"
#define buf ((volatile unsigned int*)0x3FFF0200)
#define trash ((volatile unsigned int*)0x3FFF0210)

#define PERIOD 0xA00

// This address is includes the instructions of the j-indexed loop that
// calculates the sum.  By putting it on the scratchpad, this thread
// is able to execute faster than thread1 and win the race condition
// on buf.
#define loop_mem_addr 0x4000004c

#if defined(THREAD_0)
int main()
{
  DEAD(0x300);
  int i;
  for (i = 0; i < 2; ++i) {  
    DEAD(PERIOD);
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

#elif defined(THREAD_1)
int main()
{
  DEAD(0x300);
  int i;
  for (i = 0; i < 2; ++i) {  
    DEAD(PERIOD);
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
#else
int main() { WAIT_FOR_END_SIMULATION; }
#endif
