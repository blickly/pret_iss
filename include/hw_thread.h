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

#ifndef _HW_THREAD_H_
#define _HW_THREAD_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif /* _NO_SYSTEMC_ */

#include <stdint.h>

#include "defines.h"
#include "instruction.h"
#include "memory_controller.h"
#include "register_file.h"
#include "special_reg.h"
///////////////////////////////////////////////////////////////////////
/// hw_thread
/**
 * The state of a thread is captured in this structure.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */

class hw_thread {
public:

    /** Construct an empty hardware thread.
     */
    hw_thread();

    /** Overloaded constructor.
     *
     * @param in_id Thread id.
     * @param pc Starting program counter.
     */
    hw_thread(unsigned int in_id, uint32_t pc);

    /** Copy constructor.
     *
     *  @param hardware_thread Copy from this hardware thread.
     */
    hw_thread(const hw_thread& hardware_thread);

///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
public:
    /** Returns the thread id.
     *
     * @return Thread id.
     */
    unsigned int get_id();

    /** Returns the trapped state
     *
     * @return trapped state.
     */
    short get_trapped_state();

    /** Return the current thread program counter.
     *
     * @return Thread's program counter.
     */
    uint32_t get_pc() const;

    /** Return the delayed branch address.
    *
    * @return Delayed branch address.
    */
    uint32_t get_delayed_branch_address() const;

    /** Returns the trap type of the current thread
    *
    * @return trap type
    */
    short get_trap_type() const;

    /** Returns true if an exception occurred during the execution
    *
    * @return True if an exception occurred 
    */
    bool is_trapped() const;

    /** Returns true if stall is caused by a double word instruction.
    *
    * @return True if double word instruction caused the stall, else false.
    */
    bool is_db_word_stalled() const;

    /** Returns true if stall is caused by a deadline instruction.
     *
     * @return True if deadline caused the stall, else false.
     */
    bool is_deadline_stalled() const;

    /** Return if the thread is enabled.
     *
     * @return True if thread is enabled for processing, else false.
     */
    bool is_enabled() const;

    /** Returns true if stall is caused by an instruction fetch to the memory.
     *
     * @return True if fetch caused the stall, else false.
     */
    bool is_fetch_stalled() const;

    /** Returns true if stall is caused by a data access to memory.
     *
     * @return True if memory stage caused the stall, else false.
     */
    bool is_memory_stalled() const;


    /** Overloaded assignment operator.
     *
     * @param hardware_thread Thread object to assign.
     */
    void operator=(const hw_thread& hardware_thread);

    /** Overloaded equal comparison operator.
     *
     * @param hardware_thread Thread object to compare with.
     * @return Boolean if equal.
     */
    bool operator==(const hw_thread& hardware_thread);

    /** Overloaded output (<<) operator.
     *
     * @param out Output stream.
     * @param hardware_thread Thread object to output.
     * @return Output stream.
     */
    inline friend ostream& operator<<(ostream& out, const hw_thread& hardware_thread) {
        out << "+ hw_thread id: " << hardware_thread._id << ' ' << hardware_thread.inst;
        return out;
    }

    /** Print out registers in current register window.
     */
    void regdump();

    /** Reset the register windows.
     */
    void reset_register_window();

#ifndef  _NO_SYSTEMC_
    /** Dump traces to file when using SystemC. Currently this is empty.
     *
     * @param tf Output trace file.
     * @param hardware_thread Thread object to dump to trace file.
     * @param str Associate string with object.
     */
    inline friend void sc_trace(sc_trace_file* tf, const hw_thread& hardware_thread, const string& str) {
        // Do nothing
    }
#endif /* _NO_SYSTEMC_ */

    /** Sets the trap type of this thread from an exception
    *
    * @param type The trap number that occured
    */
    void set_trap_type(const short& type);


    /** Set the state if stall is caused by a double word instruction.
     *
     * @param stall True if stalled, else false.
     */

    void set_db_word_stalled(const bool& stall);

    /** Set the state if stall is caused by a deadline instruction.
     *
     * @param stall True if stalled, else false.
     */

    void set_deadline_stalled(const bool& stall);

    /** Set the delayed branch address.
    *
    * @param address Address for delayed branch.
    */
    void set_delayed_branch_address(const uint32_t& address);

    /** Set the enabled state of the thread.
    *
    * @param enable True if thread is going to be enabled, else false.
    */
    void set_enabled(const bool& enable);

    /** Set the state if stall is caused by instruction fetch from memory.
     *
     * @param stall True if stalled, else false.
     */
    void set_fetch_stalled(const bool& stall);

    /** Set the thread id.
    *
    * @param in_id New thread identifier.
    */
    void set_id(const unsigned int& in_id);

    /** Set the state if stall is caused by a data memory access.
     *
     * @param stall True if stalled, else false.
     */
    void set_memory_stalled(const bool& stall);

    /** Set processor trapped if an exception occurred
    *
    * @param trapped True if exception occurred, else false
    */
    void set_trapped(const short& trapped);


    /** Set the thread's program counter.
     *
     * @param pc New pc of thread.
     */
    void set_pc(const uint32_t& pc);


///////////////////////////////////////////////////////////////////////
//                       public  variables                          ///

    unsigned int cnt_cycles; // Count the number of cycles this thread executes.
    unsigned int cnt_instr; // Count the number of instructions this thread executes.
    instruction inst; // Instruction data structure.
    register_file regs; // Registers.
    special_reg spec_regs; // Special registers with timers.


///////////////////////////////////////////////////////////////////////
//                       private variables                          ///
private:
    bool _enabled;          // If the thread is enabled.
    uint32_t _branch_slot;  // Delay branch  address.
    bool _db_word_stalled;  // Current instruction being executed is a double word.
    bool _deadline_stalled; // If the thread is currently in a state of replay.
    bool _memory_stalled;   // If the memory stage causes a stall.
    bool _fetch_stalled;    // If the fetch stage causes a stall.
    unsigned int _id;       // Thread identifier.
    uint32_t _pc;           // Thread's current program counter.
    short _trapped;          // If the thread encounters an exception
    short _trap_type;        // If the trap type of the thread
    

};

#endif /* _HW_THREAD_H_ */
