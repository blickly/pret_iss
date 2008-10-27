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

#include <sstream>
#include <string>
#include <assert.h>

#include "l1_scratch.h"
#include "memory_controller.h"
#include "wheeled_mem.h"
#include "dma.h"

memory_controller::memory_controller(const sc_module_name& nm, cycle_counter* cyc): module_base(nm) {
    _setup(cyc);
}

memory_controller::~memory_controller() {
    _fd.close();

    delete[] _inst_scratchpad;
    delete _main_mem;
    delete _mem_mapped_io;

#ifdef USE_PDMA
    delete _pdma_unit;
#endif /* USE_PDMA */
}

void memory_controller::behavior() {
    _main_mem->behavior();
    _mem_mapped_io->behavior();
}

void memory_controller::dump() {
    for (int i = 0; i < NUM_THREADS; ++i) {
        cout << _inst_scratchpad[i]->mem;
    }
    cout << _main_mem->mem;
}

mem_location& memory_controller::get_main_mem_loc(uint32_t addr) {
    if (addr > 0x80000000) {
        return *_mem_mapped_io;
    } else {
        return *_main_mem;
    }
}

l1_scratch& memory_controller::get_spm_mem_loc(int tid) {
    return *_inst_scratchpad[tid];
}

void memory_controller::out_file(uint32_t data) {
    /// Address map to an output file the address 0x80000600 which we
    /// use as VIDEO out
    if (_fd.is_open()) {
        //_fd << (char) data;
        _fd << data << endl;
    } else {
        cerr << "ERR: out_file(...), output file is not open" << endl;
    }
}

void memory_controller::out_uart(uint32_t data) {
    char out_char = (char) data;
    if (out_char == '\r') {
        return; // This prevents printing out ^M on newlines.
    }
    cout << (char)(data);
}

void memory_controller::register_inst_spm(l1_scratch* spm[]) {
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        _inst_scratchpad[i] = spm[i];
    }
}

uint32_t memory_controller::read_inst(int tid, uint32_t addr, bool& stalled) {
    if (tid >= MAX_THREAD) {
        return 0x01000000; // Disabled threads read no-ops everywhere.
    }
    // Return the instruction from scratchpad.
    return read_data(tid, addr, stalled);
}

uint32_t memory_controller::read_data(int tid, uint32_t addr, bool& stalled) {
    l1_scratch* spm = static_cast<l1_scratch*>(_inst_scratchpad[tid]);
    bool in_spm = spm->is_addr_in_spm(addr);
    if (!in_spm) {
        return get_main_mem_loc(addr).read(tid, addr, stalled);
    } else {
        return get_spm_mem_loc(tid).read(tid, addr, stalled);
    }
}


void memory_controller::_setup(cycle_counter* cyc) {
    _inst_scratchpad = new l1_scratch*[NUM_THREADS];
    _main_mem = new wheeled_mem(MEM_DELAY - 1, PDMA_DELAY, cyc);
    _mem_mapped_io = new l1_scratch();

#ifdef USE_PDMA
    _pdma_unit = new pdma("pdma unit",
                          dynamic_cast<wheeled_mem*>(_main_mem),
                          _inst_scratchpad);
#endif /* USE_PDMA */

    _fd.open("out_fd.dmp", ios::out);
}

void memory_controller::write_data(int tid, uint32_t addr, uint32_t data, bool& stalled) {
    l1_scratch* spm = _inst_scratchpad[tid];
    bool in_spm = spm->is_addr_in_spm(addr);
    if (!in_spm) {
        get_main_mem_loc(addr).write(tid, addr, data, stalled);
    } else {
        get_spm_mem_loc(addr).write(tid, addr, data, stalled);
    }
    if (stalled) {
        return;
    }
#ifdef DBG_UART
    /// Address map to the UART which is mapped at address 0x80000100
    /// for its data.
    if (addr == 0x80000100) {
        /// The write is happening to the APBUART, so print it out
        out_uart(data);
    }
#endif
#ifdef DBG_FOUT
    /// Address map to an output file the address 0x80000600 which we
    /// use as VIDEO out
    if (addr == 0x80000600) {
        out_file(data);
    }
#endif
}
