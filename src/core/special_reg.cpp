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

void special_reg::set_psr(uint32_t from_register) {

    // Store the implementation and version identifier (impl,ver)
    _imp_ver = (from_register & 0xFF000000) >> 24;
    // Skip out the version identifier (ver)
    // Store the icc
    _icc = (from_register & 0x00F00000) >> 20;
    // Skip out the reserved identifier (reserved)
    // Store the enable co-processor identifier (ec)
    _ec = (from_register & 0x00002000) >> 13;
    // Store the enable FPU identifier (ef)
    _ef = (from_register & 0x00001000) >> 12;
    // Store the processor interrupt level (pil)
    _pil = (from_register & 0x00000F00) >> 8;
    // Store the supervisor mode bit (s)
    _s = (from_register & 0x00000080) >> 7;
    // Store the supervisor mode at trap (ps)
    _ps = (from_register & 0x00000040) >> 6;
    // Store the enable traps identifier (et)
    _et = (from_register & 0x00000020) >> 5;
    // Store the current window pointer (cwp)
    _curr_wp = from_register & 0x0000001F;
}

uint32_t special_reg::get_psr() {
    uint32_t psr = 0x0;

    psr = ((uint32_t)(_imp_ver) << 24) |
          ((uint32_t)(_icc) << 20) |
          ((uint32_t)(_ec) << 13) |
          ((uint32_t)(_ef) << 12) |
          ((uint32_t)(_pil) << 8) |
          ((uint32_t)(_s) << 7) |
          ((uint32_t)(_ps) << 6) |
          ((uint32_t)(_et) << 5) |
          ((uint32_t)(_curr_wp));

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
            cout << setfill('0') << hex << setw(8) << _dt[i] << "       ";
        cout << endl;
    }
    cout << endl;

    cout << "pll timers: " << endl;;

    for (uint32_t i = 0; i < (NUM_PLL_TIMERS); i++)
        cout << setfill(' ') << "\t    " << setw(3) << i << "  ";
    cout << endl;
    cout << "             ";
    for (uint32_t i = 0; i < (NUM_PLL_TIMERS); i++)
        cout << setfill('0') << hex << setw(8) << _plldt[i] << "       ";
    cout << endl;
    cout << "             ";
    for (uint32_t i = 0; i < (NUM_PLL_TIMERS); i++)
        cout << setfill('0') << hex << setw(8) << _pll_load[i] << "       ";
    cout << endl;

}

bool special_reg::missed_deadline(uint32_t& deadline_register_num) {
    bool missed = false;
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        if (_dt_status[i] == MISSED) {//((dt[i] < 0) && (dt_status[i] == SET)) {
            //            dt_status[i] = MISSED;
            missed = true;
            deadline_register_num = i;
            _dt_status[i] = UNSET;
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


        _dt[i]--;
        if ((_dt[i] - 1 < 0) && (_dt_status[i] == SET)) {
            _dt_status[i] = MISSED;
        }
    }
}

void special_reg::decrement_pll_timers() {
    for (uint32_t i = 0; i < NUM_PLL_TIMERS; i++) {
        if (_plldt[i] > 0) {
            _plldt[i] --;
        } else if (_pll_load[i] != 0) {
            _plldt[i] = --_pll_load[i];
            _pll_load[i] = 0;
        }
    }
}

void special_reg::dump() {
    //cout << "icc \t ec \t ef \t pil \t s \t ps \t et \t curr_wp" << endl;
    //  cout << hex << (uint32_t)(icc) <<"\t" << ec << "\t" << ef << "\t" << pil << "\t"  << s << "\t" << ps << "\t" << et << "\t" << curr_wp << endl;
    cout << uppercase << setfill('0') << hex << "psr: " << setw(8) << get_psr() << "\t wim: " << setw(8) << _wim
         << "\t tbr: " << setw(8) << _tbr << "\t y: " << setw(8) << _y << endl;
    cout << endl;

}

bool special_reg::operator==(const special_reg& is_register) {

    bool flag = false;
    if (_curr_wp == is_register._curr_wp && _icc == is_register._icc       &&
            _s == is_register._s &&  _ps == is_register._ps &&   _tbr == is_register._tbr &&
            _wim == is_register._wim && _et == is_register._et &&  _ec == is_register._ec &&
            _ef == is_register._ef &&  _pil == is_register._pil && _y == is_register._y &&
            _pll_loaded == is_register._pll_loaded)
        flag = true;

    for (uint32_t i = 0; i < 32; i++) {
        if (_asr[i] != is_register._asr[i])
            flag = false;
    }
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        if (_dt[i] != is_register._dt[i])
            flag = false;
    }
    for (uint32_t i = 0; i < NUM_PLL_TIMERS; i++) {
        if (_plldt[i] != is_register._plldt[i] ||  _pll_load[i] != is_register. _pll_load[i])
            flag = false;
    }


    return flag;
}

