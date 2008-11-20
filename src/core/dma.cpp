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
*/

#include "dma.h"

dma::dma() {
    _initialize();
}

dma::dma(const sc_module_name& name): module_base(name) {
    _initialize();
}

dma::~dma() {

}

void dma::behavior() {
    for (unsigned int thread_id = 0; thread_id < NUM_THREADS; thread_id++) {
        /* A completed request to wheeled_mem takes a single cycle to transfer to SPM. */
        if (_transfer_complete[thread_id] == BUSY) {
            bool dma_fetch_stalled = false;
            // Receive 4 words at a time.
            _received_data[thread_id] = _mem_wheel->read_pdma(thread_id, _mem_source_reg[thread_id], dma_fetch_stalled, 4);
            if (!dma_fetch_stalled) {
                // Move data into SPM.
                for (unsigned int num_data = 0; num_data < _received_data[thread_id].size(); num_data++) {
                    _inst_spm[thread_id]->add_addr(_mem_source_reg[thread_id] + (num_data*4), _spm_target_reg[thread_id] + (num_data*4), _received_data[thread_id][num_data]);
                    //     cout << " " << hex << _received_data[thread_id][num_data];
                }

                // Clear out the received data.
                _received_data[thread_id].clear();
                // Set transfer state to complete.
                _transfer_complete[thread_id] = IDLE;
//   cout << hex << "dma::behavior: thread_id: " << thread_id << ", src: " << _mem_source_reg[thread_id]
//                       << ", target: " << _spm_target_reg[thread_id] << endl;

//    cout << "dma:: Print valid addresses in SPM" << endl;
//    _inst_spm[thread_id]->dbg_print_valid_addr();
            }
        }
    }
}

bool dma::is_transfer_idle(const int& thread_id) {
    return (_transfer_complete[thread_id] == IDLE);
}

bool dma::is_transfer_complete(const int& thread_id) {
    return (_transfer_complete[thread_id] == COMPLETE);
}

void dma::make_transfer(const hw_thread_ptr& thread_address) {
  _transfer_complete[thread_address->get_id()] = BUSY;
}

void dma::register_memory_wheel(const wheeled_mem& wheel) {
    _mem_wheel = const_cast<wheeled_mem*>(&wheel);
}

void dma::register_inst_spm(l1_scratch* spm[]) {
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        _inst_spm[i] = spm[i];
    }
}

void dma::set_mem_source(const unsigned int& thread_id, const uint32_t& source) {
    _mem_source_reg[thread_id] = source;
}

unsigned int dma::get_mem_source(const unsigned int& thread_id) const {
    return _mem_source_reg[thread_id];
}

unsigned int dma::get_spm_target(const unsigned int& thread_id) const {
    return _spm_target_reg[thread_id];
}

void dma::set_spm_target(const unsigned int& thread_id, const uint32_t& target) {
    _spm_target_reg[thread_id] = target;
}

void dma::set_transfer_state(const unsigned int& thread_id, const transfer_state_t& transfer_state) {
    _transfer_complete[thread_id] = transfer_state;
}

void dma::_initialize() {
    _mem_wheel = NULL;
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        _mem_source_reg[i] = 0;
        _spm_target_reg[i] = 0;
        _transfer_complete[i] = IDLE;
        _inst_spm[i] = NULL;
    }
}
