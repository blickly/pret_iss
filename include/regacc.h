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

#ifndef _REGACC_H_
#define _REGACC_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc>
#endif /* _NO_SYSTEMC_ */

#include "instruction_defs.h"
#include "hw_thread_ptr.h"
#include "module_base.h"
//#include "dma.h"
#include <stdint.h>

///////////////////////////////////////////////////////////////////////
/// regacc
/**
 * This class performs the register accesses for the particular
 * thread. This involves loading the values from the register file of
 * that thread.
 *
 * @author  Hiren Patel, Isaac Liu
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class regacc : public module_base {
public:
    /** Constructs the register access stage.
     *
     *  @param name Unique string identifying the instance of the
     *  register access stage.
     */
    regacc(const sc_module_name& name);

///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
#ifdef _NO_SYSTEMC_
    /** Takes an instruction from the input <i>in</i> and retrivies
     *  the register values from the register file. It sets the
     *  destination registers to be written to in the exception stage
     *  as well. The altered state of the hardware thread is forwarded
     *  to the execute stage.
     *  This is the non-SystemC version.
     *
     *  @param input_thread Incoming thread from the fetch stage.
     *  @param output_thread Outgoing thread to the execute stage.
     */
    void behavior(const hw_thread_ptr& input_thread, hw_thread_ptr& output_thread);
#else
    /** Takes an instruction from the <i>in</i> port and retrieves the
    register values from the register file.
     *  This is only used with SystemC.
     */
    void behavior();
#endif /* _NO_SYSTEMC_ */

///////////////////////////////////////////////////////////////////////
//                       public variables                           ///
    /** Input and output ports for SystemC.
     */
    sc_in< hw_thread_ptr > in;
    sc_out< hw_thread_ptr > out;
    //  sc_port< dma_if > coproc_dma;
private:
    /** Default constructor's use is disallowed.
     */
    regacc();

///////////////////////////////////////////////////////////////////////
//                       private methods                            ///
    /** Returns true if the hardware thread is not initialized yet.
     *  @param ht The hardware thread pointer.
     */
    bool _is_not_valid_hardware_thread(const hw_thread_ptr& hardware_thread);
    /** Prints debug information if <i>DBG_PRINT</i> flag is set.
     *  @param hardware_thread The hardware thread pointer.
     */
    void _debug_print(const hw_thread_ptr& hardware_thread);

    /** Checks if the current instruction is a double word
     *  instruction. If it is then a flag <i>db_word</i> is set to
     *  inform the exception stage to not forward the program counter
     *  but only increment the address for the destination register.
     *  @param hardware_thread The hardware thread pointer.
     */
    void _double_word_instruction(const hw_thread_ptr& hardware_thread);

    /** If the instruction is a deadline instruction then it must access
     *  the corresponding registers in the register file. If the
     *  destination register is greater than 7, then PLL deadlines are
     *  to be used. Otherwise regular deadlines.
     *  @param hardware_thread The hardware thread.
     */
    void _deadline_instruction(const hw_thread_ptr& hardware_thread);

    /** Processes the requests to the PLL deadlines. Sets the
     *  <i>dead_stalled</i> flag if the PLL deadline mailbox already has
     *  a value in it. Otherwise, if the mailbox is clear and no value
     *  is loaded into the PLL deadline register then the corresponding
     *  flags are set such that the specified deadline value will be
     *  loaded into the mailbox in the exception stage. If however, the
     *  mailbox is clear and there is no loaded value in the PLL
     *  deadline register, then reset the <i>pll_loaded</i> flag to
     *  false.
     *  @param hardware_thread The hardware thread.
     */
    void _destination_phaselockloop_deadlines(const hw_thread_ptr& hardware_thread);

    /** Check if the regular deadline registers are less than equal to
     *  0. If they are then set the flag to allow writing to the deadline
     *  register in the exception stage. However, if the deadline register
     *  value is not less than or equal to 0, then we must stall the
     *  thread by setting <i>dead_stalled</i> to true.
     *  @param hardware_thread The hardware thread.
     */
    void _destination_regular_deadlines(const hw_thread_ptr& hardware_thread);

    /** Check whether the immediate bit was set in the instruction. If
     *  it was then load the immediate value, otherwise get the value
     *  from the register file.
     *  @param hardware_thread The hardware thread.
     */
    void _immediate_instruction(const hw_thread_ptr& hardware_thread);

    /** Store the value in register g1 to the DMA controller's
     *  destination register 0. This is invoked by calling the STC
     *  instruction. There is done immediately in the register access
     *  stage and the rest of the stages treat this instruction as a
     *  NOOP.
     * param hardware_thread The hardware thread.
     */
    //  void _make_dma_transfer(const hw_thread_ptr& hardware_thread);

    /** Print a warning message out if the deadlines are being
     *  missed. This is only for regular deadlines and not the PLL ones.
     *  @param hardware_thread The hardware thread.
     */
    void _warn_missing_deadlines(const hw_thread_ptr& hardware_thread);
};
#endif /* _REGACC_H_ */
