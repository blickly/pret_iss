

/*               Deadlines in C                    */
//
// We have found that the following two methods most effective for
// using the deadline instruction in C programs.
//
// The fist and more straightforward approach is to use a single
// deadi instruction with the immediate encoded in it as the
// last 12 bits.
//
// Example:
// asm volatile(".word 0xC360280A");
//
// In this example, the 5 bits from 29 down to 25 encode the
// timer register to use, in this case %t1.
// 0xC3 = 11000011
//          ^^^^^ = %t1
// The lowest 12 bits encode the deadline, in this case 0x80A = 2058 cycles,
// and the bit at position 13 (the 2) tells us that there is an immediate.
//
//
// The second approach is more complicated, but also more versatile,
// in that it allows data-dependent deadlines, as well as deadlines of
// longer than 0xFFF = 4095 cycles.
//
// Example:
// int dead_reg = 5000;
// asm volatile ("mov %0, %%g1 \n\t .word 0xC3600001"
//               : /* no outputs */
//               : "r" (dead_reg)
//               : "g1");
//
// In this example, we use two assembly instructions, where the first one
// moves a value into a specified register, and the second is a
// dead instruction on that register.
//
// The idetifier dead_reg tells the compiler which C variable
// to load into the register used for the dead instruction.
//





#define PIPELINEDELAY 3;

#ifndef _CLANG_

#define LOOPCNTSTART( pret_label )    \
  asm volatile (pret_label"_loop_start:"); \
  asm volatile ("nop");

#define LOOPCNTEND( pret_label )    \
  asm volatile (pret_label"_loop_end:"); \
  asm volatile ("nop");

#define DEAD( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xC3600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")


/*Loop deadlines are defined */
#define DEAD0( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xC1600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")


#define DEAD1( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xC3600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")

#define DEAD2( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xC5600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")


#define DEAD3( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xC7600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")

#define DEAD4( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xC9600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")

#define DEAD5( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xCB600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")

#define DEAD6( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xCD600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")

#define DEAD7( timeout )         \
   asm volatile ("mov %0, %%g1 \n\t .word 0xCF600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")
#endif /* _CLANG */

/* Start and End are defined for deadlines */
#define DEADSTART0( timeout, pret_label )    \
  asm volatile (pret_label"_end:"); \
  DEAD0(timeout); \
  asm volatile (pret_label"_start:");

#define DEADEND0( pret_label )     \
  asm volatile (pret_label"_end_end:"); \
  asm volatile(".word 0xC1602000");

#define DEADSTART1( timeout, pret_label )    \
  asm volatile (pret_label"_end:"); \
  DEAD1(timeout); \
  asm volatile (pret_label"_start:");

#define DEADEND1( pret_label )    \
  asm volatile (pret_label"_end_end:"); \
  asm volatile (".word 0xC3602000");


#define DEADSTART2( timeout, pret_label )       \
  asm volatile (pret_label"_end:");  \
  DEAD2(timeout); \
  asm volatile (pret_label"_start:");

#define DEADEND2( pret_label )    \
  asm volatile (pret_label"_end_end:"); \
  asm volatile (".word 0xC5602000");

#define DEADSTART3( timeout, pret_label )    \
  asm volatile (pret_label"_end:");      \
  DEAD3(timeout); \
  asm volatile (pret_label"_start:");

#define DEADEND3( pret_label )    \
  asm volatile (pret_label"_end_end:"); \
  asm volatile (".word 0xC7602000");

#define DEADSTART4( timeout, pret_label )    \
  asm volatile (pret_label"_end:");      \
  DEAD4(timeout); \
  asm volatile (pret_label"_start:");

#define DEADEND4( pret_label )    \
  asm volatile (pret_label"_end_end:"); \
  asm volatile (".word 0xC9602000");

#define DEADSTART5( timeout, pret_label )    \
  asm volatile (pret_label"_end:");      \
  DEAD5(timeout); \
  asm volatile (pret_label"_start:");

#define DEADEND5( timeout, pret_label )    \
  asm volatile (pret_label"_end_end:"); \
  asm volatile (".word 0xCB602000");

#define DEADSTART6( timeout, pret_label )    \
  asm volatile (pret_label"_end:");  \
  DEAD6(timeout);    \
  asm volatile (pret_label"_start:");

#define DEADEND6( timeout, pret_label )    \
  asm volatile (pret_label"_end_end:"); \
  asm volatile (".word 0xCD602000");

#define DEADSTART7( timeout, pret_label )    \
  asm volatile (pret_label"_end:");  \
  DEAD7(timeout);    \
  asm volatile (pret_label"_start:");

#define DEADEND7( timeout, pret_label )    \
  asm volatile (pret_label"_end_end:");\
  asm volatile (".word 0xCF602000");



#define DEADL0( timeout, pret_label )    \
   asm volatile ("mov %0, %%g1 \n\t .word 0xC3600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
   : "g1");         \
  asm volatile (pret_label);

#define DEADI(hexstr)                                 \
  asm volatile(".word 0xC3602" hexstr);

#define DEADPLLI(hexstr)                              \
  asm volatile(".word 0xD1602" hexstr);

#define DEADPLL( timeout ) \
  asm volatile ("mov %0, %%g1 \n\t .word 0xD1600001"  \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")

#define DEADPLLI(hexstr)                              \
 asm volatile(".word 0xD1602" hexstr);

#define END_SIMULATION \
  asm(".word 0x22222222");

#define WRITE(c)                              \
 (*((volatile int*)0x80000100) = (c))

#define WRITEFILE(c)                              \
 (*((volatile int*)0x80000600) = (c))

#define DMAMV( mem_addr, spm_addr )                \
   asm volatile ("mov %0, %%g1; mov %1, %%g2;"     \
                 ".word 0xC5A00001;"               \
                  : /* no outputs */               \
                  : "r" (mem_addr), "r" (spm_addr) \
                  : "g1", "g2")

//#ifndef _DEADLINE_H
//#define _DEADLINE_H
/*
/@param period- number of microseconds to wait,
/period of loop in microseconds
*/
void assignTimeMicroSecs(unsigned int period);

/*
/@param period- number of ticks to wait
/period of loop in ticks
*/
void assignTicks(unsigned int period);

//method that replaces GetSignalReady .. get true always returns true
int GetTrue(char l, char r);

//#endif
