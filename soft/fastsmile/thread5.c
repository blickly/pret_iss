#include "deadline.h"

int main()
{
  int i;
 
  DEAD(3900);

  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(LOOP_DELAY);
     
     puts("   \\___/   ");
  }
 
  return 0;
}
