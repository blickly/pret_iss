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

  $Id$

*/

#ifndef _MEM_CONTROLLER_H_
#define _MEM_CONTROLLER_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#include <iostream>
#include <fstream>
using namespace std;
#else
#include "systemc.h"
#endif

#include "memory_unit.h"
#include "module_base.h"
#include "defines.h"
#include "srec_parser.h"
#include "mem_location.h"
#include "l1_scratch.h"
#include "cycle_counter.h"

/** Forward declaration for blocking DMA interface. */
class dma_if;

#include <string>
#include <stdint.h>
using namespace std;

/* Interface for data memory */
struct mem_stage_if : public sc_interface {
    virtual void write_data(int tid, uint32_t addr, uint32_t data, bool& stalled) = 0;
    virtual uint32_t read_data(int tid, uint32_t addr, bool& stalled) = 0;

};

/* Interface for instruction memory */
struct fetch_stage_if : public sc_interface {
    virtual uint32_t read_inst(int tid, uint32_t addr, bool& stalled) = 0;
};

///////////////////////////////////////////////////////////////////////
/// memory_controller
/**
 * The memory controller effectively acts as in interface between the
 * integer unit and the memory units.  It determines to where an access to memory
 * should go and routes it there.
 * The memory controller also defines the behavior of the memory-mapped I/O
 * addresses.
 *
 * @author  Ben Lickly
 * @version $Id$
 * @Pt.ProposedRating red blickly
 * @Pt.AcceptedRating
 */
class memory_controller : public module_base, public fetch_stage_if, public mem_stage_if {
public:
///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
    /** Construct and initialize the memory controller.
     *  @param str Unique string idetifier for the memory controller.
     *  @param cyc Cycle counter with which to synchronize controller behavior.
     */
    memory_controller(const sc_module_name& nm, cycle_counter*  cyc);

    /** Destroy this memory controller.
     */
    virtual ~memory_controller();

    /** Update any internal state of the memory locations to take into account
     *  that one cycle has passed.
     */
    virtual void behavior();

    /** Print out contents of all scratchpads and main memory to
     *  standard output.
     */
    void dump();

    /** Return the scratchpad for a given thread.
     *  Takes in an integer as a thread id number and returns a reference to
     *  the l1 scratch for the corresponding scratchpad.
     *  @param tid Thread id number of the corresponding thread.
     *  @return l1_scratch Reference to the desired scratchpad.
     */
    virtual l1_scratch& get_spm_mem_loc(int tid);

    /** Return the shared memory location corresponding to the given address.
     *  Takes in an address and returns a reference to either the main memory
     *  or the memory-mapped I/O, depending on which is more appropriate.
     *  @param addr Memory address to perform selection.
     *  @return mem_location Reference to the desired memory location.
     */
    virtual mem_location& get_main_mem_loc(uint32_t addr);

    /** Register a group of scratchpads to be used by the memory controller.
     *  @param spm[] An array of pointers to the scratchpads to use.
     */
    void register_inst_spm(l1_scratch* spm[]);

    /** Perform a read by thread <i>tid</i> to address <i>addr</i> through
     *  the data memory interface. This method uses the thread id and address
     *  to determine from which memory location the data should be read.
     *  The data at that location is returned directly from this method and
     *  a boolean to tell weather the access stalled or not is returned
     *  through the final parameter.
     *  @param tid The thread id number of the requesting thread.
     *  @param addr The 32-bit address to be accessed.
     *  @param stalled An output parameter that tells if this access stalled.
     *  @return The actual 32-bits of data that were (or would be) read.
     */
    uint32_t read_data(int tid, uint32_t addr, bool& stalled);

    /** Perform a read by thread <i>tid</i> to address <i>addr</i> through
     *  the instruction memory interface. This method uses the thread id and
     *  address to determine from which memory location the instruction
     *  should be read.
     *  The instruction at that location is returned directly from this
     *  method and a boolean to tell weather the access stalled or not is
     *  returned through the final parameter.
     *  @param tid The thread id number of the requesting thread.
     *  @param addr The 32-bit address to be accessed.
     *  @param stalled An output parameter that tells if this access stalled.
     *  @return The actual 32-bits of instruction that were (or would be) read.
     */
    uint32_t read_inst(int tid, uint32_t addr, bool& stalled);

    /** Perform a write by thread <i>tid</i> to address <i>addr</i> through
     *  the data memory interface. This method uses the thread id and address
     *  to determine to which memory location the data should be written.
     *  A boolean to tell weather the write stalled or not is returned
     *  through the final parameter.
     *  @param tid The thread id number of the writing thread.
     *  @param addr The 32-bit address at which the write is to occur.
     *  @param data The 32-bits of data to be written.
     *  @param stalled An output parameter that tells if the write stalled.
     */
    void write_data(int tid, uint32_t addr, uint32_t data, bool& stalled);

///////////////////////////////////////////////////////////////////////
///                      public variables                           ///
    /** Port to connect to blocking DMA unit.
     */
    sc_port<dma_if> blocking_dma;

protected:
///////////////////////////////////////////////////////////////////////
///                      protected variables                        ///
    /** Instantiates the objects for the protected variables. This is
     * used internally by the constructor of the memory_controller
     * object.
     * @param cyc Cycle counter  pointer.
     */
    virtual void _setup(cycle_counter* cyc);

    l1_scratch** _inst_scratchpad;  /// Instruction scratchpad memories.
    mem_location* _main_mem;        /// Main memory.
    mem_location* _mem_mapped_io; /// Memory mapping.

};
#endif
