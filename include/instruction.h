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


#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#ifndef _NO_SYSTEMC_
#include <systemc.h>
#endif

#include <iostream>
#include <iomanip>
#include "instruction_defs.h"

using namespace std;

struct instruction {

    // Flag to see if instruction is a double word instruction
    bool db_word;

    uint32_t inst;
    int pc;
    bool halt;

    short op, op2, op3;

    register_number rs1;
    register_number rs2;
    register_number rd;
    ALUType aluop;
    bool wreg;
    bool wicc;
    bool write_mem;
    bool read_mem;
    MemorySize mem_size;
    bool branch;
    bool jump;
    bool call;
    short cond;
    bool annul;
    int disp30;
    int disp22;

    SpecialRegisters sp_reg;
    short wp_increment;

    SrcMux rs1sel;
    SrcMux rs2sel;
    SrcMux rdsel;

    int op1_val;
    int op2_val;
    int op3_val; // Value of register rd
    int imm;
    bool use_imm;
    bool use_carry;

    int alu_result;
    unsigned char icc;
    bool trap;
    unsigned char traptype;
    bool unimp;
    unsigned char mux_specreg;
    bool wsreg;

    /// Whether it is a signed multiple or not
    bool mul_signed;

    /**
     * Create a NOP instruction.
     */
    instruction();

    /**
     * Create an instruction from a binary representation
     * of the machine code.
     *
     * @param binary_representation This instruction's binary representation
     */
    instruction(uint32_t binary_representation);
    bool check_end_sim();

    /**
     * Decode the binary representation into meaningful values in all the
     * other fields of the instruction object.
     */
    void decode();
    void decode_arithmetic(const int& op3);
    void decode_memory(const int& op3);
    void decode_sethi_branches(const int& op2);
    uint32_t get_write_data(uint32_t old_data, int offset);
    instruction(const instruction& mem);
    void operator=(const instruction& mem);
    bool operator == (const instruction& mem) const;
    inline friend ostream& operator<< (ostream& out, const instruction& inst) {
        out << setw(8) << setfill('0') << hex << inst.inst;
        return out;
    }

    /**
     * Stores data read from memory into the correct format in keeping
     * with the type of load that this instruction implements.
     *
     * @param data_in The data read from memory
     */
    void read_data(uint32_t data_in, int offset);
    void set_inst(uint32_t inbits);

private:
    /**
     * Initialize the data elements to be reasonable default values.
     * By default, an instruction does performs a null operation.
     */
    void initialize();
};

#endif
