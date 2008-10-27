#include "deadline.h"

int main()
{
  int i;
 
  DEAD(150000);
  
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(500000);

     printf("(|   |   |)\n");
  }
  return 0;
}
