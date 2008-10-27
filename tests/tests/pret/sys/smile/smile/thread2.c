#include "deadline.h"

int main()
{  
  int i;
 
  DEAD(100000);

  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(500000);

     if (i % 4 == 3) {
        printf(" |  ^ ^  | \n");
     } else {
        printf(" |  o o  | \n");
     }
  }
  return 0;
}
