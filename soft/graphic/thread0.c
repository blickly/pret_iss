/*
 * 					VGA control thread
 * 
 * 											Sungjun Kim
 * 
 * This file controls and tests VGA driver.
 * 
 * 
 * 2008.03.27	The file is created.
 * 
 */

#include "types.h"
#include "tool.h"

#include "vga.c"
#include "graphic.c"

#define	UP				0
#define DOWN			1
#define	LEFT			2
#define	RIGHT			3
#define BUTTON_PRESS	4
#define BUTTON_RELEASE	5

const int _input_data[ 100 ] =
{
	RIGHT,		RIGHT,		RIGHT,		RIGHT,		LEFT,		LEFT,		DOWN,		BUTTON_PRESS,		BUTTON_PRESS,		BUTTON_RELEASE,
	-1,
};


INT main( )
{
	int cnt = 0;

	graphic_init( );

	printf( "drawing start\n" );

	graphic_plot_line( 0, 0, 0, 640, 480, 5, 0x01 );
	graphic_plot_line( 0, 0, 480, 640, 0, 5, 0x01 );

	graphic_plot_line( 1, 3, 3, 60, 18, 3, 0x01 );
	graphic_plot_line( 1, 3, 3, 18, 18, 3, 0x01 );
	graphic_plot_line( 1, 18, 18, 3, 33, 3, 0x01 );
	graphic_plot_line( 1, 3, 33, 60, 18, 3, 0x01 );
	graphic_move_direct( 1, 290, 380 );
	graphic_update_screen( );

	printf( "drawing end\n" );


	while( TRUE )
	{
		graphic_update_screen( );
	}

	return 0;
}

