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

#include "decode.h"

decode::decode(): module_base() {
}

decode::decode(const sc_module_name& name): module_base(name) {
}

#ifdef _NO_SYSTEMC_
void decode::behavior(const hw_thread_ptr& in_thread, hw_thread_ptr& out_thread) {
#else
void decode::behavior() {
    hw_thread_ptr in_thread = in.read();
#endif /* _NO_SYSTEMC_ */

    if (_is_not_valid_hwthread(in_thread)) {
#ifdef _NO_SYSTEMC_
        out_thread = in_thread;
#else
        out.write(in_thread);
#endif /* _NO_SYSTEMC_ */
        return;
    }

    in_thread->inst.decode();

 #ifdef _NO_SYSTEMC_
    out_thread = in_thread;
    _dbg_pipeline(out_thread);
#else
    out.write(in_thread);
#endif /* _NO_SYSTEMC_ */


}

bool decode::_is_not_valid_hwthread(const hw_thread_ptr& hardware_thread) {
    return (hardware_thread.is_null() || !hardware_thread->is_enabled()
            || hardware_thread->is_fetch_stalled());
}

void decode::_dbg_pipeline(hw_thread_ptr& hardware_thread) {
#ifdef DBG_PIPE
#ifndef _NO_SYSTEMC_
    cout << "*decode*" << "  (" << sc_time_stamp() << ") ";
#endif
    cout << "hw_thread's id: " << hardware_thread->get_id() << ", pc: 0x" << hex << hardware_thread.get_handle()->PC << hex <<  ", " << hardware_thread->inst;
#endif /* DBG_PIPE */

}
