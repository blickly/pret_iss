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

  $Author$
  $Date$
  $Id$

*/

#ifndef _EXECUTE_H_
#define _EXECUTE_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc>
#endif

#include "module_base.h"
#include "hw_thread_ptr.h"

///////////////////////////////////////////////////////////////////////
/// execute
/**
 * This class performs the execute  for the particular
 * thread. This involves performing the ALU operations
 *
 *
 *
 * @author  Hiren Patel, Isaac Liu
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class execute : public module_base {
public:
    // Constructor
    execute(const sc_module_name& name);

///////////////////////////////////////////////////////////////////////
//                       public methods                             //
#ifdef _NO_SYSTEMC_
    /** Takes an instruction from the input in and executes the
     *  instruction. The executed instruction is forwarded to the next
     *  stage by passing it to the output out. This is the non-SystemC
     *  version.
     *  @param in_thread Incoming thread from the register access stage.
     *  @param out_thread Outgoing thread to the memory stage.
     */
    void behavior(const hw_thread_ptr& in_thread, hw_thread_ptr& out_thread);
#else
    /** Takes an instruction from the in port and executes that
     *  instruction. After the execution, the instruction is passed
     *  forward to the memory stage by writing to it to the
     *  output port (out). This is only used with SystemC.
     */
    void behavior();
#endif

    /** Perform the actual ALU operations on the thread.
     *  @param hardware_thread The hardware thread.
     */
    void perform_alu_operations(const hw_thread_ptr& hardware_thread);

    /** Get the integer condition code from this ALU operation
     *  @param hardware_thread The hardware thread.
     */
    unsigned char geticc(const hw_thread_ptr& hardware_thread);

///////////////////////////////////////////////////////////////////////
//                       public variables                           ///
    /** This is an input port which receives a hardware thread pointer
     *  from the register access stage.
     */
    sc_in< hw_thread_ptr > in;
    /** This is an output port. The changed state of the hardware thread
     *  is sent to the memory stage through this port.
     */
    sc_out< hw_thread_ptr > out;

///////////////////////////////////////////////////////////////////////
//                       private methods                            ///
private:
    /** Disallow the use of the default constructor.
     */
    execute();

    /** Check whether the current thread in a stage can processed or not.
     *  @param hardware_thread The hardware thread.
     */
    bool _is_not_valid_hwthread(const hw_thread_ptr& hardware_thread);
};
#endif /* _EXECUTE_H_ */
