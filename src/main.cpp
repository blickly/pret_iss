/*
  Copyright (c) 2008-2009 The Regents of the University of California.
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
#include <systemc.h>
#endif

#include "core.h"
#include <stdlib.h>

/// For timing the execution of the program
#include <stdio.h>
#include <sys/time.h>


// Taken from the GNU manual
int
timeval_subtract(timeval* result, timeval* x, timeval* y) {
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
       tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

void print_setup(core * c) {
#ifdef _NO_SYSTEMC_
    cout << "SystemC Simulation" << endl;
#endif

    cout << endl << "---------------------------------------------------- " << endl;
    cout << "\tPRET Configuration" << endl;
    cout << "---------------------------------------------------- " << endl;
    cout << "Number of threads         : " << dec << NUM_THREADS << endl;
    cout << "Number of register windows: " << REGISTER_WINDOWS << endl;
    cout << "Number of deadline timers : " << NUM_DEADLINE_TIMERS << endl;
    cout << "Number of PLL timers      : " << NUM_PLL_TIMERS << endl;
    cout << "\nClock setup" << endl;
#ifndef  _NO_SYSTEMC_
    cout << "Period                    : " << c->get_period() << " seconds " << endl;
#endif
    //  cout << "Duty cycle                : " << c->clock->duty_cycle() << endl;
    cout << "\nMemory hierarchy" << endl;
    cout << "Main memory delay          : " << MEM_DELAY << endl;

    cout << "Instruction/Data scratchpads  " << endl;
    
    cout << "---------------------------------------------------- " << endl;
}

#ifdef _NO_SYSTEMC_
int main(int argc, char *argv[]) {
#else
int sc_main(int argc, char * argv[]) {
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
#endif
    int num_cycles = 0;
    string ftxt;

    if (argc > 1) {
        if (argc == 3) {
            num_cycles = atoi(argv[2]);
        }
        ftxt = argv[1];
    } else {
        cout << "Usage: pret <file-name> <cycles>" << endl;
        cout << endl;
        cout << " + file-name: file with paths to srec files to load into threads" << endl;
        cout << " + cycles   : number of cycles to execute the simulator" << endl;
        return 1;
    }
    /// Set the name of the file to read the SREC files from
    core db(ftxt);
    print_setup(&db);
    ///Print out the period.
    db.get_period();

    struct timeval stv, etv, rtv;
    gettimeofday(&stv, NULL);

    /* ----------------------------- */
    /* Put the code you want to time */
    /* ----------------------------- */

    cout << endl << "---------------------------------------------------- " << endl;
    cout << "\t Start Simulation" << endl;
    cout << "---------------------------------------------------- " << endl;

    db.run(num_cycles);
    cout << "---------------------------------------------------- " << endl;

    /* ----------------------------- */
    gettimeofday(&etv, NULL);

    timeval_subtract(&rtv, &etv, &stv);


    /// Print out the instructions executed for every thread.
    cout << endl << "---------------------------------------------------- " << endl;
    cout << "\t Simulation Statistics" << endl;
    cout << "---------------------------------------------------- " << endl;

    if ((num_cycles > 0) || (num_cycles == -1)) {
        int total_instr = 0;
        int total_cycles = 0;
#ifndef _NO_SYSTEMC_
        cout << "Clock period    : " << db.get_period()
             << " seconds " << endl;
#endif
        cout << "Cycles executed : " << dec << num_cycles << endl;
        cout << endl;
        for (int i = 0; i < NUM_THREADS; i++) {
            hw_thread_ptr * hp = db.get_thread(i); 
            cout << "thread[" << i << "], "
                 << ", # cycles: " << hp->get_handle()->cnt_cycles << endl;
            cout << "thread[" << i << "], " << "# instructions: "
                 << hp->get_handle()->cnt_instr << endl;
            total_instr += hp->get_handle()->cnt_instr;
            total_cycles += hp->get_handle()->cnt_cycles;
        }
        cout << endl;

    } else {
        cout << "Error: num_cycles is 0" << endl;
    }

    return 0;
};
