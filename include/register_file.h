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

#ifndef _REGISTER_FILE_H_
#define _REGSITER_FILE_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif

#include "defines.h"
#include "instruction.h"
#include <stdint.h>

///////////////////////////////////////////////////////////////////////
/// register_file
/**
 * The register_file class provides a representation of the processor 
 * registers. It includes the global registers and the window registers.
 *
 * @author  Isaac Liu
 * @version $Id$
 * @Pt.ProposedRating red blickly
 * @Pt.AcceptedRating
 */
class register_file {

///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
 public:
  /** Create the register file 
   */
  register_file();
  
  /** Initialize the register file with values 
   *
   * @param initial_register the values to initialize the register file with
   */
  register_file(const register_file& initial_register);

  /** Get the value of a register
   *
   * @return returns the register value
   * @param register_number the register number
   * @param window_pointer the current window pointer
   */
    int get_reg(REGISTER_NUMBER register_number, WINDOW_POINTER window_pointer);


   /** Set the value of a register
   *
   * @param register_number the register number
   * @param value the value to set the register to
   * @param window_pointer the current window pointer
   */
    void set_reg(REGISTER_NUMBER register_number, uint32_t value, WINDOW_POINTER window_pointer);


    /** Print to <i>cout</i> stream the debugging output for the  
     * register file in the pipeline. This is only enabled if <i>DBG_PIPE</i> flag or
     *  other debugging flags are enabled during compilation.
     */
    void regdump(WINDOW_POINTER window_pointer);

    /** Reset the window registers to zero
     */
    void reset_register_window();


    /** Overloaded equality operator.
     *
     * @param is_register register file to check if equal.
     */
    bool operator==(const register_file& is_register);

    /** Overloaded assignment operator.
     *
     * @param from_register register to be assigned from
     */
    void operator=(const register_file& from_register);


///////////////////////////////////////////////////////////////////////
///                      private variables                          ///
 private:
    uint32_t _gReg[8]; //Global Registers
    uint32_t _wReg[16*REGISTER_WINDOWS]; //Register Window


};

#endif
