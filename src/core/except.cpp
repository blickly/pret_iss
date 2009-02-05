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

  $Author$
  $Date$
  $Id$

 */


#include "except.h"
#include <assert.h>
#include <iomanip>

uint32_t except::addr_calc(const hw_thread_ptr& hardware_thread) {
    //instruction const& inst) {
    if (hardware_thread->inst.is_branch()) {
        return hardware_thread->inst.get_pc() + (hardware_thread->inst.get_disp22()*4);
    } else if (hardware_thread->inst.is_call()) {
        return hardware_thread->inst.get_pc() + (hardware_thread->inst.get_disp30()*4);
    } else {
        assert(false);
    }

    return 0;
}
bool except::branch_check(const hw_thread_ptr& hardware_thread) {
//bool except::branch_check(const instruction& inst, unsigned char icc) {
    bool neg = hardware_thread->spec_regs.get_icc() & 0x08;
    bool zero = hardware_thread->spec_regs.get_icc() & 0x04;
    bool ovfl = hardware_thread->spec_regs.get_icc() & 0x02;
    bool carry = hardware_thread->spec_regs.get_icc() & 0x01;

    if (!hardware_thread->inst.is_branch() && ! hardware_thread->inst.is_call()) {
        return false;
    } else if (hardware_thread->inst.is_call()) {
        return true;
    } else if (hardware_thread->inst.get_conditional_branch() == BRCH_BA) {
        return true;
    } else if (hardware_thread->inst.get_conditional_branch() == BRCH_BN) {
        return false;
    } else {
        switch (hardware_thread->inst.get_conditional_branch()) {
        case BRCH_BNE:
            return !zero;
        case  BRCH_BE:
            return zero;
        case BRCH_BG:
            return !(zero | (neg ^ ovfl));
        case BRCH_BLE:
            return zero | (neg ^ ovfl);
        case  BRCH_BGE:
            return !(neg ^ ovfl);
        case  BRCH_BL:
            return neg ^ ovfl;
        case  BRCH_BGU:
            return !(carry | zero);
        case  BRCH_BLEU:
            return carry | zero;
        case  BRCH_BCC:
            return !carry;
        case  BRCH_BCS:
            return carry;
        case  BRCH_BPOS:
            return !neg;
        case  BRCH_BNEG:
            return neg;
        case  BRCH_BVC:
            return !ovfl;
        case  BRCH_BVS:
            return ovfl;
        default:
            return false;
        }
    }
}

