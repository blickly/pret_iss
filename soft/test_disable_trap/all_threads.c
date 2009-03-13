#include "deadline.h"
#include "stdio.h"
#include "setjmp.h"
#include "strings.h"

#if defined(THREAD_0)

jmp_buf buf;
jmp_buf buf2;

int main()
{
  int i;

  register_jmpbuf(1, &buf);
 

  if (setjmp(buf) == 0 ) {
    DEADLOADBRANCH(200);
    printf("Deadline branch should have expired!\n");
    DEADLOAD(0);
  }
  else {
    printf("Deadline expiration handler!!\n");
  }

  DEADBRANCH(200);
  DISABLE_TRAPS;
  for ( i = 0; i<3; i++ )
    printf("Should print till 3: %d\n", i);
  ENABLE_TRAPS;
  
  printf("***ERROR!! This should not be printed out!!***\n");
  
  END_SIMULATION;

}
#else
int main() {
    WAIT_FOR_END_SIMULATION;
}
#endif

