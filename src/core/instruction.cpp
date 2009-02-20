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

                   $Id$

*/

#include "instruction.h"

instruction::instruction() {
    initialize();
    _inst = 0x01000000; //NOP
}

instruction::instruction(uint32_t binary_representation) {
    initialize();
    _inst = binary_representation;
}

void instruction::decode() {
    _op1 = _inst >> 30;
    _op2 = (_inst & 0x01C00000) >> 22;
    _op3 = (_inst & 0X01F80000) >> 19;
    _rd = (_inst & 0x3E000000) >> 25;//0011 1110 0000
    _rs1 = (_inst & 0X0007C000) >> 14;// 0000 0000 0000 0111 1100
    _use_imm = (_inst & 0X00002000) >> 12; //0000 0000 0000 0000 0010
    _imm = (_inst & 0X00001FFF) << 19;
    _imm = _imm >> 19;
    _rs2 = (_inst & 0X0000001F);
    _conditional_branch = (_inst & 0X1E000000) >> 25;
    _annul = (_inst & 0X20000000) >> 29;

    _disp30 = (_inst & 0X3FFFFFFF) << 2;
    _disp30 = _disp30 >> 2;
    _disp22 = (_inst & 0X003FFFFF) << 10;
    _disp22 = _disp22 >> 10;

    switch (_op1) {
    case OP_CALL:
        _call = 1;
        _aluop = ALU_ADD;
        _write_registers = true;
        _rd = 15;
        break;
    case OP_SETHI_BRANCHES:
        decode_sethi_branches(static_cast<OP2>(_op2));
        break;
    case OP_MEMORY:
        decode_memory(static_cast<OP3_MEMORY>(_op3));
        break;
    case OP_ARITHMETIC_ETC:
        decode_arithmetic(static_cast<OP3_ARITHMETIC>(_op3));
        break;
    default:
        _unimplemented = true;
        break;
    };
    if (_rd == 0) {
        _write_registers = false;
    }
}

void instruction::decode_arithmetic(const OP3_ARITHMETIC& op3) {
    switch (op3) {
    case OP3_MULSCC:
        _aluop = ALU_MUL;
        _write_registers = true;
        _signed_multiply = true;
        _write_icc = true;
        break;
    case OP3_ADD:
        _aluop = ALU_ADD;
        _write_registers = true;
        break;
    case OP3_ADDX:
        _aluop = ALU_ADD;
        _write_registers = true;
        _carry = true;
        break;
    case OP3_ADDXCC:
        _aluop = ALU_ADD;
        _write_registers = true;
        _write_icc = true;
        _carry = true;
        break;
    case OP3_AND:
        _aluop = ALU_AND;
        _write_registers = true;
        break;
    case OP3_OR:
        _aluop = ALU_OR;
        _write_registers = true;
        break;
    case OP3_XOR:
        _aluop = ALU_XOR;
        _write_registers = true;
        break;
    case OP3_SUB:
        _aluop = ALU_SUB;
        _write_registers = true;
        break;
    case OP3_SUBX:
        _aluop = ALU_SUB;
        _write_registers = true;
        _carry = true;
        break;
    case OP3_SUBXCC:
        _aluop = ALU_SUB;
        _write_registers = true;
        _carry = true;
        _write_icc = true;
        break;
    case OP3_ANDN:
        _aluop = ALU_ANDN;
        _write_registers = true;
        break;
    case OP3_ORN:
        _aluop = ALU_ORN;
        _write_registers = true;
        break;
    case OP3_XNOR:
        _aluop = ALU_XNOR;
        _write_registers = true;
        break;
    case OP3_ADDCC:
        _aluop = ALU_ADD;
        _write_registers = true;
        _write_icc = true;
        break;
    case OP3_ANDCC:
        _aluop = ALU_AND;
        _write_registers = true;
        _write_icc = true;
        break;
    case OP3_ORCC:
        _aluop = ALU_OR;
        _write_registers = true;
        _write_icc = true;
        break;
    case OP3_XORCC:
        _aluop = ALU_XOR;
        _write_registers = true;
        _write_icc = true;
        break;
    case OP3_SUBCC:
        _aluop = ALU_SUB;
        _write_registers = true;
        _write_icc = true;
        break;
    case OP3_ANDNCC:
        _aluop = ALU_ANDN;
        _write_registers = true;
        _write_icc = true;
        break;
    case OP3_ORNCC:
        _aluop = ALU_ORN;
        _write_registers = true;
        _write_icc = true;
        break;
    case OP3_XNORCC:
        _aluop = ALU_XNOR;
        _write_registers = true;
        _write_icc = true;
        break;
    case OP3_JMPL:
        _jump = 1;
        _write_registers = true;
        _aluop = ALU_ADD;
        break;
    case OP3_RETT:
        _is_rett = 1;      
        _jump = 1;
        _aluop = ALU_ADD;
        _increment_window_pointer = 1;
        break;
    case OP3_SAVE:
        _increment_window_pointer = -1;
        _aluop = ALU_ADD;
        _write_registers = true;
        break;
    case OP3_RESTORE:
        _increment_window_pointer = 1;
        _aluop = ALU_ADD;
        _write_registers = true;
        break;
    case OP3_SLL:
        _aluop = ALU_SLL;
        _write_registers = true;
        break;
    case OP3_SRL:
        _aluop = ALU_SRL;
        _write_registers = true;
        break;
    case OP3_SRA:
        _aluop = ALU_SRA;
        _write_registers = true;
        break;
    case OP3_RDPSR:
        _select_special_register = SREG_PSR;
        _write_registers = true;
        break;
    case OP3_RDASR:
        _select_special_register = (_rs1 ? SREG_ASR : SREG_Y);
        _write_registers = true;
        break;
    case OP3_RDWIM:
        _select_special_register = SREG_WIM;
        _write_registers = true;
        break;
    case OP3_RDTBR:
        _select_special_register = SREG_TBR;
        _write_registers = true;
        break;
    case OP3_DEAD:
        cerr << "DETECTED DEADLINE INSTRUCTION IN WRONG PLACE\n" ;
        break;
    case OP3_WRY:
        _select_special_register = ((_rd != 0) ? SREG_ASR : SREG_Y);
        _write_special_registers = true;
        _aluop = ALU_XOR;
        break;
    case OP3_WRPSR:
        _select_special_register = SREG_PSR;
        _aluop = ALU_XOR;
        _write_special_registers = true;
        break;
    case OP3_WRWIM:
        _select_special_register = SREG_WIM;
        _aluop = ALU_XOR;
        _write_special_registers = true;
        break;
    case OP3_WRTBR:
        _select_special_register = SREG_TBR;
        _aluop = ALU_XOR;
        _write_special_registers = true;
        break;
    default:
        _unimplemented = true;
        break;
    };

}

