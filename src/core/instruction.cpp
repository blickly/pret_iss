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

#include "instruction.h"

instruction::instruction() {
    initialize();
    inst = 0x01000000; //NOP
}

instruction::instruction(uint32_t binary_representation) {
    initialize();
    inst = binary_representation;
}

void instruction::decode() {
    op = inst >> 30;
    op2 = (inst & 0x01C00000) >> 22;
    op3 = (inst & 0X01F80000) >> 19;
    rd = (inst & 0x3E000000) >> 25;//0011 1110 0000
    rs1 = (inst & 0X0007C000) >> 14;// 0000 0000 0000 0111 1100
    use_imm = (inst & 0X00002000) >> 12; //0000 0000 0000 0000 0010
    imm = (inst & 0X00001FFF) << 19;
    imm = imm >> 19;


    rs2 = (inst & 0X0000001F);
    _conditional_branch = (inst & 0X1E000000) >> 25;
    _annul = (inst & 0X20000000) >> 29;

    disp30 = (inst & 0X3FFFFFFF) << 2;
    disp30 = disp30 >> 2;
    disp22 = (inst & 0X003FFFFF) << 10;
    disp22 = disp22 >> 10;

    switch (op) {
    case OP_CALL:
        _call = 1;
        aluop = ALU_ADD;
        _write_registers = true;
        rd = 15;
        break;
    case OP_SETHI_BRANCHES:
      decode_sethi_branches(static_cast<OP2>(op2));
        break;
    case OP_MEMORY:
      decode_memory(static_cast<OP3_MEMORY>(op3));
        break;
    case OP_ARITHMETIC_ETC:
      decode_arithmetic(static_cast<OP3_ARITHMETIC>(op3));
        break;
    default:
        _unimplemented = true;
        break;
    };
    if (rd == 0) {
        _write_registers = false;
    }
}

void instruction::decode_arithmetic(const OP3_ARITHMETIC& op3) {
    switch (op3) {
    case OP3_MULSCC:
        aluop = ALU_MUL;
        _write_registers = true;
        mul_signed = true;
        wicc = true;
        break;
    case OP3_ADD:
        aluop = ALU_ADD;
        _write_registers = true;
        break;
    case OP3_ADDX:
        aluop = ALU_ADD;
        _write_registers = true;
        _carry = true;
        break;
    case OP3_ADDXCC:
        aluop = ALU_ADD;
        _write_registers = true;
        wicc = true;
        _carry = true;
        break;
    case OP3_AND:
        aluop = ALU_AND;
        _write_registers = true;
        break;
    case OP3_OR:
        aluop = ALU_OR;
        _write_registers = true;
        break;
    case OP3_XOR:
        aluop = ALU_XOR;
        _write_registers = true;
        break;
    case OP3_SUB:
        aluop = ALU_SUB;
        _write_registers = true;
        break;
    case OP3_SUBX:
        aluop = ALU_SUB;
        _write_registers = true;
        _carry = true;
        break;
    case OP3_SUBXCC:
        aluop = ALU_SUB;
        _write_registers = true;
        _carry = true;
        wicc = true;
        break;
    case OP3_ANDN:
        aluop = ALU_ANDN;
        _write_registers = true;
        break;
    case OP3_ORN:
        aluop = ALU_ORN;
        _write_registers = true;
        break;
    case OP3_XNOR:
        aluop = ALU_XNOR;
        _write_registers = true;
        break;
    case OP3_ADDCC:
        aluop = ALU_ADD;
        _write_registers = true;
        wicc = true;
        break;
    case OP3_ANDCC:
        aluop = ALU_AND;
        _write_registers = true;
        wicc = true;
        break;
    case OP3_ORCC:
        aluop = ALU_OR;
        _write_registers = true;
        wicc = true;
        break;
    case OP3_XORCC:
        aluop = ALU_XOR;
        _write_registers = true;
        wicc = true;
        break;
    case OP3_SUBCC:
        aluop = ALU_SUB;
        _write_registers = true;
        wicc = true;
        break;
    case OP3_ANDNCC:
        aluop = ALU_ANDN;
        _write_registers = true;
        wicc = true;
        break;
    case OP3_ORNCC:
        aluop = ALU_ORN;
        _write_registers = true;
        wicc = true;
        break;
    case OP3_XNORCC:
        aluop = ALU_XNOR;
        _write_registers = true;
        wicc = true;
        break;
    case OP3_JMPL:
        _jump = 1;
        _write_registers = true;
        aluop = ALU_ADD;
        break;
    case OP3_SAVE:
        wp_increment = -1;
        aluop = ALU_ADD;
        _write_registers = true;
        break;
    case OP3_RESTORE:
        wp_increment = 1;
        aluop = ALU_ADD;
        _write_registers = true;
        break;
    case OP3_SLL:
        aluop = ALU_SLL;
        _write_registers = true;
        break;
    case OP3_SRL:
        aluop = ALU_SRL;
        _write_registers = true;
        break;
    case OP3_SRA:
        aluop = ALU_SRA;
        _write_registers = true;
        break;
    case OP3_RDPSR:
        mux_specreg = SREG_PSR;
        _write_registers = true;
        break;
    case OP3_RDASR:
        mux_specreg = (rs1 ? SREG_ASR : SREG_Y);
        _write_registers = true;
        break;
    case OP3_RDWIM:
        mux_specreg = SREG_WIM;
        _write_registers = true;
        break;
    case OP3_RDTBR:
        mux_specreg = SREG_TBR;
        _write_registers = true;
        break;
    case OP3_DEAD:
        cerr << "DETECTED DEADLINE INSTRUCTION IN WRONG PLACE\n" ;
        break;
    case OP3_WRY:

        mux_specreg = ((rd != 0) ? SREG_ASR : SREG_Y);
        wsreg = true;
        aluop = ALU_XOR;
        break;
    case OP3_WRPSR:
        mux_specreg = SREG_PSR;
        aluop = ALU_XOR;
        wsreg = true;
        break;
    case OP3_WRWIM:
        mux_specreg = SREG_WIM;
        aluop = ALU_XOR;
        wsreg = true;
        break;
    case OP3_WRTBR:
        mux_specreg = SREG_TBR;
        aluop = ALU_XOR;
        wsreg = true;
        break;
        // Perform the DMA transfer
//  case OP3_CPOP1:
//    cout << "COPROC: CPOP1" << endl;
//    wsreg = true;
//    break;


    default:
        _unimplemented = true;
        break;
    };

}

