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

  $Id: $
*/

#include <sstream>
#include "static_bound_parser.h"
#include "memory_unit.h"
#include "memory_controller.h"


void static_bound_parser::set_memory_controller(memory_controller* memc) {
    _mem_cont = memc;
}

static_bound_parser::static_bound_parser(): _mem_cont(NULL) {
}

void static_bound_parser::load_data_spm(unsigned int tid, const string& dspm) {
    load_spm(tid, dspm);
}

void static_bound_parser::load_inst_spm(unsigned int tid, const string& ispm) {
    load_spm(tid, ispm);
}

void static_bound_parser::load_spm(unsigned int tid, const string& spm) {
    /* Read the memory map file for instruction SPM. */
    ifstream file_spm(spm.c_str(), ios::in);
    l1_scratch* current_scratchpad = &_mem_cont->get_spm_mem_loc(tid);
    memory_unit* main_memory = &_mem_cont->get_main_mem_loc(0x3FFFFFFF).mem;

    //cout << "HDP: DSPM - " << dspm << endl;
    if (file_spm.is_open()) {
        uint32_t min = 0;
        uint32_t max = 0;
        string line;
        istringstream ss;
        while (!file_spm.eof()) {
            getline(file_spm, line);
            //cout << "got line: " << line << endl;
            istringstream ss(line);
            ss >> hex >> min >> max;
            if (line.size() > 0) {
                uint32_t spm_addr = 0;
                for (uint32_t addr = min; addr < max + 4; addr += 4) {
                    uint32_t data = (*main_memory)[addr];
                    current_scratchpad->add_addr(addr, spm_addr, data);
                    spm_addr += 4;
                }
            }
        }
        file_spm.close();
    }
}
