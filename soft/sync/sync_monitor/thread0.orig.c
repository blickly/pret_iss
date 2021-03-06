#include "deadline.h"

/*

40001158:	03 20 00 00 	sethi  %hi(0x80000000), %g1
4000115c:	96 10 62 00 	or  %g1, 0x200, %o3	! 80000200 <LEON_REG+0x200>
40001160:	9a 10 20 33 	mov  0x33, %o5
40001164:	82 10 00 0d 	mov  %o5, %g1
40001168:	c3 60 00 01 	unknown
4000116c:	9a 10 20 0a 	mov  0xa, %o5
40001170:	98 10 20 07 	mov  7, %o4
40001174:	da 22 c0 00 	st  %o5, [ %o3 ]
40001178:	82 10 00 0c 	mov  %o4, %g1
4000117c:	c3 60 00 01 	unknown
40001180:	9a 83 7f ff 	addcc  %o5, -1, %o5
40001184:	12 bf ff fc 	bne  40001174 <main+0x1c>
40001188:	01 00 00 00 	nop 
4000118c:	81 c3 e0 08 	retl 
40001190:	90 10 20 00 	clr  %o0	! 0 <_trap_table-0x40000000>

First run to sync: 10 cycles
(1 mem, 9 sing)
mem = 2 TIL cycles; (2) + 9 = 11 cycles;
mem = 120 TIL cycles; (1+9) = 10 cycles;

Looped run: 6 cycles
(1 mem, 5 sing)
(1) + 5 = 6 

*/

int main()
{
  volatile unsigned int * buf = (unsigned int*)(0x80000200);
  unsigned int i = 0;
  /*
    T1 takes 56 cycles to reach sync point
    T0 takes 10 cycles to reach sync point
    56 - 10 = 46 cycles to synchronize

    T1 takes 7 cycles to do the loops and store in array to get back
  to sync point
  */

  //76 - 10 
  DEAD(66); //51
  
  /* Begin critical section */
  for (i = 10; i > 0; i--) {
    *buf = i;

    DEAD(27);    
  }
  /* Exit critical section */
  
  return 0;
}
