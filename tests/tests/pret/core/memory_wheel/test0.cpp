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

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include "systemc.h"
#endif /* _NO_SYSTEMC_ */
using namespace std;
#include "regression.h"
#include "wheeled_mem.h"

int sc_main(int argc, char *argv[]) {

#ifdef _NO_SYSTEMC_
    cout << "SystemC Simulation" << endl;
#else
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
#endif

    cycle_counter cyc_cnt("cycle_counter");
#ifdef _NO_SYSTEMC_
    sc_clock clock(5);
#else
    sc_clock clock("sc_clock", 5, SC_NS);
    cyc_cnt(clock);
#endif /* _NO_SYSTEMC_ */

    wheeled_mem wmem(MEM_DELAY - 1, BURST_DELAY, &cyc_cnt);

    cout << "Memory Latency: " << MEM_DELAY - 1 << endl;
    cout << "DMA Latency: " << BURST_DELAY << endl;
    /* The real test starts from here. */

    /* Begin by loading something in the main memory. */
    for (unsigned int i = 0; i < 1000; i++) {
        wmem.mem.add_address(i, 4000 + i);
    }
    cout << "Print main memory contents." << endl;
    cout << wmem.mem << endl;

    cout << "Request instructions: address (0-9) for tid 0." << endl;
#ifdef _NO_SYSTEMC_
    bool stalled[6];
    int tid = 0;
    unsigned int addr = 0;

    unsigned int count = 0;
    uint32_t data[6];

    for (unsigned int i = 0; i < 6; i ++) {
        stalled[i] = false;
        data[i] = 0;
    }
    while (count < 6) {
        /* Tick the clock. */
        /* FIXME: Ben, I find it awkward that I have to invoke
           clock tick() six times for this operation to be
           correct. Otherwise, thread0 can have access to the resource
           whenever it is within that window.

           I should be able to invoke read(0, ...) every cycle and it
           should not stalled because the wheeled memory is busy
           completing a request.
        */

        for (tid = 0; tid < 6; tid++) {
            data[tid] = wmem.read(tid, addr, stalled[tid]);
            clock.tick();
            cyc_cnt.behavior();
            wmem.behavior();
        }
        cout << "request: tid: " << 0 << ", cycle: " << cyc_cnt.get_cycles()
             << ", addr: " << addr << ", data: " << data[0]
             << ", stalled: " << stalled[0]
             << ", count: " << dec << count << endl;

        if (!stalled[0]) {
            ++count;
            addr = count;
        } else {

        }
    }
#endif /* _NO_SYSTEMC_ */

#ifndef _NO_SYSTEMC_
    sc_start(6);
#endif
    return(0);

}



