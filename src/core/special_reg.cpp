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


#include "special_reg.h"

uint32_t special_reg::get_carry_bit() {

    return (get_psr() & 0x00100000) ? 1 : 0 ;
}

void special_reg::set_psr(uint32_t psr) {

    // Store the implementation and version identifier (impl,ver)
    imp_ver = (psr & 0xFF000000) >> 24;
    // Skip out the version identifier (ver)
    // Store the icc
    icc = (psr & 0x00F00000) >> 20;
    // Skip out the reserved identifier (reserved)
    // Store the enable co-processor identifier (ec)
    ec = (psr & 0x00002000) >> 13;
    // Store the enable FPU identifier (ef)
    ef = (psr & 0x00001000) >> 12;
    // Store the processor interrupt level (pil)
    pil = (psr & 0x00000F00) >> 8;
    // Store the supervisor mode bit (s)
    s = (psr & 0x00000080) >> 7;
    // Store the supervisor mode at trap (ps)
    ps = (psr & 0x00000040) >> 6;
    // Store the enable traps identifier (et)
    et = (psr & 0x00000020) >> 5;
    // Store the current window pointer (cwp)
    curr_wp = psr & 0x0000001F;
}

uint32_t special_reg::get_psr() {
    uint32_t psr = 0x0;

    psr = ((uint32_t)(imp_ver) << 24) |
          ((uint32_t)(icc) << 20) |
          ((uint32_t)(ec) << 13) |
          ((uint32_t)(ef) << 12) |
          ((uint32_t)(pil) << 8) |
          ((uint32_t)(s) << 7) |
          ((uint32_t)(ps) << 6) |
          ((uint32_t)(et) << 5) |
          ((uint32_t)(curr_wp));

    return psr;
}

void special_reg::dump_deadline_timers() {

    cout << "deadline timers: " << endl;;

    for (uint32_t j = 0; j < 2; j++) {
        for (uint32_t i = j * (NUM_DEADLINE_TIMERS / 2); i < (j + 1)*(NUM_DEADLINE_TIMERS / 2); i++)
            cout << setfill(' ') << "\t    " << setw(3) << i << "  ";
        cout << endl;
        cout << "             ";
        for (uint32_t i = j * (NUM_DEADLINE_TIMERS / 2); i < (j + 1)*(NUM_DEADLINE_TIMERS / 2); i++)
            cout << setfill('0') << hex << setw(8) << dt[i] << "       ";
        cout << endl;
    }
    cout << endl;

    cout << "pll timers: " << endl;;

    for (uint32_t i = 0; i < (NUM_PLL_TIMERS); i++)
        cout << setfill(' ') << "\t    " << setw(3) << i << "  ";
    cout << endl;
    cout << "             ";
    for (uint32_t i = 0; i < (NUM_PLL_TIMERS); i++)
        cout << setfill('0') << hex << setw(8) << plldt[i] << "       ";
    cout << endl;
    cout << "             ";
    for (uint32_t i = 0; i < (NUM_PLL_TIMERS); i++)
        cout << setfill('0') << hex << setw(8) << pll_load[i] << "       ";
    cout << endl;

}

bool special_reg::missed_deadline(uint32_t& dt_reg) {
    bool missed = false;
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        if (dt_status[i] == MISSED) {//((dt[i] < 0) && (dt_status[i] == SET)) {
            //            dt_status[i] = MISSED;
            missed = true;
            dt_reg = i;
            dt_status[i] = UNSET;
        }
    }
    return missed;
}

void special_reg::decrement_deadline_timers() {
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        /* Always decrement the deadline counters. The handling of errors
         * is yet to be decided. However, only decrementing when the
         * counter is greated than 0 is not correct behavior.
         */


        dt[i]--;
        if ((dt[i] - 1 < 0) && (dt_status[i] == SET)) {
            dt_status[i] = MISSED;
        }
    }
}

void special_reg::decrement_pll_timers() {
    for (uint32_t i = 0; i < NUM_PLL_TIMERS; i++) {
        if (plldt[i] > 0) {
            plldt[i] --;
        } else if (pll_load[i] != 0) {
            plldt[i] = --pll_load[i];
            pll_load[i] = 0;
        }
    }
}

void special_reg::dump() {
    //cout << "icc \t ec \t ef \t pil \t s \t ps \t et \t curr_wp" << endl;
    //  cout << hex << (uint32_t)(icc) <<"\t" << ec << "\t" << ef << "\t" << pil << "\t"  << s << "\t" << ps << "\t" << et << "\t" << curr_wp << endl;
    cout << uppercase << setfill('0') << hex << "psr: " << setw(8) << get_psr() << "\t wim: " << setw(8) << wim
         << "\t tbr: " << setw(8) << tbr << "\t y: " << setw(8) << y << endl;
    cout << endl;

}

bool special_reg::operator==(const special_reg& sr) {

    bool flag = false;
    if (curr_wp == sr.curr_wp && icc == sr.icc       &&
            s == sr.s &&  ps == sr.ps &&   tbr == sr.tbr &&
            wim == sr.wim && et == sr.et &&  ec == sr.ec &&
            ef == sr.ef &&  pil == sr.pil && y == sr.y &&
            pll_loaded == sr.pll_loaded)
        flag = true;

    for (uint32_t i = 0; i < 32; i++) {
        if (asr[i] != sr.asr[i])
            flag = false;
    }
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        if (dt[i] != sr.dt[i])
            flag = false;
    }
    for (uint32_t i = 0; i < NUM_PLL_TIMERS; i++) {
        if (plldt[i] != sr.plldt[i] ||  pll_load[i] != sr. pll_load[i])
            flag = false;
    }


    return flag;
}

void special_reg::operator=(const special_reg & sr) {
    curr_wp = sr.curr_wp;
    icc = sr.icc;
    s = sr.s;
    ps = sr.ps;
    tbr = sr.tbr;
    wim = sr.wim;
    et = sr.et;
    ec = sr.ec;
    ef = sr.ef;
    pil = sr.pil;
    imp_ver = sr.imp_ver;
    y = sr.y;
    for (uint32_t i = 0; i < 32; i++) {
        asr[i] = sr.asr[i];
    }
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        dt[i] = sr.dt[i];
        dt_status[i] = sr.dt_status[i];
    }
    for (uint32_t i = 0; i < NUM_PLL_TIMERS; i++) {
        plldt[i] = sr.plldt[i];
        pll_load[i] = sr. pll_load[i];
    }
    pll_loaded = sr.pll_loaded;
}


special_reg::special_reg(const special_reg& sr) {
    this->operator=(sr);
}

special_reg::special_reg() {
    curr_wp = 0;
    icc = 0;
    s = false;
    ps = false;
    tbr = 0x20;
    wim = 0x2;
    et = false;
    ec = false;
    ef = false;
    pil = 0;
    imp_ver = 0;
    y = 0;

    for (uint32_t i = 0; i < 32; i++) {
        if (17 == i)
            asr[i] = 0xB07;
        else
            asr[i] = 0;
    }
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        dt[i] = 0;
        dt_status[i] = UNSET;
    }
    for (uint32_t i = 0; i < NUM_PLL_TIMERS; i++) {
        plldt[i] = 0;
        pll_load[i] = 0;
    }
    pll_loaded = false;

    set_psr(0xF30000E0);
}
