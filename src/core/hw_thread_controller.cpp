
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
        hw_thread* hardware_thread = new hw_thread(i, 0x40000000, (void*)this);
        hardware_thread->set_enabled(true);
        hw_thread_ptr* hp = new hw_thread_ptr(hardware_thread);

#ifdef DBG_THREAD_CONT
        cout << "Thread creation - ID: " << hp->get_handle()->get_id()
             << ", pointer = " << hp->get_handle() << endl;
#endif /* DBG_THREAD_CONT */
        _pool[i] = hp;
    }

    for (i = 0; i < NUM_THREADS; i++) {
    	_sync_table[i][0] = 0;
    	_sync_table[i][1] = 0;
    	_sync_table[i][2] = 0;
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


bool hw_thread_controller::check_sync_table(unsigned short mask, unsigned int id){
	int first_free = -1;

	for (int i = 0; i < NUM_THREADS; i++) {
		if ( _sync_table[i][0] == mask) {
			// Entry has been cleared, return true so thread moves on
			if (_sync_table[i][1] == 0) {
				//Increment the number of threads that passed the barrier
				_sync_table[i][2]++;
				//If the number of threads that passed the barrier is the number
				// of threads set waiting for it, we can clear the entry.
				if ( _sync_table[i][2] == bitCount(_sync_table[i][0]))
					_sync_table[i][0] = 0;
				return true;
			}
			// Entry has not been cleared, clear own bit and
			// return false so thread keeps waiting
			else {
				_sync_table[i][1] &= ~(0x1 << id);
				return false;
			}
		}
		//Find out where the first free entry is in case we need it
		else if (_sync_table[i][0] == 0 && first_free == -1) {
			first_free = i;
		}
	}
	// If we reach here, this means that the mask entry was not in the table
	_sync_table[first_free][0] = _sync_table[first_free][1] = mask;
	_sync_table[first_free][2] = 0; //Set the threads that passed this sync to be 0
	return false;
}


void hw_thread_controller::print_sync_table() {

	printf("-------------------------\n");
	printf("Sync Table\n");
	printf("-------------------------\n");
	for (int i = 0; i < NUM_THREADS; i++) {
		printf("0x%x\t0x%x\n", _sync_table[i][0], _sync_table[i][1]);
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

        uint32_t stack_top = THREAD0_START + THREAD_MEM_SIZE * i - 4;
        _pool[i]->get_handle()->regs.set_reg(30, stack_top, 0);
        _pool[i]->get_handle()->regs.set_reg(14, stack_top, 0);

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


//Taken from http://gurmeetsingh.wordpress.com/2008/08/05/fast-bit-counting-routines/
int hw_thread_controller::bitCount(unsigned int n)
{
  // This is for 32 bit numbers.  Need to adjust for 64 bits
  register unsigned int tmp;

  tmp = n - ((n >> 1) & 033333333333) - ((n >> 2) & 011111111111);

  return ((tmp + (tmp >> 3)) & 030707070707) % 63;
}
