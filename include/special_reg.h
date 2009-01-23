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

#ifndef _SPECIAL_REG_H_
#define _SPECIAL_REG_H_
#include <stdint.h>
#include "instruction.h"
#include "defines.h"

enum DT_STATUS {
    UNSET,
    SET,
    MISSED
};

///////////////////////////////////////////////////////////////////////
/// special_reg
/**
 * The special register class provides a representation of the processor
 * state registers. The processor state registers stores implementation
 * specific information and the special states of the processor. This
 * includes the integer condition code, the current window pointer,
 * bits to record whether there is a coprocessor or floating point unit
 * etc. To understand what each value means, please read the sparc V8
 * manual that can be found online.
 *
 * @author  Isaac Liu
 * @version $Id$
 * @Pt.ProposedRating red blickly
 * @Pt.AcceptedRating
 */
class special_reg {
public:
///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
    /** Create the register file
     */
    special_reg();

    /** Initialize the special register with predifined values
     *
     *  @param initial_register the special_reg to be copied over
     */
    special_reg(const special_reg& initial_register);

    /** Overloaded assignment operator.
     *
     * @param from_register special_reg to be assigned from
     */
    void operator=(const special_reg& from_register);

    /** Overloaded equality operator.
     *
     * @param is_register special register to check if equal.
     */
    bool operator==(const special_reg& is_register);

    /** Returns the current carry bit of the processor
     *
     * @param return The carry bit stored in the processor state register
     */
    uint32_t get_carry_bit();

    /** Returns the whole processor state register in its original register form
     *
     * @param return Returns the whole processor state register
     */
    uint32_t get_psr();

    /** Print to <i>cout</i> stream the debugging output for the processor state
     * register in the pipeline. This is only enabled if <i>DBG_PIPE</i> flag or
     *  other debugging flags are enabled during compilation.
     */
    void dump();

    /** Print to <i>cout</i> stream the debugging output for deadline registers
     * in the pipeline. This is only enabled if <i>DBG_PIPE</i> flag or
     *  other debugging flags are enabled during compilation.
     */
    void dump_deadline_timers();

    /** Decrements the deadline registers each by one if the value is not zero.
     */
    void decrement_deadline_timers();

    /** Decrements the phase lock loop registers each by one if the value is not zero.
     */
    void decrement_pll_timers();

    /** Detects if there are any missed deadlines in the pipeline. Returns true if there
     * is, and stores the number of the first register it found that missed the deadline.
     *
     * @return True if a deadline had been missed
     * @param deadline_register_num The number of the deadline register that missed its deadline
     */
    bool missed_deadline(uint32_t& deadline_register_num);

    /** Sets the processor state register with the values of the input processor state register.
     *
     * @param from_register The register to copy the processor state from
     */
    void set_psr(uint32_t from_register);

    /** Sets the implementation version of the processor
     *
     * @param imp_ver the implementation version to set our processor to
     */
    void set_imp_ver(unsigned char imp_ver);

    /** Sets the current window pointer value of the processor
     *
     * @param curr_wp the current window pointer to set our processor to
     */
    void set_curr_wp(WINDOW_POINTER curr_wp);

    /** Sets the integer condition code of the processor
     *
     * @param icc the integer condition code to set our processor to
     */
    void set_icc(unsigned char icc);

    /** Sets the supervisor bit of the processor
     *
     * @param s the supervisor bit to set our processor to
     */
    void set_s(bool s);

    /** Sets the previous supervisor bit value of the processor
     *
     * @param ps the previous supervisor bit to set our processor to
     */
    void set_ps(bool ps);

    /** Sets the trap base register value of the processor
     *
     * @param tbr the trap base register to set our processor to
     */
    void set_tbr(uint32_t tbr);

    /** Sets the Window Invalid Mask Register value of the processor
     *
     * @param wim the Window Invalid Mask Register to set our processor to
     */
    void set_wim(uint32_t wim);

    /** Sets the enable trap value of the processor
     *
     * @param et the enable trap value to set our processor to
     */
    void set_et(bool et);

    /** Sets the enable coprocessor value of the processor
     *
     * @param ec the enable coprocessor value to set our processor to
     */
    void set_ec(bool ec);

    /** Sets the enable floating point unit value of the processor
     *
     * @param ef the enable floating point unit value to set our processor to
     */
    void set_ef(bool ef);

    /** Sets the processor interrupt level of the processor
     *
     * @param pil the processor interrupt level to set our processor to
     */
    void set_pil(unsigned char pil);

    /** Sets the Ancillary State Registers value of the processor
     *
     * @param asr_value the Ancillary State Register value to set our processor to
     * @param asr_index the index of the Ancillary State Register to set the value to
     */
    void set_asr(uint32_t asr_value, int asr_index);

    /** Sets the multiply/divide value of the processor
     *
     * @param y the multiply/divide value to set our processor to
     */
    void set_y(uint32_t y);

    /** Sets the deadline registers of the processor
     *
     * @param dt_value the value to set the deadline register to
     * @param dt_index the index of the Deadline Register to set the value to
     */
    void set_dt(int32_t dt_value, int dt_index);

