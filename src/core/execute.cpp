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
         << dec << hardware_thread->inst.alu_result << ", aluwreg: " << hardware_thread->inst.wreg << ", wicc: "
         << hardware_thread->inst.wicc << ", local icc = "  << (int)hardware_thread->inst.icc << endl;

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
    switch (hardware_thread->inst.aluop) {
    case ALU_ADD:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val + hardware_thread->inst.op2_val;
        // If the carry has to be used
        if (hardware_thread->inst.use_carry) {
            hardware_thread->inst.alu_result += hardware_thread->spec_regs.get_carry_bit();
        }

        break;
    case ALU_NOP:
        hardware_thread->inst.alu_result = 0;
        break;
    case ALU_SUB:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val - hardware_thread->inst.op2_val;
        if (hardware_thread->inst.use_carry) hardware_thread->inst.alu_result -= hardware_thread->spec_regs.get_carry_bit();
        break;
    case ALU_AND:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val & hardware_thread->inst.op2_val;
        break;
    case ALU_OR:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val | hardware_thread->inst.op2_val;
        break;
    case ALU_XOR:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val ^ hardware_thread->inst.op2_val;
        break;
    case ALU_ANDN:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val & ~(hardware_thread->inst.op2_val);
        break;
    case ALU_XNOR:
        hardware_thread->inst.alu_result = ~(hardware_thread->inst.op1_val ^ hardware_thread->inst.op2_val);
        break;
    case ALU_ORN:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val | ~(hardware_thread->inst.op2_val);
        break;
    case ALU_SLL:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val << (hardware_thread->inst.op2_val & 0x1F);
        break;
    case ALU_SRL:
        // In a logical shift righardware_thread, the sign bit shifted is always 0
        signbit = hardware_thread->inst.op1_val & 0x80000000;
        shift_amount = hardware_thread->inst.op2_val & 0x1F;
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val;

        if (signbit) {
            while (shift_amount--) {
                temporary_alu = 0;
                temporary_alu = (hardware_thread->inst.alu_result >> 1) & 0x7FFFFFFF;
                hardware_thread->inst.alu_result = temporary_alu;
            }
        } else {
            hardware_thread->inst.alu_result = hardware_thread->inst.op1_val >> shift_amount;
        }

        /* FIXME: HDP, I don't know why this shift is working like this */
        /*
          signbit = hardware_thread->inst.op1_val & 0x80000000;
          shift_amount = hardware_thread->inst.op2_val & 0x1F;
          hardware_thread->inst.alu_result = hardware_thread->inst.op1_val;
          if ( signbit )
          while (shift_amount--) {
          temp = 0;
          hardware_thread->inst.alu_result = (hardware_thread->inst.alu_result >> 1) & 0x7FFFFFFF;
          }
          else
          hardware_thread->inst.alu_result = hardware_thread->inst.op1_val >> shift_amount;

        */
        break;
    case ALU_SRA:
        hardware_thread->inst.alu_result = hardware_thread->inst.op1_val >> (hardware_thread->inst.op2_val & 0x1F);
        break;
    case ALU_SETHI:
        hardware_thread->inst.alu_result = hardware_thread->inst.disp22 << 10;
        break;
    case ALU_MUL:
        /// Hiren: Going to use SystemC data types for easier
        /// maniuplation of 64 bits.
#ifndef _NO_SYSTEMC_
        /// If signed multiplication
        if (hardware_thread->inst.mul_signed == true) {
            sc_uint<32> rs1(hardware_thread->inst.op1_val);
            sc_uint<32> rs2(hardware_thread->inst.op2_val);
            sc_uint<32> y(hardware_thread->spec_regs.y);
            sc_uint<32> rsd;
            sc_uint<32> temp_rsd;

            /// Used in step (6) to replace MSB of (y) with LSB of rs1
            /// (unshifted)
            bool lsb_rs1 = rs1[0];
            /// Page 110 from Sparc v8 manual
            /// (1): Select immediate or rs2's value.
            /// rs2 holds the appropriate value.
            /// (2): Righardware_thread shift by one
            rs1 = rs1 >> 1;
            /// Replace MSB with N xor V
            bool b = (hardware_thread->spec_regs.icc & 0x08) ^(hardware_thread->spec_regs.icc & 0x02);
            rs1.set(32, b);
            /// (3): Check if LSB of Y reg is 1.
            if (y[0] == false) {
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
            y[31] = lsb_rs1;

            /// Restore updated register values
            hardware_thread->spec_regs.y = y.to_uint();
            hardware_thread->inst.op1_val = rs1.to_uint();
            hardware_thread->inst.op2_val = rs2.to_uint();
            hardware_thread->inst.alu_result = rsd.to_uint();
        }
#else
        /// If signed multiplication
        if (hardware_thread->inst.mul_signed == true) {
            uint32_t  rs1 = (hardware_thread->inst.op1_val);
            uint32_t rs2 = (hardware_thread->inst.op2_val);
            uint32_t y = (hardware_thread->spec_regs.y);
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
            bool b = (hardware_thread->spec_regs.icc & 0x08) ^(hardware_thread->spec_regs.icc & 0x02);
            rs1 |= (b << 31) ; //rs1.set(32, b);
            /// (3): Check if LSB of Y reg is 1.
            if ((y & 1) == false) { // if (y[0] == false) {
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
            hardware_thread->spec_regs.y = y;
            hardware_thread->inst.op1_val = rs1;
            hardware_thread->inst.op2_val = rs2;
            hardware_thread->inst.alu_result = rsd;
        }

#endif
        break;
    default:
        break;
    }

    //write to the icc
    if (hardware_thread->inst.wicc) {
        hardware_thread->inst.icc = geticc(hardware_thread);
    }

#ifdef DBG_PIPE
    if (hardware_thread->inst.jump) {
        cout << "ALU Result in execute stage: " << hardware_thread->inst.op1_val
             << "+" << hardware_thread->inst.op2_val << "=" << hardware_thread->inst.alu_result << endl;
    }
#endif

    //  FIXME: What was this for? Isaac?
    //  if ( hardware_thread->inst.call || hardware_thread->inst.jump ) {
    //    hardware_thread->inst.alu_result = hardware_thread->inst.pc;
    //  }

}


unsigned char execute::geticc(const hw_thread_ptr& hardware_thread) {
    unsigned char icc = hardware_thread->inst.icc;

    //zero
    if (hardware_thread->inst.alu_result == 0)
        icc |= 0x4;
    //neg
    if (hardware_thread->inst.alu_result < 0)
        icc |= 0x08;
    //overflow
    if ((((hardware_thread->inst.aluop == ALU_ADD) || (hardware_thread->inst.aluop == ALU_MUL)) &&
            (hardware_thread->inst.op1_val >> 31) == (hardware_thread->inst.op2_val >> 31) &&
            (hardware_thread->inst.op1_val >> 31) != (hardware_thread->inst.alu_result >> 31)) ||
            (hardware_thread->inst.aluop == ALU_SUB && (hardware_thread->inst.op1_val >> 31) != (hardware_thread->inst.op2_val >> 31)
             && (hardware_thread->inst.op1_val >> 31) != (hardware_thread->inst.alu_result >> 31)))
        icc |= 0x2;

    //carry
    if ((((hardware_thread->inst.aluop == ALU_ADD) || (hardware_thread->inst.aluop == ALU_MUL)) &&
            (((hardware_thread->inst.op1_val >> 31) && (hardware_thread->inst.op2_val >> 31))
             || (!(hardware_thread->inst.alu_result >> 31) && (hardware_thread->inst.op1_val >> 31 || hardware_thread->inst.op2_val >> 31)))) ||
            (hardware_thread->inst.aluop == ALU_SUB && ((!(hardware_thread->inst.op1_val >> 31) && !(~(hardware_thread->inst.op2_val) >> 31))
                    || ((!(hardware_thread->inst.op1_val >> 31) || !(~(hardware_thread->inst.op2_val) >> 31)) &&
                        (hardware_thread->inst.alu_result >> 31)))))
        icc |= 0x1;

    //FIXME: Need to add carry

    //  sc_uint<32> o1(hardware_thread->inst.op1_val);
    //   sc_uint<32> o2(hardware_thread->inst.op2_val);
    //   sc_uint<33> res_tmp = o1 - o2;
    //   sc_uint<32> ticc(icc);

    //   cout << "before: ticc: " << ticc.to_string() << ", icc: " << (uint32_t) icc << ", res_tmp: "<< res_tmp.to_string() << endl;

    //   if ((hardware_thread->inst.aluop == ALU_ADD) || (hardware_thread->inst.aluop == ALU_ADDX)) {
    //      ticc[0] = (o1[31] & o2[31]) |
    //        (!(res_tmp[32]) & (o1[31] | o2[31]));

    //     }

    //   if ((hardware_thread->inst.aluop == ALU_SUB)) {
    //      ticc[0] = ((! o1[31]) & !o2[31]) |
    //        (res_tmp[32] & ((! o1[31]) | !o2[31]));


    //   }

    //   icc = 0x0000000f & ticc.to_uint();
    //   cout << "after: ticc: " << ticc.to_string() << ", icc: " << hex << (uint32_t)icc << ", res_tmp: "<< res_tmp.to_string() << endl;

    // Took this from the LEON. Only thing is that they use a result
    // which is 32 bits. This is definitely going to be a problem for
    // us since we need to do the computation with an extra bit, but
    // we can't. So we need to temporarily do it with long longs and
    // see if it works better that way.

    /*
      long long int res_tmp = hardware_thread->inst.op1_val - hardware_thread->inst.op2_val;
      long long int mask = 0x100000000LL;
      cout << "before -> icc: " << (uint32_t)(icc) << endl;

      if ((hardware_thread->inst.aluop == ALU_ADD) || (hardware_thread->inst.aluop == ALU_ADDX)) {
      // Both 31st bit of op1_val and op2_val are 1s then carry is set
      if (((0x80000000 & hardware_thread->inst.op1_val ) && (0x80000000 & hardware_thread->inst.op2_val))
      || ( (! (mask & res_tmp)) &&
      ((0x80000000 & hardware_thread->inst.op1_val) || (0x80000000 & hardware_thread->inst.op2_val)))   {

      icc |= 0x01;
      }
      else {
      unsigned char ticc = 0;
      ticc = ticc | icc;
      icc = ticc;
      }

      }
      // Add for subtract.
      if ((hardware_thread->inst.aluop == ALU_SUB)) {
      cout << "+HDP: ALU_SUB, op1: " << hardware_thread->inst.op1_val << ", op2: " << hardware_thread->inst.op2_val
      << ", mask: " << mask << ", res_tmp: " << res_tmp << endl;
      cout << " first part: " << ((! (0x80000000 & hardware_thread->inst.op1_val)) && (! (0x80000000 & hardware_thread->inst.op2_val))) << endl;
      if (((! (0x80000000 & hardware_thread->inst.op1_val)) && (! (0x80000000 & hardware_thread->inst.op2_val)))
      || ((mask && res_tmp) &&
      ((! (0x80000000 & hardware_thread->inst.op1_val)) || (! (0x80000000 & hardware_thread->inst.op2_val)))))      {
      icc |= 0x01;
      cout << " SET CARRY " << endl;
      }
      else {
      unsigned char ticc = 0;
      ticc = ticc | icc;
      icc = ticc;
      }
      }
      cout << "after -> icc: " << (uint32_t)(icc) << endl;
    */

    return icc;
}

bool execute::_is_not_valid_hwthread(const hw_thread_ptr& hardware_thread) {
    return (hardware_thread.is_null() || !hardware_thread->is_enabled()
            || hardware_thread->is_fetch_stalled());
}
