#include <stdio.h>

#ifndef END_SIMULATION
#define END_SIMULATION return 0
#endif

#define buf ((volatile int*)0x3FFF0200)
#define PERIOD 200000

#if defined(THREAD_0)
int timed_read(int num_cycles, int defaul) {
  int input = 0;
  tryin (num_cycles) {
    while (input == 0) {
      input = *buf;
    }
    *buf = 0;
  } catch {
    printf("No value read! Using default (%d)\n", defaul);
    input = defaul;
  }
  return input;
}

int main() {
  int i;
  int sensor_value = 0;
  printf("Starting Timed Read Test\n");
  for (i = 0; i < 12; i++) {
    DEAD2(PERIOD);
    sensor_value = timed_read(PERIOD/2, sensor_value);
    printf("Read value %d\n", sensor_value);
  }
  printf("done with main\n");
  END_SIMULATION;
}
#elif defined(THREAD_1)
int data[] = {50,60,70,0,100,110,0,130,0,150,0,180};
int main() {
  int i = 0;
  for (;;) {
    DEAD(PERIOD);
    if (data[i] != 0) {
      *buf = data[i];
    }
    i++; i %= 12;
  }
  return 0;
}
#else
#include "deadline.h"
int main() { WAIT_FOR_END_SIMULATION; }
#endif
