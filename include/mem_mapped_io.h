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

  $Id: mem_mapped_io.h 1160 2008-11-07 03:37:06Z blickly $

*/

#ifndef _MEM_MAPPED_IO_H_
#define _MEM_MAPPED_IO_H_

#include "defines.h"
#include "memory_unit.h"
#include "cycle_counter.h"
#include "mem_location.h"
#include <map>

///////////////////////////////////////////////////////////////////////
/// mem_mapped_io
/**
 * The mem_mapped_io class provides a representation of the memory-mapped
 * I/O of the chip. Currently, this only includes interfaces for writing
 * to memory-mapped output addresses of
 *
 * @author  Ben Lickly
 * @version $Id:$
 * @Pt.ProposedRating red blickly
 * @Pt.AcceptedRating red blickly
 */
class mem_mapped_io : public mem_location {
public:
///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
    /** Construct and initialize a memory-mapped
     * This method is responsible for initializing any memory-mapped
     * addresses that must be set before program execution.
     */
    mem_mapped_io();

    /** Update any state that must be updated every cycle.
     * Does nothing in mem_mapped_io.
     */
    virtual void behavior();

    /** Read a given memory-mapped address, if available.
     *  @param tid The thread id of the requesting thread
     *  @param mem_addr The memory-mapped address being read
     *  @param stalled Whether this operation stalls
     *  @return The word of data read at that location.
     */
    virtual uint32_t read(int tid, uint32_t addr, bool& stalled);

    /** Writes the given word to the given memory-mapped address.
     *  @param tid The thread id of the writing thread
     *  @param addr The memory-mapped address being written to
     *  @param data The data being written
     *  @param stalled Whether this operation stalles
     */
    virtual void write(int tid, uint32_t addr, uint32_t data, bool& stalled);

protected:
///////////////////////////////////////////////////////////////////////
///                      protected methods                          ///

private:
///////////////////////////////////////////////////////////////////////
///                      private methods                            ///
    /** Write the given data to the file on our simulation platform.
     *  @param data Word of data to write.
     */
    void out_file(uint32_t data);

    /** Write the given data to standard output on our simulation platform.
     *  @param data Word of data to write.
     */
    void out_uart(uint32_t data);
    
    /** Tell if a memory-mapped IO location is stalled.
     *  Due to the single-cycle latency, this is never stalled.
     *  @param tid The thread id of the requesting thread (ignored)
     *  @param mem_addr The global address being requested
     *  @return True if the IO location is stalled. False, otherwise.
     */
    virtual bool is_stalled(int tid, uint32_t mem_addr);

};

#endif /* _MEM_MAPPED_IO_H_ */
