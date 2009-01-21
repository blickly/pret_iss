#include <stdio.h>
#include "vga.h"
#include "types.h"
#include "tool.h"
#include "debug.h"
#include "graphic.h"
#include "thread1.h"
#include "thread2.h"

#include "global_variable.c"
#include "graphic_cmd_queue.c"

struct Graphic_command_list command;
VOID graphic_plot_dot( ID_GRAPHIC_LAYER id, DWORD x, DWORD y, DWORD thickness, DWORD color )
{
	command.command_type = GRAPHIC_COMMAND_PLOT_DOT;
	command.arg[ 0 ] = id;
	command.arg[ 1 ] = x;
	command.arg[ 2 ] = y;
	command.arg[ 3 ] = thickness;
	command.arg[ 4 ] = color;
	graphic_send_command( &command );
}

VOID graphic_plot_line( ID_GRAPHIC_LAYER id, DWORD start_x, DWORD start_y, DWORD end_x, DWORD end_y, DWORD thickness, DWORD color )
{
	command.command_type = GRAPHIC_COMMAND_PLOT_LINE;
	command.arg[ 0 ] = id;
	command.arg[ 1 ] = start_x;
	command.arg[ 2 ] = start_y;
	command.arg[ 3 ] = end_x;
	command.arg[ 4 ] = end_y;
	command.arg[ 5 ] = thickness;
	command.arg[ 6 ] = color;
	graphic_send_command( &command );
}

VOID graphic_move_direct( ID_GRAPHIC_LAYER id, DWORD x, DWORD y )
{
	command.command_type = GRAPHIC_COMMAND_MOVE_DIRECT;
	command.arg[ 0 ] = id;
	command.arg[ 1 ] = x;
	command.arg[ 2 ] = y;
	graphic_send_command( &command );
}

VOID graphic_update_screen( )
{
	command.command_type = GRAPHIC_COMMAND_UPDATE_SCREEN;
	graphic_send_command( &command );
	graphic_flush_recv_command_queue( );
}


VOID graphic_init( )
{
	graphic_flush_recv_command_queue( );
}
