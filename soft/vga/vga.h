/*
 *      VGA-parameters definitions
 *
 *               Sungjun Kim
 *
 * This file comprises parameters for VGA driver.
 *
 *
 * 2008.03.27 - The file is created.
 *
 */

#ifndef VGA_H
#define VGA_H

#define VGA_RESOLUTION_WIDTH 640
#define VGA_RESOLUTION_HEIGHT 480

#define N_VGA_PIXEL_PER_ONCE 16

#define VGA_CLK     25.175

#define N_VGA_CLK_PIXEL   N_VGA_PIXEL_PER_ONCE

#define N_VGA_CLK_H_SYNC  96
#define N_VGA_CLK_H_BP   48
#define N_VGA_CLK_H_FP   24

#define N_VGA_CLK_V_SYNC  1641
#define N_VGA_CLK_V_BP   26153
#define N_VGA_CLK_V_FP   8974

#define VGA_COLOR_V_SYNC  0x00000000
#define VGA_COLOR_V_BP   0x00000000
#define VGA_COLOR_V_FP   0x00000000

#define VGA_COLOR_H_SYNC  0x00000000
#define VGA_COLOR_H_BP   0x00000000
#define VGA_COLOR_H_FP   0x00000000

#define VGA_CONTROL_V_SYNC_ON 0x03
#define VGA_CONTROL_V_SYNC_OFF 0x02
#define VGA_CONTROL_V_BP_ON  0xFF  // TBD
#define VGA_CONTROL_V_BP_OFF 0xFF  // TBD
#define VGA_CONTROL_V_FP_ON  0xFF  // TBD
#define VGA_CONTROL_V_FP_OFF 0xFF  // TBD


#define VGA_CONTROL_H_SYNC_ON 0x01
#define VGA_CONTROL_H_SYNC_OFF 0x00
#define VGA_CONTROL_H_BP_ON  0xFF  // TBD
#define VGA_CONTROL_H_BP_OFF 0xFF  // TBD
#define VGA_CONTROL_H_FP_ON  0xFF  // TBD
#define VGA_CONTROL_H_FP_OFF 0xFF  // TBD

#define VGA_COLOR_BLACK   0x00
#define VGA_COLOR_RED   0x01
#define VGA_COLOR_GREEN   0x02
#define VGA_COLOR_WHITE   0x03

#endif //VGA_H