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

  $Author$
  $Date$
  $Id$

*/

#ifndef _PDMA_H_
#define _PDMA_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif

#include "module_base.h"
#include "wheeled_mem.h"
#include "l1_scratch.h"
#include <stdint.h>

struct thread_data {
    uint32_t reg[2];
    uint32_t pc;
    // SWITCH specific thread data
    bool snooping;
    uint32_t branch_addr[2];
    // LOAD specific thread data
    bool loading;
    int words_to_load;
    int words_loaded;
};



struct pdma: public module_base {

public:
    // Ports
    sc_in<uint32_t> snoop_port;

    // Inputs
    /* Snoop instruction address from fetch stage */

    /* Access main memory for making block moves */

    /* Access to scratchpad memories */

    //sc_in< hw_thread_ptr > in;
    /// Special port for immediate access to instruction memory
    //sc_port< fetch_stage_if > instruction_mem;
    // Outputs
    //sc_out< hw_thread_ptr > out;

    // Constructors
    pdma(const sc_module_name& str, wheeled_mem* mm, l1_scratch* iscratch[NUM_THREADS]);
    ~pdma();

    // Entry function
    void behavior();
    uint32_t behavior(uint32_t snooped_addr, uint32_t fetched_inst);
    void behavior_load();
    void behavior_snoop(uint32_t snooped_addr);

    void do_lim(int reg_no, uint32_t value);
    void do_load(int bytes);
    void do_switch(uint32_t addr0, uint32_t addr1);
    void process_inst(uint32_t inst);

private:
    pdma();
    int _current_thread;
    thread_data  _state[NUM_THREADS];
    l1_scratch* _inst_scratch[NUM_THREADS];
    wheeled_mem* _main_mem;
    memory_unit _pdma_instructions;
};
#endif