void instruction::decode_memory(const OP3_MEMORY& op3) {
    switch (op3) {
    case OP3_ST:
        aluop = ALU_ADD;
        _write_memory = true;
        break;
    case OP3_STB:
        aluop = ALU_ADD;
        _write_memory = true;
        mem_size = MEM_UNSIGNED_BYTE;
        break;
    case OP3_STD:
        aluop = ALU_ADD;
        _write_memory = true;
        db_word = true;
        break;
    case OP3_STH:
        aluop = ALU_ADD;
        _write_memory = true;
        mem_size = MEM_UNSIGNED_HALFWORD;
        break;
    case OP3_LD:
        aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        break;
    case OP3_LDUB:
        aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        mem_size = MEM_UNSIGNED_BYTE;
        break;
    case OP3_LDUH:
        aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        mem_size = MEM_UNSIGNED_HALFWORD;
        break;
    case OP3_LDSB:
        aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        mem_size = MEM_SIGNED_BYTE;
        break;
    case OP3_LDSH:
        aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        mem_size = MEM_SIGNED_HALFWORD;
        break;
    case OP3_LDD:
        aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        mem_size = MEM_DOUBLEWORD;
        db_word = true;
        break;
    case OP3_DEAD:
        //cout << "DETECTED DEADLINE INSTRUCTION IN WRONG PLACE\n" ;
        aluop = ALU_ADD;
        mux_specreg = SREG_DT;
        break;
        // These are opcodes added for DMA coprocessor instructions.
    case OP3_STC:
        //        cout << "COPROC: STC" << endl;
        aluop = ALU_ADD;
        mux_specreg = SREG_CP;
        wsreg = true;
        break;
    default:
        _unimplemented = true;
        break;
    };

}

void instruction::decode_sethi_branches(const OP2& op2) {
    switch (op2) {
    case OP2_SETHI: //NOP
        aluop = ALU_SETHI;
        _write_registers = true;
        break;
    case OP2_BICC:
        _branch = 1;
        break;
    default:
        _unimplemented = true;
        break;
    }
}

