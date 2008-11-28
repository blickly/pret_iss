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
#else
#include "no_systemc.h"
#endif

#include <iostream>
#include <iomanip>
using namespace std;

#include "instruction_defs.h"
//#include "special_registers.h"

///////////////////////////////////////////////////////////////////////
/// instruction
/**
 * This class contains the data members used in decoding an
 * instruction.
 *
 * @author  Isaac Liu
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class instruction {

///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
public:
    /** Create a NOP instruction.
     */
    instruction();

    /** Create an instruction from a binary representation
     * of the machine code.
     *
     * @param binary_representation This instruction's binary representation
     */
    instruction(uint32_t binary_representation);


    /** Copy constructor.
     *
     * @param from_instruction Instruction to copy from.
     */
    instruction(const instruction& from_instruction);

    /** Terminate simulation if this returns true.
     *
     */
    bool check_end_sim();

    /** Decode the binary representation into meaningful values in all
     * the other fields of the instruction object.
     */
    void decode();

    /** Decode the binary representation for arithmetic instruction of
     * type opcode 3.
     *
     * @param op3 The opcode to be decoded.
     */
    void decode_arithmetic(const OP3_ARITHMETIC& op3);

    /** Decode the binary representation memory instructions of type
     * opcode 3.
     *
     * @param op3 The opcode to be decoded.
     */
    void decode_memory(const OP3_MEMORY& op3);

    /** Decode the binary representation for sethi instructions of type
     * opcode 2.
     *
     * @param op2 The opcode to be decoded.
     */
    void decode_sethi_branches(const OP2& op2);

    /** Return the data to be written by performing the appropriate byte
     *  alignment shifts.
     *
     * @param old_data This is the old data.
     * @param offset The offset at which to place the data.
     */
    uint32_t get_write_data(uint32_t old_data, int offset) const;

    /** Overloaded assignment operator.
     *
     * @param from_instruction Instruction assigned from.
     */
    void operator=(const instruction& from_instruction);

    /** Overloaded equality operator.
     *
     * @is_instruction Instruction to check if equal.
     */
    bool operator==(const instruction& is_instruction) const;

    /** Print out the contents of the instruction.
     *
     * @param out Output stream.
     * @param output_instruction Instruction to output.
     */
    inline friend ostream& operator<<(ostream& out, const instruction& output_instruction) {
        out << setw(8) << setfill('0') << hex << output_instruction._inst;
        return out;
    }

    /* Stores data read from memory into the correct format in keeping
     * with the type of load that this instruction implements.
     *
     * @param data_in The data read from memory.
     */
    void read_data(uint32_t data_in, int offset);

    /** Sets the bits of the current instruction.
     *
     * @param bits Instruction bits for current instruction.
     */
    void set_inst(uint32_t bits);

