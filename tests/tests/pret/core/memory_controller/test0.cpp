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
using namespace std;
#include "regression.h"
#include "cycle_counter.h"
#include "memory_controller.h"
#include "dma.h"

int sc_main(int argc, char *argv[]) {
#ifdef _NO_SYSTEMC_
    cout << "SystemC Simulation" << endl;
#else
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
#endif

#ifdef _NO_SYSTEMC_
    sc_clock clock(5);
#else
    sc_clock clock("sc_clock", 5, SC_NS);
#endif
    cycle_counter cyc_cnt("cycle_counter");
    memory_controller mem_cnt("memory_controller", &cyc_cnt);
#ifndef _NO_SYSTEMC_
    cyc_cnt.clk(clock);
    mem_cnt.clk(clock);
#endif /* _NO_SYSTEMC_ */

#ifdef _USE_SYSTEMC_
    dma dma_cont("blocking_dma");
    dma_cont.clk(clock);
    mem_cnt.blocking_dma(dma_cont);
#endif /* _USE_SYSTEMC_ */

    /* The real test is here */
    /* Let us try and test the out_uart function only for printable
       characters  */

    for (unsigned int i = 32; i < 127; i++) {
        mem_cnt.out_uart(i);
    }
    cout << endl;

    return(0);

}



