/*
 * 					VGA driver
 * 
 * 												Sungjun Kim
 * 
 * This file is a VGA driver, which draws every pixel in the monitor.
 * 
 * 
 * 
 * 2008.03.27	- The file is created.
 * 
 */

#include <stdio.h>
#include "types.h"
#include "deadline.h"
#include "vga.h"
#include "tool.h"
#include "debug.h"

#include "vga.c"
#include "emul.c"

static int n_frame = 0;

INT main( )
{
	const DWORD N_COL_SCREEN = 	VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE;
	DWORD i = 0;
	DWORD j = 0;
	volatile DWORD * p_color_line = NULL;
	
	PRINT( "The VGA driver started\n" );

	// initialize the driver
	SET_PLL_CLK( VGA_CLK );
	if( vga_emul_init( ) == FALSE )
	{
		PRINT( "FILE OPEN ERROR!\n" );
	}

	while( !vga_is_refreshing( ) );
	printf( "vga refresh\n" );

	DEAD_PLL( N_VGA_CLK_V_SYNC );

	while( TRUE )
	{
		// process superfield
		
		// process vsync
		IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_V_SYNC );
		IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_V_SYNC_ON );
		DEAD_PLL( N_VGA_CLK_V_BP );
		IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_V_SYNC_OFF );
		PRINT( "VSYNC PROCESSED\n" );
		
		// process back-porch line
		// during back-porch line, refresh the screen buffer
		if( vga_is_refreshing( ) )
		{
			vga_toggle_graphic_buffer( );
			printf( "vga refreshed\n" );
			n_frame++;
		}

		DEAD_PLL( N_VGA_CLK_H_SYNC );
		PRINT( "BP LINE PROCESSED\n" );
		
		// draw field
		for( i = 0; i < VGA_RESOLUTION_HEIGHT; i++ )
		{
			// process hsync
			IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_H_SYNC );
			IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_H_SYNC_ON );
			//p_color_line = &_screen[ _current_buffer ][ i ][ 0 ];	// access the screen memory in advance
			p_color_line = ( _screen + ( *( _current_buffer ) * VGA_RESOLUTION_HEIGHT * N_COL_SCREEN ) + ( i * N_COL_SCREEN ) );	// access the screen memory in advance
			j = N_COL_SCREEN;
			DEAD_PLL( N_VGA_CLK_H_BP );
			IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_H_SYNC_OFF );
			PRINT( "HSYNC PROCESSED\n" );
			int tmp = *p_color_line;
			int limit = ( int ) ( p_color_line + N_COL_SCREEN - 1 );
			// process back-porch pixel
			PRINT( "BP PIXEL PROCESSED\n" );

			// draw horizontal pixels
			do
			{
				DEADPLLI( "10" );
				IOWR_32DIRECT( IO_ADDR_VGA, 0, tmp );
				tmp = *( p_color_line++ );
			} while( ( int ) p_color_line != limit );

			// process front-porch pixel
			DEAD_PLL( N_VGA_CLK_H_FP );
			IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_H_FP );
			DEAD_PLL( N_VGA_CLK_H_SYNC );
			PRINT( "FP PIXEL PROCESSED\n" );
		}

		printf( "FRAME DRAWING END: %d\n", i );

		// process front-porch line
		IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_V_FP );
		DEAD_PLL( N_VGA_CLK_V_FP - N_VGA_CLK_H_SYNC );
		DEAD_PLL( N_VGA_CLK_V_SYNC );
		PRINT( "FP LINE PROCESSED\n" );
	}
	
	if( vga_emul_terminate( ) == FALSE )
	{
		PRINT( "FILE CLOSE ERROR!\n" );
	}
	
	return 0;
}
