#include "deadline.h"

int main()
{
  int i;
 
  DEAD(250000);
 
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(500000);
     
     printf("   \\___/   \n");
  }
 
  return 0;
}
