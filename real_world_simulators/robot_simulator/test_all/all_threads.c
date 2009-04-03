#include "deadline.h"

#define WHEEL ((volatile unsigned int*) 0x80000400)
enum direction {
  LEFT = 1, RIGHT = 2
};
#define SPEED ((volatile unsigned int*) 0x80000404)
enum speed {
  STOP = 1, GO = 2, BACKUP = 3
};

int main() {
#if defined(THREAD_0)
  int i;
  for (i = 0; i < 10; ++i) {
    *SPEED = GO;
    *WHEEL = LEFT;
    *WHEEL = RIGHT;
  }
  while (1){
    DEAD(100);
    *SPEED = STOP;
    *WHEEL = LEFT;
    *WHEEL = LEFT;
    *SPEED = GO;
  }
  *SPEED = STOP;
#endif
  WAIT_FOR_END_SIMULATION;
}
