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

#include "core.h"

#ifdef _NO_SYSTEMC_
core::core(const string& ftxt) {
    _clock = new sc_clock(5);
    _pll_clock = new sc_clock(1);
    _cycle_counter = new cycle_counter("cycle_counter");

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        _instruction_spm[i] = new l1_scratch();
    }
    _memory_controller = new memory_controller("memory_controller", _cycle_counter);
    _memory_controller->register_inst_spm(_instruction_spm);

    srec_parser* _srec_parser = new srec_parser();
    _srec_parser->set_memory_unit(_memory_controller->get_main_mem_loc(0x3FFFFFFF).mem);

    static_bound_parser* _static_bound_parser = new static_bound_parser();
    _static_bound_parser->set_memory_controller(_memory_controller);

    _dma_controller = new dma("blocking_dma");
    _dma_controller->register_memory_wheel(*dynamic_cast<wheeled_mem*>(&_memory_controller->get_main_mem_loc(0x3FFFFFFF)));
    _dma_controller->register_inst_spm(_instruction_spm);
    _memory_controller->blocking_dma(*_dma_controller);

    _thread_controller = new hw_thread_controller("thread_control_unit");
    _thread_controller->set_txtname(ftxt);
    _thread_controller->parse_srec_files(*_srec_parser, *_static_bound_parser);

    // The parsers are not needed during simulation.
    delete _srec_parser;
    delete _static_bound_parser;

    _fetch_stage = new fetch("fetch_stage");
    _fetch_stage->instruction_memory(*_memory_controller);
    //    _fetch_stage->blocking_dma(*_dma_controller);

    _decode_stage = new decode("decode_stage");

    _regacc_stage = new regacc("registeraxs_stage");
    //    _regacc_stage->coproc_dma(*_dma_controller);
    _execute_stage = new execute("exe_stage");

    _memory_stage = new mem("mem_stage");
    _memory_stage->data_mem(*_memory_controller);

    _except_stage = new except("except_stage");
    _except_stage->coproc_dma(*_dma_controller);
}
#else
core::core(const string& ftxt) {
    _clock = new sc_clock("sc_clock", 5, SC_NS);
    _pll_clock = new sc_clock("pll_clock", 1, SC_NS);

    _cycle_counter = new cycle_counter("cycle_counter");
    _cycle_counter->clk(*_clock);

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        _instruction_spm[i] = new l1_scratch();
    }
    _memory_controller = new memory_controller("memory_controller", _cycle_counter);
    _memory_controller->register_inst_spm(_instruction_spm);
    _memory_controller->clk(*_clock);

    srec_parser* _srec_parser = new srec_parser();
    //_srec_parser->set_memory_controller(_memory_controller);
    _srec_parser->set_memory_unit(_memory_controller->get_main_mem_loc(0x3FFFFFFF).mem);

    static_bound_parser* _static_bound_parser = new static_bound_parser();
    _static_bound_parser->set_memory_controller(_memory_controller);

    _dma_controller = new dma("blocking_dma");
    _dma_controller->clk(*_clock);
    _dma_controller->register_memory_wheel(*dynamic_cast<wheeled_mem*>(&_memory_controller->get_main_mem_loc(0x3FFFFFFF)));
    //    _dma_controller->register_memory_wheel(*dynamic_cast<wheeled_mem*>(_memory_controller->_main_mem));
    _dma_controller->register_inst_spm(_instruction_spm);
    _memory_controller->blocking_dma(*_dma_controller);
    _thread_controller = new hw_thread_controller("thread_control_unit");
    _thread_controller->clk(*_clock);
    _thread_controller->pll_clock(*_pll_clock);
    _thread_controller->out(_pool_to_f);
    /// set_txtname must come before parse_srec_files.
    _thread_controller->set_txtname(ftxt);
    _thread_controller->parse_srec_files(*_srec_parser, *_static_bound_parser);
    delete _srec_parser;
    //thread_pool->instruction_mem(*mem_cont);

    _fetch_stage = new fetch("fetch_stage");
    _fetch_stage->clk(*_clock);
    _fetch_stage->in(_pool_to_f);
    _fetch_stage->out(_f_to_d);
    _fetch_stage->instruction_memory(*_memory_controller);
    //    _fetch_stage->blocking_dma(*_dma_controller);

    _decode_stage = new decode("decode_stage");
    _decode_stage->clk(*_clock);
    _decode_stage->in(_f_to_d);
    _decode_stage->out(_d_to_ra);

    _regacc_stage = new regacc("registeraxs_stage");
    _regacc_stage->clk(*_clock);
    _regacc_stage->in(_d_to_ra);
    _regacc_stage->out(_ra_to_ex);
    //    _regacc_stage->coproc_dma(*_dma_controller);

    _execute_stage = new execute("exe_stage");
    _execute_stage->clk(*_clock);
    _execute_stage->in(_ra_to_ex);
    _execute_stage->out(_ex_to_mem);

    _memory_stage = new mem("mem_stage");
    _memory_stage->clk(*_clock);
    _memory_stage->in(_ex_to_mem);
    _memory_stage->out(_mem_to_except);
    _memory_stage->data_mem(*_memory_controller);

    _except_stage = new except("except_stage");
    _except_stage->clk(*_clock);
    _except_stage->in(_mem_to_except);
    _except_stage->coproc_dma(*_dma_controller);
}
#endif /* _NO_SYSTEMC_ */