#ifdef _NO_SYSTEMC_
void except::behavior(hw_thread_ptr & hardware_thread) {
#else
void except::behavior() {
    hw_thread_ptr hardware_thread = in.read();
#endif /* _NO_SYSTEMC_ */

    
    /* Check if the hardware thread is enabled */
    if (_is_not_valid_hwthread(hardware_thread)) {
        return;
    }
    /* Everytime an instruction goes through this stage, increase its
       cycle count to 6 */
    hardware_thread->cnt_cycles += 6;


    /* If fetch stage is stalled then just increment the cycle count
    and return. */
    if (hardware_thread->is_fetch_stalled()) {
        return;
    }

    hardware_thread->set_fetch_stalled(false);

    debug(hardware_thread);

    /* If the current instruction is a double word then set a flag to
       note it */
    set_dword_state(hardware_thread);

    /* If deadline is causing a stall then let skip out of this stage */
    if (dead_stalled(hardware_thread)) {
        return;
    }

    /* If the instruction fetch caused a stall then skip out of this
       stage */
    if (fetch_stalled(hardware_thread)) {
        return;
    }
    /* If memory is causing a stall then skip out of this stage */
    if (mem_stalled(hardware_thread)) {
        return;
    }

    if (!hardware_thread->is_db_word_stalled()) {
        /* Increment the PC based */
        inc_pc(hardware_thread);
    }

    write_regs(hardware_thread);
    write_special_regs(hardware_thread);

    /* Increment the number of instructions executed */
    hardware_thread->cnt_instr++;

}

bool except::dead_stalled(const hw_thread_ptr& hardware_thread) {

    /****************************************
     //ADDED FOR DEADLINE INSTRUCTION
     //IF DEADLINE IS NOT REACHED, NULL MEANS DON'T COMMIT
     //ANYTHING, SO WE CAN JUST RETURN FROM THIS STAGE
     ****************************************************************/
    if (hardware_thread->is_deadline_stalled()) {
#ifdef DBG_REG
        //   hardware_thread->spec_regs.dump();
        hardware_thread->spec_regs.dump_deadline_timers();

#endif
        //HACK!!! NEED TO CHANGE!!
        //STALLED BUT WANT TO UPDATE MAILBOX
        if (hardware_thread->spec_regs.get_pll_loaded() && hardware_thread->inst.is_write_special_registers()) {
            hardware_thread->spec_regs.set_pll_load(hardware_thread->inst.get_alu_result(), hardware_thread->inst.get_rd() - 8);
        }
        return true;
    }
    return false;
}

void except::debug(const hw_thread_ptr& hardware_thread) {
#ifdef DBG_PIPE
    cout << "*eXcept*" << "  (" << sc_time_stamp() << ") ";
    cout << "hw_thread's id: " << hardware_thread->get_id() << ", pc: 0x" << hex << hardware_thread._handle->PC << ", " << hex << hardware_thread->inst;
    cout << "\t+ pc: " << hex << hardware_thread->PC << " branch: " << hardware_thread->get_delayed_branch_address() << " icc: " << hex << (uint32_t)(hardware_thread->spec_regs.icc) << dec << endl;
#endif

#ifdef DBG_INST_TRACE

    cout <<  "( " << sc_time_stamp() << " ) "
         << hex <<  "pc: " << setw(8) << setfill('0') << hardware_thread->PC << "\t"
         <<  hardware_thread->inst << endl;
#endif

#ifdef DBG_UNMIP
    if (hardware_thread->inst.unimp) {
        cerr << "thread id: " << hardware_thread->get_id()
             << ", pc: 0x" << hex << hardware_thread._handle->PC
             << ", " << hex << hardware_thread->inst << endl;
        cerr << "\t+ INSTRUCTION IS NOT IMPLEMENTED" << endl;
    }
#endif
}

except::except(): module_base() {

}

except::except(const sc_module_name & str): module_base(str) {
#ifndef _NO_SYSTEMC_
    dont_initialize();
#endif
}

bool except::fetch_stalled(const hw_thread_ptr& hardware_thread) {
    /* A stall from the fetch stage causes the exception stage to skip out
     just after the deadline registers are updated */
    return (hardware_thread->is_fetch_stalled());
};

bool except::_is_not_valid_hwthread(const hw_thread_ptr& hardware_thread) {
    return (hardware_thread.is_null() || !hardware_thread->is_enabled());
}

bool except::mem_stalled(const hw_thread_ptr& hardware_thread) {
    /* A stall from the memory causes the exception stage to skip out
     just after the deadline registers are updated */
    return (hardware_thread->is_memory_stalled());
};

void except::set_dword_state(const hw_thread_ptr& hardware_thread) {

    // HDP: If the instruction is a double anything we need to redo the
    // instruction except we need to increment the destination address.
    if ((hardware_thread->inst.is_db_word()) && (!hardware_thread->is_db_word_stalled())) {
        // The PC is no longer incremented if a db_word is recognized.
        // Set the thread's double word flag to true.
        hardware_thread->set_db_word_stalled(true);
        //    cout << "DOUBLE WORD: " << " rs1: " << hardware_thread->inst.rs1 << endl;
    }

}

void except::inc_pc(const hw_thread_ptr& hardware_thread) {

    //If no exception
    if (hardware_thread->inst.is_write_icc()) {
        hardware_thread->spec_regs.set_icc(hardware_thread->inst.get_icc());
    }

    // We must execute the delayed instruction stored in the branch
    // slot. If there is none in the branch_slot then we can simply
    // forward the PC.
    if (hardware_thread->get_delayed_branch_address()) {
        // If there is double instruction in the delay slot then we need to make
        // sure to not set the PC to the branch slot instruction until we have
        // finished processing the delay slot instruction.

        // Make sure the instruction's db_word is not set.
        if (!hardware_thread->inst.is_db_word()) {
            hardware_thread->set_pc(hardware_thread->get_delayed_branch_address());
            hardware_thread->set_delayed_branch_address(0);
        }
    } else {
        hardware_thread->set_pc(hardware_thread->get_pc() + 4);
    }
    
    if (branch_check(hardware_thread)) {
        //    if (branch_check(hardware_thread->inst, hardware_thread->spec_regs.icc)) {
        if (!(hardware_thread->inst.is_branch() && hardware_thread->inst.is_annul() && hardware_thread->inst.get_conditional_branch() == BRCH_BA)) {
            hardware_thread->set_delayed_branch_address(hardware_thread->get_pc() + addr_calc(hardware_thread) - 4);
        } else {
            hardware_thread->set_pc(hardware_thread->get_pc() + addr_calc(hardware_thread) - 4);
        }
    } else if (hardware_thread->inst.is_jump()) {
#ifdef DBG_PIPE
        cout << "\t+ inst.pc = " << hardware_thread->inst.pc << endl;
#endif
        hardware_thread->set_delayed_branch_address(hardware_thread->inst.get_alu_result());
    }  else
        if (hardware_thread->inst.is_branch() && hardware_thread->inst.is_annul()) {
            hardware_thread->set_pc(hardware_thread->get_pc() + 4);
        }

    if (hardware_thread->inst.is_jump() || hardware_thread->inst.is_call()) {
        hardware_thread->inst.set_alu_result(hardware_thread->get_pc() - 4);
    }


#ifdef DBG_PIPE
    cout << "\t+ next PC: " << hex << hardware_thread->PC << ", branch slot: " << hardware_thread->get_delayed_branch_address() << dec << endl;


    //   cout << "Increment PC @ time " << sc_time_stamp() << ", old pc: "
    //        << hardware_thread->PC - 4 << ", new pc: " << hardware_thread->PC << endl;
    // FIXME: Add case to deal with thread stalls from memory accesses.

    cout << "\t+ ALU Result in exception stage: " << hardware_thread->inst.get_alu_result() << endl;
#endif

};


void except::write_regs(const hw_thread_ptr& hardware_thread) {

    if (hardware_thread->inst.is_write_registers()) {
        //    if ( hardware_thread->inst.is_call() || hardware_thread->inst.is_jump() )

        switch (hardware_thread->inst.get_select_special_register()) {
        case SREG_Y:
            hardware_thread->regs.set_reg(hardware_thread->inst.get_rd(), hardware_thread->spec_regs.get_y() , hardware_thread->spec_regs.get_curr_wp());

            break;
        case SREG_ASR:
            hardware_thread->regs.set_reg(hardware_thread->inst.get_rd(), hardware_thread->spec_regs.get_asr(hardware_thread->inst.get_rs1()) , hardware_thread->spec_regs.get_curr_wp());
            break;
        case SREG_PSR:
            hardware_thread->regs.set_reg(hardware_thread->inst.get_rd(), hardware_thread->spec_regs.get_psr() , hardware_thread->spec_regs.get_curr_wp());
            break;
        case SREG_WIM:
            hardware_thread->regs.set_reg(hardware_thread->inst.get_rd(), hardware_thread->spec_regs.get_wim() , hardware_thread->spec_regs.get_curr_wp());
            break;
        case SREG_TBR:
            hardware_thread->regs.set_reg(hardware_thread->inst.get_rd(), hardware_thread->spec_regs.get_tbr() , hardware_thread->spec_regs.get_curr_wp());
            break;
        default:
            hardware_thread->regs.set_reg(hardware_thread->inst.get_rd(), hardware_thread->inst.get_alu_result(), hardware_thread->spec_regs.get_curr_wp());
            break;
        }


#ifdef DBG_REG
        hardware_thread->regs.regdump(hardware_thread->spec_regs.get_curr_wp());
#endif


#ifdef DBG_REG
        hardware_thread->spec_regs.dump();
        hardware_thread->spec_regs.dump_deadline_timers();
#endif /* DBG_REG */

    }
}

void except::write_special_regs(const hw_thread_ptr& hardware_thread) {

//   if (hardware_thread->inst.get_select_special_register() == SREG_MEM2SP) {
//     cout << "write_special: " << true<< endl;
//     cout << "MEM2SP, is_write: " << hardware_thread->inst.is_write_special_registers() << endl;
//   }
    if (hardware_thread->inst.is_write_special_registers()) {
        switch (hardware_thread->inst.get_select_special_register()) {
	
        case SREG_Y:
            hardware_thread->spec_regs.set_y(hardware_thread->inst.get_alu_result());
            break;
        case SREG_ASR:
            hardware_thread->spec_regs.set_asr(hardware_thread->inst.get_alu_result(), hardware_thread->inst.get_rd());
            break;
        case SREG_PSR:
            hardware_thread->spec_regs.set_psr(hardware_thread->inst.get_alu_result());
            break;
        case SREG_WIM:
            hardware_thread->spec_regs.set_wim(hardware_thread->inst.get_alu_result());
            break;
        case SREG_TBR:
            hardware_thread->spec_regs.set_tbr(hardware_thread-> inst.get_alu_result());
            break;
        case SREG_DT:
            if (hardware_thread->inst.get_rd() < NUM_DEADLINE_TIMERS) {
                hardware_thread->spec_regs.set_dt(hardware_thread->inst.get_alu_result(), hardware_thread->inst.get_rd());

                /* If the value being set is 0, then it should not cause a warning. */
                if (hardware_thread->inst.get_alu_result() != 0) {
                    //                    cout << "dt value: " << hex << hardware_thread->inst.get_alu_result() << endl;
                    hardware_thread->spec_regs.set_dt_status(SET, hardware_thread->inst.get_rd());
                }

            } else {
                hardware_thread->spec_regs.set_pll_load(hardware_thread->inst.get_alu_result(), hardware_thread->inst.get_rd() - 8);
            }

            // printf("Should have written %d  to timer %d\n", hardware_thread->inst.get_alu_result(), hardware_thread->inst.get_rd());
            break;
        case SREG_MEM2SP:
            /*FIXME: Hiren More DMA to its own arch.version */
	  //	  cout << "DMA: Perform DMA functions" << endl;
                coproc_dma->set_mem_source(hardware_thread->get_id(), hardware_thread->inst.get_alu_result());
                coproc_dma->set_spm_target(hardware_thread->get_id(), hardware_thread->inst.get_op3_value());
                coproc_dma->make_transfer(hardware_thread);
                break;
        default:
            cout << "THIS SHOULD NEVER HAPPEN" << endl;
            break;
        }


#ifdef DBG_REG
        hardware_thread->spec_regs.dump();
        hardware_thread->spec_regs.dump_deadline_timers();
#endif
    }
}
