#include "deadline.h"

#define WHEEL ((volatile unsigned int*) 0x80000400)
enum direction {
	LEFT = 1, RIGHT = 2
};
#define SPEED ((volatile unsigned int*) 0x80000404)
#define BUMP ((volatile unsigned int*) 0x80000508)
enum speed {
	STOP = 1, GO = 2, BACKUP = 3
};

int main() {
#if defined(THREAD_0)

	while (1){
		DEAD(30);
		*SPEED = GO;
		DEAD(30);
		*SPEED = BACKUP;
	}
	*SPEED = STOP;

#endif
	WAIT_FOR_END_SIMULATION;
}
