#include "deadline.h"
#include "stdio.h"
#include "setjmp.h"
#include "strings.h"


/* Example to show th sync instruction. It shows several things:
 * 1) Each thread takes a different amount of time to enter main, a different
 *    delay has also been added to futher show the synchornization. 
 * 2) The print statements from each thread will now print in the correct order
 * 3) All threads have "END_SIMULATION" command, but since there is a 
 *    sync instruction before it, it won't end other threads prematurally until
 *    all threads execute to that point. It seems we don't need "WAIT_FOR_SIMULATION" now
 *    
 */

#if defined(THREAD_0)
int main()
{
  DEADLOAD(5012);
  SYNC("0F");
  DEADLOAD(100000);
  DEAD(0);
  printf("Should print out second from thread 0\n");

  SYNC("3F");
  END_SIMULATION;

}
#elif defined(THREAD_1)
int main()
{
  DEADLOAD(1234);
  DEAD(0);
  SYNC("0F");
  printf("Should print out first from thread 1\n");

  SYNC("3F");
  END_SIMULATION;

}
#elif defined(THREAD_2)
int main()
{
  DEADLOAD(332);
  DEAD(0);
  SYNC("0F");
  DEADLOAD(300000);
  DEAD(0);
  printf("Should print out last from thread 2\n");

  SYNC("3F");
  END_SIMULATION;

}
#elif defined(THREAD_3)
int main()
{
  SYNC("0F");
  DEADLOAD(200000);
  DEAD(0);
  printf("Should print out third from thread 3\n");

  SYNC("3F");
  END_SIMULATION;

}
#else
int main() {

  SYNC("3F");
  END_SIMULATION;
}
#endif

