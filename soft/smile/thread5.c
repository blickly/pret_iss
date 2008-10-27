#include "deadline.h"

int main()
{
  int i;
 
  DEAD(25000);
 
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(50000);
     
     printf("   \\___/   \n");
  }
 
  return 0;
}
