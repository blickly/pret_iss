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

#ifndef _FETCH_H_
#define _FETCH_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif /* _NO_SYSTEMC_ */

#include "instruction.h"
#include "hw_thread_ptr.h"
#include "memory_controller.h"
#include "module_base.h"
//#include "dma.h"

///////////////////////////////////////////////////////////////////////
/// fetch
/**
 * The fetch class gets the instructions from the instruction
 * memories. These instruction fetch requests are currently received
 * from the scratchpad memories. The connection between the fetch
 * stage and the memory uses an interface method.
 *
 * @author  Hiren Patel, Isaac Liu
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class fetch : public module_base {
public:
    /** Construct and instance of the fetch stage in the processor.
     *   @param name Unique identifier for for the fetch stage.
     */
    fetch(const sc_module_name& name);

    virtual ~fetch();
///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
public:
#ifdef _NO_SYSTEMC_
    /** The behavior of the fetch stage takes the hardware thread
     *  pointer and fetches the next instruction from that thread's
     *  respective scratchpad memory. It stores it in the thread's
     *  internal instruction object and proceeds to write that changed
     *  hardware thread object to the output for decoding in the decode
     *  stage.
     *  @param input_thread This is the incoming thread from the hardware
     *  thread controller.
     *  @param output_thread This is the thread written to for us in decode stage.
     */
    void behavior(const hw_thread_ptr& input_thread, hw_thread_ptr& output_thread);
#else
    /** The behavior of the fetch stage takes the hardware thread
     *  pointer and fetches the next instruction from that thread's
     *  respective scratchpad memory. It stores it in the thread's
     *  internal instruction object and proceeds to write that changed
     *  hardware thread object to the output for decoding in the decode
     *  stage. This is invoked by SystemC's scheduler and thread objects
     *  are received from ports.
     */
    void behavior();
#endif /* _NO_SYSTEMC_ */

///////////////////////////////////////////////////////////////////////
//                       public variables                           ///
public:
    /** This is an input port which receives a hardware thread pointer
     *  from the thread controller.
     */
    sc_in< hw_thread_ptr > in;

    /** This is a port that access the instruction memory through an
     *  interface method. The interface is fetch_stage_if, which should
     *  only allow calls that read the instruction memory. Writes should
     *  not be allowed.
     */
    sc_port< fetch_stage_if > instruction_memory;

    //    sc_port< dma_if > blocking_dma;
    /** This is an output port. The changed state of the hardware thread
     *  is sent to the decode stage through this port.
     */
    sc_out< hw_thread_ptr > out;

///////////////////////////////////////////////////////////////////////
//                       private methods                            ///
private:
    /** Disallow the use of the default constructor.
     */
    fetch();
    bool _is_not_valid_hardware_thread(const hw_thread_ptr& hardware_thread);
    /** Print to <i>cout</i> stream the debugging output for this stage
     *  in the pipeline. This is only enabled if <i>DBG_PIPE</i> flag is
     *  enabled during compilation.
     *  @param hardware_thread The hardware thread.
     */
    void _debug_pipeline(const hw_thread_ptr& hardware_thread);
};
#endif /* _FETCH_H_ */
