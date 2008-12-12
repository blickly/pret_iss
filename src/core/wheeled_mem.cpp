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

  $Author$
  $Date$
  $Id$

*/


#include "wheeled_mem.h"
#include "assert.h"

#define ROUND_UP(x,y) ( (((x) + (y) - 1) / (y)) * (y) )
#ifndef USE_BURST
#define MAX_WORDS 1
#else
#define MAX_WORDS 4
#endif

/*
 * Create a new wheeled memory with the given delays and memory window size
 * (measured in cycles).
 *
 */
wheeled_mem::wheeled_mem(int l_d, int t_d, cycle_counter* cyc, int mem_win)
        : lat_d(l_d),
        throughput_d(t_d),
        MEMORY_WINDOW(mem_win) {
    counter = cyc;
    accessing = false;
    remaining_cycles = 0;
}

/*
 * Create a new wheeled memory with the given delays.
 *
 * Since this constructor does not specify a memory window size,
 * the minimal size that can accomodate the longest burst access is assumed.
 */
wheeled_mem::wheeled_mem(int l_d, int t_d, cycle_counter* cyc)
        : lat_d(l_d),
        throughput_d(t_d),
        MEMORY_WINDOW(ROUND_UP(l_d + t_d * (MAX_WORDS - 1), NUM_THREADS) + 1) {
    counter = cyc;
    accessing = false;
    remaining_cycles = 0;
}

bool wheeled_mem::is_stalled(int tid, uint32_t addr) {
    return is_stalled_helper(GLOBAL, tid, addr, 1);
}

bool wheeled_mem::is_stalled_burst(int tid, uint32_t addr, int num_words) {
    return is_stalled_helper(BURST, tid, addr, num_words);
}

bool wheeled_mem::is_stalled_helper(AccessorType acc, int tid, uint32_t addr, int num_words) {
    // We only keep track of the requested addr to assert that we don't
    // get multiple conflicting accesses to the wheel.
    static uint32_t requested_addr;
    
    // If it isn't our turn, we must stall
    if (tid != current_thread() || (!is_scheduled(acc, tid) && !accessing)) {
        assert(!accessing);
        return true;
    }

    // If this is a "clean" load, we set up the stall time.
    if (!accessing) {
        remaining_cycles = get_delay(acc, num_words);
        requested_addr = addr;
    }

    // If we have stalled all of the needed cycles, then we are done.
    if (remaining_cycles <= 0) {
        accessing = true;
    } else {
        accessing = false;
    }

    assert(requested_addr == addr);
    return accessing;
}

/*
 * Return the data that is stored at the given memory location,
 * as well as a boolean for whether the read needs to stall for
 * longer before the memory can be accessed.
 */
uint32_t wheeled_mem::read(int tid, uint32_t addr, bool& stalled) {

    stalled = is_stalled(tid, addr);
    return mem[addr];
}

vector<uint32_t> wheeled_mem::read_burst(int tid, uint32_t start_addr, bool& stalled, int num_words) {
    stalled = is_stalled_burst(tid, start_addr, num_words);
    vector<uint32_t> data_stream;
    for (uint32_t addr = start_addr ; addr < start_addr + 4 * num_words;
            addr += 4) {
        data_stream.push_back(mem[addr]);
    }
    return data_stream;
}

int wheeled_mem::burst_words_transferred(int tid, int num_words) {
    // If we aren't accessing, we know zero bytes have been transferred
    if (tid != current_thread() || !accessing) {
        assert(remaining_cycles == 0);
        return 0;
    } else {
        int counted_cycles = get_delay(BURST, num_words) - remaining_cycles;
        if (counted_cycles < lat_d) {
            return 0;
        } else {
            return 1 + (counted_cycles - lat_d) / throughput_d;
        }
    }

}

void wheeled_mem::behavior() {
    if (remaining_cycles != 0) {
        --remaining_cycles;
    }
}


/*
 * Determine whether the given accessor and thread are scheduled to start
 * an acess or not.
 *
 * The is_scheduled method is the heart of the logic for controlling the
 * wheel.
 */
bool wheeled_mem::is_scheduled(AccessorType acc, int tid) {
    if (tid != current_thread()) {
        return false;
    }

    // Only start a memory access if it has enough time to complete
    unsigned int start_window = MEMORY_WINDOW - get_delay(acc, MAX_WORDS);
    if (counter->get_cycles() % MEMORY_WINDOW <= start_window) {
        return true;
    } else {
        return false;
    }
}

/*
 * Return the delay for the given access method.
 *
 * The get_delay method should be overridden where different
 * behavior is desired.
 */
int wheeled_mem::get_delay(AccessorType acc, int num_words) {
    return lat_d + throughput_d * (num_words - 1);
}

int wheeled_mem::current_thread() {
    int window_cycle = counter->get_cycles() % (MEMORY_WINDOW * NUM_THREADS);
    int tid = window_cycle / MEMORY_WINDOW;
    return (1 + tid) % NUM_THREADS;
}
