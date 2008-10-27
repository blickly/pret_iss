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
*/
#ifndef _DMA_IF_H_
#define _DMA_IF_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc>
#endif /* _NO_SYSTEMC_ */

#include "defines.h"
#include "hw_thread_ptr.h"

enum transfer_state_t {
    IDLE,
    COMPLETE,
    BUSY,
    SCHEDULED
};

///////////////////////////////////////////////////////////////////////
/// dma_if
/**
 * This is a coprocessor DMA component. The hardware blocking DMA
 * interface methods are directly invoked from the core. Instructions
 * for the coprocessor that invoke these interface methods are STC
 * (store coprocessor) at a specific register location defining the
 * main memory starting address and STC at a specific register
 * location for the scratchpad memory target.
 *
 * @author  Hiren Patel
 * @version $Id: dma.h 816 2008-07-30 00:18:11Z hiren $
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class dma_if : public sc_interface {
public:
    virtual bool is_transfer_complete(const int& thread_id) = 0;
    virtual bool is_transfer_idle(const int& thread_id) = 0;
    virtual void set_mem_source(const unsigned int& thread_id, const uint32_t& source) = 0;
    virtual void set_spm_target(const unsigned int& thread_id, const uint32_t& target) = 0;
    virtual void set_transfer_state(const unsigned int& thread_id, const transfer_state_t& transfer_state) = 0;
    virtual void make_transfer(const hw_thread_ptr& thread_address) = 0;
    virtual unsigned int get_mem_source(const unsigned int& thread_id) const = 0;
    virtual unsigned int get_spm_target(const unsigned int& thread_id) const = 0;

};
#endif /* _DMA_IF_H_ */
