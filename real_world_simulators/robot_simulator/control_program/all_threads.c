#include "deadline.h"


#define WHEEL ((volatile unsigned int*) 0x80000400)
enum direction {LEFT = 1, RIGHT = 2};

#define SPEED ((volatile unsigned int*) 0x80000404)
#define BUMP ((volatile unsigned int*) 0x80000508)
enum speed {STOP = 1, GO = 2};

int main() {
#if defined(THREAD_0)
  int i;
  *WHEEL = LEFT;
  *SPEED = GO;
#define CYCLESPERSQUARE 100000
  // Robot goes 1 square in 100,000 clock cycles.
  // On a virtual processor (1/6th the speed) this translates
  // to a wait of 16,666 2/3
  DEADLOAD(CYCLESPERSQUARE/6);
  for (i = 0; i < 10; i++) {
    DEAD(CYCLESPERSQUARE/6);
    *WHEEL = RIGHT;
    DEAD(CYCLESPERSQUARE/6);
    *WHEEL = LEFT;
  }
  *SPEED = STOP;
#else
	WAIT_FOR_END_SIMULATION;
#endif
}
