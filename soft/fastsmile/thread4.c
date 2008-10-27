#include "deadline.h"

int main()
{
  int i;
 
  DEAD(2800);
   
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(LOOP_DELAY);

//     puts(" |  ___  | ");
//     puts("  \\ \\_/ /  ");

     puts(" |  ___  |\n"
          "  \\ \\_/ /");

  }
 
  return 0;
}
