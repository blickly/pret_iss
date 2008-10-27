#include "deadline.h"

int main()
{
  int i;
  
  DEAD(700);
 
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(LOOP_DELAY);

     puts("  /     \\  ");
  }
  return 0;
}
