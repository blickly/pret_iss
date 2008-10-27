#include "deadline.h"

int main()
{
  WRITE('c');
  WRITEFILE('c');
  register int i;
  register int j;
  for (i = 0; i < 2; ++i ) {
    for (j = 0; j < 10; ++j);
  } 
  WRITE('\n');
  END_SIMULATION;
  return 0;
}
