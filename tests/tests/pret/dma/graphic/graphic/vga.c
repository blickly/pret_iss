#include <stdio.h>
#include "types.h"
#include "vga.h"
#include "tool.h"
#include "debug.h"

#include "global_variable.c"

VOID vga_set_color( DWORD row, DWORD col, BYTE color )
{
	DWORD shift = ( col % N_VGA_PIXEL_PER_ONCE ) * 2;
	//_screen[ _current_buffer ][ row ][ col / N_VGA_PIXEL_PER_ONCE ] &= ~( 0x03 << shift );
	//_screen[ _current_buffer ][ row ][ col / N_VGA_PIXEL_PER_ONCE ] |= ( ( color & 0x03 ) << shift );
	*( _screen + ( ( *( _current_buffer ) ^ 1 ) * VGA_RESOLUTION_HEIGHT * ( VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE ) + ( row * ( VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE ) ) + ( col / N_VGA_PIXEL_PER_ONCE ) ) ) &= ~( 0x03 << shift );
	*( _screen + ( ( *( _current_buffer ) ^ 1 ) * VGA_RESOLUTION_HEIGHT * ( VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE ) + ( row * ( VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE ) ) + ( col / N_VGA_PIXEL_PER_ONCE ) ) ) |= ( ( color & 0x03 ) << shift );
}

VOID vga_copy_into_back_buffer( DWORD * source, DWORD start_x, DWORD start_y, DWORD width, DWORD height )
{

	DWORD i = 0;
	DWORD j = 0;

//*
	for( i = start_y * VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE + start_x / N_VGA_PIXEL_PER_ONCE; i < ( start_y + height ) * VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE + ( start_x + width ) / N_VGA_PIXEL_PER_ONCE; i++ )
	{
		//*( &_screen[ _current_buffer ^ 1 ][ 0 ][ 0 ] + i ) = *( source + i );
		*( _screen + i + ( *( _current_buffer ) ^ 1 ) * VGA_RESOLUTION_HEIGHT * VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE ) = *( source + j++ );
	}
//*/
}

VOID vga_refresh_screen( )
{
	*( _is_refreshing ) = TRUE;
	while( *( _is_refreshing ) );
}

BOOL vga_is_refreshing( )
{
	return *( _is_refreshing );
}

VOID vga_toggle_graphic_buffer( )
{
	*( _current_buffer ) ^= 1;
	*( _is_refreshing ) = FALSE;
}

