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

#ifndef _SPECIAL_REG_H_
#define _SPECIAL_REG_H_


#include "instruction.h"
#include "defines.h"
#include <stdint.h>

enum DT_STATUS {
    UNSET,
    SET,
    MISSED
};

struct special_reg {

    special_reg();
    special_reg(const special_reg& sr);
    bool operator==(const special_reg& sr);
    void operator=(const special_reg& sr);

    uint32_t get_carry_bit();
    void set_psr(uint32_t psr);
    uint32_t get_psr();
    void dump_deadline_timers();
    void dump();
    void decrement_deadline_timers();
    bool missed_deadline(uint32_t& dt_reg);
    void decrement_pll_timers();

    // Data members
    unsigned char imp_ver;
    WindowPointer curr_wp;  // Thread's current window pointer
    unsigned char icc;
    bool s;
    bool ps;
    uint32_t tbr;
    uint32_t wim;
    bool et;
    bool ec;
    bool ef;
    unsigned char pil;

    uint32_t asr[32];
    uint32_t y;
    // Deadline timers
    //uint32_t dt[NUM_DEADLINE_TIMERS];
    int32_t dt[NUM_DEADLINE_TIMERS];
    // PLL timers
    uint32_t plldt[NUM_PLL_TIMERS];
    // PLL timer double register used to store the load value
    uint32_t pll_load[NUM_PLL_TIMERS];
    bool pll_loaded;
    DT_STATUS dt_status[NUM_DEADLINE_TIMERS];

};
#endif
