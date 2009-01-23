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

#ifndef _HW_THREAD_CONTROLLER_H_
#define _HW_THREAD_CONTROLLER_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif /* _NO_SYSTEMC_ */

#include "defines.h"
#include "hw_thread.h"
#include "hw_thread_ptr.h"
#include "module_base.h"
#include "srec_parser.h"
#include "static_bound_parser.h"

///////////////////////////////////////////////////////////////////////
/// hw_thread_controller
/**
 * This class implements the round-robin scheduling of the hardware
 * threads. At every main clock cycle, a hardware thread is sent to
 * the fetch stage and the order is always the same. We identify each
 * thread with an identifier from 0 to 5 and the order in which
 * threads are send to the fetch stage are 0, 1, 2, 3, 4, 5, 0, 1, 2
 * ... This class is also loading each thread's scratchpad memory
 * units with the corresponding program code via the help of the
 * <i>srec_parser</i> class.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
#ifdef _NO_SYSTEMC_
class hw_thread_controller {
#else
class hw_thread_controller : public sc_module {
#endif /*_NO_SYSTEMC_ */
public:
    /** Construct a hardware thread controller object with a unique name
        identified by <i>str</i>.
     *
     * @param str Unique identifier for the hardware thread controller.
     */
    hw_thread_controller(const sc_module_name& str);

    /** Destroy the dynamically created thread objects.
     */
    ~hw_thread_controller();

///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
public:
#ifndef _NO_SYSTEMC_
    /** Implements a round-robin scheduling of the threads. There are
     *  six hardware threads with identifiers 0 through 5. Every cycle a
     *  new thread is assigned to the fetch stage in a round-robin
     *  order.
     *  This method is used only with SystemC.
     */
    void behavior();
#else
    /** Implements a round-robin scheduling of the threads. There are
     *  six hardware threads with identifiers 0 through 6. Every cycle a
     *  new thread is assigned to the fetch stage in a round-robin
     *  order.
     *  This is only used when <i>_NO_SYSTEMC_</i> flag is set
     *  during compilation.
     *
     *  @param out The hardware thread sent to the
     *  fetch stage.
     */
    void behavior(hw_thread_ptr& out);
#endif /* _NO_SYSTEMC_ */

    /** Returns a pointer to the thread with the <i>id</i>. This is
       *  mainly used in the main() function to display internals of the
       *  threads such as the instruction and cycle counts. It is purely
       *  for debugging use.
       *
       *  @param id Thread identifier.
       *  @return Pointer to the thread with the id.
       */
    hw_thread_ptr* get_thread(unsigned int id) const;

    /** Parse the SREC files specified by <i>_txt_str</i> and load it
     * into the respective scratchpad memory units.
     *
     * @param srec_parser SREC parser that is associated with each memory unit.
     * @param bound_parser Parser for reading in static bound data.
     */
    void parse_srec_files(srec_parser& parser, static_bound_parser& bound_parser);

    /** Decrements all PLL deadline registers by 1.
     */
    void pll_behavior();

    /** Sets the name of a file with a location of where the SREC files
      are stored.
      *
      * @param str The name of the file with the location of SREC files.
      */
    void set_txtname(const string& str);

#ifndef _NO_SYSTEMC_
    /** SystemC specific macro that creates a SystemC process for this class.
     */
    SC_HAS_PROCESS(hw_thread_controller);
#endif /* _NO_SYSTEMC_ */

///////////////////////////////////////////////////////////////////////
//                       public variables                           ///
public:
    /** Output port connected to the fetch stage.
     */
    sc_out< hw_thread_ptr > out;

    /** Instances of main clock (<i>clk</i>) and a phase-lock loop clock
     * (<i>pll_clock</i>) for both SystemC and no SystemC.
     */
#ifdef _NO_SYSTEMC_
    pret_clock clk;
    pret_clock pll_clock;
#else
    sc_in_clk pll_clock;
    sc_in_clk clk;
#endif /* _NO_SYSTEMC_ */

///////////////////////////////////////////////////////////////////////
//                       private methods                          ///
private:
    /** Disable the default constructor.
     */
    hw_thread_controller();

///////////////////////////////////////////////////////////////////////
//                       private variables                          ///

    /** An array of six hardware threads. These are allocated in the
     *  constructor and deleted in the destructor. <i>NUM_THREADS</i> is
     *  set in defines.h.
     */
    hw_thread_ptr* _pool[NUM_THREADS];
    /** File name of location with SREC files.
     */
    string _txt_str;
    /** Thread count used to select the thread to send to the fetch
      stage.
    */
    unsigned int _tcount;
};
#endif /* _HW_THREAD_CONTROLLER_H_ */
