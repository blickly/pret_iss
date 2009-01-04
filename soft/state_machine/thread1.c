#include "deadline.h"
#define observe ((volatile unsigned int*)0x3FFF0204)

#define PERIOD 6000

int main()
{
  int i;
  DEAD(PERIOD);
  //for (i = 0; i < 20; ++i) {  
  for(;;) {
    DEAD(PERIOD);
    WRITE(*observe);
    WRITE('\n');
  }
  //DEAD(2000);
  //END_SIMULATION;
}
