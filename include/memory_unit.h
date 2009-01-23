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

#ifndef _MEMORY_UNIT_H_
#define _MEMORY_UNIT_H_

#include <map>
#include <ostream>

class srec_parser;
#include "srec_parser.h"
#include <stdint.h>

using namespace std;

///////////////////////////////////////////////////////////////////////
/// memory_unit
/**
 * This class uses an STL map<> object to model a memory unit. The
 * datastructure is simple. The keys are addresses and the values are
 * the data at the addresses. There are members to add and remove
 * addresses and values into the memory unit. There is a member to
 * check whether an address exists in the memory and finally one to
 * retrieve it.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */
class memory_unit {
public:
    /** Default constructor. It does nothing.
     */
    memory_unit();
///////////////////////////////////////////////////////////////////////
//                       public methods                             ///
public:
    /** Adds the address and data into the memory unit. If
     *  the address specified by <i>addr</i> already contains data then
     *  it is overwritten.
     *  @param addr The address.
     *  @param data The data to store at the address.
     */
    void add_address(const uint32_t& addr, const uint32_t& data);

    /** Overloaded operator to return the data requested at the address
     *  <i>addr</i>. If the address specified has no value assigned to
     *  it, then 0 is returned. Note: This cannot be used on the
     *  right-hand side of an equation. In that case, <i>add_address</i>
     *  should be used instead.
     *
     *  @param addr The memory unit address.
     *  @return The data at the address.
     */
    uint32_t operator[](const uint32_t& addr);

    /** This outputs the contents of the memory unit to the stream it is
     *  used with.
     * @param out Output stream such as cout.
     * @param munit Memory unit.
     * @return Output stream.
     */
    inline friend ostream& operator<<(ostream& out, memory_unit& munit) {
        for (map<uint32_t, uint32_t>::iterator i = munit._mem.begin();
                i != munit._mem.end(); ++i) {
            out << hex << setfill('0') << setw(8) << i->first << ": " << setfill('0') << setw(8) << i->second << endl;
        }
        return out;
    };
    /** Removes an address and its associated data from the memory unit.
     * @param addr The address in the memory unit.
     */
    void remove_address(const uint32_t& addr);

    /** Checks if an address has an assigned value in the memory unit.
     *  @param addr The address in the memory unit.
     *  @return True if the address has previously been assigned a value
     *  otherwise false.
     */
    bool has_address(const uint32_t& addr) const;

///////////////////////////////////////////////////////////////////////
//                       private variables                          ///
private:
    /** STL map<> datastructure to store the key and value pair of
     * address and data.
     */
    map< uint32_t, uint32_t > _mem;
};
#endif /* _MEMORY_UNIT_H_ */
