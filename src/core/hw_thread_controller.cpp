
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

#include <sstream>
#include <stdlib.h>
#include "hw_thread_controller.h"

#define MAX(x,y) (((x)>(y))?(x):(y))

hw_thread_controller::hw_thread_controller() {
}

hw_thread_controller::hw_thread_controller(const sc_module_name& name)  {
    _txt_str = "none";

#ifdef DBG_THREAD_CONT
    cout << "-------------------- hw_thread_controller ------------------------- " << endl;
#endif /* DBG_THREAD_CONT */

    unsigned int i;
    for (i = 0; i < NUM_THREADS; i++) {
        hw_thread* hardware_thread = new hw_thread(i, 0x40000000);
        hardware_thread->set_enabled(true);
        hw_thread_ptr* hp = new hw_thread_ptr(hardware_thread);

#ifdef DBG_THREAD_CONT
        cout << "Thread creation - ID: " << hp->get_handle()->get_id()
             << ", pointer = " << hp->get_handle() << endl;
#endif /* DBG_THREAD_CONT */
        _pool[i] = hp;
    }
    _tcount = 0;

#ifndef _NO_SYSTEMC_
    SC_METHOD(behavior);
    sensitive << clk.pos();
    SC_METHOD(pll_behavior);
    sensitive << pll_clock.pos();
#endif /* _NO_SYSTEMC_ */
}

hw_thread_controller::~hw_thread_controller() {
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        delete _pool[i]->get_handle();
        delete _pool[i];
    }
}

#ifdef _NO_SYSTEMC_
void hw_thread_controller::behavior(hw_thread_ptr& out) {
#else
void hw_thread_controller::behavior() {
#endif /* _NO_SYSTEMC_ */

#ifdef DBG_THREAD_CONT
    cout << name() << " (" << sc_time_stamp() << ") " << endl;
#endif /* DBG_THREAD_CONT */

    hw_thread_ptr * hp = _pool[_tcount++ % MAX(NUM_THREADS,6)];

#ifdef _NO_SYSTEMC_
    out = *hp;
#else
    out.write(*hp);
#endif /* _NO_SYSTEMC_ */

#ifdef DBG_THREAD_CONT
    cout << "Next hw_thread's ID: " << hp->get_handle()->get_id() << endl;
#endif /* _NO_SYSTEMC_ */
}


hw_thread_ptr* hw_thread_controller::get_thread(unsigned int id) const {
    if (_pool[id] != NULL) {
        return _pool[id];
    }     else {
        return NULL;
    }
}

void hw_thread_controller::parse_srec_files(srec_parser& parser,
        static_bound_parser& bound_parser) {
    /* Check if the _txt_str is a directory or a file */
    bool is_dir = true;
    string dir;
    ifstream txt;
    ifstream ftxt;
    string test_dir = _txt_str + "/thread0.srec";

    txt.open(test_dir.c_str());
    if (!txt.is_open()) {
        is_dir = false;
    } else {
        dir = _txt_str;
    }

    ftxt.open(_txt_str.c_str());
    if (!is_dir && ftxt.is_open()) {
        getline(ftxt, dir);
    } else
        if (!is_dir && !ftxt.is_open()) {
            cout << "Error: File or directory not found, " << _txt_str << endl;

#ifdef _NO_SYSTEMC_
            exit(1);
#else
            sc_stop();
#endif /* _NO_SYSTEMC_ */
        }

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        /* We assume that the file format is thread[0-5].srec*/
        stringstream ss;
        string file_srec = dir;
        ss << i;
        file_srec = file_srec + "/thread" + ss.str() + ".srec";
        string file_ispm = dir + "/thread" + ss.str() + ".ispm";
        string file_dspm = dir + "/thread" + ss.str() + ".dspm";
        uint32_t starting_addr = parser.parse(file_srec.c_str());
        bound_parser.load_inst_spm(i, file_ispm);
        bound_parser.load_data_spm(i, file_dspm);

#ifdef DBG_THREAD_CONT
        cout << "Starting PC for thread " << i << " is " << hex << starting_addr << endl;;
#endif /* DBG_THREAD_CONT */

        _pool[i]->get_handle()->set_pc(starting_addr);
    }
}

void hw_thread_controller::pll_behavior() {
    /* Decrement all counters by 1.*/
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        _pool[i]->get_handle()->spec_regs.decrement_pll_timers();
    }
}

void hw_thread_controller::set_txtname(const string& str) {
    _txt_str = str;
}

