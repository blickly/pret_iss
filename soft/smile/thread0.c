#include "deadline.h"

int main()
{
  int i;
  for (i = 0; /* FOREVER */ ; i++) {
     DEAD(50000);
     printf("   ooooo   \n");
  }
  return 0;
}
