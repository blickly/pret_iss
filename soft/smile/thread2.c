#include "deadline.h"

int main()
{  
  int i;
 
  DEAD(10000);

  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(50000);

     if (i % 4 == 3) {
        printf(" |  ^ ^  | \n");
     } else {
        printf(" |  o o  | \n");
     }
  }
  return 0;
}