    /** Sets the Phase Lock Loop Deadline Registers of the processor
     *
     * @param plldt_value the value to set the phase lock loop deadline register to
     * @param plldt_index the index of the Phase Lock Loop Deadline Register to set the value to
     */
    void set_plldt(uint32_t plldt_value, int plldt_index);

    /** Sets the Phase Lock Loop Load Registers of the processor
     *
     * @param pll_load_value the value to set the phase lock loop load register to
     * @param pll_load_index the index of the Phase Lock Loop Load Register to set the value to
     */
    void set_pll_load(uint32_t pll_load_value, int pll_load_index);

    /** Sets the Phase Lock Loop Loaded value of the processor
     *
     * @param set_pll_loaded the Phase Lock Loop Loaded value to set our processor to
     */
    void set_pll_loaded(bool pll_loaded);

    /** Sets the deadline registers' status in the processor
     *
     * @param dt_status_value the value to set the deadline register status to
     * @param dt_status_index the index of the Deadline Register status to set the value to
     */
    void set_dt_status(DT_STATUS dt_status_value, int dt_status_index);


    /** Gets the implementation version of the processor
     *
     * @return The implementation version of our processor
     */
    unsigned char get_imp_ver();

    /** Gets the current window pointer value of the processor
     *
     * @return The current window pointer of our processor
     */
    WINDOW_POINTER get_curr_wp();

    /** Gets the integer condition code of the processor
     *
     * @return the integer condition code of our processor
     */
    unsigned char get_icc();

    /** Gets the supervisor bit of the processor
     *
     * @return The supervisor bit of our processor
     */
    bool get_s();

    /** Gets the previous supervisor bit value of the processor
     *
     * @return the previous supervisor bit of our processor
     */
    bool get_ps();

    /** Gets the trap base register value of the processor
     *
     * @return the trap base register of our processor
     */
    uint32_t get_tbr();

    /** Gets the Window Invalid Mask Register value of the processor
     *
     * @return the Window Invalid Mask Register of our processor
     */
    uint32_t get_wim();

    /** Gets the enable trap value of the processor
     *
     * @return the enable trap value of our processor
     */
    bool get_et();

    /** Gets the enable coprocessor value of the processor
     *
     * @return enable coprocessor value from our processor
     */
    bool get_ec();

    /** Gets the enable floating point unit value of the processor
     *
     * @return the enable floating point unit value of our processor
     */
    bool get_ef();

    /** Gets the processor interrupt level of the processor
     *
     * @return the processor interrupt level of our processor
     */
    unsigned char get_pil();

    /** Gets the Ancillary State Registers value of the processor
     *
     * @return the Ancillary State Register value of our processor
     * @param asr_index the index of the Ancillary State Register to get the value of
     */
    uint32_t get_asr(int asr_index);

    /** Gets the multiply/divide value of the processor
     *
     * @return the multiply/divide value of our processor
     */
    uint32_t get_y();

    /** Gets the deadline registers of the processor
     *
     * @return the value of the deadline register
     * @param dt_index the index of the Deadline Register to get the value of
     */
    int32_t get_dt(int dt_index);

    /** Gets the Phase Lock Loop Deadline Registers of the processor
     *
     * @return the value of the phase lock loop deadline register
     * @param plldt_index the index of the Phase Lock Loop Deadline Register to get the value of
     */
    uint32_t get_plldt(int plldt_index);

    /** Gets the Phase Lock Loop Load Registers of the processor
     *
     * @return the value of the phase lock loop load register
     * @param pll_load_index the index of the Phase Lock Loop Load Register to get the value of
     */
    uint32_t get_pll_load(int pll_load_index);

    /** Gets the Phase Lock Loop Loaded value of the processor
     *
     * @return the Phase Lock Loop Loaded value of our processor
     */
    bool get_pll_loaded();

    /** Gets the deadline registers' status in the processor
     *
     * @return the value of the deadline register status
     * @param dt_status_index the index of the Deadline Register status to get the value of
     */
    DT_STATUS get_dt_status(int dt_status_index);


///////////////////////////////////////////////////////////////////////
//                       private variables                          ///

private:
    unsigned char _imp_ver; //Implementation Version Number
    WINDOW_POINTER _curr_wp;  //Current window pointer
    unsigned char _icc; //Integer Condition Code
    bool _s;  //Supervisor Bit
    bool _ps; //Previous Supervisor Bit
    uint32_t _tbr; //Trap base register
    uint32_t _wim; //Window Invalid Mask Register
    bool _et; //Enable Traps
    bool _ec; //Enable Coprocessor
    bool _ef; //Enable Floating point unit
    unsigned char _pil; //Processor Interrupt Level

    uint32_t _asr[32]; //Ancillary State Registers
    uint32_t _y; //Multiply Divide Register
    int32_t _dt[NUM_DEADLINE_TIMERS]; //Deadline Registers
    uint32_t _plldt[NUM_PLL_TIMERS]; //Phase Lock Loop Deadline Registers
    uint32_t _pll_load[NUM_PLL_TIMERS]; //Phase Lock Loop Double Buffer Registers
    bool _pll_loaded; // If Phase Lock Loop Registers are loaded
    DT_STATUS _dt_status[NUM_DEADLINE_TIMERS]; // Deadline registers status
};
#endif /* _SPECIAL_REG_H_ */
