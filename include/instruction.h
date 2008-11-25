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
#include "special_registers.h"

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
        out << setw(8) << setfill('0') << hex << output_instruction.inst;
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
public:    // Flag to see if instruction is a double word instruction
    bool db_word;

    uint32_t inst;
    int pc;
    bool halt;

  short op;
  short op2;
  short op3;

    REGISTER_NUMBER rs1;
    REGISTER_NUMBER rs2;
    REGISTER_NUMBER rd;
    ALU aluop;
    bool wreg;
    bool wicc;
    bool write_mem;
    bool read_mem;
    MEMORY_SIZE mem_size;



    int disp30;
    int disp22;

    special_registers sp_reg;
    int op1_val;
    int op2_val;
    int op3_val; // Value of register rd
    unsigned char icc;

    unsigned char mux_specreg;
    bool wsreg;
    short wp_increment;  
    /// Whether it is a signed multiple or not
    bool mul_signed;
  bool is_annul() const;
  bool is_branch() const;
  bool is_carry() const;
  bool is_call() const;
  bool is_immediate() const;
  bool is_jump() const;
  bool is_unimplemented() const;  
  
  int get_alu_result() const;
  short get_conditional_branch() const;
  int get_immediate_value() const;
  
  void set_alu_result(const int& result);
  void set_annul(const bool& annul);
  void set_branch(const bool& branch);
  void set_call(const bool& call);
  void set_carry(const bool& carry);
  void set_conditional_branch(const short& conditional);
  void set_immediate(const bool& immediate);
  void set_immediate_value(const int& immediate_value);
  void set_jump(const bool& jump);
  void set_unimplemented(const bool& unimplemented);
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
      int _alu_result;
      bool _annul;
      bool _branch;
      short _conditional_branch;
    bool _jump;

  bool _call;
    SRCMUX rs1sel;
    SRCMUX rs2sel;
    SRCMUX rdsel;
    bool _carry;
    bool use_imm;
      int imm;
      bool _unimplemented;
  
  /* FIXME: These are probably unimplemented.
   */
      bool trap;
      unsigned char traptype;

};

#endif /* _INSTRUCTION_H_ */
