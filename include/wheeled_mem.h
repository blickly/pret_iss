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

#ifndef _WHEELED_MEM_H_
#define _WHEELED_MEM_H_

#include "defines.h"
#include "memory_unit.h"
#include "cycle_counter.h"
#include "mem_location.h"
#include <vector>
#include <stdint.h>

class wheeled_mem : public mem_location {
public:
    enum AccessorType { burst, GLOBAL, DMA };
    wheeled_mem(int latency_delay, int throughput_delay, cycle_counter* cyc);
    wheeled_mem(int l_d, int t_d, cycle_counter* cyc, int mem_win);
    virtual ~wheeled_mem();
    virtual bool is_stalled(int tid, uint32_t addr);
    virtual uint32_t read(int tid, uint32_t addr, bool& stalled);

    virtual vector<uint32_t> read_burst(int tid,
                                        uint32_t addr,
                                        bool& stalled,
                                        int num_words);
    virtual int burst_words_returned(int tid, int num_words);
    virtual void behavior();

protected:
    bool is_stalled_burst(int tid, uint32_t addr, int num_words);
    bool is_stalled_helper(AccessorType acc, int tid, uint32_t addr, int num_words);
    int current_thread();
    vector<uint32_t> get_data_stream(uint32_t start_addr, int num_words);

    virtual bool is_scheduled(AccessorType acc, int tid);
    virtual int get_delay(AccessorType acc, int num_words);

    cycle_counter* counter;
    bool accessing;
    int remaining_cycles;
    int accessor_tid;
    uint32_t requested_addr;

    const int lat_d;
    const int throughput_d;
    const int MEMORY_WINDOW;

};

#endif
