#include "deadline.h"

#define BLOCK_LENGTH 1024

#define X_COORD ((volatile int*) 0x80000500)
#define Y_COORD ((volatile int*) 0x80000504)

#define WHEEL ((volatile int*) 0x80000400)
enum direction {LEFT = 1, RIGHT = 2};

#define MOTOR ((volatile int*) 0x80000404)
enum motor {STOP = 1, GO = 2};

void uturnleft() {
   *WHEEL = LEFT; *WHEEL = LEFT;
}

void uturnright() {
   *WHEEL = RIGHT; *WHEEL = RIGHT;
}

void mainloop() {
  enum {L, R} state = R;
  *MOTOR = GO;

  for (;;) {
    switch (state) {
      case L:
        while (*X_COORD > BLOCK_LENGTH * 4);
        uturnright();
        state = R;
        break;
      case R:
        while (*X_COORD < BLOCK_LENGTH * 5);
        uturnleft();
        state = L;
        break;
    }
  }
  *MOTOR = STOP;
}

int main() {
#if defined(THREAD_0)
  void* addr;
  for (addr = (void*)&uturnleft; addr < (void*)&mainloop; addr += 16) {
    DMAMV(addr, (unsigned int)addr % 0x1000);
  }
  mainloop();
#endif
  WAIT_FOR_END_SIMULATION;
}

void sentinal() {}