void special_reg::operator=(const special_reg & from_register) {
    _curr_wp = from_register._curr_wp;
    _icc = from_register._icc;
    _s = from_register._s;
    _ps = from_register._ps;
    _tbr = from_register._tbr;
    _wim = from_register._wim;
    _et = from_register._et;
    _ec = from_register._ec;
    _ef = from_register._ef;
    _pil = from_register._pil;
    _imp_ver = from_register._imp_ver;
    _y = from_register._y;
    for (uint32_t i = 0; i < 32; i++) {
        _asr[i] = from_register._asr[i];
    }
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        _dt[i] = from_register._dt[i];
        _dt_status[i] = from_register._dt_status[i];
    }
    for (uint32_t i = 0; i < NUM_PLL_TIMERS; i++) {
        _plldt[i] = from_register._plldt[i];
        _pll_load[i] = from_register. _pll_load[i];
    }
    _pll_loaded = from_register._pll_loaded;
}


special_reg::special_reg(const special_reg& initial_register) {
    this->operator=(initial_register);
}

special_reg::special_reg() {
    _curr_wp = 0;
    _icc = 0;
    _s = false;
    _ps = false;
    _tbr = 0x20;
    _wim = 0x2;
    _et = false;
    _ec = false;
    _ef = false;
    _pil = 0;
    _imp_ver = 0;
    _y = 0;

    for (uint32_t i = 0; i < 32; i++) {
        if (17 == i)
            _asr[i] = 0xB07;
        else
            _asr[i] = 0;
    }
    for (uint32_t i = 0; i < NUM_DEADLINE_TIMERS; i++) {
        _dt[i] = 0;
        _dt_status[i] = UNSET;
    }
    for (uint32_t i = 0; i < NUM_PLL_TIMERS; i++) {
        _plldt[i] = 0;
        _pll_load[i] = 0;
    }
    _pll_loaded = false;

    set_psr(0xF30000E0);
}

void special_reg::set_imp_ver(unsigned char imp_ver) {
    _imp_ver = imp_ver;
}


void special_reg::set_curr_wp(WINDOW_POINTER curr_wp) {
    _curr_wp = curr_wp;
}


void special_reg::set_icc(unsigned char icc) {
    _icc = icc;
}

void special_reg::set_s(bool s) {
    _s = s;
}

void special_reg::set_ps(bool ps) {
    _ps = ps;
}

void special_reg::set_tbr(uint32_t tbr) {
    _tbr = tbr;
}


void special_reg::set_wim(uint32_t wim) {
    _wim = wim;
}

void special_reg::set_et(bool et) {
    _et = et;
}


void special_reg::set_ec(bool ec) {
    _ec = ec;
}

void special_reg::set_ef(bool ef) {
    _ef = ef;
}

void special_reg::set_pil(unsigned char pil) {
    _pil = pil;
}

void special_reg::set_asr(uint32_t asr_value, int asr_index) {
    _asr[asr_index] = asr_value;
}

void special_reg::set_y(uint32_t y) {
    _y = y;
}

void special_reg::set_dt(int32_t dt_value, int dt_index) {
    _dt[dt_index] = dt_value;
}


void special_reg::set_plldt(uint32_t plldt_value, int plldt_index) {
    _plldt[plldt_index] = plldt_value;
}


void special_reg::set_pll_load(uint32_t pll_load_value, int pll_load_index) {
    _pll_load[pll_load_index] = pll_load_value;
}

void special_reg::set_pll_loaded(bool pll_loaded) {
    _pll_loaded = pll_loaded;
}


void special_reg::set_dt_status(DT_STATUS dt_status_value, int dt_status_index) {
    _dt_status[dt_status_index] = dt_status_value;
}


unsigned char special_reg::get_imp_ver() {
    return _imp_ver;
}


WINDOW_POINTER special_reg::get_curr_wp() {
    return _curr_wp;
}


unsigned char special_reg::get_icc() {
    return _icc;
}

bool special_reg::get_s() {
    return _s;
}


bool special_reg::get_ps() {
    return _ps;
}

uint32_t special_reg::get_tbr() {
    return _tbr;
}


uint32_t special_reg::get_wim() {
    return _wim;
}

bool special_reg::get_et() {
    return _et;
}

bool special_reg::get_ec() {
    return _ec;
}

bool special_reg::get_ef() {
    return _ef;
}


unsigned char special_reg::get_pil() {
    return _pil;
}


uint32_t special_reg::get_asr(int asr_index) {
    return _asr[asr_index];
}

uint32_t special_reg::get_y() {
    return _y;
}


int32_t special_reg::get_dt(int dt_index) {
    return _dt[dt_index];
}


uint32_t special_reg::get_plldt(int plldt_index) {
    return _plldt[plldt_index];
}


uint32_t special_reg::get_pll_load(int pll_load_index) {
    return _pll_load[pll_load_index];
}

bool special_reg::get_pll_loaded() {
    return _pll_loaded;
}


DT_STATUS special_reg::get_dt_status(int dt_status_index) {
    return _dt_status[dt_status_index];
}

