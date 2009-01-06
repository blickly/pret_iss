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

#ifndef _CYCLE_COUNTER_H_
#define _CYCLE_COUNTER_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif /* _NO_SYSTEMC_ */

#include "module_base.h"

///////////////////////////////////////////////////////////////////////
/// cycle_counter
/**
 * This class implements a simple cycle counter. It is used to ensure
 * a consistent notion of time whether using SystemC or standard C++.
 * Without it, standard C++ modules that must keep track of cycles
 * may have different behavior depending on the order in which they
 * are called.
 *
 * @author  Ben Lickly
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class cycle_counter : public module_base {
public:
    /** Construct an instance of the cycle_counter.
     */
    cycle_counter(const sc_module_name& nm);
///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
public:
    /** Increments the cycles variable every invocation of cycles.
     */
    void behavior();

    /** Returns the cycle count stored in _cycles.
     *  @return The current cycle count in _cycles.
     */
    unsigned int get_cycles() const;

private:
    /** Default constructor is hidden. Disallow its use.
     */
    cycle_counter();

///////////////////////////////////////////////////////////////////////
//                       private  variables                         ///
#ifdef _NO_SYSTEMC_
    /** Variable to count the cycles.
     */
    unsigned int _cycles;
#else
    sc_signal<unsigned int> _cycles;
#endif /* _NO_SYSTEMC_ */
};
#endif /* _CYCLE_COUNTER_H_ */
