#include "deadline.h"

int main()
{
  int i;
 
  DEAD(2100);
  
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(LOOP_DELAY);

     puts("(|   |   |)");
  }
  return 0;
}
