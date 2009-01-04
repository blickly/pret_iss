#include "deadline.h"
#define value ((volatile unsigned int*)0x3FFF0200)
#define observe ((volatile unsigned int*)0x3FFF0204)

// This address includes the instructions of the j-indexed loop that
// calculates the sum.  By putting it on the scratchpad, this thread
// is able to execute faster than thread1 and win the race condition
// on value.
//#define raise_addr 0x40000000
#define raise_addr 0x40001318
//#define lower_addr 0x40000000
#define lower_addr 0x400013d8

#define PERIOD 6000

enum STATE {RAISE, LOWER, LOG};

/* Produce very simple stream of "random" data.
 */
int getState()
{
  const int data[10] = 
          {RAISE,LOG,RAISE,LOG,LOWER,RAISE,LOG,LOWER,LOG,LOG};
//        {RAISE,LOWER,LOG,LOWER,LOG,RAISE,RAISE,RAISE,LOWER,LOWER};
  static int i = 0;
  i %= 10;
  return data[i++];
}

int main()
{
  int i;
  *observe = '_';
  *value = 90;
  for (i = 0; i < 20; ++i) {  
    DEAD(PERIOD);
    switch (getState()) {
      case RAISE: {
        DMAMV(raise_addr,0); DMAMV(raise_addr+4,4); DMAMV(raise_addr+8,8);
        int val = *value;
        int j;
        for (j = 0; j < 10; j++) {
          val = val + j;
        }
        *value = val;
        *observe = 'R';
        break;
      }
      case LOWER: {
        DMAMV(lower_addr,0); DMAMV(lower_addr+4,4); DMAMV(lower_addr+8,8);
        int val = *value;
        int j;
        for (j = 0; j < 10; j++) {
          val = val - j;
        }
        *value = val;
        *observe = 'L';
        break;
      }
      case LOG: {
        WRITEFILE(*value);
        *observe = '.';
        break;
      }
    }
    //WRITEFILE(*value);
    //WRITE(*observe);
  }
  DEAD(PERIOD);
  //WRITE('\n');
  END_SIMULATION;
}
