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


#include "execute.h"

execute::execute(): module_base() {

}

execute::execute(const sc_module_name& name): module_base(name) {

}

#ifdef _NO_SYSTEMC_
void execute::behavior(const hw_thread_ptr& hardware_thread, hw_thread_ptr& out_thread) {
#else
void execute::behavior() {
    hw_thread_ptr hardware_thread = in.read();
#endif

    if (hardware_thread.is_null() || !hardware_thread->is_enabled()
            || hardware_thread->is_fetch_stalled()) {
#ifdef _NO_SYSTEMC_
        out_thread = hardware_thread;
#else
        out.write(hardware_thread);
#endif
        return;
    }

    perform_alu_operations(hardware_thread);

    #ifdef DBG_PIPE
    cout << "*execute*" << " (" << sc_time_stamp() << ") ";
    cout << "hw_thread's id: " << hardware_thread->get_id() << ", pc: 0x" << hex << hardware_thread.get_handle()->PC << hex <<  ", " << hardware_thread->inst;
    cout << "\t+ " << hex << "alu_result: "
         << dec << hardware_thread->inst.get_alu_result() << ", aluwreg: " << hardware_thread->inst.wreg << ", wicc: "
         << hardware_thread->inst.is_write_icc() << ", local icc = "  << (int)hardware_thread->inst.get_icc() << endl;

#endif

#ifdef _NO_SYSTEMC_
    out_thread = hardware_thread;
#else
    out.write(hardware_thread);
#endif
}

void execute::perform_alu_operations(const hw_thread_ptr& hardware_thread) {

    short shift_amount;
    bool signbit;
    int temporary_alu = 0;

    //ALU Operations
    switch (hardware_thread->inst.get_aluop()) {
    case ALU_ADD:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() + hardware_thread->inst.get_op2_value());
        // If the carry has to be used
        if (hardware_thread->inst.is_carry()) {
            hardware_thread->inst.set_alu_result(hardware_thread->inst.get_alu_result() + hardware_thread->spec_regs.get_carry_bit());
        }

        break;
    case ALU_ADD_DEAD:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op2_value());
	break;
    case ALU_NOP:
        hardware_thread->inst.set_alu_result(0);
        break;
    case ALU_SUB:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() - hardware_thread->inst.get_op2_value());
        if (hardware_thread->inst.is_carry())
            hardware_thread->inst.set_alu_result(hardware_thread->inst.get_alu_result() - hardware_thread->spec_regs.get_carry_bit());
        break;
    case ALU_AND:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() & hardware_thread->inst.get_op2_value());
        break;
    case ALU_OR:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() | hardware_thread->inst.get_op2_value());
        break;
    case ALU_XOR:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() ^ hardware_thread->inst.get_op2_value());
        break;
    case ALU_ANDN:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() & ~(hardware_thread->inst.get_op2_value()));
        break;
    case ALU_XNOR:
        hardware_thread->inst.set_alu_result(~(hardware_thread->inst.get_op1_value() ^ hardware_thread->inst.get_op2_value()));
        break;
    case ALU_ORN:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() | ~(hardware_thread->inst.get_op2_value()));
        break;
    case ALU_SLL:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() << (hardware_thread->inst.get_op2_value() & 0x1F));
        break;
    case ALU_SRL:
        // In a logical shift righardware_thread, the sign bit shifted is always 0
        signbit = hardware_thread->inst.get_op1_value() & 0x80000000;
        shift_amount = hardware_thread->inst.get_op2_value() & 0x1F;
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value());

        if (signbit) {
            while (shift_amount--) {
                temporary_alu = 0;
                temporary_alu = (hardware_thread->inst.get_alu_result() >> 1) & 0x7FFFFFFF;
                hardware_thread->inst.set_alu_result(temporary_alu);
            }
        } else {
            hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() >> shift_amount);
        }

        /* FIXME: HDP, I don't know why this shift is working like this */
        /*
          signbit = hardware_thread->inst.get_op1_value() & 0x80000000;
          shift_amount = hardware_thread->inst.get_op2_value() & 0x1F;
          hardware_thread->inst.set_alu_result( hardware_thread->inst.get_op1_value());
          if ( signbit )
          while (shift_amount--) {
          temp = 0;
          hardware_thread->inst.set_alu_result( (hardware_thread->inst.alu_result >> 1) & 0x7FFFFFFF);
          }
          else
          hardware_thread->inst.set_alu_result( hardware_thread->inst.get_op1_value() >> shift_amount);

        */
        break;
    case ALU_SRA:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_op1_value() >> (hardware_thread->inst.get_op2_value() & 0x1F));
        break;
    case ALU_SETHI:
        hardware_thread->inst.set_alu_result(hardware_thread->inst.get_disp22() << 10);
        break;
    case ALU_MUL:
        /// Hiren: Going to use SystemC data types for easier
        /// maniuplation of 64 bits.
        /// If signed multiplication
        if (hardware_thread->inst.is_signed_multiply()) {
            uint32_t  rs1 = (hardware_thread->inst.get_op1_value());
            uint32_t rs2 = (hardware_thread->inst.get_op2_value());
            uint32_t y = (hardware_thread->spec_regs.get_y());
            uint32_t rsd;
            uint32_t  temp_rsd;

            /// Used in step (6) to replace MSB of (y) with LSB of rs1
            /// (unshifted)
            bool lsb_rs1 = rs1 & 1; //rs1[0];
            /// Page 110 from Sparc v8 manual
            /// (1): Select immediate or rs2's value.
            /// rs2 holds the appropriate value.
            /// (2): Righardware_thread shift by one
            rs1 = rs1 >> 1;
            /// Replace MSB with N xor V
	    bool n = hardware_thread->spec_regs.get_icc() & 0x08;
	    bool v = hardware_thread->spec_regs.get_icc() & 0x02;
            bool b = n ^ v;
	    // error code 
	    // (hardware_thread->spec_regs.get_icc() & 0x08) ^ (hardware_thread->spec_regs.get_icc() & 0x02);
            rs1 |= (b << 31) ; //rs1.set(32, b);
            /// (3): Check if LSB of Y reg is 1.
            if (!(y & 0x00000001)) { // if (y[0] == false) {
                /// Add 0 to the shifted value in rs1 ?
                temp_rsd = rs1;
            } else {
                /// rs1 is added to the multiplier (rs2)
	      temp_rsd = rs1 + rs2;
            }
            /// Step (4)
            rsd = temp_rsd;
            /// Step (5) should be done from setting wicc=true and adding ALU_MUL cases
            /// Step (6)
            y = y >> 1;
            y |= (lsb_rs1 << 31); //y[31] = lsb_rs1;

            /// Restore updated register values
            hardware_thread->spec_regs.set_y(y);
            hardware_thread->inst.set_op1_value(rs1);
            hardware_thread->inst.set_op2_value(rs2);
            hardware_thread->inst.set_alu_result(rsd);
        }
        break;
    default:
        break;
    }

    //write to the icc
    if (hardware_thread->inst.is_write_icc()) {

        hardware_thread->inst.set_icc(geticc(hardware_thread));
    }

