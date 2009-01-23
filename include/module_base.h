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

#ifndef _MODULE_BASE_H_
#define _MODULE_BASE_H_

/* This must be before the _NO_SYSTEMC_. This is because it defines
   _USE_SYSTEMC_. */
#include "defines.h"

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif /* _NO_SYSTEMC_ */

#ifndef _NO_SYSTEMC_
///////////////////////////////////////////////////////////////////////
/// SystemC module_base
/**
 * This is a base class for all classes that are sensitive to the main
 * clock. These classes will have a SystemC process (METHOD) enabled
 * within them.
 *
 * @author  Hiren Patel
 * $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class module_base: public sc_module {
public:
    /** Construct a SystemC SC_METHOD process.
     */
    module_base();

    /** Construct a SystemC SC_METHOD process with a unique name.
     *  @param str Unique string identifying the process.
     */
    module_base(const sc_module_name& name);
    /** Destroy the objects correctly if the mapped to the base class and destroyed.
     */
    virtual ~module_base();

///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
    /** Implements the behavior specific to the process.
     */
    virtual void behavior();

    /** SystemC MACRO to instantiate a process.
     */
    SC_HAS_PROCESS(module_base);

///////////////////////////////////////////////////////////////////////
//                       public variables                           ///
public:
    /** SystemC main clock.
     */
    sc_in_clk clk;
};
#else
///////////////////////////////////////////////////////////////////////
/// C++ module_base
/**
 * This is a dummy base class to avoid code change overhead when
 * switching between C++ and SystemC.
 *
 * @author  Hiren Patel
 * $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class module_base {
public:
    /** Construct a SystemC SC_METHOD process.
     */
    module_base();

    /** Construct a SystemC SC_METHOD process with a unique name.
     *  @param str Unique string identifying the process.
     */
    module_base(const string& name);

    /** Destroy the objects correctly if the mapped to the base class and destroyed.
     */
    virtual ~module_base();

///////////////////////////////////////////////////////////////////////
//                       private variables                           ///
private:
    /** Unique string identifier for the classes.
     */
    string _name;
};
#endif /* _NO_SYSTEMC_ */
#endif /* _MODULE_BASE_H_ */
