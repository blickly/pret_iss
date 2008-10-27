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

#include "cycle_counter.h"

cycle_counter::cycle_counter(const sc_module_name& nm): module_base(nm) {
    _cycles = 0;
}

cycle_counter::cycle_counter() {
}

void cycle_counter::behavior() {
    /* Increment the cycle counter. */
    unsigned int next_cycle = 1 + get_cycles();
#ifdef _NO_SYSTEMC_
    _cycles = next_cycle;
#else
    _cycles.write(next_cycle);
#endif /* _NO_SYSTEMC_ */
}

unsigned int cycle_counter::get_cycles() const {
#ifdef _NO_SYSTEMC_
    return _cycles;
#else
    return _cycles.read();
#endif /* _NO_SYSTEMC_ */
}

