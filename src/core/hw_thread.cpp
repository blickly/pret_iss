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

  $Author$
  $Date$
  $Id$

*/


#include "hw_thread.h"

unsigned int hw_thread::get_id() {
    return _id;
}

uint32_t hw_thread::get_pc() const {
    return _pc;
}

uint32_t hw_thread::get_delayed_branch_address() const {
    return _branch_slot;
}

short hw_thread::get_trap_type() const{
  return _trap_type;
}

hw_thread::hw_thread(): _id(MAX_THREAD) {
    _enabled = false;
}

hw_thread::hw_thread(const hw_thread& hardware_thread) {
    this->operator=(hardware_thread);
}

hw_thread::hw_thread(unsigned int in_id, uint32_t pc, void* thread_control) : _id(in_id) {
    _pc = pc;
    cnt_cycles = 0;
    cnt_instr = 0;
    _branch_slot = 0;
    _trap_type = 0;
    _db_word_stalled = false;
    _enabled = false;
    _deadline_stalled = false;
    _memory_stalled = false;
    _fetch_stalled = false;
    _fixed_schedule = true;
    _trapped = XCPT_NORMAL;
    controller = thread_control;
}

bool hw_thread::is_db_word_stalled() const {
    return _db_word_stalled;
}

bool hw_thread::is_deadline_stalled() const {
    return _deadline_stalled;
}

bool hw_thread::is_enabled() const {
    return _enabled;
}


bool hw_thread::is_fetch_stalled() const {
    return _fetch_stalled;
}

bool hw_thread::is_fixed_schedule() const {
  return _fixed_schedule;
}

bool hw_thread::is_memory_stalled() const {
    return _memory_stalled;
}

bool hw_thread::is_trapped() const {
  return _trapped > 0;
}

void hw_thread::operator=(const hw_thread & hardware_thread) {
    _id = hardware_thread._id;
    inst = hardware_thread.inst;
    cnt_cycles = hardware_thread.cnt_cycles;
    cnt_instr = hardware_thread.cnt_instr;
    _pc = hardware_thread._pc;
    _enabled = hardware_thread._enabled;
    spec_regs = hardware_thread.spec_regs;
    regs = hardware_thread.regs;
    _branch_slot = hardware_thread._branch_slot;
    _db_word_stalled = hardware_thread._db_word_stalled;
    _deadline_stalled = hardware_thread._deadline_stalled;
    _memory_stalled = hardware_thread._memory_stalled;
    _fetch_stalled = hardware_thread._fetch_stalled;
    _fixed_schedule = hardware_thread._fixed_schedule;
    _trapped = hardware_thread._trapped;
    _trap_type = hardware_thread._trap_type;
    controller = hardware_thread.controller;

}

bool hw_thread::operator==(const hw_thread& hardware_thread) {
    /// cnt_instr and cnt_cycles are not behaviors of a thread so no need to compare them.
    return (
               (_id == hardware_thread._id)
               && (inst == hardware_thread.inst)
               && (_pc == hardware_thread._pc)
               && (_enabled == hardware_thread._enabled)
               && (spec_regs == hardware_thread.spec_regs)
               && (_branch_slot == hardware_thread._branch_slot)
               && (regs == hardware_thread.regs)
               && (_deadline_stalled == hardware_thread._deadline_stalled)
               && (_memory_stalled == hardware_thread._memory_stalled)
               && (_fetch_stalled == hardware_thread._fetch_stalled)
	       && (_fixed_schedule == hardware_thread._fixed_schedule)
	       && (_trapped == hardware_thread._trapped)
	       && (_trap_type == hardware_thread._trap_type)
	       && (controller == hardware_thread.controller)
           );

}

void hw_thread::regdump() {
    regs.regdump(spec_regs.get_curr_wp());
    cout << hex << setfill('0') << "pc : " << setw(8) << _pc << "    ";

    // This is assuming that if the _branch_slot is 0 then basically npc
    // = pc +4 but if it is set then that is the next instruction we are
    // going to execute.
    if (_branch_slot == 0)
        cout << "npc: " <<  hex << setfill('0') <<  setw(8) << _pc + 4 << endl;
    else
        cout << "npc: " <<  hex << setfill('0') <<  setw(8) << _branch_slot << endl;

    spec_regs.dump();
}

void hw_thread::reset_register_window() {
    regs.reset_register_window();
}

void hw_thread::set_trap_type(const short& type) {
  _trap_type = type;
}


void hw_thread::set_id(const unsigned int& in_id) {
    _id = in_id;
}

void hw_thread::set_db_word_stalled(const bool& stall) {
    _db_word_stalled = stall;
}

void hw_thread::set_deadline_stalled(const bool& stall) {
    _deadline_stalled = stall;
}

void hw_thread::set_delayed_branch_address(const uint32_t& address) {
    _branch_slot = address;
}

void hw_thread::set_enabled(const bool& enable) {
    _enabled = enable;
}

void hw_thread::set_fetch_stalled(const bool& stall) {
    _fetch_stalled = stall;
}

void hw_thread::set_fixed_schedule(const bool& fixed_schedule) {
  _fixed_schedule = fixed_schedule;
}

void hw_thread::set_memory_stalled(const bool& stall) {
    _memory_stalled = stall;
}

void hw_thread::set_trapped(const short& trapped) {
  _trapped = trapped;
}

void hw_thread::set_pc(const uint32_t& pc) {
    _pc = pc;

}

short hw_thread::get_trapped_state(){
  return _trapped;
}