void instruction::read_data(uint32_t data_in, int offset) {
    int shamt = offset * 8;
    switch (mem_size) {
    case MEM_SIGNED_BYTE:
        _alu_result = ((signed int) data_in << shamt) >> 24;
        break;
    case MEM_UNSIGNED_BYTE:
        _alu_result = (data_in << shamt) >> 24;
        break;
    case MEM_SIGNED_HALFWORD:
        _alu_result = ((signed int) data_in << shamt) >> 16;
        break;
    case MEM_UNSIGNED_HALFWORD:
        _alu_result = (data_in << shamt) >> 16;
        break;
        //  case MEM_WORD:
    default:
        _alu_result = data_in;
        break;
    }
}

void instruction::set_inst(uint32_t inbits) {
    initialize();
    inst = inbits;
}

uint32_t instruction::get_write_data(uint32_t old_data, int offset) const {

    int shamt = 0;
    int mask = 0xFFFFFFFF;

    switch (mem_size) {
    case MEM_SIGNED_BYTE:
        shamt = (3 - offset) * 8;
        mask = 0xFF << shamt;
        break;
    case MEM_UNSIGNED_BYTE:
        shamt = (3 - offset) * 8;
        mask = 0xFF << shamt;
        break;
    case MEM_SIGNED_HALFWORD:
        shamt = (2 - offset) * 8;
        mask = 0xFFFF << shamt;
        break;

    case MEM_UNSIGNED_HALFWORD:
        shamt = (2 - offset) * 8;
        mask = 0xFFFF << shamt;
        break;
    default:
        shamt = 0;
        mask = 0xFFFFFFFF;
        break;
    }
    return ((op3_val << shamt) & mask) | (old_data & ~mask);
}

instruction::instruction(const instruction& mem) {
    this->operator=(mem); //*this = mem;
}

void instruction::operator=(const instruction & mem) {
    _unimplemented = mem._unimplemented;
    halt = mem.halt;
    pc = mem.pc;
    inst = mem.inst;

    op = mem.op;
    op2 = mem.op2;
    op3 = mem.op3;

    rs1 = mem.rs1;
    rs2 = mem.rs2;
    rd = mem.rd;
    aluop = mem.aluop;
    _write_registers = mem._write_registers;
    _write_memory = mem._write_memory;
    _read_memory = mem._read_memory;
    mem_size = mem.mem_size;
    rs1sel = mem.rs1sel;
    rs2sel = mem.rs2sel;
    rdsel = mem.rdsel;

    wp_increment = mem.wp_increment;
    sp_reg = mem.sp_reg;

    op1_val = mem.op1_val;
    op2_val = mem.op2_val;
    op3_val = mem.op3_val;
    imm = mem.imm;
    use_imm = mem.use_imm;
    _carry = mem._carry;

    _alu_result = mem._alu_result;


    _branch = mem._branch;
    _jump = mem._jump;
    _conditional_branch = mem._conditional_branch;
    _annul = mem._annul;
    _call = mem._call;
    disp30 = mem.disp30;
    wicc = mem.wicc;
    icc = mem.icc;
    disp22 = mem.disp22;
    trap = mem.trap;
    traptype = mem.traptype;
    mux_specreg = mem.mux_specreg;
    wsreg = mem.wsreg;
    db_word = mem.db_word;
    mul_signed = mem.mul_signed;

}
bool instruction::operator==(const instruction & mem) const {
    return halt == mem.halt &&
           _unimplemented == mem._unimplemented &&
           pc == mem.pc &&
           inst == mem.inst &&
           op == mem.op &&
           op2 == mem.op2 &&
           op3 == mem.op3 &&
           rs1 == mem.rs1 &&
           rs2 == mem.rs2 &&
           rd == mem.rd &&
           aluop == mem.aluop &&
           _write_registers == mem._write_registers &&
           _write_memory == mem._write_memory &&
           _read_memory == mem._read_memory &&
           mem_size == mem.mem_size &&
           rs1sel == mem.rs1sel &&
           rs2sel == mem.rs2sel &&
           op1_val == mem.op1_val &&
           op2_val == mem.op2_val &&
           op3_val == mem.op3_val &&
           imm == mem.imm &&
           use_imm == mem.use_imm &&
           _alu_result == mem._alu_result &&
           _branch == mem._branch &&
           _jump == mem._jump &&
           _conditional_branch == mem._conditional_branch &&
           _annul == mem._annul &&
           _call == mem._call &&
           disp30 == mem.disp30 &&
           wicc == mem.wicc &&
           icc == mem.icc &&
           disp22 == mem.disp22 &&
           trap == mem.trap &&
           traptype == mem.traptype &&
           mux_specreg == mem.mux_specreg &&
           wsreg == mem.wsreg &&
           db_word == mem.db_word &&
           _carry == mem._carry &&
           mul_signed == mem.mul_signed;
}


