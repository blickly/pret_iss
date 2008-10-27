#include "timestamp.h"
#include "deadline.h"

volatile int * ref = ( int ) 0x90000000;

int main() {

	int stamp = INITIAL_DELAY + CYCLE_1;

	while( 1 )
	{
		WAIT_TIMESTAMP( stamp );
		DEAD( CYCLE_2 );
		stamp += TOTAL_CYCLE;

		*( ref ) = *( ref ) * 2;
	}

	return 0;
}
