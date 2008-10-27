#include "deadline.h"

int main()
{  
  int i;
 
  DEAD(1400);

  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(LOOP_DELAY);

     if (i % 4 == 3) {
        puts(" |  ^ ^  | ");
     } else {
        puts(" |  o o  | ");
     }
  }
  return 0;
}
