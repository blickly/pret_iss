#include "deadline.h"

#define BLOCK_LENGTH 1024

#define X_COORD ((volatile int*) 0x80000500)
#define Y_COORD ((volatile int*) 0x80000504)

#define WHEEL ((volatile int*) 0x80000400)
enum direction {LEFT = 1, RIGHT = 2};

#define SPEED ((volatile int*) 0x80000404)
enum speed {STOP = 1, GO = 2};

int main() {
#if defined(THREAD_0)
  enum {L, R} state;
  *SPEED = GO;
  state = R;
  for (;;) {
    switch (state) {
      case L:
        while (*X_COORD > BLOCK_LENGTH * 4);
        *WHEEL = RIGHT; *WHEEL = RIGHT;
        state = R;
        break;
      case R:
        while (*X_COORD < BLOCK_LENGTH * 10);
        *WHEEL = LEFT; *WHEEL = LEFT;
        state = L;
        break;
    }
  }
  *SPEED = STOP;
#endif
  WAIT_FOR_END_SIMULATION;
}
