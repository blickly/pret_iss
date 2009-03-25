#include "deadline.h"


#define WHEEL ((volatile unsigned int*) 0x80000400)
enum direction {LEFT = 1, RIGHT = 2};

#define SPEED ((volatile unsigned int*) 0x80000404)
#define BUMP ((volatile unsigned int*) 0x80000508)
enum speed {STOP = 1, GO = 2};

int main() {
#if defined(THREAD_0)
  int i;
  *SPEED = GO;
  for (i = 0; i < 10; i++) {
    *WHEEL = LEFT;
    *WHEEL = RIGHT;
  }
  *SPEED = STOP;
#else
	WAIT_FOR_END_SIMULATION;
#endif
}
