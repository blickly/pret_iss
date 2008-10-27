/*
 *      Macro functions
 *
 *            Sungjun Kim
 *
 * This file has macro functions that is frequently used.
 *
 *
 * 2008.03.27 - The file is created.
 *
 */

#ifndef TOOL_H
#define TOOL_H

#define DEAD( timeout )      asm( "mov %0, %%g1 \n\t .word 0xC3600001" : : "r" ( timeout ) : "g1" )
#define DEAD_PLL( timeout )     asm( "mov %0, %%g1 \n\t .word 0xC3600001" : : "r" ( timeout ) : "g1" )

#define SET_PLL_CLK( hz )

#define IOWR_8DIRECT( base, offset, data ) { *( base + offset ) = ( data & 0xFF ); }
#define IOWR_16DIRECT( base, offset, data ) { *( base + offset ) = ( data & 0xFFFF ); }
#define IOWR_32DIRECT( base, offset, data ) { *( base + offset ) = ( data ); }

#define IORD_8DIRECT( base, offset )  ( ( ( const unsigned char ) *( base + offset ) ) & 0xFF )
#define IORD_16DIRECT( base, offset )  ( ( ( const unsigned short ) *( base + offset ) ) & 0xFFFF )
#define IORD_32DIRECT( base, offset )  ( ( const unsigned int ) *( base + offset ) )

#endif //TOOL_H
