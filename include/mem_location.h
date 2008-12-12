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

#ifndef _MEM_LOCATION_H_
#define _MEM_LOCATION_H_

#include "cycle_counter.h"
#include "defines.h"
#include "memory_unit.h"

///////////////////////////////////////////////////////////////////////
/// mem_location
/**
 * The mem_location class is an interface that provides a way to
 * represent locations in memory that may be stalled upon reads or
 * writes.  mem_location is a pure virtual class which must be
 * implemented before it can be used.
 *
 * @author  Ben Lickly
 * @version $Id$
 * @Pt.ProposedRating red blickly
 * @Pt.AcceptedRating red blickly
 */
class mem_location {
public:
///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
    /** Return the word of memory at the given address, as well as whether
     *  that access stalls.
     *  @param tid The thread id of the requesting thread
     *  @param addr The address being requested
     *  @param stalled Whether this memory location is stalled or not.
     *  @return The word read at that address by that thread.
     */
    virtual uint32_t read(int tid, uint32_t addr, bool& stalled) = 0;

    /** Writes the given word to the given address, if possible.
     *  @param tid The thread id of the writing thread
     *  @param addr The address being written to
     *  @param data The data being written
     *  @param stalled Whether this memory location is stalled or not
     */
    virtual void write(int tid, uint32_t addr, uint32_t data, bool& stalled) = 0;

    /** Update any internal state of the memory location to take into account
     *  that one cycle has passed.
     *  Since there is no guarantee that memory accesses will be made
     *  every cycle, any behavior that should take place every cycle needs to
     *  be called here.
     */
    virtual void behavior() = 0;

    /** Deallocate storage used by this memory location.
     */
    virtual ~mem_location() {};

///////////////////////////////////////////////////////////////////////
///                      public variables                           ///
    /** The underlying memory unit can be accessed for cases such as
     *  debugging where the simultaion time and stall pattern should
     *  not be altered.
     */
    memory_unit mem;
};


#endif
