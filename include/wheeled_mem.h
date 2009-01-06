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

#ifndef _WHEELED_MEM_H_
#define _WHEELED_MEM_H_

#include "defines.h"
#include "memory_unit.h"
#include "cycle_counter.h"
#include "mem_location.h"
#include <vector>
#include <stdint.h>

enum AccessorType { BURST, GLOBAL };

///////////////////////////////////////////////////////////////////////
/// wheeled_mem
/**
 * The wheeled memory class provides a time triggered interface to memory.
 * It uses a simple round-robin schedule to arbitrate among multiple threads.
 * Each thread is allocated a certain amount of time, called its memory
 * window, in which it must make its access.  If a thread tries to make an
 * access when it is not its turn, that thread must stall until the start of
 * its next memory window.
 *
 * @author  Ben Lickly
 * @version $Id$
 * @Pt.ProposedRating red blickly
 * @Pt.AcceptedRating
 */
class wheeled_mem : public mem_location {
public:
///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
    /** Create a new wheeled memory unit with the given delays.
     *  The latency delay is how many cycles it takes to get the first word,
     *  and the throughput delay is how long it takes to get subsequent words
     *  when in burst mode.  The memory window parameter tells the length of
     *  a single thread's memory window.
     *  @param latency_delay Cycles to get first word.
     *  @param throughput_delay Cycles to get streaming words in burst mode.
     *  @param cyc Pointer to core cycle counter.
     *  @param mem_win Cycles of the memory window of a single thread.
     */
    wheeled_mem(int latency_delay, int throughput_delay, cycle_counter* cyc, int mem_win);

    /** Create a new wheeled memory unit with the given delays.
     *  The latency delay is how many cycles it takes to get the first word,
     *  and the throughput delay is how long it takes to get subsequent words
     *  when in burst mode.
     *  Since this constructor does not specify a memory window size,
     *  the minimal size that can accomodate the longest burst access is assumed.
     *  @param latency_delay Cycles to get first word.
     *  @param throughput_delay Cycles to get streaming words in burst mode.
     *  @param cyc Pointer to core cycle counter.
     */
    wheeled_mem(int latency_delay, int throughput_delay, cycle_counter* cyc);

    /** Return the word of data at the given address, as well as whether
     *  that access stalls.
     *  @param tid The thread id of the requesting thread
     *  @param addr The address being requested
     *  @param stalled Whether this memory location is stalled or not.
     *  @return The word read at that address by that thread.
     */
    virtual uint32_t read(int tid, uint32_t addr, bool& stalled);

    /** Writes the given word to the given address, if possible.
     *  @param tid The thread id of the writing thread
     *  @param addr The address being written to
     *  @param data The data being written
     *  @param stalled Whether this memory location is stalled or not.
     */
    virtual void write(int tid, uint32_t addr, uint32_t data, bool& stalled);

    /** Return a burst of words from memory starting at the given address,
     *  as well as whether that access stalls.
     *  @param tid The thread id of the requesting thread
     *  @param addr The address being requested
     *  @param stalled Whether this memory location is stalled or not.
     *  @param num_words The number of words in the burst.
     *  @return The sequence of words starting at that address.
     */
    virtual vector<uint32_t> read_burst(int tid,
                                        uint32_t addr,
                                        bool& stalled,
                                        int num_words);

    /** Return the number of words that have been successfully transferred
     *  in an ongoing burst transfer.
     *  @param tid The thread id of the requesting thread
     *  @param num_words The total number of words in the burst.
     *  @return The number of words successfully transferred.
     */
    virtual int burst_words_transferred(int tid, int num_words);

    /** Update internal state of the memory wheel to take into account a
     *  single cycle passing.
     */
    virtual void behavior();

protected:
///////////////////////////////////////////////////////////////////////
//                      protected methods                           ///
    /** Determine if the memory wheel is stalled on a single access
     *  for the given thread and address.
     *  @param tid The thread id of the requesting thread.
     *  @param addr The address being requested.
     *  @return True if the memory wheel is stalled.  Otherwise false.
     */
    virtual bool is_stalled(int tid, uint32_t addr);

    /** Determine if the memory wheel is stalled on a burst access
     *  for the given thread and address.
     *  @param tid The thread id of the requesting thread.
     *  @param addr The address being requested.
     *  @param num_wrods The number of words requested.
     *  @return True if the memory wheel is stalled.  Otherwise false.
     */
    bool is_stalled_burst(int tid, uint32_t addr, int num_words);

    /** Determine if the memory wheel is stalled for the given
     *  thread, address, and access type.
     *  @param acc The type of wheel access.
     *  @param tid The thread id of the requesting thread.
     *  @param addr The address being requested.
     *  @param num_words The number of words requested.
     *  @return True if the memory wheel is stalled.  Otherwise false.
     */
    bool is_stalled_helper(AccessorType acc, int tid, uint32_t addr, int num_words);

    /** Return the thread ID of the thread whose memory window is currently
     *  active.
     *  @return The thread id of the requesting thread.
     */
    int current_thread();

    /** Determine whether the given accessor and thread are allowed to
     *  start an acess or not.
     *  @param acc The type of wheel access.
     *  @param tid The thread id of the requesting thread.
     *  @return True, if the thread has time to make the requested access
     *      within its window. False, otherwise.
     */
    virtual bool is_scheduled(AccessorType acc, int tid);


    /** Return the total delay of the given access method.
     *  @param acc The type of wheel access.
     *  @param num_words The number of words requested.
     *  @return The number of cycles the given access would take.
     */
    virtual int get_delay(AccessorType acc, int num_words);

private:
///////////////////////////////////////////////////////////////////////
//                       private variables                          ///
    /** Pointer to the cycle counter of this core.
     */
    cycle_counter* counter;

    /** Boolean is true if a thread is currently making a memory access
     *  through the wheel.
     */
    bool accessing;

    /** The number of cycles remaining in the current memory access.
     */
    int remaining_cycles;

    /** The thread ID of the thread that is making the current memory access.
     */
    int accessor_tid;

    /** Lantency in cycles to get a access a single word,
     *  or the first word of a burst transfer.
     */
    const int lat_d;

    /** Cycles needed to get subsequent words after the first during a
     *  burst transfer.
     */
    const int throughput_d;

    /** Cycles in the memory window of a single thread.
     */
    const int MEMORY_WINDOW;
};

#endif
