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

  $Id$
*/

#ifndef _HW_THREAD_PTR_H_
#define _HW_THREAD_PTR_H_

#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include <systemc.h>
#endif

#include "hw_thread.h"

///////////////////////////////////////////////////////////////////////
/// hw_thread_ptr
/**
 * Encapsulate the hardware thread for convenience.
 *
 * @author  Hiren Patel
 * @version $Id$
 * @Pt.ProposedRating red hiren
 * @Pt.AcceptedRating
 */

class hw_thread_ptr {
public:
    /** Construct a hardware thread pointer without a handle address being set.
     *
     */
    hw_thread_ptr();

    /** Construct a hardware thread pointer and set the handle address.
     *
     * @param thread_address Address of hardware thread.
     */
    hw_thread_ptr(const hw_thread* thread_address);

    /** Construct a hardware thread pointer and set the handle address.
     *
     * @param thread_address Address of hardware thread.
     */
    hw_thread_ptr(const hw_thread_ptr& thread_address);

    /** Get a handle to the hardware thread.
    *
    * @return Reference to hardware thread handle.
    */
    hw_thread* get_handle();


    /** Check if the hardware thread handle is NULL.
     *
     * @return True if NULL, otherwise false.
     */
    bool is_null() const;

    /** Overload equality operator.
     *
     * @param thread_address Address of hardware thread.
     */
    bool operator==(const hw_thread_ptr& thread_address);

    /** Overload assignment operator.
     *
     * @param thread_address Address of hardware thread.
     */
    void operator=(const hw_thread_ptr& thread_address);

    /** Overload pointer reference  operator to extract the handle.
     *
     * @return Pointer to hardware thread handle
     */
    hw_thread* operator->() const;

#ifndef _NO_SYSTEMC_
    /** Generate a trace file. This is mandatory for SystemC objects.
     *
     * @param trace_file Name of trace file
     * @param thread_address Hardware thread pointer object to trace
     * @param name Name of object.
     */
    inline friend void sc_trace(sc_trace_file* trace_file, const hw_thread_ptr& thread_address, const string& name) {
        // Do nothing
    }
#endif /* _NO_SYSTEMC_ */
    /** Output the handle address.
     *
     * @param out Output stream
     * @param thread_address Hardware thread pointer object.
     */
    inline friend ostream& operator<< (ostream& out, const hw_thread_ptr& thread_address) {
      out << "hw_thread_ptr: " << thread_address._handle << endl;
        return out;
    }
private:
    hw_thread* _handle;
};
#endif
