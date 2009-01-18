

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

#define DEAD( timeout )                               \
   asm volatile ("mov %0, %%g1 \n\t .word 0xC3600001" \
                  : /* no outputs */                  \
                  : "r" (timeout)                     \
                  : "g1")

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