#ifdef DBG_PIPE
    if (hardware_thread->inst.jump) {
        cout << "ALU Result in execute stage: " << hardware_thread->inst.get_op1_value()
             << "+" << hardware_thread->inst.get_op2_value() << "=" << hardware_thread->inst.get_alu_result() << endl;
    }
#endif

    //  FIXME: What was this for? Isaac?
    //  if ( hardware_thread->inst.call || hardware_thread->inst.jump ) {
    //    hardware_thread->inst.set_alu_result( hardware_thread->inst.pc);
    //  }

}


unsigned char execute::geticc(const hw_thread_ptr& hardware_thread) {
    unsigned char icc = hardware_thread->inst.get_icc();

    //zero
    if (hardware_thread->inst.get_alu_result() == 0)
        icc |= 0x4;
    //neg
    if (hardware_thread->inst.get_alu_result() < 0)
        icc |= 0x08;
    //overflow
    if ((((hardware_thread->inst.get_aluop() == ALU_ADD) || (hardware_thread->inst.get_aluop() == ALU_MUL)) &&
            (hardware_thread->inst.get_op1_value() >> 31) == (hardware_thread->inst.get_op2_value() >> 31) &&
            (hardware_thread->inst.get_op1_value() >> 31) != (hardware_thread->inst.get_alu_result() >> 31)) ||
            (hardware_thread->inst.get_aluop() == ALU_SUB && (hardware_thread->inst.get_op1_value() >> 31) != (hardware_thread->inst.get_op2_value() >> 31)
             && (hardware_thread->inst.get_op1_value() >> 31) != (hardware_thread->inst.get_alu_result() >> 31)))
        icc |= 0x2;

    //carry
    if ((((hardware_thread->inst.get_aluop() == ALU_ADD) || (hardware_thread->inst.get_aluop() == ALU_MUL)) &&
            (((hardware_thread->inst.get_op1_value() >> 31) && (hardware_thread->inst.get_op2_value() >> 31))
             || (!(hardware_thread->inst.get_alu_result() >> 31) && (hardware_thread->inst.get_op1_value() >> 31 || hardware_thread->inst.get_op2_value() >> 31)))) ||
            (hardware_thread->inst.get_aluop() == ALU_SUB && ((!(hardware_thread->inst.get_op1_value() >> 31) && !(~(hardware_thread->inst.get_op2_value()) >> 31))
                    || ((!(hardware_thread->inst.get_op1_value() >> 31) || !(~(hardware_thread->inst.get_op2_value()) >> 31)) &&
                        (hardware_thread->inst.get_alu_result() >> 31)))))
        icc |= 0x1;

    return icc;
}

bool execute::_is_not_valid_hwthread(const hw_thread_ptr& hardware_thread) {
    return (hardware_thread.is_null() || !hardware_thread->is_enabled()
            || hardware_thread->is_fetch_stalled());
}
