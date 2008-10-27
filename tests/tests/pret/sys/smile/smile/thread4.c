#include "deadline.h"

int main()
{
  int i;
 
  DEAD(200000);
   
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(500000);

     printf(" |  ___  | \n");
     printf("  \\ \\_/ /  \n");

  }
 
  return 0;
}
