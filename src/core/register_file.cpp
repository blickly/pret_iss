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


#include "register_file.h"

void register_file::operator=(const register_file & rf) {

    for (uint32_t i = 0; i < 16*REGISTER_WINDOWS; i++) {
        wReg[i] = rf.wReg[i];
    }

    for (uint32_t i = 0; i < 8; i++) {
        gReg[i] = rf.gReg[i];
    }
}

bool register_file::operator==(const register_file& rf) {

    for (uint32_t i = 0; i < 16*REGISTER_WINDOWS; i++) {
        if (wReg[i] != rf.wReg[i])
            return false;
    }

    for (uint32_t i = 0; i < 8; i ++) {
        if (gReg[i] != rf.gReg[i])
            return false;
    }

    return true;
}

int register_file::get_reg(REGISTER_NUMBER rs, WINDOW_POINTER wp) {
    if (rs < 8) {
        return gReg[rs];
    } else {
        unsigned short window_addr = wp * 16;
        unsigned short window_offset = rs - 8;
        unsigned short window_size = 16 * REGISTER_WINDOWS;
        return wReg[(window_addr + window_offset) % window_size];
    }
}

void register_file::set_reg(REGISTER_NUMBER rs, uint32_t value, WINDOW_POINTER wp) {
    if (rs < 8) {
        gReg[rs] = value;
    } else {
        unsigned short window_addr = wp * 16;
        unsigned short window_offset = rs - 8;
        unsigned short window_size = 16 * REGISTER_WINDOWS;
        wReg[(window_addr + window_offset) % window_size] = value;
    }
}


void register_file::regdump(WINDOW_POINTER wp) {
    cout << "-----------------------------------------------------" << endl;
#ifndef _NO_SYSTEMC_
    cout << "                     RegDump: " << sc_time_stamp() << endl;
#endif
    cout << "-----------------------------------------------------" << endl;
    cout << setfill(' ') << "       \t  INS" << setw(4) << " " << "    LOCALS"
         <<  setw(4) << " " << "   OUTS" << setw(4) << " " << " GLOBALS" << endl;
    for (uint32_t i = 0; i < 8; i++) {
        cout <<  uppercase << setfill(' ') << setw(8) << i << ":" << " ";
        cout <<  uppercase << setfill('0') << setw(8) << hex << get_reg(i + 24, wp) << "   "; //ins
        cout <<  uppercase << setfill('0') << setw(8) << hex << get_reg(i + 16, wp) << "   "; //locals
        cout <<  uppercase << setfill('0') << setw(8) << hex << get_reg(i + 8, wp) <<  "   "; // outs
        cout <<  uppercase << setfill('0') << setw(8) << hex << gReg[i]; // globals
        cout << endl;
    }

    cout << "\nwindow ptr: " << dec << wp;
    //    cout << "g0 = " << hex << gReg[0] << ", g1 = " << gReg[1] << ", g2 = "<< gReg[2];
    //   cout << ", g3 = " << gReg[3]  << ", g4 = "<< gReg[4] << ", g5 = "<< gReg[5];
    //   cout << ", g6 = "<< gReg[6]  << ", g7 = "<< gReg[7] << endl;
    //   cout << "Window Pointer: " << dec << wp << endl;
    //   cout << "(8)out0 = " << hex << get_reg(8, wp) << ", (9)out1 = " << get_reg(9, wp) << ", (10)out2 = "<< get_reg(10, wp);
    //   cout << ", (11)out3 = " << get_reg(11, wp)  << endl<< "(12)out4 = "<< get_reg(12, wp) << ", (13)out5 = "<< get_reg(13, wp);
    //   cout << ", (14)out6 = "<< get_reg(14, wp)  << ", (15)out7 = "<< get_reg(15, wp) << endl;
    //   cout << "(16)local0 = " << hex << get_reg(16, wp) << ", (17)local1 = " << get_reg(17, wp) << ", (18)local2 = "<< get_reg(18, wp);
    //   cout << ", (19)local3 = " << get_reg(19, wp)  << endl << "(20)local4 = "<< get_reg(20, wp) << ", (21)local5 = "<< get_reg(21, wp);
    //   cout << ", (22)local6 = "<< get_reg(22, wp)  << ", (23)local7 = "<< get_reg(23, wp) << endl;
    //   cout << "(24)in0 = " << hex << get_reg(24, wp) << ", (25)in1 = " << get_reg(25, wp) << ", (26)in2 = "<< get_reg(26, wp);
    //   cout << ", (27)in3 = " << get_reg(27, wp)  << endl << "(28)in4 = "<< get_reg(28, wp) << ", (29)in5 = "<< get_reg(29, wp);
    //   cout << ", (30)in6 = "<< get_reg(30, wp)  << ", (31)in7 = "<< get_reg(31, wp) << endl;
    //   cout << "-----------------------------------------------------" << endl;
    cout << endl;
}

// void register_file::regdump(WINDOW_POINTER wp){
//   cout << "-----------------------------------------------------" << endl;
//   cout << "                     RegDump: " << sc_time_stamp() << endl;
//   cout << "-----------------------------------------------------" << endl;
//   cout << "g0 = " << hex << gReg[0] << ", g1 = " << gReg[1] << ", g2 = "<< gReg[2];
//   cout << ", g3 = " << gReg[3]  << ", g4 = "<< gReg[4] << ", g5 = "<< gReg[5];
//   cout << ", g6 = "<< gReg[6]  << ", g7 = "<< gReg[7] << endl;
//   cout << "Window Pointer: " << dec << wp << endl;
//   cout << "(8)out0 = " << hex << get_reg(8, wp) << ", (9)out1 = " << get_reg(9, wp) << ", (10)out2 = "<< get_reg(10, wp);
//   cout << ", (11)out3 = " << get_reg(11, wp)  << endl<< "(12)out4 = "<< get_reg(12, wp) << ", (13)out5 = "<< get_reg(13, wp);
//   cout << ", (14)out6 = "<< get_reg(14, wp)  << ", (15)out7 = "<< get_reg(15, wp) << endl;
//   cout << "(16)local0 = " << hex << get_reg(16, wp) << ", (17)local1 = " << get_reg(17, wp) << ", (18)local2 = "<< get_reg(18, wp);
//   cout << ", (19)local3 = " << get_reg(19, wp)  << endl << "(20)local4 = "<< get_reg(20, wp) << ", (21)local5 = "<< get_reg(21, wp);
//   cout << ", (22)local6 = "<< get_reg(22, wp)  << ", (23)local7 = "<< get_reg(23, wp) << endl;
//   cout << "(24)in0 = " << hex << get_reg(24, wp) << ", (25)in1 = " << get_reg(25, wp) << ", (26)in2 = "<< get_reg(26, wp);
//   cout << ", (27)in3 = " << get_reg(27, wp)  << endl << "(28)in4 = "<< get_reg(28, wp) << ", (29)in5 = "<< get_reg(29, wp);
//   cout << ", (30)in6 = "<< get_reg(30, wp)  << ", (31)in7 = "<< get_reg(31, wp) << endl;
//   cout << "-----------------------------------------------------" << endl;
//   cout << endl;
// }

void register_file::reset_register_window() {
    for (uint32_t i = 0; i < 16*REGISTER_WINDOWS; i++) {
        wReg[i] = 0;
    }

}

register_file::register_file(const register_file& rf) {
    this->operator=(rf);
}

register_file::register_file() {
    for (uint32_t i = 0; i < 8; i++) {
        gReg[i] = 0;
    }
}
