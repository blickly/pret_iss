#include "vga.h"
#include "graphic.h"

#ifndef GLOBAL_VARIABLE_C
#define GLOBAL_VARIABLE_C

// for vga
volatile DWORD * IO_ADDR_VGA = ( DWORD ) 0x88000000;

volatile BOOL * _is_refreshing = ( DWORD ) 0x90000000;
volatile DWORD * _current_buffer = ( DWORD ) 0x90000004;
volatile DWORD * _screen = ( DWORD ) 0x90000008;	// three dimensional, N_VGA_GRAPHIC_BUFFER, VGA_RESOLUTION_HEIGHT, ( VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE )
//volatile DWORD _screen[ N_VGA_GRAPHIC_BUFFER ][ VGA_RESOLUTION_HEIGHT ][ ( VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE ) ] = { 0x00000000, };


// for graphic

volatile struct Graphic_layer * _graphic_layer = ( DWORD ) 0x91000000;
volatile DWORD * _graphic_buffer = ( DWORD ) 0x92000000;
//volatile struct Graphic_layer _graphic_layer[ N_ID_GRAPHIC_LAYER ] = { 0, };
//volatile DWORD _graphic_buffer[ VGA_RESOLUTION_HEIGHT * ( VGA_RESOLUTION_WIDTH / N_VGA_PIXEL_PER_ONCE ) ] = { 0, };


// commands from the outside are stored in the command queue
#define SIZE_COMMAND_QUEUE	20
#define N_COMMAND_QUEUE		2
DWORD * _current_send_command_queue = ( DWORD ) 0x93000000;
DWORD * _current_recv_command_queue = ( DWORD ) 0x93000004;
BOOL * _is_flushing_recv_command		= ( DWORD ) 0x93000008;
//struct Graphic_command_list * _p_command_queue_front[ N_COMMAND_QUEUE ] = { NULL, NULL };
//struct Graphic_command_list * _p_command_queue_end[ N_COMMAND_QUEUE ] = { NULL, NULL };
//struct Graphic_command_list _command_queue[ N_COMMAND_QUEUE ][ SIZE_COMMAND_QUEUE ] = { 0, };

struct Graphic_command_list ** _p_command_queue_front = 0x9300000C;
struct Graphic_command_list ** _p_command_queue_end = 0x93000100;
struct Graphic_command_list * _command_queue = 0x93000200;

//DWORD * _is_queue_init = 0x93001000;

#endif	// GLOBAL_VARIABLE_C
