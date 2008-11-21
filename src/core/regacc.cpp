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


#include "regacc.h"

regacc::regacc(): module_base() {

}

regacc::regacc(const sc_module_name& name): module_base(name) {

}

#ifdef _NO_SYSTEMC_
void regacc::behavior(const hw_thread_ptr& input_thread, hw_thread_ptr& output_thread) {
#else
void regacc::behavior() {
    hw_thread_ptr input_thread = in.read();
#endif /* _NO_SYSTEMC_ */

    if (_is_not_valid_hardware_thread(input_thread)) {
        //    if (input_thread.handle == NULL || !input_thread->enabled) {
#ifdef _NO_SYSTEMC_
        output_thread = input_thread;
#else
        out.write(input_thread);
#endif /* _NO_SYSTEMC_ */
        return;
    }

    /* Added to handle deadlines.  This checks if the current
     * instruction being processed is a deadline instruction. This
     * also checks if the deadline has been reached.
     */
    _deadline_instruction(input_thread);

//     /* Store g1 to DMA controller register. */
//     _make_dma_transfer(input_thread);

    /* Check and process if the current instruction is tagged as a
     * double word instruction.
     */
    _double_word_instruction(input_thread);

    /* Check and process if the instruction was set to be an immediate.
     */
    _immediate_instruction(input_thread);

    input_thread->inst.op3_val = input_thread->regs.get_reg(input_thread->inst.rd, input_thread->spec_regs.curr_wp);
    input_thread->spec_regs.curr_wp += input_thread->inst.wp_increment;
    input_thread->spec_regs.curr_wp %= REGISTER_WINDOWS;
    // FIXME: delete sp_reg.wp in instruction class
    input_thread->inst.sp_reg.wp = input_thread->spec_regs.curr_wp;

    _debug_print(input_thread);
#ifdef _NO_SYSTEMC_
    output_thread = input_thread;
#else
    out.write(input_thread);
#endif /* _NO_SYSTEMC_ */
}

bool regacc::_is_not_valid_hardware_thread(const hw_thread_ptr& hardware_thread) {
    return (hardware_thread.is_null() || !hardware_thread->is_enabled()
            || hardware_thread->is_fetch_stalled());
}

void regacc::_debug_print(const hw_thread_ptr& hardware_thread) {
#ifdef DBG_PIPE
    cout << "*regacc*" << "  (" << sc_time_stamp() << ") ";
    cout << "hw_thread's id: " << hardware_thread->get_id() << ", pc: 0x" << hex << hardware_thread._handle->get_pc() << hex <<  ", " << hardware_thread->inst ;
    cout   << "\t+ "  << "op1_val: "  << dec << hardware_thread->inst.op1_val
           << ", op2_val: "  << hardware_thread->inst.op2_val
           << ", wp: " << hardware_thread->spec_regs.curr_wp << endl;
#endif /* DBG_PIPE */
}

void regacc::_double_word_instruction(const hw_thread_ptr& hardware_thread) {
    if ((hardware_thread->db_word) && (!hardware_thread->is_deadline_stalled())) {
        hardware_thread->inst.rd += 1;
        // Increment rs1 by 4
        hardware_thread->inst.op1_val =  hardware_thread->regs.get_reg(hardware_thread->inst.rs1, hardware_thread->spec_regs.curr_wp) + 4;
        /* If the instruction is a double LD, then we have to increment
           the destination register as well */
        // Reset the flag indicating the double word was handled.
        hardware_thread->db_word = false;
        // Reset the instruction's double word flag off since it's handled.
        hardware_thread->inst.db_word = false;
    } else {
        hardware_thread->inst.op1_val = hardware_thread->regs.get_reg(hardware_thread->inst.rs1, hardware_thread->spec_regs.curr_wp);
    }

}

void regacc::_deadline_instruction(const hw_thread_ptr& hardware_thread) {
    if (hardware_thread->inst.mux_specreg == SREG_DT) {
        if (hardware_thread->inst.rd > 7)  {
            _destination_phaselockloop_deadlines(hardware_thread);
        } else {
            _destination_regular_deadlines(hardware_thread);
        }
    }

}

void regacc::_destination_phaselockloop_deadlines(const hw_thread_ptr& hardware_thread) {
    /* This implements a mailbox transfer mechanism.
       If something is in the mailbox then we will stall no matter what.
    */
    if (hardware_thread->spec_regs.pll_load[hardware_thread->inst.rd-8] != 0) {
        hardware_thread->set_deadline_stalled(true);
    }
    //IF WE HAVEN'T LOADED ANYTHING, AND MAILBOX IS CLEAR, WE LOAD IN MAILBOX
    //AND STALL UNTIL MAILBOX IS CLEARED
    else if (hardware_thread->spec_regs.pll_load[hardware_thread->inst.rd - 8] == 0 && !hardware_thread->spec_regs.pll_loaded) {
        hardware_thread->set_deadline_stalled(true);
        hardware_thread->inst.wsreg = true;
        hardware_thread->spec_regs.pll_loaded = true;
    }
    //IF WE LOADED SOMETHING ALREADY, AND MAILBOX IS CLEARED, WE ARE GOOD TO GO
    else if (hardware_thread->spec_regs.pll_load[hardware_thread->inst.rd-8] == 0 && hardware_thread->spec_regs.pll_loaded) {
        //USED TO RESET THE PLL_LOADED BIT
        hardware_thread->spec_regs.pll_loaded = false;
    }

}

void regacc::_destination_regular_deadlines(const hw_thread_ptr& hardware_thread) {
    /*  If the deadline is less than or equal to 0 then we
    *  should reset it if there is a new one availabile.
    */
    if (hardware_thread->spec_regs.dt[hardware_thread->inst.rd] <= 0) {
        hardware_thread->inst.wsreg = true;
    } else {
        hardware_thread->set_deadline_stalled(true);
    }
    /*  If a deadline is being missed then print ouf a warning
     *  saying that the deadlines are being missed.
     */
#ifdef DBG_WARN_MISSED_DEADLINE
    _warn_missing_deadlines(hardware_thread);
#endif /* DBG_WARN_MISSED_DEADLINE */
}

void regacc::_immediate_instruction(const hw_thread_ptr& hardware_thread) {

    if (hardware_thread->inst.use_imm) {
        hardware_thread->inst.op2_val = hardware_thread->inst.imm;
    } else {
        hardware_thread->inst.op2_val = hardware_thread->regs.get_reg(hardware_thread->inst.rs2, hardware_thread->spec_regs.curr_wp);
    }

}

// void regacc::_make_dma_transfer(const hw_thread_ptr& hardware_thread) {
//   if (hardware_thread->inst.op3 == OP3_CPOP1) {
//    cout << "regacc:: Make the DMA transfer. Fetch stall state true." << endl;
//    coproc_dma->make_tranfer(hardware_thread);
//   }
// }
void regacc::_warn_missing_deadlines(const hw_thread_ptr& hardware_thread) {
    uint32_t dead_timer = 0;
    bool missed_deadline = hardware_thread->spec_regs.missed_deadline(dead_timer);
    if (missed_deadline) {
        cout << "Warning: Missed deadline in thread " << hardware_thread->get_id() << ", dt reg: "
             << dead_timer << ", pc: "  << hex << hardware_thread->get_pc() << endl;
    }
}
