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

void l1_scratch::behavior() {

}

bool l1_scratch::is_stalled(int tid, uint32_t addr) {
#ifdef USE_PDMA
    return (!is_addr_in_spm(addr));
#else
    // For simplicity, never make scratchpads stall when not using PDMA.
    return false;
#endif
}
/*
 * Return the data that is stored at the given memory location,
 * as well as a boolean for whether the read needs to stall for
 * longer before the memory can be accessed.
 */
uint32_t l1_scratch::read(int tid, uint32_t addr, bool& stalled) {
    stalled = is_stalled(tid, addr);
    return mem[addr];
}

void l1_scratch::remove_addr(uint32_t mem_addr) {
    /* This virtual address is no longer allocated to the SPM */
    valid_addr.erase(mem_addr);
    /* It's data is scratched too for now. Note that this operation is
       not necessary if we add a member that simply replaces the data at
       that address.  */
    mem.remove_address(mem_addr);
}


void l1_scratch::update_addr(uint32_t mem_addr, uint32_t inst) {
    /* Modify the instruction in the SPM */
    mem.add_address(mem_addr,  inst);
}

map<uint32_t, uint32_t>::iterator l1_scratch::find_spm_addr(uint32_t spm_addr) {
    map<uint32_t, uint32_t>::iterator it = valid_addr.end();

    for (it = valid_addr.begin(); it != valid_addr.end(); it++) {
        if (it->second == spm_addr)
            break;
    }

    return it;
}

void l1_scratch::add_addr(uint32_t mem_addr, uint32_t spm_addr,
                          uint32_t inst) {
    map<uint32_t, uint32_t>::iterator it = find_spm_addr(spm_addr);
    if (it != valid_addr.end()) {
        /* key is a const, so we must delete it and readd it */
        remove_addr(it->first);
        /* Iterator it is invalid now */
    }
    /* Add the instruction into the valid address table and replace the
       memory contents. */
    it = valid_addr.find(mem_addr);
    /* Virtual address not found */
    if (it == valid_addr.end()) {
        valid_addr.insert(pair<uint32_t, uint32_t>(mem_addr, spm_addr));
    } else {
        /* Virtual address already has been allocated. Overwrite it */
        valid_addr[mem_addr] = spm_addr;
    }

    /* Add the instruction to the SPM */
    mem.add_address(mem_addr,  inst);
}


bool l1_scratch::is_addr_in_spm(uint32_t mem_addr) {
    map<uint32_t, uint32_t>::iterator it = valid_addr.find(mem_addr);

    if (it != valid_addr.end()) {
        return true;
    }
    return false;
}

void l1_scratch::dbg_print_valid_addr() {
    cout << "\t [mem_addr] = spm_addr" << endl;
    map<uint32_t, uint32_t>::iterator it;
    for (it = valid_addr.begin(); it != valid_addr.end(); it++) {
        cout << "valid_addr[" << it->first << "] = " << it->second << endl;
    }
}

void l1_scratch::_dbg_print_mem_contents() {
    cout << "l1_scratch:: memory contents" << endl;
    cout << mem << endl;
}
