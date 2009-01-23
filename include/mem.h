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

#ifndef _MEM_H_
#define _MEM_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc>
#endif /* _NO_SYSTEMC_ */

#include "hw_thread_ptr.h"
#include "memory_controller.h"
#include "module_base.h"
#include <stdint.h>

///////////////////////////////////////////////////////////////////////
/// mem
/**
 *
 * @author  Ben Lickly
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class mem : public module_base {
public:
    /** Construct the memory stage with a unique identifier.
     *  @param str Unique string idetifier for the memory stage.
     */
    mem(const sc_module_name& str);

///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
#ifdef _NO_SYSTEMC_
    /** Beform the behavior of the memory access pipeline stage.
     *  If the current operation for the thread is a memory operation
     *  then this is where the access to the data memory is performed
     *  and contents changed. We ensure the loads and stores are word
     *  aligned, by ignoring the lower order bits.
     *  This version of <i>behavior</i> is the default and is used
     *  when SystemC is not supported.
     *
     * @param in Hardware thread received from the register access stage.
     * @param out Hardware thread sent to the execute stage.
     */
    void behavior(const hw_thread_ptr& in, hw_thread_ptr& out);
#else
    /** Beform the behavior of the memory access pipeline stage.
     *  If the current operation for the thread is a memory operation
     *  then this is where the access to the data memory is performed
     *  and contents changed. We ensure the loads and stores are word
     *  aligned, by ignoring the lower order bits.
     *  This version of <i>behavior</i> is only used when support for
     *  SystemC is specified at compilation time.
     */

    void behavior();
#endif /* _NO_SYSTEMC_ */

///////////////////////////////////////////////////////////////////////
//                       public variables                             ///
public:
    /** Connection to the data memory unit.
     */
    sc_port< mem_stage_if > data_mem;

    /** Receive hardware threads from the register access stage.
     */
    sc_in< hw_thread_ptr > in;

    /** After performing the memory operation for the hardware thread,
     * send it out to the execute stage.
     */
    sc_out< hw_thread_ptr > out;
private:
    /** Disable default constructor.
     */
    mem();
///////////////////////////////////////////////////////////////////////
//                       private methods                             ///
    /** Print to <i>cout</i> stream the debugging output for this
     *  stage in the pipeline. This is only enabled if <i>DBG_PIPE</i>
     *  and <i>DBG_MEM</i> flags are enabled during compilation.
     *  @param ht Hardware thread.
     */
    void _dbg_pipeline(const hw_thread_ptr& hardware_thread);


    /** Check whether the current thread in a stage can processed or not.
     *  @param hardware_thread The hardware thread.
     */
    bool _is_not_valid_hwthread(const hw_thread_ptr& hardware_thread);


};
#endif /* _MEM_H_ */
