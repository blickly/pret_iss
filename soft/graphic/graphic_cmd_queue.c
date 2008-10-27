#include <stdio.h>
#include "vga.h"
#include "types.h"
#include "tool.h"
#include "debug.h"
#include "graphic.h"
#include "thread1.h"
#include "thread2.h"

#include "global_variable.c"


VOID graphic_init_queue( )
{
	// connect the command queues as a circle
	DWORD i = 0;
	DWORD j = 0;
	for( i = 0; i < N_COMMAND_QUEUE; i++ )
	{
		for( j = 0; j < SIZE_COMMAND_QUEUE - 1; j++ )
		{
			//_command_queue[ i ][ j ].next = ( struct Graphic_command_list * ) &_command_queue[ i ][ j + 1 ];
			( _command_queue + i * SIZE_COMMAND_QUEUE + j )->next = _command_queue + i * SIZE_COMMAND_QUEUE + j + 1;
		}
		//_command_queue[ i ][ SIZE_COMMAND_QUEUE - 1 ].next = ( struct Graphic_command_list * ) &_command_queue[ i ][ 0 ];
		( _command_queue + i * SIZE_COMMAND_QUEUE + SIZE_COMMAND_QUEUE - 1 )->next = ( _command_queue + i * SIZE_COMMAND_QUEUE );

		// the end position of the queue is always greater than the front position of the queue
		//_p_command_queue_front[ i ] = &_command_queue[ i ][ 0 ];
		//_p_command_queue_end[ i ] = &_command_queue[ i ][ 1 ];
		*( _p_command_queue_front + i ) = _command_queue + i * SIZE_COMMAND_QUEUE;
		*( _p_command_queue_end + i ) = _command_queue + i * SIZE_COMMAND_QUEUE + 1;
	}

	*( _current_send_command_queue ) = 0;
	*( _current_recv_command_queue ) = 1;

	//for( i = 0; i < 30; i++ )
	//{
	//	struct Graphic_command_list temp = ** _p_command_queue_end;
	//	PRINT( "temp.next: %02x\n", temp.next );
	//	*(_p_command_queue_end) = temp.next;
	//	PRINT( "*(_p_command_queue_end): %02x\n", *(_p_command_queue_end) );
	//}

	//PRINT( "_p_command_queue_front: %02x\n", _p_command_queue_front );
	//PRINT( "*_p_command_queue_front: %02x\n", *_p_command_queue_front );
	//PRINT( "**_p_command_queue_front: %02x\n", **_p_command_queue_front );

	//PRINT( "_p_command_queue_front + 1: %02x\n", _p_command_queue_front + 1 );
	//PRINT( "*_p_command_queue_front + 1: %02x\n", *( _p_command_queue_front + 1 ) );
	//PRINT( "**_p_command_queue_front + 1: %02x\n", **( _p_command_queue_front + 1 ) );
}

BOOL graphic_send_command( const struct Graphic_command_list const * p_command )
{
	// check whether the end of the queue is reached
	if( *( _p_command_queue_end + *( _current_send_command_queue ) ) == *( _p_command_queue_front + *( _current_send_command_queue ) ) )
	{
		PRINT( "In sending a command, the end of the queue is reached\n" );
		return FALSE;
	}
	
	// insert the command into the queue
	struct Graphic_command_list * temp;
	temp = *( _p_command_queue_end + *( _current_send_command_queue ) );
	temp->command_type = p_command->command_type;
	DWORD i = 0;
	for( ; i < N_MAX_GRAPHIC_COMMAND_ARG; i++ )
	{
		temp->arg[ i ] = p_command->arg[ i ];
	}
	
	// move the position of the queue end into the next empty-queue
	temp = *( _p_command_queue_end + *( _current_send_command_queue ) );
	*( _p_command_queue_end + *( _current_send_command_queue ) ) = temp->next;

	PRINT( "temp: %02x, temp.next: %02x\n", temp, temp->next );
	//PRINT( "( _p_command_queue_end + *( _current_send_command_queue ) ): %02x\n", ( _p_command_queue_end + *( _current_send_command_queue ) ) );
	//PRINT( "*( _p_command_queue_end + *( _current_send_command_queue ) ): %02x\n", *( _p_command_queue_end + *( _current_send_command_queue ) ) );
	
	return TRUE;
}

BOOL graphic_recv_command( struct Graphic_command_list * p_command )
{
	struct Graphic_command_list temp = **( _p_command_queue_front + *( _current_recv_command_queue ) );
	// check whether the commands are remained in the queue
	if( temp.next == *( _p_command_queue_end + *( _current_recv_command_queue ) ) )
	{
		PRINT( "In receiving a command, no more commands are remained in the queue\n" );
		return FALSE;
	}
	
	// move the position of the command queue into the oldest-command position
	temp = **( _p_command_queue_front + *( _current_recv_command_queue ) );
	*( _p_command_queue_front + *( _current_recv_command_queue ) ) = temp.next;
	
	// get the oldest command from the queue
	temp = **( _p_command_queue_front + *( _current_recv_command_queue ) );
	p_command->command_type = temp.command_type;
	DWORD i = 0;
	for( i = 0; i < N_MAX_GRAPHIC_COMMAND_ARG; i++ )
	{
		p_command->arg[ i ] = temp.arg[ i ];
	}
	
	return TRUE;
}

VOID graphic_toggle_command_queue( )
{
	*( _current_send_command_queue ) ^= 1;
	*( _current_recv_command_queue ) ^= 1;
}

VOID graphic_flush_recv_command_queue( )
{
	//prevent compiler optimization
	volatile int t = 0xFFFF;
	printf( "acuire request\n" );
	*( _is_flushing_recv_command ) = TRUE;
	while( *( _is_flushing_recv_command ) == TRUE )
	{
		t--;
	}

	printf( "queue toggled\n" );
}
