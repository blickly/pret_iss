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
#include <assert.h>
#include "srec_parser.h"
#include "memory_unit.h"
#include "memory_controller.h"

#define MEMORY_ALIGNMENT 4

void srec_parser::set_memory_unit(memory_unit& mem_unit) {
    _mem = &mem_unit;
}

srec_parser::srec_parser(): _mem(NULL) {
    _starting_addr = 0x40000000;
}

uint32_t srec_parser::parse(const char* filename) {
    if (_mem == NULL) {
        cerr << "Error: address of memory unit is NULL" << endl;
        return 0;
    }

    FILE* srec_file = fopen(filename, "r");
    if (srec_file == NULL) {
        cerr << "Error: Unable to find file " << '"' << filename << '"' << endl;
        return 0;
    } else {
        int return_val;
        do {
            return_val = _readline(srec_file);
            fflush(srec_file);
        } while (return_val != EOF);
        fclose(srec_file);
    }

    //#ifdef DEBUGFLAG
    //  cout << "Memory initialized: " << _mem << endl << *_mem << endl;
    //#endif

    return _starting_addr;
}

int srec_parser::_readline(FILE* const srec_file) {
    char s_type[2];
    int remaining_bytes;
    int current_addr;
    char trash[515];
    //for (int i = 0; i < 515; ++i) { trash[i] = 0; }

    if (fscanf(srec_file, "%2s", s_type) == EOF) {
        return EOF;
    }
    //cout << "s_type: " << s_type << endl;
    assert(fscanf(srec_file, "%2x", &remaining_bytes) == 1);
    //cout << "remaining_bytes: " << dec << remaining_bytes << endl;

    switch (s_type[1]) {
    case '1':
    case '9':
        assert(fscanf(srec_file, "%4x", &current_addr) == 1);
        remaining_bytes -= 2;
        break;
    case '2':
    case '8':
        assert(fscanf(srec_file, "%6x", &current_addr) == 1);
        remaining_bytes -= 3;
        break;
    case '3':
    case '7':
        assert(fscanf(srec_file, "%8x", &current_addr) == 1);
        remaining_bytes -= 4;
        break;
    default:
        // Read in line and discard it
        int return_val = fscanf(srec_file, "%s", trash);
        //cout << "trash: \'" << trash  << "\'" << endl;
        return return_val;
        break;
    }

    // If we are reading in a starting execution address, then set it correctly.
    if (s_type[1] == '7' || s_type[1] == '8' || s_type[1] == '9') {
        _starting_addr = current_addr;
        // Ignore the checksum
        return fscanf(srec_file, "%2s", trash);
    }

    if (remaining_bytes % 4 != 1) {
        cerr << endl << "Warning: SREC file malformed!!!" << endl;
        return EOF;
    }

    int next_word;
    while (remaining_bytes != 1) {
        remaining_bytes -= 4;
        assert(fscanf(srec_file, "%8x", &next_word) == 1);
        _mem->add_address(current_addr, next_word);
        /* If spm_alloc has a map then allocate it to SPM as well. */
        current_addr += MEMORY_ALIGNMENT;
    }
    // Ignore the checksum
    return fscanf(srec_file, "%2s", trash);

}

