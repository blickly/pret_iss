#include "deadline.h"

/*
40101158:	9d e3 bf 70 	save  %sp, -144, %sp
4010115c:	03 20 00 00 	sethi  %hi(0x80000000), %g1
40101160:	94 10 62 00 	or  %g1, 0x200, %o2	! 80000200 <LEON_REG+0x200>
40101164:	92 07 bf f8 	add  %fp, -8, %o1
40101168:	82 07 bf d0 	add  %fp, -48, %g1
4010116c:	b0 10 20 09 	mov  9, %i0
40101170:	c0 20 40 00 	clr  [ %g1 ]
40101174:	b0 86 3f ff 	addcc  %i0, -1, %i0
40101178:	1c bf ff fe 	bpos  40101170 <main+0x18>
4010117c:	82 00 60 04 	add  %g1, 4, %g1

10 [to loop] + (9 x 4) [loop 10 times more] = 46
(1 mem, 9 sing) + (1 mem, 8 sing) x 4

40101180:	9a 10 20 04 	mov  4, %o5
40101184:	82 10 00 0d 	mov  %o5, %g1
40101188:	c3 60 00 01 	unknown
4010118c:	96 10 20 04 	mov  4, %o3
40101190:	98 02 7f d8 	add  %o1, -40, %o4
40101194:	b0 10 20 09 	mov  9, %i0
40101198:	da 02 80 00 	ld  [ %o2 ], %o5
4010119c:	da 23 00 00 	st  %o5, [ %o4 ]
401011a0:	82 10 00 0b 	mov  %o3, %g1
401011a4:	c3 60 00 01 	unknown
401011a8:	b0 86 3f ff 	addcc  %i0, -1, %i0
401011ac:	1c bf ff fb 	bpos  40101198 <main+0x40>
401011b0:	98 03 20 04 	add  %o4, 4, %o4
401011b4:	a0 02 7f d8 	add  %o1, -40, %l0
401011b8:	23 10 04 2a 	sethi  %hi(0x4010a800), %l1
401011bc:	b0 10 20 09 	mov  9, %i0
401011c0:	d2 04 00 00 	ld  [ %l0 ], %o1
401011c4:	40 00 00 1b 	call  40101230 <printf>
401011c8:	90 14 61 60 	or  %l1, 0x160, %o0
401011cc:	b0 86 3f ff 	addcc  %i0, -1, %i0
401011d0:	1c bf ff fc 	bpos  401011c0 <main+0x68>
401011d4:	a0 04 20 04 	add  %l0, 4, %l0
401011d8:	81 c7 e0 08 	ret 
401011dc:	91 e8 20 00 	restore  %g0, 0, %o0




First run to sync: 46  (set to arr to 0) + 10 = 56 cycles.
(1 mem, 9 sing) + (1 mem, 8 sing)x4 + (2 mem [1 ld], 8 sing)

mem = 2 TIL cycles; (11) + (10)x4 + (11) = 62 cycles
mem = 120 TIL cycles; (1+9) + (1+8)x4 + (1+ 21 + 8) = 76 cycles

Loop run back to sync: 7.
(2 mem [1 ld], 5 sing)
mem = 2 TIL cycles; (3) + (5) = 8
mem = 120 TIL cycles; (1+21) + (5) = 27

*/
int main()
{
  volatile unsigned int * buf = (unsigned int*)(0x80000200);
  unsigned int i = 0;
  int arr[10];

  for (i =0; i<10; i++)
    arr[i] = 0;

  /* T0 loop takes 5 cycles to get to sync point */
  
  DEAD(6); //4
  /* Enter critical section */
  for (i = 0; i<10 ; i++) {
   
    arr[i] = *buf;
    DEAD(6); //4
  }
  /* Exit critical section */

  for (i = 0; i< 10; i++ )
    printf("%d ", arr[i]);
  
  return 0;
}
