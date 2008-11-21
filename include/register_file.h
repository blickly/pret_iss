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

#ifndef _REGISTER_FILE_H_
#define _REGSITER_FILE_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif

#include "defines.h"
#include "instruction.h"
#include <stdint.h>

struct register_file {

    register_file();
    register_file(const register_file& rf);

    int get_reg(register_number rs, window_pointer wp);
    void set_reg(register_number rs, uint32_t value, window_pointer wp);
    void regdump(window_pointer wp);
    void reset_register_window();

    bool operator==(const register_file& rf);
    void operator=(const register_file& rf);

    // Data members
    uint32_t gReg[8];
    uint32_t wReg[16*REGISTER_WINDOWS];


};

#endif
