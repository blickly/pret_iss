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
#endif /* _NO_SYSTEMC_ */

#include <iostream>
#include <iomanip>
using namespace std;

#include "instruction_defs.h"

///////////////////////////////////////////////////////////////////////
/// instruction
/**
 * This class contains the variables used in storing instruction bits,
 * and decoding the instruction.
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

    /** Get the disp22 field.
     *
     * @return disp22 field.
     */
    int get_disp22() const;

    /** Get the disp30 field.
     *
     * @return disp30 field.
     */
    int get_disp30() const;

    /** Get the instruction condition code (ICC).
     *
     * @return Instruction condition code.
     */
    unsigned char get_icc() const;

    /** Get the immediate operand value.
     *
     * @return Immediate value.
     */
    int get_immediate_value() const;

    /** Get the window pointer increment.
     *
     * @return Window pointer increment.
     */
    short get_increment_window_pointer() const;

    /** Return the memory size being addressed by this instruction.
     *
     * @return Memory size.
     */
    MEMORY_SIZE get_memory_size() const;

    /** Get the OP1 code value.
     *
     * @return Opcode one value.
     */
    int get_op1_value() const;

    /** Get the OP2 code value.
     *
     * @return Opcode two value.
     */
    int get_op2_value() const;

    /** Get the OP3 code value.
     *
     * @return Opcode three value.
     */
    int get_op3_value() const;

    /** FIXME: Get the program counter. Why are there two PCs. In the
    * instruction and the hardware thread.
    *
    * @return Program counter.
    */
    int get_pc() const;

    /** Get the register number for rs1 operand. Source operand 1.
     *
     * @return Register number for rs1.
     */
    REGISTER_NUMBER get_rs1() const;

    /** Get the register number for rs2 operand. Source operand 2.
     *
     * @return Register number for rs2.
     */
    REGISTER_NUMBER get_rs2() const;

    /** Get the register number for rd operand. Destination register.
     *
     * @return Register number for rd.
     */
    REGISTER_NUMBER get_rd() const;

    /** Get the special register selection.
     *
     * @return Special register selected.
     */
    unsigned char get_select_special_register() const ;

    /** Return the data to be written by performing the appropriate byte
     *  alignment shifts.
     *
     * @param old_data This is the old data.
     * @param offset The offset at which to place the data.
     */
    uint32_t get_write_data(uint32_t old_data, int offset) const;

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

    /** Set the ALU result.
     *
     * @param result The result of the ALU operation.
     */
    void set_alu_result(const int& result);

    /** Set the annul bit.
     *
     * @ param annul Anul bit.
     */
    void set_annul(const bool& annul);

    /** Identify this instruction as a branch instruction.
     *
     * @param branch True if instruction is of type branch, else false.
     */
    void set_branch(const bool& branch);

    /** Identify this instruction as a call instruction.
     *
     * @param call True if instruction is call type, else false.
     */
    void set_call(const bool& call);

    /** Set the carry bit for this instruction.
     *
     * @param carry True if carry bit used, else false.
     */
    void set_carry(const bool& carry);

    /** Identify this instruction as a conditional branch.
     *
     * @param conditional True if conditional branch, else false.
     */
    void set_conditional_branch(const short& conditional);

    /** Identify this instruction as performing a double word
     * instruction.
     *
     * @param db_word True if double word instruction, else false.
     */
    void set_db_word(const bool& db_word);

    /** Set the disp22 field.
     *
     * @param disp22 Instruction field.
     */
    void set_disp22(const int& disp22);

    /** Set the disp30 field.
     *
     * @param disp30 Instruction field.
     */
    void set_disp30(const int& disp30);

    /** Set the instruction condition codes.
     *
     * @param icc Instruction condition code.
     */
    void set_icc(const unsigned char& icc);

    /** Identify this instruction as using the immediate field.
     *
     * @param immediate True if instruction uses immediate field, else
     * false.
     */
    void set_immediate(const bool& immediate);

    /** Sets the immediate field.
     *
     * @param immediate_value Immediate value.
     */
    void set_immediate_value(const int& immediate_value);

    /** Increments the window pointer.
     *
     * @param window_pointer Window pointer to increment by.
     */
    void set_increment_window_pointer(const short& window_pointer);

    /** Identify this instruction as a jump instruction.
     *
     * @param jump True if instruction is jump, else false.
     */
    void set_jump(const bool& jump);

    /** Set the op1 value.
     *
     * @param value Operand value.
     */
    void set_op1_value(const int& value);

    /** Set the op2 value.
     *
     * @param value Operand value.
     */
    void set_op2_value(const int& value);

    /** Set the op3 value.
     *
     * @param value Operand value.
     */
    void set_op3_value(const int& value);

    /** Set the program counter.
     *
     * @param pc New program counter.
     */
    void set_pc(const int& pc);

    /** Set the register number for the destination.
     *
     * @param rd Register number of destination register.
     */
    void set_rd(const REGISTER_NUMBER& rd);

    /** Identify instruction as reading from memory.
     *
     * @param read True if reads from memory, else false.
     */
    void set_read_memory(const bool& read);

    /** Identify this instruction as performing a signed multiply.
     *
     * @param signed_multiply True if instruction does a signed
     * multiply, else false.
     */
    void set_signed_multiply(const bool& signed_multiply);

    /** Select the special register.
     *
     * @param select_special Special register.
     */
    void set_select_special_register(const unsigned char& select_special);

    /** Instruction is not implemented.
     *
     * @param unimplemented True if instruction is not implemented, else
     * false.
     */
    void set_unimplemented(const bool& unimplemented);

    /** Identifies this instruction as writing to the instruction
     * condition codes.
     *
     * @param write_icc True if writing back to ICC, else false.
     */
    void set_write_icc(const bool& write_icc);

    /** Identifies this instruction as writing to standard registers.
     *
     * @param write True if writing to standard registers, else false.
     */
    void set_write_registers(const bool& write);

    /** Identifies this instruction as writing to special registers.
     *
     * @param write_special True if writing to the special registers,
     * else false.
     */
    void set_write_special_registers(const bool& write_special);

    /** Identifies this instruction as writing to the memory.
     *
     * @param write_memory True if instruction writes to the memory,
     * else false.
     */
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
    ALU _aluop; /// ALU operation type.
    int _alu_result; /// ALU computation result.
    bool _annul; /// Instruction annul state.
    bool _branch; /// Instruction is of type branch.
    bool _call; /// Instruction is of type call.
    bool _carry; /// Instruction is going to use the carry bit.
    short _conditional_branch; /// Instruction is of conditional branch type.
    bool _db_word_instruction; /// Instruction performs a double word operation.
    int _disp30; /// Instruction's disp30 field.
    int _disp22; /// Instruction's disp22 field.

    unsigned char _icc; /// Instruction condition code.
    short _increment_window_pointer; /// Increment window pointer.
    int _imm; /// Instruction's immediate field.
    uint32_t _inst; /// Instruction in binary format.
    bool _jump; /// Instruction if of type jump.
    MEMORY_SIZE _memory_size; /// Memory size of the read/write to memory.
    short _op1; /// Opcode type 1.
    int _op1_value; /// Opcode 1 value.
    short _op2; /// Opcode type 2.
    int _op2_value; /// Opcode 2 value.
    short _op3; /// Opcode 3.
    int _op3_value; /// Opcode 3 value.
    int _pc; /// Program counter.
    bool _read_memory; /// Instruction performs a read from memory.
    REGISTER_NUMBER _rs1; /// Decode register number for source operand 1.
    SRCMUX _rs1sel; /// Multiplex and select the corresponding register for source 1.
    REGISTER_NUMBER _rs2; /// Decode register number for source operand 2.
    SRCMUX _rs2sel; /// Multiplex and select the corresponding register for source 2.
    REGISTER_NUMBER _rd; /// Decode register number for destination operand.
    SRCMUX _rdsel; /// Multiplex and select the corresponding register for destination.
    unsigned char _select_special_register; /// Select the correct special register.
    bool _signed_multiply; /// Instruction performs a signed multiplication.
    bool _unimplemented; /// Instruction is not implemented.
    bool _use_imm; /// Use instruction's immediate field.
    bool _write_icc; /// Instruction write's to the ICC.
    bool _write_registers; /// Instruction writes to the standard registers.
    bool _write_special_registers; /// Instruction writes to special registers.
    bool _write_memory; /// Instruction writes to the memory.

};

#endif /* _INSTRUCTION_H_ */
