#include "deadline.h"


#define WHEEL ((volatile unsigned int*) 0x80000400)
enum direction {LEFT = 1, RIGHT = 2};

#define SPEED ((volatile unsigned int*) 0x80000404)
enum speed {STOP = 1, GO = 2};

int main() {
#if defined(THREAD_0)
	*SPEED = GO;
	while (*SPEED); // wait for the data to be read and reset to zero

	for (;;) {
		*WHEEL = LEFT;
	}
	return *SPEED;

#else
	WAIT_FOR_END_SIMULATION;
#endif
}
