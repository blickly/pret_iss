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

#ifndef _HW_THREAD_H_
#define _HW_THREAD_H_


#include "defines.h"
#include "instruction.h"
#include "memory_controller.h"
#include "register_file.h"
#include "special_reg.h"
#include <stdint.h>

struct hw_thread {

    unsigned int get_id();
    hw_thread();
    hw_thread(const hw_thread & hwth);
    hw_thread(unsigned int in_id, uint32_t pc);

    bool operator==(const hw_thread & hwth);
    void operator=(const hw_thread & hwth);
    inline friend ostream & operator << (ostream & out, hw_thread const & hwth) {
        //    out << "---------------------------- hw_thread ------------------------------------------" << endl;
        out << "+ hw_thread id: " << hwth.id << ' ' << hwth.inst;
        //  out << "------------------------------------- ------------------------------------------" << endl;
        return out;
    }

    // Print out registers in current register window.
    void regdump();
    // Register Access functions
    void reset_register_window();
#ifndef  _NO_SYSTEMC_
    inline friend void sc_trace(sc_trace_file *tf, const hw_thread & hwth, const string & str) {
        // Do nothing
    }
#endif /* _NO_SYSTEMC_ */
    void set_id(unsigned int in_id);


    // Data members
    unsigned int cnt_cycles;
    unsigned int cnt_instr;
    unsigned int id;
    instruction inst;
    uint32_t PC;
    uint32_t branch_slot;
    register_file regs;
    special_reg spec_regs;
    bool enabled;
    bool db_word;
    bool dead_stalled; // If the thread is currently in a state of replay.
    bool mem_stalled;
    bool fetch_stalled;
};

#endif /* _HW_THREAD_H_ */
