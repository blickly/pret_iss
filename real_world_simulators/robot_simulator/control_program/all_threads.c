#include "deadline.h"

#define WHEEL ((volatile unsigned int*) 0x80000400)
#define SPEED ((volatile unsigned int*) 0x80000404)

int main() {
#if defined(THREAD_0)
	*SPEED = 1;
	while (*SPEED); // wait for the data to be read and reset to zero

	for (;;) {
		*WHEEL = 1;
	}
	return *SPEED;

#else
	WAIT_FOR_END_SIMULATION;
#endif
}
