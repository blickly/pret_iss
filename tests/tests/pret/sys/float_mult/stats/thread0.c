/* stats.c */

#define END_SIM \
  asm(".word 0x22222222");

/* Statistics Program:
 *
 * This program exhibits a bug in our simulator on floating point
 * multiplication instructions.
 */


int main ()
{
   float x, y, z;
   int* ptr = (int*) &z;
   x = 3.1415926535897932;
   y = 9.1415926535897932;
   z = x * y;
   printf("x * y = %12.12f (hex %x)\n", z, *ptr);
//   z = x * z;
//   printf("x * x * y = %12.12f (hex %x)\n", z, *ptr);
   END_SIM;
   return 0;
}
