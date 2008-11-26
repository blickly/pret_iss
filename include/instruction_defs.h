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

#ifndef _INSTRUCTION_DEFS_H_
#define _INSTRUCTION_DEFS_H_

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////
/// REGISTER_NUMBER
/**
 * Representing register windows as short unsigned integers.
 */
typedef short unsigned int REGISTER_NUMBER;

///////////////////////////////////////////////////////////////////////
/// WINDOW_POINTER
/**
 * Representing register windows as short unsigned integers.
 */
typedef short unsigned int WINDOW_POINTER;


///////////////////////////////////////////////////////////////////////
/// ALU
/**
 * Encoding of the ALU operation.
 *
 */
enum ALU {
    ALU_NOP,
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR,
    ALU_SLL,
    ALU_SRL,
    ALU_SRA,
    ALU_XNOR,
    ALU_ANDN,
    ALU_ORN,
    ALU_SETHI,
    ALU_MUL
};

///////////////////////////////////////////////////////////////////////
/// BRANCH_CONDITIONS
/**
 * Encoding of supported branching conditions.
 *
 */
enum BRANCH_CONDITIONS {
    BRCH_BA = 8,
    BRCH_BN = 0,
    BRCH_BNE = 9,
    BRCH_BE = 1,
    BRCH_BG = 10,
    BRCH_BLE = 2,
    BRCH_BGE = 11,
    BRCH_BL = 3,
    BRCH_BGU = 12,
    BRCH_BLEU = 4,
    BRCH_BCC = 13,
    BRCH_BCS = 5,
    BRCH_BPOS = 14,
    BRCH_BNEG = 6,
    BRCH_BVC = 15,
    BRCH_BVS = 7
};

///////////////////////////////////////////////////////////////////////
/// MEMORY_SIZE
/**
 * Encoding of supported memory sizes.
 *
 */
enum MEMORY_SIZE {
    MEM_SIGNED_BYTE,
    MEM_UNSIGNED_BYTE,
    MEM_SIGNED_HALFWORD,
    MEM_UNSIGNED_HALFWORD,
    MEM_WORD,
    MEM_DOUBLEWORD
};

///////////////////////////////////////////////////////////////////////
/// OP
/**
 * Encoding of opcode of type 1.
 *
 */
enum OP {
    OP_SETHI_BRANCHES = 0,
    OP_CALL = 1,
    OP_ARITHMETIC_ETC = 2,
    OP_MEMORY = 3
};

///////////////////////////////////////////////////////////////////////
/// OP2
/**
 * Encoding of opcode type 2.
 *
 */
enum OP2 {
    OP2_BICC = 2,
    OP2_SETHI = 4,
    OP2_FBFCC = 6,
    OP2_CBCCC = 7
};


///////////////////////////////////////////////////////////////////////
/// OP3_ARITHMETIC
/**
 * Encoding of opcode type 3 for arithmetic operations.
 *
 */
enum OP3_ARITHMETIC {
    OP3_ADD = 0x0,
    OP3_AND = 0x1,
    OP3_OR = 0x2,
    OP3_XOR = 0x3,
    OP3_SUB = 0x4,
    OP3_SUBX = 0xC,
    OP3_SUBXCC = 0x1C,
    OP3_ANDN = 0x5,
    OP3_ORN = 0x6,
    OP3_XNOR = 0x7,
    OP3_ADDCC = 0x10,
    OP3_ADDX = 0x8,
    OP3_ADDXCC = 0x18,
    OP3_ANDCC = 0x11,
    OP3_ORCC = 0x12,
    OP3_XORCC = 0x13,
    OP3_SUBCC = 0x14,
    OP3_ANDNCC = 0x15,
    OP3_ORNCC = 0x16,
    OP3_XNORCC = 0x17,
    OP3_SLL = 0x25,
    OP3_SRL = 0x26,
    OP3_SRA = 0x27,
    OP3_RDY = 0x28,
    OP3_RDASR = 0x28,
    OP3_RDPSR = 0x29,
    OP3_RDWIM = 0x2A,
    OP3_RDTBR = 0x2B,
    OP3_DEAD = 0x2C,
    OP3_WRY = 0x30,
    OP3_WRASR = 0x30,
    OP3_WRPSR = 0x31,
    OP3_WRWIM = 0x32,
    OP3_WRTBR = 0x33,
    OP3_JMPL = 0x38,
    OP3_SAVE = 0x3c,
    OP3_RESTORE = 0x3d,
    OP3_MULSCC = 0x24

};

///////////////////////////////////////////////////////////////////////
/// OP3_COPROC
/**
 * Encoding of opcode type 3 for coprocessor instructions.
 *
 */
enum OP3_COPROC {
    // Coprocessor opcodes.
    OP3_STC = 0x34,
    OP3_CPOP1 = 0x36
};

///////////////////////////////////////////////////////////////////////
/// OP3_MEMORY
/**
 * Encoding of opcode type 3 for memory operations.
 *
 */
enum OP3_MEMORY {
    OP3_LD = 0x0,
    OP3_ST = 0x4,
    OP3_STB = 0x5,
    OP3_STD = 0x7,
    OP3_STH = 0x6,
    OP3_LDSB = 0x9,
    OP3_LDSH = 0xa,
    OP3_LDUB = 0x1,
    OP3_LDUH = 0x2,
    OP3_LDD = 0x3
};

///////////////////////////////////////////////////////////////////////
/// SRCMUX
/**
 * Multiplexes the source operand registers for each of the stages.
 *
 */
enum SRCMUX {
    SRCMUX_RA,
    SRCMUX_EX,
    SRCMUX_ME,
    SRCMUX_IMM,
    SRCMUX_ZERO,
    SRCMUX_XC
};

///////////////////////////////////////////////////////////////////////
/// SREG_MUX
/**
 * Multiplexes the selection of special registers.
 *
 */
enum SREG_MUX {
    SREG_Y = 1,
    SREG_ASR = 2,
    SREG_PSR = 3,
    SREG_WIM = 4,
    SREG_TBR = 5,
    SREG_DT = 6,
    SREG_CP = 7
};


#endif /* _INSTRUCTION_DEFS_H_ */
