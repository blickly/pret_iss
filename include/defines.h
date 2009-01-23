/*
  Copyright (c) 2007-2009 The Regents of the University of California.
  All rights reserved.

  Permission is hereby granted, without written agreement and without
  license or royalty fees, to use, copy, modify, and distribute this
  software and its documentation for any purpose, provided that the
  above copyright notice and the following two paragraphs appear in all
  copies of this software.

  IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
  ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
  THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.

  THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
  PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
  CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
  ENHANCEMENTS, OR MODIFICATIONS.

  PT_COPYRIGHT_VERSION_2
  COPYRIGHTENDKEY

  $Id$
*/

#ifndef _DEFINES_H_
#define _DEFINES_H_

///////////////////////////////////////////////////////////////////////
//                       Debug specific flags.                      ///


/** Print out debugging information for the pipeline.
 */
//#define DBG_PIPE

/** Print out detailed memory access information.
 */
//#define DBG_MEM

/** Print out register state information.
 */
//#define DBG_REG

/** Perform instruction tracing.
 */
//#define DBG_INST_TRACE

/** Write to the screen when written to the UART address.
 */
#define DBG_UART

/** Print out a message if there are any instructions not implemented yet.
 */
//#define DBG_UNMIP

/** Print out the threads being fed into the pipeline from the hardware
 *  thread controller.
 */
//#define DBG_THREAD_CONT

/** Write output to a file.
 */
#define DBG_FOUT

/** Warn if there are missing deadlines.
 */
//#define DBG_WARN_MISSED_DEADLINE

///////////////////////////////////////////////////////////////////////
//                       Thread specific defines.                   ///
/** Maximum number of threads.
 */
#define MAX_THREAD 1000

/** Number of threads in current architecture.
 */
#define NUM_THREADS 6

///////////////////////////////////////////////////////////////////////
//                       Memory specific defines.                   ///
/** The number of cycles for a slot in the memory wheel's window.
 */
#define MEM_DELAY 15

/** The access delay to the scratchpad memories.
 */
#define SCRATCH_DELAY 1

/** The scratchpad size.
 */
#define SCRATCH_SIZE 0x10000 /* Normal size (0x10000 = 64 KB) */

/** Access delay from the programmable DMA.
 */
#define BURST_DELAY 4

///////////////////////////////////////////////////////////////////////
//                 Register window specific defines.                ///
/** Number of register windows.
 */
#define REGISTER_WINDOWS 128

///////////////////////////////////////////////////////////////////////
//                 Deadline timer specific defines.                 ///
/**  Number of deadline timers.
 */
#define NUM_DEADLINE_TIMERS 8

/** Number of PLL timers.
 */
#define NUM_PLL_TIMERS 1

///////////////////////////////////////////////////////////////////////
//                 DMA specific defines.                            ///
/** Conventional DMA that allows adding instructions in the program to
 *  transfer the instructions from main memory to scratchpad.
 */
#define USE_BLOCKING_DMA

///////////////////////////////////////////////////////////////////////
//                 Simulation specific defines.                     ///
/** Return from the simulation with special instruction.
 */
#define END_SIMULATION \
  asm(".word 0x22222222");

/** If _NO_SYSTEMC_ is not defined then it will use SystemC.
 */
#ifndef _NO_SYSTEMC_
#define _USE_SYSTEMC_
#endif

#endif /* _DEFINES_H_ */
