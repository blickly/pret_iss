/*
  Copyright (c) 2007-2009 The Regents of the University of California.
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
*/
#ifndef _DMA_H_
#define _DMA_H_

#include "defines.h"

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc>
#endif /* _NO_SYSTEMC_ */

#include "module_base.h"
#include "hw_thread_ptr.h"
#include "wheeled_mem.h"
#include "dma_if.h"
using namespace std;

///////////////////////////////////////////////////////////////////////
/// dma
/**
 * This is the hardware DMA coprocessor. It implements a blocking DMA
 * transfer for instructions.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class dma : public module_base, public dma_if  {
public:
    /** Construct a direct memory access coprocessor.
     *
     * @param name Identifier for the component.
     */
    dma(const sc_module_name& name);

    /** Destroy the object.
     */
    virtual ~dma();

    /** Perform the pending memory transfer.  For every thread, check if
     *  there is a pending transfer. If there is one, then attempt to
     *  make the transfer. Not ethat it may happen that a transfer is
     *  delayed because accesses to the main memory occur through the
     *  memory wheel. If the transfer is stalled, then it is retried
     *  during the next cycle. This continues until the transfer
     *  completes. In the mean time, the thread making the transfer is
     *  blocked at the transfer.
     *
     */
    void behavior();

    /** Check if the transfer state has been changed to complete.
     *
     *  @param thread_id Thread identifier.
     *  @return True if the transfer has completed.
     */
    bool is_transfer_complete(const int& thread_id);

    /** Check if the transfer state is in the idle state.
     *
     *  @param thread_id Thread identifier.
     *  @return True if the transfer is in idle state.
     */
    bool is_transfer_idle(const int& thread_id);

    /** Get the source address from where the transfer will occur.
     *  TODO: Currently, this is only from the main memory. Ideally for
     *  data it should support transfer from the scratchpad as well.
     *
     *  @param thread_id Thread identifier.
     *  @return Address from where the transfer will take place.
     */
    unsigned int get_mem_source(const unsigned int& thread_id) const;

    /** Get the target address.
     *  TODO: Currently, this is only to the scratchpads. Ideally for
     *  data it should support transfer to the main memory as well.
     *
     *  @param thread_id Thread identifier.
     *  @return Target address.
     */
    unsigned int get_spm_target(const unsigned int& thread_id) const;

    /** Register the memory wheel address within the DMA.
     *
     * @param wheel Memory wheel.
     */
    void register_memory_wheel(const wheeled_mem& wheel);

    /** Register instruction scratchpads with DMA.
     *
     * @param spm[] Array of instruction scratchpads.
     */
    void register_inst_spm(l1_scratch* spm[]);

    /** Set the transfer source register.
     *
     * @param thread_id Thread identifier.
     * @param source Address from where the transfer will start.
     */
    void set_mem_source(const unsigned int& thread_id, const uint32_t& source);

    /** Set the transfer target register.
     *
     * @param thread_id Thread identifier.
     * @param target Target address.
     */
    void set_spm_target(const unsigned int& thread_id, const uint32_t& target);

    /** Set the transfer state.
     *
     * @param thread_id Thread identifier.
     * @param transfer_state The resulting transfer state.
     */
    void set_transfer_state(const unsigned int& thread_id, const transfer_state_t& transfer_state);

    /** Perform the DMA transfer.
     *
     * @param thread_address Thread address.
     */
    void make_transfer(const hw_thread_ptr& thread_address);
private:

    /** Default constructor disabled.
     */
    dma();

    /** Initialize the private data members of the DMA component.
     */
    void _initialize();
private:
    /** Registers: source and target.
     *  TODO: Currently it only supports instructions.
     */
    uint32_t _mem_source_reg[NUM_THREADS];
    uint32_t _spm_target_reg[NUM_THREADS];
    /** Transfer state.
     */
    transfer_state_t _transfer_complete[NUM_THREADS];

    /** Wheeled memory and instruction scratchpad addresses.
     */
    wheeled_mem* _mem_wheel;
    l1_scratch* _inst_spm[NUM_THREADS];
    /** Instructions received and to be transfered.
     */
    vector<uint32_t> _received_data[NUM_THREADS];

};
#endif /* _DMA_H_ */
