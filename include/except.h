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

*/

#ifndef _EXCEPT_H_
#define _EXCEPT_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc>
#endif

#include "module_base.h"
#include "hw_thread_ptr.h"
#include "dma.h"
///////////////////////////////////////////////////////////////////////
/// exception
/**
 * This class commits the instruction once no exception is detected.
 * When an instruction is committed, it is written back to the register 
 * file. This stage also modifies the PC after it detects a branch.
 *
 *
 * @author: hiren
 * $Date$
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class except : public module_base {
 public:
	//Constructor
    except(const sc_module_name& str);
    // Ports
	
    // Inputs
    sc_in< hw_thread_ptr > in;

    sc_port< dma_if > coproc_dma;
    // Constructor


    uint32_t addr_calc(const hw_thread_ptr& ht);
    //instruction const& inst);

    //FIXME: Might need to move the branch PC stuff to execute stage
    bool branch_check(const hw_thread_ptr& ht);
    //    bool branch_check(instruction const &inst, unsigned char icc);

///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
#ifdef _NO_SYSTEMC_
    /** Takes an instruction from the input in and checks the
     *  instruction for any stalls. If a stall occured, then 
     *  the instruction is not committed, or else the instruction
     *  is committed by writing the register values to the registers
     *  or setting the next PC value if it's a branch.
     *  @param in_thread Incoming thread from the memory stage.
     */
    void behavior(hw_thread_ptr& in);
#else
    /** Takes an instruction from the in port and checks the
     *  instruction for any stalls. If a stall occured, then 
     *  the instruction is not committed, or else the instruction
     *  is committed by writing the register values to the registers
     *  or setting the next PC value if it's a branch.
     *  @param in_thread Incoming thread from the memory stage.
     */
    void behavior();
#endif /* _NO_SYSTEMC_ */

    /** Print to <i>cout</i> stream the debugging output for this stage
       *  in the pipeline. This is only enabled if <i>DBG_PIPE</i> flag is
       *  enabled during compilation.
       */
    void debug(const hw_thread_ptr& ht);
    bool dead_stalled(const hw_thread_ptr& ht);

    bool fetch_stalled(const hw_thread_ptr& ht);
    void inc_pc(const hw_thread_ptr& ht);

    bool mem_stalled(const hw_thread_ptr& ht);
    void set_dword_state(const hw_thread_ptr& ht);
    void write_regs(const hw_thread_ptr& ht);
    void write_special_regs(const hw_thread_ptr& ht);

///////////////////////////////////////////////////////////////////////
///                      private methods                             ///
 private:
    /** Disable default constructor.
     */
    except();

    /** Check whether the current thread in a stage can processed or not.
     *  @param hardware_thread The hardware thread.
     */
    bool _is_not_valid_hwthread(const hw_thread_ptr& ht);
};
#endif
