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
#include "vga.h"
#include "tool.h"
#include "debug.h"

volatile static DWORD _io_addr_vga = 0x00000000;			// to be deleted when real address is allocated
volatile static DWORD * IO_ADDR_VGA = &_io_addr_vga;		// TBD

volatile static DWORD _screen[ VGA_RESOLUTION_HEIGHT ][ ( VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE ) ] = { 0x00000000, };

VOID vga_set_color( DWORD row, DWORD col, BYTE color )
{
	DWORD shift = ( col % N_VGA_PIXEL_PER_ONCE ) * 2;
	_screen[ row ][ col / N_VGA_PIXEL_PER_ONCE ] &= ~( 0x03 << shift );
	_screen[ row ][ col / N_VGA_PIXEL_PER_ONCE ] |= ( color << shift );
}

static FILE * _p_file = NULL;
static CHAR * _file_name = "vga_emul_result.txt";

// initialize VGA emulator (file open)
static BOOL vga_emul_init( )
{
	if( _p_file != NULL )
	{
		return FALSE;
	}
	
	_p_file = fopen( _file_name, "w+" );
	if( _p_file == NULL )
	{
		return FALSE;
	}
	
	return TRUE;
}

// terminate VGA emulator (file close)
static BOOL vga_emul_terminate( )
{
	if( _p_file == NULL )
	{
		return FALSE;
	}
	
	fclose( _p_file );
	_p_file = NULL;
	
	return TRUE;
}

// emulate VGA monitor (draw pixel in the emul file)
static BOOL vga_emul_plot_pixel( DWORD color )
{
	const DWORD N_BIT_TRANSFER_PER_ONCE = 32;
	static DWORD h_cnt = 0x00;
	static DWORD v_cnt = 0x00;

	if( _p_file == NULL )
	{
		return FALSE;
	}
		
	BYTE pixel = 0x00;
	CHAR buffer[ 100 ] = { '\0', };
	DWORD i = 0;
	for( i = 0; i < N_BIT_TRANSFER_PER_ONCE; i += ( N_BIT_TRANSFER_PER_ONCE / N_VGA_PIXEL_PER_ONCE ) ) 
	{
		pixel = ( color >> i ) & 0x03;
		
		sprintf( buffer, "%d", pixel );
		fwrite( buffer, sizeof( CHAR ), strlen( buffer ), _p_file );
	}
	
	h_cnt += N_VGA_PIXEL_PER_ONCE;
	if( h_cnt == VGA_RESOLUTION_WIDTH )
	{
		h_cnt = 0;
		v_cnt++;
		sprintf( buffer, "\n" );
		fwrite( buffer, sizeof( CHAR ), strlen( buffer ), _p_file );
	}
	
	if( v_cnt == VGA_RESOLUTION_HEIGHT )
	{
		v_cnt = 0;
		sprintf( buffer, "\nEND OF FRAME\n" );
		fwrite( buffer, sizeof( CHAR ), strlen( buffer ), _p_file );
	}
	
	return TRUE;
}

INT main( )
{
	const DWORD N_COL_SCREEN = 	VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE;
	DWORD i = 0;
	DWORD j = 0;
	volatile DWORD * p_color_line = NULL;
	
	PRINT( "VGA driver started\n" );
	
	// initialize the driver
	SET_PLL_CLK( 25175000 );
	if( vga_emul_init( ) == FALSE )
	{
		PRINT( "FILE OPEN ERROR!\n" );
	}
	
	// plot pixels for test
	vga_set_color( 0, 5, 0x03 );
	
	DWORD loop = 3;
	while( loop-- )
	{
		// process superfield
		
		// process vsync
		IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_V_SYNC );
		IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_V_SYNC_ON );
		DEAD_PLL( N_VGA_CLK_V_SYNC );
		IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_V_SYNC_OFF );
		PRINT( "VSYNC PROCESSED\n" );
		
		// process back-porch line
		DEAD_PLL( N_VGA_CLK_V_BP );
		PRINT( "BP LINE PROCESSED\n" );
		
		// draw field
		for( i = 0; i < VGA_RESOLUTION_HEIGHT; i++ )
		{
			// process hsync
			IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_H_SYNC );
			IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_H_SYNC_ON );
			p_color_line = &_screen[ i ][ 0 ];	// access the screen memory in advance
			j = N_COL_SCREEN;					// assign the horizontal loop value in advance
			DEAD_PLL( N_VGA_CLK_H_SYNC );
			IOWR_8DIRECT( IO_ADDR_VGA, 1, VGA_CONTROL_H_SYNC_OFF );
			PRINT( "HSYNC PROCESSED\n" );

			// process back-porch pixel
			DEAD_PLL( N_VGA_CLK_H_BP );
			PRINT( "BP PIXEL PROCESSED\n" );
			
			// draw horizontal pixels
			while( j-- )
			{
				p_color_line++;
				DEAD_PLL( N_VGA_CLK_PIXEL );
				IOWR_32DIRECT( IO_ADDR_VGA, 0, *p_color_line );
			}
			
			// process front-porch pixel
			IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_H_FP );
			DEAD_PLL( N_VGA_CLK_H_FP );
			PRINT( "FP PIXEL PROCESSED\n" );
		}
		
		// process front-porch line
		IOWR_32DIRECT( IO_ADDR_VGA, 0, VGA_COLOR_V_FP );
		DEAD_PLL( N_VGA_CLK_V_FP );
		PRINT( "FP LINE PROCESSED\n" );
	}
	
	if( vga_emul_terminate( ) == FALSE )
	{
		PRINT( "FILE CLOSE ERROR!\n" );
	}
	
	return 0;
}
