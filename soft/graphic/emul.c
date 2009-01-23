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

	char pixel = 0x00;
	volatile char c = 0x00;
	volatile DWORD i = 0;
	volatile DWORD j = 0;
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
			file_write( ( char * ) &c, 1 );
			//sprintf( buffer, "%02X ", c );
			//file_write( buffer, strlen( buffer ) );
			c = 0x00;
		}
		else if( i == 30 )
		{
			file_write( ( char * ) &c, 1 );
			//sprintf( buffer, "%02X ", c );
			//file_write( buffer, strlen( buffer ) );
		}
	}
	
	return TRUE;
}
