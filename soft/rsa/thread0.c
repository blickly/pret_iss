#include "deadline.h"

#define NULL 0

int main()
{
  int *iptr=NULL;
  
  iptr= (int*)malloc(sizeof(int));
  
  *iptr = 2;

  printf("Wow i think malloc works! Int is: %d\n", *iptr);

  return 0;
}