///////////////////////////////////////////////////////////////////////
///                      public variables                           ///
public:


  /** Returns true if the annul bit is set for the instruction.
   *
   * @return True if annul is set, else false.
   */
    bool is_annul() const;

  /** Returns true if instruction is of branch type.
   *
   * @return True if branch instruction, else false.
   */
  bool is_branch() const;

  /** Returns true if instruction's carry bit is set.
   *
   * @return True if carry bit set, else false.
   */
    bool is_carry() const;

  /** Returns true if call instruction.
   *
   * @return True if call instruction, else false.
   */
    bool is_call() const;

  /** Returns true if instruction is a double word instruction.
   *
   * @return True if double word instruction, else false.
   */
    bool is_db_word() const;

    /** Returns true if instruction uses immediate field.
   *
   * @return True if immediate field is used, else false.
   */
    bool is_immediate() const;

  /** Returns true if instruction is of jump type.
   *
   * @return True if jump instruction, else false.
   */
    bool is_jump() const;

  /** Returns true if instruction is going to read from memory.
   *
   * @return True if memory read instruction, else false.
   */
    bool is_read_memory() const;

  /** Returns true if instruction performs a signed multiply.
   *
   * @return True if signed multiply instruction, else false.
   */
    bool is_signed_multiply() const;

    /** Returns true if instruction is not implemented.
   *
   * @return True if branch instruction, else false.
   */
    bool is_unimplemented() const;

    /** Returns true if instruction writes to the instruction
   * condition codes.
   *
   * @return True if writing to ICC, else false.
   */
    bool is_write_icc() const;

  /** Returns true if commit to registers is required.
   *
   * @return True if instruction writes to registers, else false.
   */
    bool is_write_registers() const;

    /** Returns true if instruction writes to special registers such
   * as deadline timers.
   *
   * @return True if commit to special registers, else false.
   */
    bool is_write_special_registers() const;

  /** Returns true if instruction writes to the memory.
   *
   * @return True if write to memory, else false.
   */
    bool is_write_memory() const;

  /** Get the ALU operation.
   *
   * @return ALU operation.
   */
    ALU get_aluop() const;

  /** Get result from ALU operation.
   *
   * @return ALU result.
   */
    int get_alu_result() const;

  /** Get the conditional branch.
   *
   * @return Conditional branch.
   */
    short get_conditional_branch() const;
    int get_disp22() const;
    int get_disp30() const;
    unsigned char get_icc() const;
    int get_immediate_value() const;
    short get_increment_window_pointer() const;
    MEMORY_SIZE get_memory_size() const;
    int get_op1_value() const;
    int get_op2_value() const;
    int get_op3_value() const;
    int get_pc() const;
    REGISTER_NUMBER get_rs1() const;
    REGISTER_NUMBER get_rs2() const;
    REGISTER_NUMBER get_rd() const;
    unsigned char get_select_special_register() const ;
  //    special_registers get_special_registers() const;
  //    WINDOW_POINTER get_window_pointer() const;

    void set_alu_result(const int& result);
    void set_annul(const bool& annul);
    void set_branch(const bool& branch);
    void set_call(const bool& call);
    void set_carry(const bool& carry);
    void set_conditional_branch(const short& conditional);
    void set_db_word(const bool& db_word);
    void set_disp22(const int& disp22);
    void set_disp30(const int& disp30);
    void set_icc(const unsigned char& icc);
    void set_immediate(const bool& immediate);
    void set_immediate_value(const int& immediate_value);
    void set_increment_window_pointer(const short& window_pointer);
    void set_jump(const bool& jump);
    void set_op1_value(const int& value);
    void set_op2_value(const int& value);
    void set_op3_value(const int& value);
    void set_pc(const int& pc);
    void set_rd(const REGISTER_NUMBER& rd);
    void set_read_memory(const bool& read);
    void set_signed_multiply(const bool& signed_multiply);
    void set_select_special_register(const unsigned char& select_special);
    void set_unimplemented(const bool& unimplemented);
  //    void set_window_pointer(const WINDOW_POINTER& window_pointer);

    void set_write_icc(const bool& write_icc);
    void set_write_registers(const bool& write);
    void set_write_special_registers(const bool& write_special);
    void set_write_memory(const bool& write_memory);
///////////////////////////////////////////////////////////////////////
///                      private methods                          ///
private:
    /** Initialize the data elements to be reasonable default values.
     * By default, an instruction does performs a null operation.
     */
    void initialize();

///////////////////////////////////////////////////////////////////////
///                      private variables                          ///
private:
    ALU _aluop;
    int _alu_result;
    bool _annul;
    bool _branch;
    bool _call;
    bool _carry;
    short _conditional_branch;
    bool _db_word_instruction;
    int _disp30;
    int _disp22;

    unsigned char _icc;
    short _increment_window_pointer;
    int _imm;
    uint32_t _inst;
    bool _jump;
    MEMORY_SIZE _memory_size;
    short _op1;
    int _op1_value;
    short _op2;
    int _op2_value;
    short _op3;
    int _op3_value; // Value of register rd
    int _pc;
    bool _read_memory;
    REGISTER_NUMBER _rs1;
    SRCMUX _rs1sel;
    REGISTER_NUMBER _rs2;
    SRCMUX _rs2sel;
    REGISTER_NUMBER _rd;
    SRCMUX _rdsel;
    unsigned char _select_special_register;
    bool _signed_multiply;
    bool _unimplemented;
    bool _use_imm;
    bool _write_icc;
    bool _write_registers;
    bool _write_special_registers;
    bool _write_memory;

};

#endif /* _INSTRUCTION_H_ */
