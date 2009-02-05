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

#include "fetch.h"

fetch::fetch(): module_base() {
}

fetch::fetch(const sc_module_name& name): module_base(name) {
}

fetch::~fetch() {

}

#ifdef _NO_SYSTEMC_
void fetch::behavior(const hw_thread_ptr& input_thread, hw_thread_ptr& output_thread) {
#else
void fetch::behavior() {
    hw_thread_ptr input_thread = in.read();
#endif /* _NO_SYSTEMC_ */


    if (_is_not_valid_hardware_thread(input_thread)) {
#ifdef _NO_SYSTEMC_
        output_thread = input_thread;
#else
        out.write(input_thread);
#endif /* _NO_SYSTEMC_ */
        return;
    }
    input_thread->spec_regs.decrement_deadline_timers();
    /* If we had a previously dead_stalled set, then we will reset it in
       the register access stage when we identify what particular
       register it is that we are considered with. */
    input_thread->set_deadline_stalled(false);

    if (input_thread->is_memory_stalled()) {
#ifdef _NO_SYSTEMC_
        output_thread = input_thread;
#else
        out.write(input_thread);
#endif /* _NO_SYSTEMC_ */
      return;
   }
    //    cout << hex << ht->PC << "\t" << ht->PC << endl;
    if ( !input_thread->is_fetch_stalled() )
      {
	bool fetch_stall = false;
	uint32_t instruction_bits =
	  instruction_memory->read_inst(input_thread->get_id(), input_thread->get_pc(), fetch_stall);
    input_thread->set_fetch_stalled(fetch_stall);
    input_thread->inst.set_inst(instruction_bits);
      }

    

    //    cout << "fetch:: stalled: " << input_thread->fetch_stalled << endl;
    /** FIXME: This should be handled by throwing a trap and catching
       it to terminate the simulation.  Currently, for the sake of
       termination, check if 0x22222222 was seen.
    */
    if (input_thread->inst.check_end_sim()) {
        /* Just throw an integer exception. We are only throwing this
        one exception so we are okay for now.
         */
        throw 20;
    }

    _debug_pipeline(input_thread);

#ifdef _NO_SYSTEMC_
    output_thread = input_thread;
#else
    out.write(input_thread);
#endif /* _NO_SYSTEMC_ */
}

void fetch::_debug_pipeline(const hw_thread_ptr& hardware_thread) {
#ifdef DBG_PIPE
#ifdef _NO_SYSTEMC_
    cout << "hdp::*fetch*" << "   ";
#else
    cout << "*fetch*" << "   (" << sc_time_stamp() << ") ";
#endif /* _NO_SYSTEMC */
    cout << "hw_thread's id: " << hardware_thread->get_id() << ", pc: 0x" << hex
         << hardware_thread->PC << hex <<  ", " << hardware_thread->inst
         << ", stalled: " << hardware_thread->is_fetch_stalled() << endl;
#endif /* DBG_PIPE */
}

bool fetch::_is_not_valid_hardware_thread(const hw_thread_ptr& hardware_thread) {
  return (hardware_thread.is_null() || !hardware_thread->is_enabled());

}
