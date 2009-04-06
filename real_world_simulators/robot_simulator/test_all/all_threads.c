#include "deadline.h"

#define BLOCK_LENGTH 1024

#define WHEEL ((volatile unsigned int*) 0x80000400)
enum direction {
  LEFT = 1, RIGHT = 2
};
#define SPEED ((volatile unsigned int*) 0x80000404)
enum speed {
  STOP = 1, GO = 2, BACKUP = 3
};
#define X_COORD ((volatile unsigned int*) 0x80000500)
#define Y_COORD ((volatile unsigned int*) 0x80000504)

int main() {
#if defined(THREAD_0)
  while (*Y_COORD < 2 * BLOCK_LENGTH) {
    *SPEED = GO;
    *WHEEL = LEFT;
    *WHEEL = RIGHT;
  }
  while (1){
    DEAD(30000);
    *SPEED = STOP;
    *WHEEL = LEFT;
    *WHEEL = LEFT;
    *SPEED = GO;
  }
  *SPEED = STOP;
#endif
  WAIT_FOR_END_SIMULATION;
}
