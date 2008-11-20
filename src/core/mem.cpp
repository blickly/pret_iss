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


#include "mem.h"

mem::mem(): module_base() {

}

mem::mem(const sc_module_name& str): module_base(str) {
}

#ifdef _NO_SYSTEMC_
void mem::behavior(const hw_thread_ptr& ht, hw_thread_ptr& out) {
#else
void mem::behavior() {
    hw_thread_ptr ht = in.read();
#endif /* _NO_SYSTEMC_ */

    //    if (ht.is_null() || !ht->is_enabled() || ht->is_fetch_stalled()) {
    if (_is_not_valid_hwthread(ht)) {
#ifdef _NO_SYSTEMC_
        out = ht;
#else
        out.write(ht);
#endif /* _NO_SYSTEMC_ */
        return;
    }

    uint32_t addr = ht->inst.alu_result;
    uint32_t data_to_write = 0;
    // Add the write_data for storing halfwords : ht->inst.op3_val;

    // Do a read from memory
    int aligned_addr = 4 * (addr / 4);
    int addr_offset  = addr % 4;
    bool memory_stall = false;
    if (ht->inst.read_mem) {
        uint32_t raw_data = data_mem->read_data(ht->get_id(), aligned_addr, memory_stall);
	ht->set_memory_stalled(memory_stall);
        if (!ht->is_memory_stalled()) {
            ht->inst.read_data(raw_data, addr_offset);
        }
    } else if (ht->inst.write_mem) {
        uint32_t old_data = 0xdeadbeef;
        if (ht->inst.mem_size != MEM_WORD) {
            bool nul;  // FIXME: Implement store byte and store halfword correctly so that
            // they stall the pipeline for longer than normal stores.
            old_data = data_mem->read_data(ht->get_id(), aligned_addr, nul);
        }
        data_to_write = ht->inst.get_write_data(old_data, addr_offset);
        data_mem->write_data(ht->get_id(), aligned_addr, data_to_write, memory_stall);
	ht->set_memory_stalled(memory_stall);
    }

//     if (ht->get_id() == 0) {
//         bool null;
// #ifdef _NO_SYSTEMC_
//     cout << "*mem*" << "     " << ", mem stalled: " << ht->is_memory_stalled() << endl;
// #else
//     cout << "*mem*" << "     (" << sc_time_stamp() << ") " << ", mem stalled: " << ht->is_memory_stalled() << endl;
// #endif /* _NO_SYSTEMC_ */
//     cout << "hw_thread's id: " << ht->get_id() << ", pc: 0x" << hex << ht.handle->PC << hex <<  ", " << ht->inst << endl;
//     cout << "\t+ "  << "load: " << ht->inst.read_mem  << ", store: " << ht->inst.write_mem << endl;

//     if (ht->inst.read_mem) {
//         cout << "\t+ " << "to be read from: " << hex << addr << endl;
//         cout << "\t+ " << "read:            " << hex << data_mem->read_data(ht->get_id(), aligned_addr, null) << endl;
//     }

//     if (ht->inst.write_mem) {
//         cout << "\t+ " << "addr  written: " << hex << addr << endl;
//         cout << "\t+ " << "to be written: " << hex << data_to_write << endl;
//         cout << "\t+ " << "written:       " << hex << data_mem->read_data(ht->get_id(), aligned_addr, null) << endl;
//     }
//     }
    //#endif /* DBG_PIPE || DBG_MEM */

    //    _dbg_pipeline(ht);

#ifdef _NO_SYSTEMC_
    out = ht;
#else
    out.write(ht);
#endif /* _NO_SYSTEMC_ */
}

void mem::_dbg_pipeline(const hw_thread_ptr& ht) {

#if defined (DBG_PIPE) || defined (DBG_MEM)
    bool null;
#ifdef _NO_SYSTEMC_
    cout << "*mem*" << "     " << ", mem stalled: " << ht->is_memory_stalled() << endl;
#else
    cout << "*mem*" << "     (" << sc_time_stamp() << ") " << ", mem stalled: " << ht->is_memory_stalled() << endl;
#endif /* _NO_SYSTEMC_ */
    cout << "hw_thread's id: " << ht->get_id() << ", pc: 0x" << hex << ht._handle->PC << hex <<  ", " << ht->inst << endl;
    cout << "\t+ "  << "load: " << ht->inst.read_mem  << ", store: " << ht->inst.write_mem << endl;

    if (ht->inst.read_mem) {
        cout << "\t+ " << "to be read from: " << hex << addr << endl;
        cout << "\t+ " << "read:            " << hex << data_mem->read_data(ht->get_id(), aligned_addr, null) << endl;
    }

    if (ht->inst.write_mem) {
        cout << "\t+ " << "addr  written: " << hex << addr << endl;
        cout << "\t+ " << "to be written: " << hex << data_to_write << endl;
        cout << "\t+ " << "written:       " << hex << data_mem->read_data(ht->get_id(), aligned_addr, null) << endl;
    }
#endif /* DBG_PIPE || DBG_MEM */
}

 bool mem::_is_not_valid_hwthread(const hw_thread_ptr& hardware_thread) {
  return ( hardware_thread.is_null() || !hardware_thread->is_enabled()
	   || hardware_thread->is_fetch_stalled());
 }
