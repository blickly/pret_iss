/*
  Copyright (c) 2007-2008 The Regents of the University of California.
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

  $Author$
  $Date$
  $Id$

*/

#ifndef _DEFINES_H_
#define _DEFINES_H_

/* -------------- DEBUG specifics ---------------- */
//#define DBG_PIPE
//#define DBG_MEM /* Print out detailed information on memory accesses */
//#define DBG_REG /* Print out register dump */
//#define DBG_INST_TRACE /*Instruction trace*/
#define DBG_UART /* Debug UART */
//#define DBG_UNMIP /* Print out error for unimplemented instructions */
//#define DBG_THREAD_CONT /* Print out thread controller debug */
#define DBG_FOUT /* Debug driver for file output */
//#define DBG_WARN_MISSED_DEADLINE /* Debug information on missed deadlines */

/* -------------- Thread specifics ---------------- */
#define MAX_THREAD 1000
#define NUM_THREADS 6

/* -------------- Memory specifics ---------------- */
#define MEM_DELAY 15
#define SCRATCH_DELAY 1
#define PDMA_DELAY 4

/* -------------- Register window specifics ---------------- */
#define REGISTER_WINDOWS 128

/*--------------- Deadline Timers----------------------*/
#define NUM_DEADLINE_TIMERS 8
#define NUM_PLL_TIMERS 1

/*--------------- Blocking DMA  ----------------------*/
#define USE_BLOCKING_DMA

/*--------------- Quit Simulation ----------------------*/
#define END_SIMULATION \
  asm(".word 0x22222222");

#ifndef _NO_SYSTEMC_
#define _USE_SYSTEMC_
#endif

#endif /* _DEFINES_H_ */
