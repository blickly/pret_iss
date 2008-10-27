#include "deadline.h"

int main()
{
  int i;
 
  DEAD(15000);
  
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(50000);

     printf("(|   |   |)\n");
  }
  return 0;
}
