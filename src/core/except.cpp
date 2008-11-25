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


#include "except.h"
#include <assert.h>
#include <iomanip>

uint32_t except::addr_calc(const hw_thread_ptr& ht) {
    //instruction const& inst) {
    if (ht->inst.is_branch()) {
        return ht->inst.pc + (ht->inst.disp22*4);
    } else if (ht->inst.is_call()) {
        return ht->inst.pc + (ht->inst.disp30*4);
    } else {
        assert(false);
    }

    return 0;
}
bool except::branch_check(const hw_thread_ptr& ht) {
//bool except::branch_check(const instruction& inst, unsigned char icc) {
    bool neg = ht->spec_regs.icc & 0x08;
    bool zero = ht->spec_regs.icc & 0x04;
    bool ovfl = ht->spec_regs.icc & 0x02;
    bool carry = ht->spec_regs.icc & 0x01;

    if (!ht->inst.is_branch() && ! ht->inst.is_call()) {
        return false;
    } else if (ht->inst.is_call()) {
        return true;
    } else if (ht->inst.get_conditional_branch() == BRCH_BA) {
        return true;
    } else if (ht->inst.get_conditional_branch() == BRCH_BN) {
        return false;
    } else {
        switch (ht->inst.get_conditional_branch()) {
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
void except::behavior(hw_thread_ptr & ht) {
#else
void except::behavior() {
    hw_thread_ptr ht = in.read();
#endif /* _NO_SYSTEMC_ */

    /* Check if the hardware thread is enabled */
    if (_is_not_valid_hwthread(ht)) {
        return;
    }
    /* Everytime an instruction goes through this stage, increase its
       cycle count to 6 */
    ht->cnt_cycles += 6;

    /* If fetch stage is stalled then just increment the cycle count
    and return. */
    if (ht->is_fetch_stalled()) {
        return;
    }

    debug(ht);

    /* If the current instruction is a double word then set a flag to
       note it */
    set_dword_state(ht);

    /* If deadline is causing a stall then let skip out of this stage */
    if (dead_stalled(ht)) {
        return;
    }

    /* If the instruction fetch caused a stall then skip out of this
       stage */
    if (fetch_stalled(ht)) {
        return;
    }
    /* If memory is causing a stall then skip out of this stage */
    if (mem_stalled(ht)) {
        return;
    }

    if (!ht->is_db_word_stalled()) {
        /* Increment the PC based */
        inc_pc(ht);
    }

    write_regs(ht);
    write_special_regs(ht);

    /* Increment the number of instructions executed */
    ht->cnt_instr++;

}

bool except::dead_stalled(const hw_thread_ptr& ht) {

    /****************************************
     //ADDED FOR DEADLINE INSTRUCTION
     //IF DEADLINE IS NOT REACHED, NULL MEANS DON'T COMMIT
     //ANYTHING, SO WE CAN JUST RETURN FROM THIS STAGE
     ****************************************************************/
    if (ht->is_deadline_stalled()) {
#ifdef DBG_REG
        //   ht->spec_regs.dump();
        ht->spec_regs.dump_deadline_timers();

#endif
        //HACK!!! NEED TO CHANGE!!
        //STALLED BUT WANT TO UPDATE MAILBOX
        if (ht->spec_regs.pll_loaded && ht->inst.wsreg) {
	  ht->spec_regs.pll_load[ht->inst.rd-8] = ht->inst.get_alu_result();
        }
        return true;
    }
    return false;
}

void except::debug(const hw_thread_ptr& ht) {
#ifdef DBG_PIPE
    cout << "*eXcept*" << "  (" << sc_time_stamp() << ") ";
    cout << "hw_thread's id: " << ht->get_id() << ", pc: 0x" << hex << ht._handle->PC << ", " << hex << ht->inst;
    cout << "\t+ pc: " << hex << ht->PC << " branch: " << ht->get_delayed_branch_address() << " icc: " << hex << (uint32_t)(ht->spec_regs.icc) << dec << endl;
#endif

#ifdef DBG_INST_TRACE

    cout <<  "( " << sc_time_stamp() << " ) "
         << hex <<  "pc: " << setw(8) << setfill('0') << ht->PC << "\t"
         <<  ht->inst << endl;
#endif

#ifdef DBG_UNMIP
    if (ht->inst.unimp) {
        cerr << "thread id: " << ht->get_id()
             << ", pc: 0x" << hex << ht._handle->PC
             << ", " << hex << ht->inst << endl;
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

bool except::fetch_stalled(const hw_thread_ptr& ht) {
    /* A stall from the fetch stage causes the exception stage to skip out
     just after the deadline registers are updated */
    return (ht->is_fetch_stalled());
};

bool except::_is_not_valid_hwthread(const hw_thread_ptr& ht) {
    return (ht.is_null() || !ht->is_enabled());
}

bool except::mem_stalled(const hw_thread_ptr& ht) {
    /* A stall from the memory causes the exception stage to skip out
     just after the deadline registers are updated */
    return (ht->is_memory_stalled());
};

void except::set_dword_state(const hw_thread_ptr& ht) {

    // HDP: If the instruction is a double anything we need to redo the
    // instruction except we need to increment the destination address.
    if ((ht->inst.db_word) && (!ht->is_db_word_stalled())) {
        // The PC is no longer incremented if a db_word is recognized.
        // Set the thread's double word flag to true.
        ht->set_db_word_stalled(true);
        //    cout << "DOUBLE WORD: " << " rs1: " << ht->inst.rs1 << endl;
    }

}

void except::inc_pc(const hw_thread_ptr& ht) {

    //If no exception
    if (ht->inst.wicc) {
        ht->spec_regs.icc = ht->inst.icc;
    }

    // We must execute the delayed instruction stored in the branch
    // slot. If there is none in the branch_slot then we can simply
    // forward the PC.
    if (ht->get_delayed_branch_address()) {
        // If there is double instruction in the delay slot then we need to make
        // sure to not set the PC to the branch slot instruction until we have
        // finished processing the delay slot instruction.

        // Make sure the instruction's db_word is not set.
        if (!ht->inst.db_word) {
            ht->set_pc(ht->get_delayed_branch_address());
            ht->set_delayed_branch_address(0);
        }
    } else {
        ht->set_pc(ht->get_pc() + 4);
    }

    if (branch_check(ht)) {
        //    if (branch_check(ht->inst, ht->spec_regs.icc)) {
        if (!(ht->inst.is_branch() && ht->inst.is_annul() && ht->inst.get_conditional_branch() == BRCH_BA)) {
            ht->set_delayed_branch_address(ht->get_pc() + addr_calc(ht) - 4);
        } else {
            ht->set_pc(ht->get_pc() + addr_calc(ht) - 4);
        }
    } else if (ht->inst.is_jump()) {
#ifdef DBG_PIPE
        cout << "\t+ inst.pc = " << ht->inst.pc << endl;
#endif
        ht->set_delayed_branch_address(ht->inst.get_alu_result());
    }  else
        if (ht->inst.is_branch() && ht->inst.is_annul()) {
            ht->set_pc(ht->get_pc() + 4);
        }

    if (ht->inst.is_jump() || ht->inst.is_call()) {
      ht->inst.set_alu_result(ht->get_pc() - 4);
    }


#ifdef DBG_PIPE
    cout << "\t+ next PC: " << hex << ht->PC << ", branch slot: " << ht->get_delayed_branch_address() << dec << endl;


    //   cout << "Increment PC @ time " << sc_time_stamp() << ", old pc: "
    //        << ht->PC - 4 << ", new pc: " << ht->PC << endl;
    // FIXME: Add case to deal with thread stalls from memory accesses.

    cout << "\t+ ALU Result in exception stage: " << ht->inst.get_alu_result() << endl;
#endif

};


void except::write_regs(const hw_thread_ptr& ht) {

  if (ht->inst.is_write_registers()) {
        //    if ( ht->inst.is_call() || ht->inst.is_jump() )

        switch (ht->inst.mux_specreg) {
        case SREG_Y:
            ht->regs.set_reg(ht->inst.rd, ht->spec_regs.y , ht->spec_regs.curr_wp);

            break;
        case SREG_ASR:
            ht->regs.set_reg(ht->inst.rd, ht->spec_regs.asr[ht->inst.rs1] , ht->spec_regs.curr_wp);
            break;
        case SREG_PSR:
            ht->regs.set_reg(ht->inst.rd, ht->spec_regs.get_psr() , ht->spec_regs.curr_wp);
            break;
        case SREG_WIM:
            ht->regs.set_reg(ht->inst.rd, ht->spec_regs.wim , ht->spec_regs.curr_wp);
            break;
        case SREG_TBR:
            ht->regs.set_reg(ht->inst.rd, ht->spec_regs.tbr , ht->spec_regs.curr_wp);
            break;
        default:
	  ht->regs.set_reg(ht->inst.rd, ht->inst.get_alu_result(), ht->spec_regs.curr_wp);
            break;
        }


#ifdef DBG_REG
        ht->regs.regdump(ht->spec_regs.curr_wp);
#endif


#ifdef DBG_REG
        ht->spec_regs.dump();
        ht->spec_regs.dump_deadline_timers();
#endif /* DBG_REG */

    }
}

void except::write_special_regs(const hw_thread_ptr& ht) {

    if (ht->inst.wsreg) {
        switch (ht->inst.mux_specreg) {
        case SREG_Y:
	  ht->spec_regs.y = ht->inst.get_alu_result();
            break;
        case SREG_ASR:
	  ht->spec_regs.asr[ht->inst.rd] = ht->inst.get_alu_result();
            break;
        case SREG_PSR:
	  ht->spec_regs.set_psr(ht->inst.get_alu_result());
            break;
        case SREG_WIM:
	  ht->spec_regs.wim = ht->inst.get_alu_result();
            break;
        case SREG_TBR:
	  ht->spec_regs.tbr = ht-> inst.get_alu_result();
            break;
        case SREG_DT:
            if (ht->inst.rd < NUM_DEADLINE_TIMERS) {
	      ht->spec_regs.dt[ht->inst.rd] = ht->inst.get_alu_result();

                /* If the value being set is 0, then it should not cause a warning. */
	      if (ht->inst.get_alu_result() != 0) {
                    //                    cout << "dt value: " << hex << ht->inst.get_alu_result() << endl;
                    ht->spec_regs.dt_status[ht->inst.rd] = SET;
                }

            } else {
	      ht->spec_regs.pll_load[ht->inst.rd-8] = ht->inst.get_alu_result();
            }

            // printf("Should have written %d  to timer %d\n", ht->inst.get_alu_result(), ht->inst.rd);
            break;
        case SREG_CP:
            //            cout << "except:: SREG_CP write special registers" << endl;
            /*FIXME: Hiren More DMA to its own arch.version */
            // To a memory source address.
            if (ht->inst.rd == 0) {
                //                cout << "except:: memory source, tid: " << ht->id << ", value: " << hex << ht->inst.get_alu_result() << endl;
	      coproc_dma->set_mem_source(ht->get_id(), ht->inst.get_alu_result());
            }

            // To a spm target address. Start the transfer.
            if (ht->inst.rd == 1) {
                //                cout << "except:: SPM target, tid: " << ht->id << ", value: " << hex << ht->inst.get_alu_result() << endl;
	      coproc_dma->set_spm_target(ht->get_id(), ht->inst.get_alu_result());
                coproc_dma->make_transfer(ht);
            }
            break;
        default:
            cout << "THIS SHOULD NEVER HAPPEN" << endl;
            break;
        }


#ifdef DBG_REG
        ht->spec_regs.dump();
        ht->spec_regs.dump_deadline_timers();
#endif
    }
}