void instruction::decode_memory(const OP3_MEMORY& op3) {
    switch (op3) {
    case OP3_ST:
        _aluop = ALU_ADD;
        _write_memory = true;
        break;
    case OP3_STB:
        _aluop = ALU_ADD;
        _write_memory = true;
        _memory_size = MEM_UNSIGNED_BYTE;
        break;
    case OP3_STD:

      _aluop = ALU_ADD;
        _write_memory = true;
        _db_word_instruction = true;
        break;
    case OP3_STH:
        _aluop = ALU_ADD;
        _write_memory = true;
        _memory_size = MEM_UNSIGNED_HALFWORD;
        break;
    case OP3_LD:
        _aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        break;
    case OP3_LDUB:
        _aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        _memory_size = MEM_UNSIGNED_BYTE;
        break;
    case OP3_LDUH:
        _aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        _memory_size = MEM_UNSIGNED_HALFWORD;
        break;
    case OP3_LDSB:
        _aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        _memory_size = MEM_SIGNED_BYTE;
        break;
    case OP3_LDSH:
        _aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        _memory_size = MEM_SIGNED_HALFWORD;
        break;
    case OP3_LDD:
        _aluop = ALU_ADD;
        _write_registers = true;
        _read_memory = true;
        _memory_size = MEM_DOUBLEWORD;
        _db_word_instruction = true;
        break;
    case OP3_DEAD:
        //cout << "DETECTED DEADLINE INSTRUCTION IN WRONG PLACE\n" ;
        _aluop = ALU_ADD_DEAD;
        _select_special_register = SREG_DT;
        break;
        // These are opcodes added for DMA coprocessor instructions.
    case OP3_MEM2SP:
        // FIXME: Why do deadline and DMA use _select_special_register?
      //      cout << "COPROC: STC" << endl;
        _write_special_registers = true;
        _aluop = ALU_ADD; 
        _select_special_register = SREG_MEM2SP;
        break;
    default:
        _unimplemented = true;
        break;
    };

}

