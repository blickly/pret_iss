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
#ifndef _NO_SYSTEMC_FILE_H_
#define _NO_SYSTEMC_FILE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include "pret_clock.h"

#define sc_main main
///////////////////////////////////////////////////////////////////////
/// sc_module_name
/** This is a global typedef when <i>_NO_SYSTEMC_</i> flag is set.
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
typedef string sc_module_name;

///////////////////////////////////////////////////////////////////////
/// sc_module_name
/** This is a global typedef when <i>_NO_SYSTEMC_</i> flag is set for
 *  the main clock to use <i>pret_clock</i> instead of using the
 *  SystemC clock.
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
typedef pret_clock sc_clock;

///////////////////////////////////////////////////////////////////////
/// sc_interface
/**
 * This class redefines the SystemC port interface <i>sc_interface</i>
 * to compile when using the <i>_NO_SYSTEMC_</i> flag.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class sc_interface {
public:
    virtual ~sc_interface() {
    };
};

///////////////////////////////////////////////////////////////////////
/// sc_in
/**
 * This class redefines the SystemC input port class <i>sc_in</i>
 * to compile when using the <i>_NO_SYSTEMC_</i> flag.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
template <typename T>
class sc_in {
///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
public:
    /** Read an input from the input port.
     *
     * @return T Input port.
    */
    T read()  {
        return _object;
    }
///////////////////////////////////////////////////////////////////////
///                      private variables                          ///
private:
    T _object;
};

///////////////////////////////////////////////////////////////////////
/// sc_out
/**
 * This class redefines the SystemC input port class <i>sc_out</i>
 * to compile when using the <i>_NO_SYSTEMC_</i> flag.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
template <typename T>
class sc_out {
public:
///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
    /** Write the output value.
     *
     * @param output Output value
     */
    void write(const T& output) {
        _object = output;
    }
///////////////////////////////////////////////////////////////////////
///                      private variables                          ///
private:
    T _object;
};

///////////////////////////////////////////////////////////////////////
/// sc_port
/**
 * This class redefines the SystemC base port class <i>sc_port</i>
 * to compile when using the <i>_NO_SYSTEMC_</i> flag.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
template <typename T>
class sc_port {
public:
///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
    /** Set the port object.
     *
     * @param port Port object to be set.
     */
    void operator()(T& port) {
        _object = &port;
    }

    /** Return the port by using the -> syntax.
     *
     * @return Pointer to the port.
     */
    T* operator->() {
        return _object;
    }
///////////////////////////////////////////////////////////////////////
///                      private variables                             ///
private:
    T* _object;
};

class sc_module {
};
#endif /* _NO_SYSTEMC_FILE_H_ */