core::~core() {
    delete _clock;
    delete _pll_clock;
    delete _cycle_counter;
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        delete _instruction_spm[i];
    }
    delete _memory_controller;
    delete _dma_controller;
    delete _thread_controller;
    delete _fetch_stage;
    delete _decode_stage;
    delete _regacc_stage;
    delete _execute_stage;
    delete _memory_stage;
    delete _except_stage;
}


void core::behavior() {
#ifdef _NO_SYSTEMC_
    _except_stage->behavior(_mem_to_except);
    _memory_controller->behavior();
    _dma_controller->behavior();
    _memory_stage->behavior(_ex_to_mem, _mem_to_except);
    _execute_stage->behavior(_ra_to_ex, _ex_to_mem);
    _regacc_stage->behavior(_d_to_ra, _ra_to_ex);
    _decode_stage->behavior(_f_to_d, _d_to_ra);
    _fetch_stage->behavior(_pool_to_f, _f_to_d);
    _thread_controller->behavior(_pool_to_f);
    _cycle_counter->behavior();
#endif /* _NO_SYSTEMC_ */
}

bool core::is_memory_read(int thread_id) const {
    return _thread_controller->get_thread(thread_id)->get_handle()->inst.is_read_memory();
}


bool core::is_memory_write(int thread_id) const {
    return _thread_controller->get_thread(thread_id)->get_handle()->inst.is_write_memory();
}

int core::get_aligned_memory_address(int thread_id) const {
    return 4*(_thread_controller->get_thread(thread_id)->get_handle()->inst.get_alu_result() / 4);
}

unsigned int core::get_instruction_count(int thread_id) const {
    return _thread_controller->get_thread(thread_id)->get_handle()->cnt_instr;
}

unsigned int core::get_cycle_count() const {
    if (_cycle_counter == NULL) {
        return 0;
    }

    return _cycle_counter->get_cycles();
}

uint32_t core::read_memory(uint32_t addr) const {
    return _memory_controller->get_main_mem_loc(addr).mem[addr];
}

void core::write_memory(const uint32_t addr, const uint32_t data) {
    _memory_controller->get_main_mem_loc(addr).mem.add_address(addr, data);
}

uint32_t core::get_pc(int thread_id) const {
    return _thread_controller->get_thread(thread_id)->get_handle()->get_pc();
}

long int core::get_period() const {
    if (_clock == NULL) {
        return -1;
    }
#ifdef _NO_SYSTEMC_
    long int t = _clock->get_period();
#else
    long int t = _clock->period().value();
#endif /* _NO_SYSTEMC_ */

    return t;
}

hw_thread_ptr * core::get_thread(int thread_id) const {
    if ((_thread_controller != NULL) && (_thread_controller->get_thread(thread_id) != NULL)) {
        return _thread_controller->get_thread(thread_id);
    } else {
        return NULL;
    }
}

bool core::is_fetch_stalled(int thread_id) const {
    return _thread_controller->get_thread(thread_id)->get_handle()->is_fetch_stalled();
}

bool core::is_memory_stalled(int thread_id) const {
    return _thread_controller->get_thread(thread_id)->get_handle()->is_memory_stalled();
}

void core::print_memory() {
    cout << "Data Memory contents for thread " << _thread_controller->get_thread(0)->get_handle()->get_id()
         << "(PC = " << hex <<  _thread_controller->get_thread(0)->get_handle()->get_pc() << ")" << endl;
    // FIX: The dump because it's one big memory now and it needs to
    // spit out only from the address that is data.
    _memory_controller->dump();
}


void core::print_registers(int thread_id) {
    _thread_controller->get_thread(thread_id)->get_handle()->regdump();
}

void core::print_timers(int thread_id) {
    _thread_controller->get_thread(thread_id)->get_handle()->spec_regs.dump_deadline_timers();
}

void core::print_sync_table() {
	_thread_controller->print_sync_table();
}

bool core::run(int cycle_count) {
#ifdef _NO_SYSTEMC_
    if (cycle_count == -1) {
        while (1) {
            try {
                _clock->tick();
                _pll_clock->tick();
                if (_clock->get_tick()) {
                    behavior();
                }
                if (_pll_clock->get_tick()) {
                    _thread_controller->pll_behavior();
                }
            } catch (int n) {
                return false;
            }
        }
    }

    for (int i = 0; i < cycle_count; i++) {
        try {
            _clock->tick();
            _pll_clock->tick();
            if (_clock->get_tick()) {
                behavior();
            }
            if (_pll_clock->get_tick()) {
                _thread_controller->pll_behavior();
            }
        } catch (int n) {
            return false;
        }
    }

#else
    try {
        sc_start(cycle_count);
    } catch (int n) {
        return false;
    }
#endif /* _NO_SYSTEMC_ */

    return true;
}

bool core::run_for_instruction(int instruction_count) {
#ifdef _NO_SYSTEMC_
    long int t = _clock->get_period();
    return run(instruction_count * t);
#else
    if (instruction_count < 0)
        return false;

    long int t = _clock->period().value();
    try {
        sc_start(instruction_count * t, SC_PS);
    } catch (int n) {
        return false;
    }
#endif /* _NO_SYSTEMC_ */
    return true;
}
