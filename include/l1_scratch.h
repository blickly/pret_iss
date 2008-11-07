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

///////////////////////////////////////////////////////////////////////
/// l1_scratch
/**
 * The l1_scratch class provides a representation of a small on-chip
 * scratchpad memory. It adhers to the interface specified by
 * mem_location, but it provides additional methods for managing data in
 * terms of its address on the scratchpad.
 * The size of the scratchpad is taken from SCRATCH_SIZE in defines.h and
 * is strictly enforced.
 * For the purposes of simulation, the scratchpad allows reads addressed
 * by the main memory address. This allows the same program to be run with
 * or without scratchpad use.
 *
 * @author  Ben Lickly
 * @version $Id$
 * @Pt.ProposedRating red blickly
 * @Pt.AcceptedRating red blickly
 */
class l1_scratch : public mem_location {
public:
///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
    /** Create an empty scratchpad.
     */
    l1_scratch();

    /** Update any state that must be updated every cycle.
     * Does nothing in l1_scratch.
     */
    virtual void behavior();

/////////            Normal memory location interface.
    /** Tell if a scratchpad is stalled.
     * Due to the single-cycle latency, this scratchpad is never stalled.
     *  @param tid The thread id of the requesting thread (ignored)
     *  @param mem_addr The global address being requested
     *  @return False, since the scratchpad is never stalled.
     */
    virtual bool is_stalled(int tid, uint32_t mem_addr);

    /** Read a given (main-memory) address from the scratchpad, if available.
     * Note that in order to conform to the mem_location interface, the
     * address parameter corresponds to the global address of the data
     * (that in the main memory) rather than the address within the
     * scratchpad. An error is output when that address is not
     * in the scratchpad.
     *  @param tid The thread id of the requesting thread (ignored)
     *  @param mem_addr The global address being requested
     *  @param stalled Always returns false, since the scratchpad is never stalled.
     *  @return The word of data read at that location.
     */
    virtual uint32_t read(int tid, uint32_t mem_addr, bool& stalled);

/////////            Scratchpad management interface.
    /** Move the given data with the given global address into the given
     * location in the scratchpad.
     * If there is existing data at that scratchpad location, it will be
     * overwritten.
     *  @param mem_addr The global address being moved to the scratchpad.
     *  @param spm_addr The scratchpad location being written to.
     *  @param data The data being written to the scratchpad.
     */
    void add_addr(uint32_t mem_addr, uint32_t spm_addr, uint32_t data);

    /** Check if a given global address is contained on the scratchpad.
     *  @param mem_addr The global address to check for.
     *  @return True if the address is in the scratchpad. False, otherwise.
     */
    bool is_addr_in_spm(uint32_t mem_addr);

    /** Update the data in the scratchpad at a particular global address.
     * If the global address is not aleady in the scratchpad, this will
     * give an error.
     *  @param mem_addr The global address being written to.
     *  @param data The data being written to the scratchpad.
     */
    void update_addr(uint32_t mem_addr, uint32_t data);

    /* Debug members */
    void _dbg_print_mem_contents();
    void dbg_print_valid_addr();

///////////////////////////////////////////////////////////////////////
///                      protected methods                          ///
protected:
    /** Check if a given scratchpad address is valid on the scratchpad.
     *  @param spm_addr The scratchpad address to check.
     *  @return True if it is a valid scratchpad address. False, otherwise.
     */
    bool in_spm_range(uint32_t spm_addr);

///////////////////////////////////////////////////////////////////////
///                     protected variables                         ///
    /** Map of scratchpad addresses to main memory adresses.
     * mem_location already contains a memory unit for the l1_scratch.
     * This contains the physical address of main memory and the
     * data associated with it. We also need a list that identifies
     * which SPM addresses correspond to which main memory addresses.
     * This is that list.
     */
    uint32_t main_memory_address[SCRATCH_SIZE/4];

};

#endif /* _L1_SCRATCH_H_ */
