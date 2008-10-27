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

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include "systemc.h"
#endif /* _NO_SYSTEMC_ */

#include "memory_unit.h"


int sc_main(int argc, char *argv[]) {
#ifdef _NO_SYSTEMC_
    cout << "SystemC Simulation" << endl;
#else
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
#endif

    memory_unit mem;

    /* Populate the memory unit */
    for (unsigned int addr = 0; addr < 1000; addr++) {
        mem.add_address(addr, addr + 1000);
    }

    cout << "Print the contents of memory unit" << endl;
    cout << mem << endl;

    cout << "Check the has_address" << endl;
    for (unsigned int addr = 0; addr < 1000; addr++) {
        cout << "addr: " << addr << ", data: " << mem[addr]
             << ", yes: " << (addr + 1000 == mem[addr])
             << ", no: " << (addr + 1000 == mem[addr + 1000]) << endl;
    }

    cout << "Remove all addresses" << endl;
    for (unsigned int addr = 0; addr < 1000; addr++) {
        mem.remove_address(addr);
    }

    cout << "Print memory contents after being emptied" << endl;
    cout << mem << endl;

    cout << "Remove further memory content" << endl;
    mem.remove_address(103);

    cout << "End test" << endl;
    return(0);

}



