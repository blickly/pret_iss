#include "deadline.h"
#define value ((volatile unsigned int*)0x3FFF0200)
#define observe ((volatile unsigned int*)0x3FFF0204)
#define sync ((volatile unsigned int*)0x3FFF0208)

// This address includes the instructions of the j-indexed loop that
// calculates the sum.  By putting it on the scratchpad, this thread
// is able to execute faster than thread1 and win the race condition
// on value.
//#define raise_addr 0x40000000
#define raise_addr 0x40001310
//#define lower_addr 0x40000000
#define lower_addr 0x400013c8

#define PERIOD 300000

enum STATE {RAISE, LOWER, LOG};

#if defined(PROC_0)
/* Produce very simple stream of "random" data.
 */
int getState()
{
  const int data[10] = 
          {RAISE,LOG,RAISE,LOG,LOWER,RAISE,LOG,LOWER,LOG,LOG};
  static int i = 0;
  i %= 10;
  return data[i++];
}

void raise() {
  int val = *value;
  int j;
  for (j = 0; j < 1000; j++) {
    if (j % 100 == 0)
      val = val + j/100;
  }
  *value = val;
  *observe = 'R';
}

void lower() {
  int val = *value;
  //DMAMV(&val, 0x100); 
  int j;
  for (j = 0; j < 1000; j++) {
    if (j % 100 == 0)
      val = val - j/100;
  }
  *value = val;
  *observe = 'L';
}

int main()
{
  int i;
  *observe = '_';
  *value = 90;
  *sync = 0;
  for (i = 0; i < 20; ++i) {  
    DEAD(PERIOD);
    *observe = '_';
    switch (getState()) {
      case RAISE: {
        int off;
        for (off = 0; off < 0x50; off += 0x10) {
           DMAMV(&raise + off, off); 
        }
        raise();
        break;
      }
      case LOWER: {
        int off;
        for (off = 0; off < 0x50; off += 0x10) {
           DMAMV(&lower + off, off); 
        }
        lower();
        break;
      }
      case LOG: {
        WRITEFILE(*value);
        *observe = '-';
        break;
      }
    }
    //WRITE(*observe);
    //WRITE(*value);
  }
  DEAD(PERIOD);
  WRITE('\n');
  END_SIMULATION;
}

#elif defined(PROC_1)
int main()
{
  int i;
  *sync = 1;
  while (*sync);
  DEAD(PERIOD);
  //for (i = 0; i < 20; ++i) {  
  for(;;) {
    DEAD(PERIOD);
    WRITE(*observe);
    //WRITE('\n');
  }
  //DEAD(2000);
  //END_SIMULATION;
}

// more #elif defined(PROC_?) cases can go here
#else
int main() { WAIT_FOR_END_SIMULATION; }
#endif
