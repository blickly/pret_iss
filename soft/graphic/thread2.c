/*					Graphic driver
 * 
 * 											Sungjun Kim
 * 
 * This driver processes grahic-related works. This driver draws pixels,
 * lines, texts and color-filled areas. This driver consumes a thread in order
 * to show smooth graphic-motions.
 * 
 * 2008.04.04	The file is created.
 * 
 */

#include <stdio.h>
#include "vga.h"
#include "types.h"
#include "tool.h"
#include "debug.h"
#include "graphic.h"
#include "thread1.h"
#include "thread2.h"

#include "vga.c"
#include "graphic.c"


DWORD _background[ 480 ][ 640 / 16 ] = {0, };
DWORD _sprite[ 64 ][ 64 / 16 ] = { 0, };

DWORD _sprite_position_x[ 5 ] = { 0,};
DWORD _sprite_position_y[ 5 ] = { 0,};

static VOID graphic_layer_activate( ID_GRAPHIC_LAYER id, BOOL on )
{
	_graphic_layer[ id ].is_activated = on;
}

/*
static VOID plot_dot( ID_GRAPHIC_LAYER id, DWORD x, DWORD y, DWORD thickness, DWORD color )
{
	// activate the selected layer
	graphic_layer_activate( id, TRUE );
	
	// plot a dot on the selected layer with the selected thickness
	DWORD i = 0;
	DWORD j = 0;
	for( i = 0; i < thickness; i++ )
	{
		for( j = 0; j < thickness; j++ )
		{
			_graphic_layer[ id ].pixel[ i ][ j ] = color;
		}
	}
}

static VOID plot_line( ID_GRAPHIC_LAYER id, DWORD start_x, DWORD start_y, DWORD end_x, DWORD end_y, DWORD thickness, DWORD color )
{
	// activate the selected layer
	graphic_layer_activate( id, TRUE );
	
	// draw line on the selected layer with the selected thickness
	float a = ( end_y - start_y ) / ( end_x - start_x );
	int i = 0;
	for( i = start_x; i < end_x; i++ )
	{
		plot_dot( id, i, a * ( i - start_x ) + start_y, thickness, color );
	}
}
//*/

static VOID move_direct( ID_GRAPHIC_LAYER id, DWORD x, DWORD y )
{
//	_graphic_layer[ id ].pos_x = x;
//	_graphic_layer[ id ].pos_y = y;
	_sprite_position_x[ id ] = x;
	_sprite_position_y[ id ] = y;
}

static VOID move_vector( ID_GRAPHIC_LAYER id, DWORD x, DWORD y, DWORD scale, DWORD angle )
{
	// TBD
}

static VOID move_arc( ID_GRAPHIC_LAYER id, DWORD x, DWORD y, DWORD angle )
{
	// TBD
}

static VOID plot_dot( ID_GRAPHIC_LAYER id, DWORD x, DWORD y, DWORD thickness, DWORD color )
{
	// activate the selected layer
	//graphic_layer_activate( id, TRUE );
	
	// plot a dot on the selected layer with the selected thickness
	DWORD i = 0;
	DWORD j = 0;
	for( i = 0; i < thickness; i++ )
	{
		DWORD shift = 0;
		for( j = 0; j < thickness; j++ )
		{
			switch( id )
			{
				case 0:
				//vga_set_color( y + j, x + i, color );
				shift = ( ( x + j ) % N_VGA_PIXEL_PER_ONCE ) * 2;
				_background[ y + i ][ ( x + j ) / N_VGA_PIXEL_PER_ONCE ] &= ~( 0x03 << shift );
				_background[ y + i ][ ( x + j ) / N_VGA_PIXEL_PER_ONCE ] |= ( ( color & 0x03 ) << shift );
				break;
				case 1:
				shift = ( ( x + j ) % N_VGA_PIXEL_PER_ONCE ) * 2;
				_sprite[ y + i ][ ( x + j ) / N_VGA_PIXEL_PER_ONCE ] &= ~( 0x03 << shift );
				_sprite[ y + i ][ ( x + j ) / N_VGA_PIXEL_PER_ONCE ] |= ( ( color & 0x03 ) << shift );
				break;
				default:
				break;
			}
		}
	}
}

