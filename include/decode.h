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
*/

#ifndef _DECODE_H_
#define _DECODE_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc>
#endif /* _NO_SYSTEMC_ */

#include "module_base.h"
#include "hw_thread_ptr.h"

///////////////////////////////////////////////////////////////////////
/// decode
/**
 * This class decodes an instruction that has been fetched.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class decode : public module_base {
public:
    // Constructor
    decode(const sc_module_name& name);

///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
#ifdef _NO_SYSTEMC_
    /** Takes an instruction from the input in and decodes the
     *  instruction. The decoded instruction is forwarded to the next
     *  stage by passing it to the output out. This is the non-SystemC
     *  version.
     *  @param in_thread Incoming thread from the fetch stage.
     *  @param out_thread Outgoing thread to the register access stage.
     */
    void behavior(const hw_thread_ptr& in_thread, hw_thread_ptr& out_thread);
#else
    /** Takes an instruction from the in port and decodes that
     *  instruction. After the decoding, the instruction is passed
     *  forward to the register access stage by writing to it to the
     *  output port (out). This is only used with SystemC.
     */
    void behavior();
#endif /* _NO_SYSTEMC_ */

///////////////////////////////////////////////////////////////////////
//                       public variables                           ///
    /** Input and output ports for SystemC.
     */
    sc_in< hw_thread_ptr > in;
    sc_out< hw_thread_ptr > out;

private:
    /** Default constructor's use is disallowed.
     */
    decode();

    /** Check whether the current thread in a stage can processed or not.
     *  @param hardware_thread The hardware thread.
     */
    bool _is_not_valid_hwthread(const hw_thread_ptr& hardware_thread);

    /** Print to <i>cout</i> stream the debugging output for this stage
       *  in the pipeline. This is only enabled if <i>DBG_PIPE</i> flag is
       *  enabled during compilation.
       */
    void _dbg_pipeline(hw_thread_ptr& hardware_thread);
};
#endif /* _DECODE_H_ */
