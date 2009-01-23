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
*/

#ifndef _CORE_H_
#define _CORE_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif /* _NO_SYSTEMC_ */

#include <vector>
#include <algorithm>

#include "cycle_counter.h"
#include "hw_thread.h"
#include "hw_thread_ptr.h"
#include "hw_thread_controller.h"
#include "fetch.h"
#include "decode.h"
#include "except.h"
#include "execute.h"
#include "mem.h"
#include "l1_scratch.h"
#include "dma.h"
#include "memory_controller.h"
#include "regacc.h"
#include "srec_parser.h"
#include "static_bound_parser.h"

///////////////////////////////////////////////////////////////////////
/// core
/**
 * The core class creates instances of the PRET architecture. It
 * connects the instances together. This class is used to encapsulate
 * methods that execute the processor as well.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */

class core {
public:
    /** Construct an instance of the PRET processor.
     *   @param name Unique identifier for the processor.
     */
    core(const string& name);

    /** Destroy dynamically created instances for the processor core.
     */
    ~core();

///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
public:
    /** Execute one cycle of the processor.
     */
    void behavior();

    /** Return if there is a memory read operation.
     *
     * @param thread_id Thread identifier.
     * @return True if memory read operation, otherwise false.
     */
    bool is_memory_read(int thread_id) const;

    /** Return if there is a memory write operation.
     *
     * @param thread_id Thread identifier.
     * @return True if memory write operation, otherwise false.
     */
    bool is_memory_write(int thread_id) const;

    /** Return aligned memory address.
     *
     * @param thread_id Thread identifier.
     * @return Aligned memory address.
     */
    int get_aligned_memory_address(int thread_id) const;

    /** Check if access to main memory is stalled.
     *
     * @param thread_id Thread identifier.
     * @return True if memory wheel causes a stall, othewrise false.
     */
    bool is_memory_stalled(int thread_id) const;

    /** Return the current cycle count.
     *  @return The current cycle count.
     */
    unsigned int get_cycle_count() const;

    /** Return the current instruction count.
     *  @return The current instruction count.
     */
    unsigned int get_instruction_count(int thread_id) const;

    /** Return the value at the specified memory address.
     *  @param addr Address in memory.
     *  @return The memory contents at address addr.
     */
    uint32_t get_memory_data(uint32_t addr) const;

    /** Return the program counter for a specific thread.
     *  @param thread_id The thread number.
     *  @return The program counter for thread thread_id.
     */
    uint32_t get_pc(int thread_id) const;

    /** Return the period of the main system clock.
     *  @return The period of main system clock.
     */
    long int get_period() const;

    /** Return a pointer to the thread.
     *  @param thread_id The thread number.
     *  @return A pointer to the thread.
     */
    hw_thread_ptr*  get_thread(int thread_id) const;

    /** Returns whether fetch stage has caused a stall.
     *  @return True if fetch is stalled otherwise false.
     */
    bool is_fetch_stalled(int thread_id) const;

    /** Print the contents of main memory.
     */
    void print_memory();

    /** Print the contents of the registers for a thread.
     *  @param thread_id The thread number.
     */
    void print_registers(int thread_id);

    /** Print the contents of deadline timers for a thread.
     *  @thread_id The thread number.
     */
    void print_timers(int thread_id);

    /** Run the PRET simulator for cycle_count number of cycles.
     *  @param cycle_count The number of cycles.
     *  @return True if the processor has halted. Otherwise false.
     */
    bool run(int cycle_count);

    /** Run the PRET simulator for num_inst number of instructions.
     *  @param num_inst The number of instructions.
     *  @return True if the processor has halted. Otherwise false.
     */
    bool run_for_instruction(int instruction_count);

///////////////////////////////////////////////////////////////////////
//                       private variables                          ///
private:
    /** Pointers assigned to the instances of main clock (_clock) and a
     *  phase lock-loop clock (_pll_clock).
     */
    sc_clock* _clock;
    sc_clock* _pll_clock;

    /** Pointers assigned to an instance of a cycle counter.
     */
    cycle_counter* _cycle_counter;

    /** Pointer assigned to memory controller.
     */
    memory_controller* _memory_controller;

    /** The six instruction scratchpads.
     */
    l1_scratch* _instruction_spm[NUM_THREADS];
    /** Pointer assigned to the thread controller.
     */
    hw_thread_controller* _thread_controller;

    /** Pointer assigned to the blocking dma controller.
     */
    dma * _dma_controller;
    /** Pointers assigned to the respective stages of the pipeline.
     */
    fetch* _fetch_stage;
    decode* _decode_stage;
    regacc* _regacc_stage;
    execute* _execute_stage;
    mem* _memory_stage;
    except* _except_stage;

    /** These are connections or channels between the stages. The
     *  SystemC version uses sc_signal channels. The C++ version uses
     *  instances of hw_thread_ptr.
     */
#ifdef _NO_SYSTEMC_
    hw_thread_ptr _pool_to_f, _f_to_d, _d_to_ra, \
    _ra_to_ex, _ex_to_mem, _mem_to_except;
#else
    sc_signal< hw_thread_ptr > _pool_to_f, _f_to_d, _d_to_ra, \
    _ra_to_ex, _ex_to_mem, _mem_to_except;
#endif /* _NO_SYSTEMC_ */
};
#endif /* _CORE_H_ */