static VOID plot_line( ID_GRAPHIC_LAYER id, DWORD start_x, DWORD start_y, DWORD end_x, DWORD end_y, DWORD thickness, DWORD color )
{
	// activate the selected layer
	//graphic_layer_activate( id, TRUE );
	
	// draw line on the selected layer with the selected thickness
	int a = ( 1000 * ( ( int ) ( end_y - start_y ) ) ) / ( ( int ) ( end_x - start_x ) );
	int i = 0;

	if( end_x >= start_x )
	{
		for( i = start_x; i <= end_x; i++ )
		{
			plot_dot( id, i, ( int ) ( ( a * ( ( int ) ( i - start_x ) ) ) / 1000 ) + start_y, thickness, color );
			PRINT( "x: %d, y: %d, a: %d\n", i, ( int ) ( ( a * ( ( int ) ( i - start_x ) ) ) / 1000 ) + start_y, a );
		}
	}
	else
	{
		for( i = start_x; i >= end_x; i-- )
		{
			plot_dot( id, i, ( int ) ( ( a * ( ( int ) ( i - start_x ) ) ) / 1000 ) + start_y, thickness, color );
			PRINT( "x: %d, y: %d, a: %d\n", i, ( int ) ( ( a * ( ( int ) ( i - start_x ) ) ) / 1000 ) + start_y, a );
		}
	}
}

void locate_sprite_direct( int id, int x, int y, int width, int height )
{
	if( id == 0 )
	{
		return;
	}

	int i = 0;
	int j = 0;
	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			if( ( _sprite[ i ][ j / 16 ] >> ( 2 * ( j % 16 ) ) ) & 0x03 != 0 )
			{
				//plot_dot( 0, 100 + j, 100 + i, 1, ( _sprite[ i ][ j / 16 ] >> ( j % 16 ) ) & 0x03 );
				vga_set_color( i + y, j + x, ( _sprite[ i ][ j / 16 ] >> ( 2 * ( j % 16 ) ) ) & 0x03 );
			}
		}
	}
}

void update_screen( )
{
	// refresh the vga screen buffer
	vga_copy_into_back_buffer( *_background, 0, 0, 640, 480 );

	int i = 0;
	for( i = 1; i < 5; i++ )
	{
		locate_sprite_direct( i, _sprite_position_x[ i ], _sprite_position_y[ i ], 64, 64 );
	}

	vga_refresh_screen( );

}

INT main( )
{
//*
	PRINT( "The graphic driver started\n" );
	
	struct Graphic_command_list current_command;
	
	// initialize graphic driver
	graphic_init_queue( );

	printf("queue init\n");

//*	
	while( TRUE )
	{
		// get command
		// copy graphic buffers into vga buffer if empty
		if( !graphic_recv_command( &current_command ) )
		{
			// if new command request received, toggle command queue to get the command
			if( *( _is_flushing_recv_command ) )
			{
				//printf( "%d\n", *( _is_call_new_command ) );
				graphic_toggle_command_queue( );
			}
			*( _is_flushing_recv_command ) = FALSE;
			continue;
		}
		
		// execute command according to the queue's data
		switch( current_command.command_type )
		{
			case GRAPHIC_COMMAND_PLOT_DOT:
			plot_dot( current_command.arg[ 0 ], current_command.arg[ 1 ], current_command.arg[ 2 ], current_command.arg[ 3 ], current_command.arg[ 4 ] );
			break;
			case GRAPHIC_COMMAND_PLOT_LINE:
			plot_line( current_command.arg[ 0 ], current_command.arg[ 1 ], current_command.arg[ 2 ], current_command.arg[ 3 ], current_command.arg[ 4 ], current_command.arg[ 5 ], current_command.arg[ 6 ] );
			printf("plot line\n");			
			break;
			case GRAPHIC_COMMAND_MOVE_DIRECT:
			move_direct( current_command.arg[ 0 ], current_command.arg[ 1 ], current_command.arg[ 2 ] );
			break;
			case GRAPHIC_COMMAND_MOVE_VECTOR:
			break;
			case GRAPHIC_COMMAND_MOVE_ARC:
			break;
			case GRAPHIC_COMMAND_UPDATE_SCREEN:
			update_screen( );
			default:
			break;
		}
//*/		
	}
	
	PRINT( "The graphic driver terminated\n" );
//*/	
	return 0;
}
