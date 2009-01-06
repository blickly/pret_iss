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

  $Author: blickly $
  $Date:$
  $Id:$
*/

#ifndef _STATIC_BOUND_PARSER_H_
#define _STATIC_BOUND_PARSER_H_

#include "instruction.h"
#include <stdint.h>
#include <vector>

using namespace std;
struct memory_controller;

///////////////////////////////////////////////////////////////////////
/// static_bound_parser
/**
 * For static DMA the addresses held in a scratchpad are constant, and
 * these constants are specified in an external file.
 * The static bound parser reads these bounds from those files.
 *
 * @author  Ben Lickly, Hiren Paterl
 * @version $Id:$
 * @Pt.ProposedRating red blickly
 * @Pt.AcceptedRating
 */

class static_bound_parser {
public:
///////////////////////////////////////////////////////////////////////
///                      public methods                             ///
    /** Construct a static bound parser with no associated memory controller.
     */
    static_bound_parser();

    /** Associate the given memory controller with the static bound parser.
     *  The memory controller is used to determine where to store the bounds
     *  that are parsed.
     *  @param memc Pointer to the memory controller.
     */
    void set_memory_controller(memory_controller* memc);

    /** Parse the given file and set the bounds of the data scratchpad
     *  for the given thread.
     *  @param tid Thread ID of the data scratchpad.
     *  @param dspm Name of the file that contains the bounds.
     */
    void load_data_spm(const unsigned int tid, const string& dspm);

    /** Parse the given file and set the bounds of the instruction scratchpad
     *  for the given thread.
     *  @param tid Thread ID of the instruction scratchpad.
     *  @param ispm Name of the file that contains the bounds.
     */
    void load_inst_spm(const unsigned int tid, const string& ispm);

private:
///////////////////////////////////////////////////////////////////////
///                      private methods                            ///
    /** Parse the given file and set the bounds of the scratchpad
     *  for the given thread, starting at the given scratchpad address
     *  (to distinguish between instruction addresses and data addresses).
     *  @param tid Thread ID of the scratchpad.
     *  @param spm Name of the file that contains the bounds.
     *  @param start_addr The address of the beginning of this scratchpad.
     */
    void load_spm(const unsigned int tid, const string& spm, uint32_t start_addr);

///////////////////////////////////////////////////////////////////////
///                      private variables                          ///
    /** Pointer to the associated memory controller.  This is used to
     *  determine where to store the bounds that are parsed.
     */
    memory_controller* _mem_cont;

};
#endif
