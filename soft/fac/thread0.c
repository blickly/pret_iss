/* MDH WCET BENCHMARK SUITE. File version $Id: thread0.c 757 2008-07-11 00:15:11Z blickly $ */
/*
 * Changes: CS 2006/05/19: Changed loop bound from constant to variable.
 */

#define DEBUG 1
#define END_SIM \
  asm(".word 0x22222222");

#ifdef DEBUG 
#include <stdio.h>
#endif



int fac (int n)
{
  if (n == 0)
     return 1;
  else
     return (n * fac (n-1));
}


int main (void)
{
  int i;
  int s = 0;
  volatile int n;

  n = 5;
  for (i = 0;  i <= n; i++)
   	s += fac (i);

#ifdef DEBUG
  printf("Sum of factorials from [0, %d] is %d\n", n, s );
#endif
  END_SIM;
  return (s);
}

