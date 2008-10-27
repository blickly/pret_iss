#include "deadline.h"

int main()
{
  int i;
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(LOOP_DELAY);

     puts("   ooooo   ");
  }
  return 0;
}
