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
#endif
#include "pdma.h"


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

    pdma pcore("test pdma core");

    uint32_t inststream[] = {0x00000000, 0x40000000, 0x800000FF,
                             0x10000000, 0x7FFFFFFF, 0xC0127FFF,
                             0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                             0x10000000, 0x40000040, 0x800000FF,
                             0x10000040, 0x40000000, 0x800000FF
                            };

    uint32_t addrstream[] = {0x00000000, 0x00000004, 0x80000008,
                             0x1000000C, 0x00000010, 0x00000014,
                             0x00000018, 0x0000001C, 0x00000020,
                             0x40000000, 0x40000004, 0x40000008,
                             0x40000040, 0x00000044, 0x40000048
                            };

    uint32_t pc = 0;
    /* Run through the instructions */
    for (unsigned int i = 0; i < 16; i++) {
        //cout << "Running: PC = " << pc/4 << endl;
        pc = pcore.behavior(addrstream[i], inststream[pc/4]);
    }


    cout << "End test " << false << endl;
    return(0);

}



