#include "deadline.h"

int main() { 
#if defined(THREAD_0)
  END_SIMULATION;
#else
  WAIT_FOR_END_SIMULATION;
#endif
}
