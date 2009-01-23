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
*/

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include "systemc.h"
#endif
#include "l1_scratch.h"


#ifdef _NO_SYSTEMC_
int main(int argc, char *argv[]) {
#else
int sc_main(int argc, char *argv[]) {
#endif

#ifdef _NO_SYSTEMC_
    cout << "SystemC Simulation" << endl;
#else
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
#endif

    l1_scratch spm;

    /* Populate the memory unit */
    for (unsigned int addr = 0; addr < 1000; addr++) {
        /*              (mem, spm, inst) */
        spm.add_addr(addr, addr + 1000, 1000 - addr);
    }

    cout << "Print the contents of the valid address in SPM" << endl;
    spm.dbg_print_valid_addr();
    cout << endl;

    cout << "Print the contents of memory unit" << endl;
    cout << spm.mem << endl;

    cout << "Replace (2,x) with (2,4) where (addr,inst) by adding on spm addr 3" << endl;
    spm.add_addr(2, 3, 4);

    cout << "Get the changed address" << endl;
    bool stalled = false;
    unsigned int v = spm.read(0, 2, stalled);
    cout << "Instruction returned: (2," << v << "), stalled = " << stalled << endl;

    cout << "Get an address that is beyond range" << endl;
    v = spm.read(0, 1000, stalled);
    cout << "Instruction returned: (2," << v << "), stalled = " << stalled << endl;

    cout << "Find based on SPM address 3. Should be 4." << endl;
    map<unsigned int, unsigned int>::iterator it = spm.find_spm_addr(3);
    if (it == spm.find_spm_addr(-1)) {
        cout << "Error in returning an interator based on SPM address" << endl;
        cout << "Exiting" << endl;
        return 0;
    }
    unsigned int memaddr = it->first;
    v = spm.read(0, memaddr, stalled);
    cout << "Instruction returned: (2," << v << "), stalled = " << stalled << endl;

    cout << "Update an SPM address 3's associated memory address. Should be 2222" << endl;
    spm.update_spm_addr(3, 21, 2222);
    v = spm.read(0, 21, stalled);
    cout << dec << "Instruction returned: (21," << v << "), stalled = " << stalled << endl;

    cout << "Remove address 2" << endl;
    spm.rm_addr(2);

    cout << "Get an address that is in range, but not allocated (2,x). Should be 0." << endl;
    v = spm.read(0, 2, stalled);
    cout << "Instruction returned: (2," << v << "), stalled = " << stalled << endl
         ;

    cout << "End test " << false << endl;
    return(0);

}



