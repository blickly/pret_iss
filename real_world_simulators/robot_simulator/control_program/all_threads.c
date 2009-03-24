#include "deadline.h"

#define WHEEL ((volatile unsigned int*) 0x80000400)

int main() { 
#if defined(THREAD_0)
  for (;;) {
    *WHEEL = 1;
  }
#else
  WAIT_FOR_END_SIMULATION;
#endif
}
