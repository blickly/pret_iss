#include "deadline.h"

int main()
{
  int i;
 
  DEAD(20000);
   
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(50000);

     printf(" |  ___  | \n");
     printf("  \\ \\_/ /  \n");

  }
 
  return 0;
}