void instruction::decode_sethi_branches(const OP2& op2) {
    switch (op2) {
    case OP2_SETHI: //NOP
        _aluop = ALU_SETHI;
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
    switch (_memory_size) {
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
    _inst = inbits;
}

uint32_t instruction::get_write_data(uint32_t old_data, int offset) const {

    int shamt = 0;
    int mask = 0xFFFFFFFF;

    switch (_memory_size) {
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
    return ((_op3_value << shamt) & mask) | (old_data & ~mask);
}

instruction::instruction(const instruction& mem) {
    this->operator=(mem); //*this = mem;
}

void instruction::operator=(const instruction & from_instruction) {
    _unimplemented = from_instruction._unimplemented;
    _pc = from_instruction._pc;
    _inst = from_instruction._inst;

    _op1 = from_instruction._op1;
    _op2 = from_instruction._op2;
    _op3 = from_instruction._op3;

    _rs1 = from_instruction._rs1;
    _rs2 = from_instruction._rs2;
    _rd = from_instruction._rd;
    _aluop = from_instruction._aluop;
    _write_registers = from_instruction._write_registers;
    _write_memory = from_instruction._write_memory;
    _read_memory = from_instruction._read_memory;
    _memory_size = from_instruction._memory_size;
    _rs1sel = from_instruction._rs1sel;
    _rs2sel = from_instruction._rs2sel;
    _rdsel = from_instruction._rdsel;

    _increment_window_pointer = from_instruction._increment_window_pointer;

    _op1_value = from_instruction._op1_value;
    _op2_value = from_instruction._op2_value;
    _op3_value = from_instruction._op3_value;
    _imm = from_instruction._imm;
    _use_imm = from_instruction._use_imm;
    _carry = from_instruction._carry;

    _alu_result = from_instruction._alu_result;


    _branch = from_instruction._branch;
    _jump = from_instruction._jump;
    _conditional_branch = from_instruction._conditional_branch;
    _annul = from_instruction._annul;
    _call = from_instruction._call;
    _disp30 = from_instruction._disp30;
    _write_icc = from_instruction._write_icc;
    _icc = from_instruction._icc;
    _disp22 = from_instruction._disp22;
    _select_special_register = from_instruction._select_special_register;
    _write_special_registers = from_instruction._write_special_registers;
    _db_word_instruction = from_instruction._db_word_instruction;
    _signed_multiply = from_instruction._signed_multiply;
    _is_rett = from_instruction._is_rett;

}

bool instruction::operator==(const instruction& compare_instruction) const {
    return _unimplemented == compare_instruction._unimplemented &&
           _pc == compare_instruction._pc &&
           _inst == compare_instruction._inst &&
           _op1 == compare_instruction._op1 &&
           _op2 == compare_instruction._op2 &&
           _op3 == compare_instruction._op3 &&
           _rs1 == compare_instruction._rs1 &&
           _rs2 == compare_instruction._rs2 &&
           _rd == compare_instruction._rd &&
           _aluop == compare_instruction._aluop &&
           _write_registers == compare_instruction._write_registers &&
           _write_memory == compare_instruction._write_memory &&
           _read_memory == compare_instruction._read_memory &&
           _memory_size == compare_instruction._memory_size &&
           _rs1sel == compare_instruction._rs1sel &&
           _rs2sel == compare_instruction._rs2sel &&
           _rdsel == compare_instruction._rdsel &&
           _op1_value == compare_instruction._op1_value &&
           _op2_value == compare_instruction._op2_value &&
           _op3_value == compare_instruction._op3_value &&
           _imm == compare_instruction._imm &&
           _use_imm == compare_instruction._use_imm &&
           _alu_result == compare_instruction._alu_result &&
           _branch == compare_instruction._branch &&
           _jump == compare_instruction._jump &&
           _conditional_branch == compare_instruction._conditional_branch &&
           _annul == compare_instruction._annul &&
           _call == compare_instruction._call &&
           _disp30 == compare_instruction._disp30 &&
           _write_icc == compare_instruction._write_icc &&
           _icc == compare_instruction._icc &&
           _disp22 == compare_instruction._disp22 &&
           _select_special_register == compare_instruction._select_special_register &&
           _write_special_registers == compare_instruction._write_special_registers &&
           _db_word_instruction == compare_instruction._db_word_instruction &&
           _carry == compare_instruction._carry &&
           _is_rett == compare_instruction._is_rett &&
           _signed_multiply == compare_instruction._signed_multiply;
    
}


void instruction::initialize() {
    _op1 = OP_SETHI_BRANCHES;
    _op2 = OP2_SETHI;
    _op3 = OP3_ADD;

    _db_word_instruction = false;

    _rs1 = 0;
    _rs2 = 0;
    _imm = 0;
    _rd = 0;
    _branch = 0;
    _jump = 0;
    _conditional_branch = 0;
    _annul = 0;
    _call = 0;
    _disp30 = 0;
    _write_icc = 0;
    _icc = 0x0;
    _disp22 = 0;

    _increment_window_pointer = 0;

    _rs1sel = SRCMUX_RA;
    _rs2sel = SRCMUX_RA;
    _rdsel = SRCMUX_RA;

    _aluop = ALU_NOP;
    _write_registers = false;
    _use_imm = false;
    _write_memory = false;
    _read_memory = false;
    _memory_size = MEM_WORD;

    _op1_value = 0;
    _op2_value = 0;
    _op3_value = 0;
    _carry = false;

    _alu_result = 0;
    _unimplemented = false;

    _pc = 0;
    _select_special_register = 0;
    _write_special_registers = false;
    _signed_multiply = false;
    _is_rett = false;
}

bool instruction::check_end_sim() {
    /* This is to simply halt the simulation */
    if (_inst == 0x22222222) {
        return true;
    } else {
        return false;
    }
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

bool instruction::is_db_word() const {
    return _db_word_instruction;
}

bool instruction::is_immediate() const {
    return _use_imm;
}

bool instruction::is_jump() const {
    return _jump;
}

bool instruction::is_read_memory() const {
    return _read_memory;
}

bool instruction::is_signed_multiply() const {
    return _signed_multiply;
}

bool instruction::is_unimplemented() const {
    return _unimplemented;
}

bool instruction::is_write_icc() const {
    return _write_icc;
}

bool instruction::is_write_registers() const {
    return _write_registers;
}

bool instruction::is_write_special_registers() const {
    return _write_special_registers;
}

bool instruction::is_write_memory() const {
    return _write_memory;
}

ALU instruction::get_aluop() const {
    return _aluop;
}

int instruction::get_alu_result() const {
    return _alu_result;
}

short instruction::get_conditional_branch() const {
    return _conditional_branch;
}

int instruction::get_disp22() const {
    return _disp22;
}

int instruction::get_disp30() const {
    return _disp30;
}

unsigned char instruction::get_icc() const {
    return _icc;
}

int instruction::get_immediate_value() const {
    return _imm;
}

short instruction::get_increment_window_pointer() const {
    return _increment_window_pointer;
}

uint32_t instruction::get_instruction() const {
    return _inst;
}

MEMORY_SIZE instruction::get_memory_size() const {
    return _memory_size;
}

int instruction::get_op1_value() const {
    return _op1_value;
}

int instruction::get_op2_value() const {
    return _op2_value;
}

int instruction::get_op3_value() const {
    return _op3_value;
}

int instruction::get_pc() const {
    return _pc;
}

REGISTER_NUMBER instruction::get_rs1() const {
    return _rs1;
}

REGISTER_NUMBER instruction::get_rs2() const {
    return _rs2;
}

REGISTER_NUMBER instruction::get_rd() const {
    return _rd;
}

unsigned char instruction::get_select_special_register() const {
    return _select_special_register;
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

void instruction::set_db_word(const bool& db_word) {
    _db_word_instruction = db_word;
}

void instruction::set_conditional_branch(const short& conditional) {
    _conditional_branch = conditional;
}

void instruction::set_disp22(const int& disp22) {
    _disp22 = disp22;
}

void instruction::set_disp30(const int& disp30) {
    _disp30 = disp30;
}

void instruction::set_icc(const unsigned char& icc) {
    _icc = icc;
}

void instruction::set_immediate(const bool& immediate) {
    _use_imm = immediate;
}

void instruction::set_increment_window_pointer(const short& window_pointer) {
    _increment_window_pointer = window_pointer;
}

void instruction::set_immediate_value(const int& immediate_value) {
    _imm = immediate_value;
}

void instruction::set_jump(const bool& jump) {
    _jump = jump;
}

void instruction::set_op1_value(const int& value) {
    _op1_value = value;
}

void instruction::set_op2_value(const int& value) {
    _op2_value = value;
}

void instruction::set_op3_value(const int& value) {
    _op3_value = value;
}

void instruction::set_pc(const int& pc) {
    _pc = pc;
}

void instruction::set_rd(const REGISTER_NUMBER& rd) {
    _rd = rd;
}

void instruction::set_read_memory(const bool& read_memory) {
    _read_memory = read_memory;
}

void instruction::set_select_special_register(const unsigned char& special_register) {
    _select_special_register = special_register;
}

void instruction::set_unimplemented(const bool& unimplemented) {
    _unimplemented = unimplemented;
}

void instruction::set_write_icc(const bool& write_icc) {
    _write_icc = write_icc;
}

void instruction::set_write_registers(const bool& write) {
    _write_registers = write;
}

void instruction::set_write_special_registers(const bool& write_special) {
    _write_special_registers = write_special;
}

void instruction::set_write_memory(const bool& write_memory) {
    _write_memory = write_memory;
}

void instruction::set_signed_multiply(const bool& signed_multiply) {
    _signed_multiply = signed_multiply;
}

void instruction::set_is_rett(const bool& is_rett){
  _is_rett = is_rett;
}


bool instruction::get_is_rett() {
  return _is_rett;
}
