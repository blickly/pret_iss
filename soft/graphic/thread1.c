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

//#include <stdio.h>
#include "types.h"
#include "deadline.h"
#include "vga.h"
#include "tool.h"
#include "debug.h"

#include "vga.c"
#include "emul.c"

#define VGA_EMUL
#ifdef VGA_EMUL
#undef DEADPLL
#define DEADPLL
#undef	IOWR_32DIRECT
#define	IOWR_32DIRECT	//
#undef	IOWR_8DIRECT
#define	IOWR_8DIRECT	//
//#undef PRINTS
//#define	PRINTS	puts
//#undef PRINT
//#define	PRINT	printf

#else
#endif

#ifdef VGA_EMUL
volatile static int n_frame = 0;
#endif

INT main( )
{
	const DWORD N_COL_SCREEN = 	VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE;
	DWORD i = 0;
	DWORD j = 0;
	volatile DWORD * p_color_line = NULL;

	PRINTS( "The VGA driver started\n" );

#ifdef VGA_EMUL
	puts( "VGA emul mode\n" );

	const int DUMP_FRAME = 3;

	// initialize the driver
	if( vga_emul_init( ) == FALSE )
	{
		PRINTS( "FILE OPEN ERROR!\n" );
	}
#endif

	while( !vga_is_refreshing( ) );
	puts( "vga initial refresh\n" );

	DEADPLL( N_VGA_CLK_V_SYNC );

	while( TRUE )
	{
		// process superfield
		
		// process vsync
		IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_V_SYNC );
		IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_V_SYNC_ON );
		DEADPLL( N_VGA_CLK_V_BP );
		IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_V_SYNC_OFF );
		PRINTS( "VSYNC PROCESSED\n" );
		
		// process back-porch line
		// during back-porch line, refresh the screen buffer
		if( vga_is_refreshing( ) )
		{
			vga_toggle_graphic_buffer( );
			puts( "vga refreshed\n" );
#ifdef	VGA_EMUL
			n_frame++;
			//			puts( "n_frame: %d\n", n_frame );
			if( n_frame == DUMP_FRAME )
			{
				puts( "start dump\n" );
			}
#endif
		}

		DEADPLL( N_VGA_CLK_H_SYNC );
		PRINTS( "BP LINE PROCESSED\n" );
		
		// draw field
		for( i = 0; i < VGA_RESOLUTION_HEIGHT; i++ )
		{
			// process hsync
			IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_H_SYNC );
			IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_H_SYNC_ON );
			//p_color_line = &_screen[ _current_buffer ][ i ][ 0 ];	// access the screen memory in advance
			p_color_line = ( _screen + ( *( _current_buffer ) * VGA_RESOLUTION_HEIGHT * N_COL_SCREEN ) + ( i * N_COL_SCREEN ) );	// access the screen memory in advance
			j = N_COL_SCREEN - 1;
			DEADPLL( N_VGA_CLK_H_BP );
			IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_H_SYNC_OFF );
			PRINTS( "HSYNC PROCESSED\n" );
			int tmp = *p_color_line;
			int limit = ( int ) ( p_color_line + N_COL_SCREEN - 1 );
			// process back-porch pixel
			PRINTS( "BP PIXEL PROCESSED\n" );

			// draw horizontal pixels
			do
			{
#ifdef VGA_EMUL
				if( n_frame == DUMP_FRAME )
				{
					vga_emul_plot_pixel( *p_color_line );
				}
#else
				DEADPLLI( "10" );
				IOWR_32DIRECT( IO_ADDR_VGA, 0, *p_color_line );
#endif
			} while( ( int ) p_color_line++ < ( int ) limit );

			// process front-porch pixel
			DEADPLL( N_VGA_CLK_H_FP );
			IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_H_FP );
			DEADPLL( N_VGA_CLK_H_SYNC );
			PRINTS( "FP PIXEL PROCESSED\n" );
		}

		//puts( "FRAME DRAWING END: %d\n", i );

#ifdef VGA_EMUL
		if( n_frame == DUMP_FRAME )
		{
			puts( "end dump\n" );
		}
#endif

		// process front-porch line
		IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_V_FP );
		DEADPLL( N_VGA_CLK_V_FP - N_VGA_CLK_H_SYNC );
		DEADPLL( N_VGA_CLK_V_SYNC );
		PRINTS( "FP LINE PROCESSED\n" );
	}
	
	return 0;
}
