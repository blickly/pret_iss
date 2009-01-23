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

#include "memory_unit.h"

memory_unit::memory_unit() {
}

void memory_unit::add_address(const uint32_t& addr, const uint32_t& data) {
    _mem[addr] = data;
}

bool memory_unit::has_address(const uint32_t& addr) const {
    if (_mem.find(addr) == _mem.end()) {
        return false;
    } else {
        return true;
    }
}

uint32_t memory_unit::operator[](const uint32_t& addr) {
    if (has_address(addr) == false) {
        //        _mem[addr] = 0x0;
        return 0;
    }
    return _mem[addr];
}

void memory_unit::remove_address(const uint32_t& addr) {
    if (has_address(addr)) {
        map<uint32_t, uint32_t>::iterator it = _mem.find(addr);
        _mem.erase(it);
    }
}

