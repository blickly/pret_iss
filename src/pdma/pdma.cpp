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
#include "pdma.h"
#include "wheeled_mem.h"
#include <assert.h>

void pdma::behavior() {
    // Increment the currently running thread
    // FIXME:  Or should the _current_thread change only as fast as the memory wheel turns?
    _current_thread = (_current_thread + 1) % NUM_THREADS;

    // Snoop on address bus
    uint32_t snooped_addr = snoop_port.read();

    // Fetch the correct PDMA insruction
    const uint32_t PC =  _state[_current_thread].pc;
    uint32_t fetched_inst = _pdma_instructions[PC];

    // And execute the correct current behavior
    behavior(snooped_addr, fetched_inst);
}

uint32_t pdma::behavior(uint32_t snooped_addr, uint32_t fetched_inst) {
    if (_state[_current_thread].snooping) {
        behavior_snoop(snooped_addr);
    } else if (_state[_current_thread].loading) {
        behavior_load();
    } else {
        process_inst(fetched_inst);
        _state[_current_thread].pc += 4;
    }
    return  _state[_current_thread].pc;
}

void pdma::behavior_load() {
    bool stalled;
    vector<uint32_t> pdma_words = _main_mem->read_pdma(_current_thread,
                                  _state[_current_thread].reg[0],
                                  stalled,
                                  _state[_current_thread].words_to_load);
    if (stalled) {
        return;
    }
    int words_fetched = _main_mem->pdma_words_returned(_current_thread,
                        _state[_current_thread].words_to_load);
    if (_state[_current_thread].words_loaded != words_fetched) {
        assert(_state[_current_thread].words_loaded + 1 == words_fetched);
        uint32_t next_word = pdma_words.at(words_fetched - 1);
        _inst_scratch[_current_thread]->add_addr(_state[_current_thread].reg[0],
                _state[_current_thread].reg[1],
                next_word);
        _state[_current_thread].words_loaded = words_fetched;
    }
}

void pdma::behavior_snoop(uint32_t snooped_addr) {
    for (int i = 0; i < 2; ++i) {
        if (snooped_addr ==  _state[_current_thread].reg[0]) {
            _state[_current_thread].pc =  _state[_current_thread].branch_addr[0];
            _state[_current_thread].snooping = false;
        }
    }
}

void pdma::do_lim(int reg_no, uint32_t value) {
    _state[_current_thread].reg[reg_no] = value;
}

void pdma::do_load(int words) {
    _state[_current_thread].words_loaded = 0;
    _state[_current_thread].words_to_load = words;

    uint32_t mm_addr =  _state[_current_thread].reg[0];
    uint32_t spm_addr =  _state[_current_thread].reg[1];
    cout << "Load " << dec << words << " words from 0x" << hex << mm_addr
         << " in main mem to 0x" << hex << spm_addr
         << " in scratchpad" << endl;
}

void pdma::do_switch(uint32_t pdma_addr0, uint32_t pdma_addr1) {
    _state[_current_thread].snooping = true;
    _state[_current_thread].branch_addr[0] = pdma_addr0;
    _state[_current_thread].branch_addr[1] = pdma_addr1;

    uint32_t snoop_addr0 =  _state[_current_thread].reg[0];
    uint32_t snoop_addr1 =  _state[_current_thread].reg[1];
    cout << "Switch:" << endl
         << "\t0x" << hex << snoop_addr0 << " => 0x" << pdma_addr0 << endl
         << "\t0x" << hex << snoop_addr1 << " => 0x" << pdma_addr1 << endl;
}

pdma::pdma(const sc_module_name& str, wheeled_mem* mm, l1_scratch* iscratch[NUM_THREADS]) {
    _main_mem = mm;
    _current_thread = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        _state[i].snooping = false;
        _state[i].pc = 0;
        _inst_scratch[i] = iscratch[i];
    }
}

pdma::pdma() {

}

pdma::~pdma() {

}

void pdma::process_inst(uint32_t inst) {
    enum INST_TYPE {LIMR0 = 0, LIMR1 = 1, LOAD = 2, SWITCH = 3};
    switch ((inst >> 30) & 0x3) {
    case LIMR0:
    case LIMR1: {
        int reg_no = (inst >> 30) & 0x1;
        uint32_t value = inst << 2;
        do_lim(reg_no, value);
        break;
    }
    case LOAD: {
        int words = (inst & 0xFF);
        do_load(words);
        break;
    }
    case SWITCH: {
        uint32_t addr0 = (inst >> 15) & 0x7FFF;
        uint32_t addr1 = inst & 0x7FFF;
        do_switch(addr0, addr1);
        break;
    }
    default: {
        cerr << "PDMA instruction not understood";
        break;
    }
    }
}
