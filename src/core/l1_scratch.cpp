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


#include "l1_scratch.h"
#include "assert.h"

l1_scratch::l1_scratch() {
    uint32_t spm_addr;
    for (spm_addr = 0; spm_addr < SCRATCH_SIZE; spm_addr += 4) {
        main_memory_address[spm_addr/4] = (uint32_t) NULL;
    }
}

void l1_scratch::behavior() {}

/*
 * Return the data that is stored at the given memory location,
 * as well as a boolean for whether the read needs to stall for
 * longer before the memory can be accessed.
 */
uint32_t l1_scratch::read(int tid, uint32_t mem_addr, bool& stalled) {
    if (!is_addr_in_spm(mem_addr)) {
        cerr << "Cannot read address not in scratchpad: 0x"
             << hex << mem_addr << endl;
    }
    // Scratchpads never stall
    stalled = false;
    return mem[mem_addr];
}

void l1_scratch::write(int tid, uint32_t mem_addr, uint32_t data, bool& stalled) {
    if (!is_addr_in_spm(mem_addr)) {
        cerr << "Cannot write address not in scratchpad: 0x"
             << hex << mem_addr << ", data: 0x" << data << endl;
    }
    // Scratchpads never stall
    stalled = false;
    mem.add_address(mem_addr, data);
}

void l1_scratch::update_addr(uint32_t mem_addr, uint32_t inst) {
    if (!is_addr_in_spm(mem_addr)) {
        cerr << "Cannot update address not in scratchpad: 0x"
             << hex << mem_addr << endl;
        return;
    }
    /* Modify the instruction in the SPM */
    mem.add_address(mem_addr,  inst);
}

bool l1_scratch::in_spm_range(uint32_t spm_addr) {
    if (spm_addr % 4 == 0 && spm_addr < SCRATCH_SIZE) {
        return true;
    } else {
        return false;
    }
}

void l1_scratch::add_addr(uint32_t mem_addr, uint32_t spm_addr,
                          uint32_t data) {
    if (!in_spm_range(spm_addr)) {
        cerr << "Error: Request made to invalid scratchpad address 0x"
             << hex << spm_addr << endl;
        return;
    }

    /* The value being overwritten must be removed first. */
    uint32_t old_mem_addr = main_memory_address[spm_addr/4];
    mem.remove_address(old_mem_addr);

    /* Add the data to the SPM */
    main_memory_address[spm_addr/4] = mem_addr;
    mem.add_address(mem_addr,  data);
}


bool l1_scratch::is_addr_in_spm(uint32_t mem_addr) {
    if (mem_addr == (uint32_t) NULL) {
        return false;
    }
    return mem.has_address(mem_addr);
}

void l1_scratch::dbg_print_valid_addr() {
    cout << "\t [mem_addr] = spm_addr" << endl;
    for (uint32_t spm_addr = 0; spm_addr < SCRATCH_SIZE; spm_addr += 4) {
        cout << "valid_addr[" << spm_addr << "] = "
             << main_memory_address[spm_addr/4] << endl;
    }
}

void l1_scratch::_dbg_print_mem_contents() {
    cout << "l1_scratch:: memory contents" << endl;
    cout << mem << endl;
}
