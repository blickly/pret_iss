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


#include "hw_thread.h"

unsigned int hw_thread::get_id() {
    return id;
}

hw_thread::hw_thread(): id(MAX_THREAD) {
    enabled = false;
}

hw_thread::hw_thread(const hw_thread & hwth) {
    this->operator=(hwth);
}

hw_thread::hw_thread(unsigned int in_id, uint32_t pc) : id(in_id) {
    PC = pc;
    cnt_cycles = 0;
    cnt_instr = 0;
    branch_slot = 0;
    enabled = false;
    db_word = false;
    dead_stalled = false;
    mem_stalled = false;
    fetch_stalled = false;
}


void hw_thread::operator=(const hw_thread & hwth) {
    id = hwth.id;
    inst = hwth.inst;
    cnt_cycles = hwth.cnt_cycles;
    cnt_instr = hwth.cnt_instr;
    PC = hwth.PC;
    enabled = hwth.enabled;
    spec_regs = hwth.spec_regs;
    regs = hwth.regs;
    branch_slot = hwth.branch_slot;
    db_word = hwth.db_word;
    dead_stalled = hwth.dead_stalled;
    mem_stalled = hwth.mem_stalled;
    fetch_stalled = hwth.fetch_stalled;

}

bool hw_thread::operator==(const hw_thread & hwth) {
    /// cnt_instr and cnt_cycles are not behaviors of a thread so no need to compare them.
    return (id == hwth.id) && (inst == hwth.inst) && (PC == hwth.PC) && (enabled == hwth.enabled) && (spec_regs == hwth.spec_regs) &&
           (branch_slot == hwth.branch_slot) && (regs == hwth.regs) && (dead_stalled == hwth.dead_stalled) && (mem_stalled == hwth.mem_stalled) && (fetch_stalled == hwth.fetch_stalled);

}

void hw_thread::regdump() {
    regs.regdump(spec_regs.curr_wp);
    cout << hex << setfill('0') << "pc : " << setw(8) << PC << "    ";

    // This is assuming that if the branch_slot is 0 then basically npc
    // = pc +4 but if it is set then that is the next instruction we are
    // going to execute.
    if (branch_slot == 0)
        cout << "npc: " <<  hex << setfill('0') <<  setw(8) << PC + 4 << endl;
    else
        cout << "npc: " <<  hex << setfill('0') <<  setw(8) << branch_slot << endl;

    spec_regs.dump();
}

void hw_thread::reset_register_window() {
    regs.reset_register_window();
}

void hw_thread::set_id(unsigned int in_id) {
    id = in_id;
}
