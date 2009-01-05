#include <stdio.h>
#include "types.h"
#include "vga.h"
#include "tool.h"
#include "debug.h"

volatile unsigned int * _fd = ( DWORD* )( 0x80000600 );
void file_write( char * buf, int len )
{
        int i = 0;
        for( i = 0; i < len; i++ )
        {
                *_fd = *( buf + i );
        }
}

// initialize VGA emulator (file open)
static BOOL vga_emul_init( )
{	
	PRINT( "init\n" );
	CHAR * type_specifier = "P4\n640 480\n"; 
	file_write( type_specifier, strlen( type_specifier ) );
	return TRUE;
}

// terminate VGA emulator (file close)
static BOOL vga_emul_terminate( )
{
	return TRUE;
}

// emulate VGA monitor (draw pixel in the emul file)
static BOOL vga_emul_plot_pixel( DWORD color )
{
	const DWORD N_BIT_TRANSFER_PER_ONCE = 32;
	static DWORD h_cnt = 0x00;
	static DWORD v_cnt = 0x00;

	PRINT( "plot\n" );
		
	BYTE pixel = 0x00;
	CHAR buffer[ 100 ] = { '\0', };
	BYTE c = 0;
	DWORD i = 0;
	DWORD j = 0;
	for( i = 0; i < N_BIT_TRANSFER_PER_ONCE; i += ( N_BIT_TRANSFER_PER_ONCE / N_VGA_PIXEL_PER_ONCE ) ) 
	{
		pixel = ( color >> i ) & 0x03;
		if( pixel == 0 )
		{
			c = ( c << 1 ) & 0xFE;
		}
		else
		{
			c = ( c << 1 ) | 0x01;
		}
		
		if( i == 14 )
		{
			file_write( &c, 1 );
			//sprintf( buffer, "%02X ", c );
			//file_write( buffer, strlen( buffer ) );
			c = 0x00;
		}
		else if( i == 30 )
		{
			file_write( &c, 1 );
			//sprintf( buffer, "%02X ", c );
			//file_write( buffer, strlen( buffer ) );
		}
	}
/*	
	h_cnt += N_VGA_PIXEL_PER_ONCE;
	if( h_cnt == VGA_RESOLUTION_WIDTH )
	{
		h_cnt = 0;
		v_cnt++;
		sprintf( buffer, "\n" );
		file_write( buffer, strlen( buffer ) );
	}
	
	if( v_cnt == VGA_RESOLUTION_HEIGHT )
	{
		v_cnt = 0;
		sprintf( buffer, "\nEND OF FRAME\n" );
	}
//*/
	
	return TRUE;
}
