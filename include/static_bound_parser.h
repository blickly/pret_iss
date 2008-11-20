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

  $Author: blickly $
  $Date:$
  $Id:$

*/

#ifndef _STATIC_BOUND_PARSER_H_
#define _STATIC_BOUND_PARSER_H_

#include "instruction.h"
#include <stdint.h>
#include <vector>

using namespace std;
struct memory_controller;

///////////////////////////////////////////////////////////////////////
/// static_bound_parser
/**
 * For static DMA the addresses held in a scratchpad are constant, and
 * these constants are specified in an external file.
 * The static_bound_parser reads these bounds from those files.
 *
 * @author  Ben Lickly, Hiren Paterl
 * @version $Id:$
 * @Pt.ProposedRating red ben
 * @Pt.AcceptedRating
 */

class static_bound_parser {

public:
    static_bound_parser();
    void set_memory_controller(memory_controller* memc);
    void load_data_spm(const unsigned int tid, const string& dspm);
    void load_spm(unsigned int tid, const string& spm);
    void load_inst_spm(unsigned int tid, const string& ispm);
private:
    memory_controller* _mem_cont;
    uint32_t spm_addr;

};
#endif
