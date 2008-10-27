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

#ifndef _L1_SCRATCH_H_
#define _L1_SCRATCH_H_

#include "defines.h"
#include "memory_unit.h"
#include "cycle_counter.h"
#include "mem_location.h"
#include <map>

class l1_scratch : public mem_location {
public:
    void add_addr(uint32_t mem_addr, uint32_t spm_addr, uint32_t inst);
    virtual void behavior();
    virtual bool is_stalled(int tid, uint32_t addr);
    virtual uint32_t read(int tid, uint32_t addr, bool& stalled);
    void remove_addr(uint32_t rm);

    bool is_addr_in_spm(uint32_t mmem_addr);
    map<uint32_t, uint32_t>::iterator find_spm_addr(uint32_t spm_addr);

    void update_addr(uint32_t mem_addr, uint32_t inst) ;

    /* Debug members */
    void _dbg_print_mem_contents();
    void dbg_print_valid_addr();

protected:
    /* mem_location contains the memory unit for the l1_scratch. This
       structure contains the physical address of main memory and the
       data associated with it (instruction). We need to add another
       list that identifies which physical address is valid in the SPM. */

    /* Addresses valid in the scratchpad. Indexed by virtual address
       (Main Mem) */
    map< uint32_t, uint32_t > valid_addr;

};

#endif /* _L1_SCRATCH_H_ */
