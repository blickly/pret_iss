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

  $Author$
  $Date$
  $Id$

*/

#ifndef _SREC_PARSER_H_
#define _SREC_PARSER_H_

#include "instruction.h"
#include <stdint.h>
#include <vector>

using namespace std;
struct memory_controller;
struct memory_unit;

///////////////////////////////////////////////////////////////////////
/// srec_parser
/**
 * The srec_parser class is responsible for parsing the contents of
 * SREC files and placing them in the appropriate memory location.
 *
 * @author  Ben Lickly
 * @version $Id$
 * @Pt.ProposedRating red ben
 * @Pt.AcceptedRating
 */

class srec_parser {

public:
    /** Parse the given file as an SREC file and save the contents
     *  into the current memory unit.
     *  @param filename Name of the SREC file to parse.
     *  @return Starting address specified by SREC file.
     */
    uint32_t parse(const char* const filename);
    /** Set the given memory unit to be the target of subsequent parses.
     *  @param mem_unit Memory unit in which data will be saved.
     */
    void set_memory_unit(memory_unit& mem_unit);
    /** Construct an SREC parser with no associated memory unit.
     */
    srec_parser();
private:
    /** Read a line from the given file and save the contents.
     *  @param file Pointer to the file being parsed.
     *  @return EOF on hitting end of file.
     */
    int _readline(FILE*);
    /** Memory unit in which to store data that is read.
     */
    memory_unit* _mem;
    /** Address from which the SREC specified program must start execution.
     */
    uint32_t _starting_addr;

};
#endif