void instruction::initialize() {
    op = OP_SETHI_BRANCHES;
    op2 = OP2_SETHI;
    op3 = OP3_ADD;

    db_word = false;
    halt = false;
    rs1 = 0;
    rs2 = 0;
    imm = 0;
    rd = 0;
    _branch = 0;
    _jump = 0;
    _conditional_branch = 0;
    _annul = 0;
    _call = 0;
    disp30 = 0;
    wicc = 0;
    icc = 0x0;
    disp22 = 0;

    sp_reg.set_window_pointer(0);
    wp_increment = 0;

    rs1sel = SRCMUX_RA;
    rs2sel = SRCMUX_RA;
    rdsel = SRCMUX_RA;

    aluop = ALU_NOP;
    _write_registers = false;
    use_imm = false;
    _write_memory = false;
    _read_memory = false;
    mem_size = MEM_WORD;

    op1_val = 0;
    op2_val = 0;
    op3_val = 0;
    _carry = false;

    _alu_result = 0;
    trap = false;
    _unimplemented = false;
    traptype = 0;

    pc = 0;
    mux_specreg = 0;
    wsreg = false;
    mul_signed = false;
}

bool instruction::check_end_sim() {
    /* This is to simply halt the simulation */
    if (inst == 0x22222222) {
        return true;
    } else {
        return false;
    }
    //    cerr << "instruction:: Received 0x22222222 in program to terminate simulation" << endl;
// #ifdef _NO_SYSTEMC_
//     exit(1);
// #else
//     sc_stop();
// #endif
//   }
}

bool instruction::is_annul() const {
  return _annul;
}

bool instruction::is_carry() const {
  return _carry;
}

bool instruction::is_branch() const {
  return _branch;
}

bool instruction::is_call() const {
  return _call;
}

bool instruction::is_immediate() const {
  return use_imm;
}

bool instruction::is_jump() const {
  return _jump;
}

bool instruction::is_read_memory() const {
  return _read_memory;
}

bool instruction::is_unimplemented() const {
  return _unimplemented;
}

bool instruction::is_write_registers() const {
  return _write_registers;
}

bool instruction::is_write_memory() const {
  return _write_memory;
}

int instruction::get_alu_result() const {
  return _alu_result;
}

short instruction::get_conditional_branch() const {
  return _conditional_branch;
}

int instruction::get_immediate_value() const {
  return imm;
}


void instruction::set_alu_result(const int& result) {
  _alu_result = result;
}

void instruction::set_annul(const bool& annul) {
  _annul = annul;
}

void instruction::set_branch(const bool& branch) {
  _branch = branch;
}

void instruction::set_carry(const bool& carry) {
  _carry = carry;
}

void instruction::set_call(const bool& call) {
  _call = call;
}

void instruction::set_conditional_branch(const short& conditional) {
  _conditional_branch = conditional;
}

void instruction::set_immediate(const bool& immediate) {
  use_imm = immediate;
}


void instruction::set_immediate_value(const int& immediate_value) {
  imm = immediate_value;
}

void instruction::set_jump(const bool& jump) {
  _jump = jump;
}

void instruction::set_read_memory(const bool& read_memory) {
  _read_memory = read_memory;
}

void instruction::set_unimplemented(const bool& unimplemented) {
  _unimplemented = unimplemented;
}

void instruction::set_write_registers(const bool& write) {
  _write_registers = write;
}

void instruction::set_write_memory(const bool& write_memory) {
  _write_memory = write_memory;
}
