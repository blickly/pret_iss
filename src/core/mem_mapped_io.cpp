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

  $Id: mem_mapped_io.cpp 1160 2008-11-07 03:37:06Z blickly $

*/

#include "mem_mapped_io.h"

void mem_mapped_io::behavior() {}

bool mem_mapped_io::is_stalled(int tid, uint32_t addr) {
    return false;
}

uint32_t mem_mapped_io::read(int tid, uint32_t addr, bool& stalled) {
    stalled = is_stalled(tid, addr);
    return mem[addr];
}

void mem_mapped_io::write(int tid, uint32_t addr, uint32_t data, bool& stalled) {
    stalled = is_stalled(tid, addr);
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
    mem.add_address(addr, data);
}

void mem_mapped_io::out_file(uint32_t data) {
    static ofstream _fd;
    if (!_fd.is_open()) {
        _fd.open("out_fd.dmp", ios::out);
        if (!_fd.is_open()) {
            cerr << "ERR: output file could not be opened" << endl;
            return;
        }
    }
    /// Address map to an output file the address 0x80000600 which we
    /// use as VIDEO out
    _fd << (char) data << flush;
}

void mem_mapped_io::out_uart(uint32_t data) {
    char out_char = (char) data;
    if (out_char == '\r') {
        return; // This prevents printing out ^M on newlines.
    }
    cout << (char)(data);
}
