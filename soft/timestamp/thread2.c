#include "timestamp.h"
#include "deadline.h"

volatile int * ref = ( int ) 0x90000000;
volatile unsigned int * _fd = ( int )( 0x80000600 );

int main() {
	int stamp = INITIAL_DELAY + CYCLE_1 + CYCLE_2;

	while( 1 )
	{
		WAIT_TIMESTAMP( stamp );
		DEAD( CYCLE_3 );
		stamp += TOTAL_CYCLE;

		*_fd = *ref;
	}

	return 0;
}
